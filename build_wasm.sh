#!/usr/bin/env bash
#
# Compile and package the game as WebAssembly (Emscripten).
#
# Usage: ./build_wasm.sh [--clean] [--debug] [--zip]
#   --clean  remove build-wasm/ and dist/ before building
#   --debug  build with -O0 -g instead of -O2
#   --zip    also create dist.zip
#
# Output: dist/index.html (+ .js, .wasm, .data). Serve it with:
#   python3 -m http.server -d dist 8080
#
# All SDL libraries are built from source as static libs for wasm and cached
# in build-wasm/prefix, so later runs only rebuild the game.

set -euo pipefail

# ---- Settings (override from the environment if needed) --------------------
SDL_TAG="${SDL_TAG:-release-3.4.8}"
SDL_IMAGE_TAG="${SDL_IMAGE_TAG:-release-3.4.8}"
SDL_TTF_TAG="${SDL_TTF_TAG:-release-3.2.2}"
SDL_MIXER_TAG="${SDL_MIXER_TAG:-release-3.2.4}"
JOBS="${JOBS:-$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)}"

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD="$ROOT/build-wasm"
PREFIX="$BUILD/prefix"
DIST="$ROOT/dist"

CLEAN=0; DEBUG=0; ZIP=0
for arg in "$@"; do
    case "$arg" in
        --clean) CLEAN=1 ;;
        --debug) DEBUG=1 ;;
        --zip)   ZIP=1 ;;
        -h|--help) sed -n '2,13p' "$0"; exit 0 ;;
        *) echo "Unknown option: $arg" >&2; exit 1 ;;
    esac
done

if [[ $DEBUG -eq 1 ]]; then BUILD_TYPE=Debug; else BUILD_TYPE=Release; fi

# ---- Prerequisites ----------------------------------------------------------
# Fall back to the cmake bundled with CLion when none is on the PATH.
if ! command -v cmake >/dev/null 2>&1; then
    for dir in "$HOME"/Applications/CLion*.app /Applications/CLion*.app; do
        bundled="$dir/Contents/bin/cmake/mac/$(uname -m | sed 's/arm64/aarch64/')/bin"
        if [[ -x "$bundled/cmake" ]]; then
            export PATH="$bundled:$PATH"
            echo "==> Using CLion's bundled cmake: $bundled"
            break
        fi
    done
fi

for tool in emcc emcmake cmake git; do
    command -v "$tool" >/dev/null 2>&1 || {
        echo "error: '$tool' not found. Install Emscripten (brew install emscripten) and cmake/git." >&2
        exit 1
    }
done

if [[ $CLEAN -eq 1 ]]; then
    echo "==> Cleaning"
    rm -rf "$BUILD" "$DIST" "$ROOT/dist.zip"
fi
mkdir -p "$BUILD/src" "$PREFIX"

# ---- Dependencies -----------------------------------------------------------
# build_dep <name> <repo-url> <tag> <marker-file> [extra cmake args...]
build_dep() {
    local name="$1" url="$2" tag="$3" marker="$4"; shift 4
    if [[ -e "$PREFIX/$marker" ]]; then
        echo "==> $name already built (cached)"
        return
    fi
    echo "==> Building $name ($tag)"
    local src="$BUILD/src/$name"
    if [[ ! -d "$src" ]]; then
        git clone --depth 1 --branch "$tag" --recurse-submodules --shallow-submodules "$url" "$src"
    fi
    emcmake cmake -S "$src" -B "$BUILD/obj/$name" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DCMAKE_INSTALL_PREFIX="$PREFIX" \
        -DCMAKE_PREFIX_PATH="$PREFIX" \
        -DCMAKE_FIND_ROOT_PATH="$PREFIX" \
        -DBUILD_SHARED_LIBS=OFF \
        -DSDL_TESTS=OFF -DSDL_EXAMPLES=OFF \
        "$@"
    cmake --build "$BUILD/obj/$name" -j "$JOBS"
    cmake --install "$BUILD/obj/$name"
}

build_dep SDL        https://github.com/libsdl-org/SDL        "$SDL_TAG"        lib/libSDL3.a \
    -DSDL_STATIC=ON -DSDL_SHARED=OFF
build_dep SDL_image  https://github.com/libsdl-org/SDL_image  "$SDL_IMAGE_TAG"  lib/libSDL3_image.a \
    -DSDLIMAGE_VENDORED=ON -DSDLIMAGE_SAMPLES=OFF -DSDLIMAGE_TESTS=OFF \
    -DSDLIMAGE_AVIF=OFF -DSDLIMAGE_JXL=OFF -DSDLIMAGE_TIF=OFF -DSDLIMAGE_WEBP=OFF
build_dep SDL_ttf    https://github.com/libsdl-org/SDL_ttf    "$SDL_TTF_TAG"    lib/libSDL3_ttf.a \
    -DSDLTTF_VENDORED=ON -DSDLTTF_SAMPLES=OFF -DSDLTTF_HARFBUZZ=ON
build_dep SDL_mixer  https://github.com/libsdl-org/SDL_mixer  "$SDL_MIXER_TAG"  lib/libSDL3_mixer.a \
    -DSDLMIXER_VENDORED=ON -DSDLMIXER_SAMPLES=OFF -DSDLMIXER_TESTS=OFF \
    -DSDLMIXER_MP3=OFF -DSDLMIXER_FLAC=OFF -DSDLMIXER_OPUS=OFF -DSDLMIXER_VORBIS=OFF \
    -DSDLMIXER_MIDI=OFF -DSDLMIXER_MOD=OFF -DSDLMIXER_WAVPACK=OFF -DSDLMIXER_GME=OFF

# ---- Game -------------------------------------------------------------------
# A small CMake project is generated so the static libs' transitive
# dependencies (freetype, harfbuzz, libpng, ...) are linked automatically.
GAME_DIR="$BUILD/game"
mkdir -p "$GAME_DIR"
cat > "$GAME_DIR/CMakeLists.txt" <<EOF
cmake_minimum_required(VERSION 3.16)
project(flappybird CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)
find_package(SDL3_mixer REQUIRED CONFIG)

file(GLOB GAME_SOURCES "$ROOT/src/*.cpp")
add_executable(index \${GAME_SOURCES})
target_include_directories(index PRIVATE "$ROOT/include")
target_link_libraries(index PRIVATE SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf SDL3_mixer::SDL3_mixer SDL3::SDL3)

# ASYNCIFY lets the blocking Game::RunLoop() yield to the browser.
# Assets are preloaded into the virtual FS, which SDL_GetBasePath() maps to "/".
target_link_options(index PRIVATE
    -sASYNCIFY -sALLOW_MEMORY_GROWTH=1
    "SHELL:--shell-file $GAME_DIR/shell.html"
    "SHELL:--preload-file $ROOT/Graphics@/Graphics"
    "SHELL:--preload-file $ROOT/Fonts@/Fonts"
    "SHELL:--preload-file $ROOT/Sounds@/Sounds")
set_target_properties(index PROPERTIES SUFFIX ".html")
EOF

cat > "$GAME_DIR/shell.html" <<'EOF'
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Flappy Bird</title>
  <style>
    html, body { margin: 0; height: 100%; background: #222; }
    body { display: flex; align-items: center; justify-content: center; }
    canvas { display: block; max-width: 100vw; max-height: 100vh; outline: none; }
  </style>
</head>
<body>
  <canvas id="canvas" tabindex="-1" oncontextmenu="event.preventDefault()"></canvas>
  <script>
    var Module = { canvas: document.getElementById('canvas'),
                   print: console.log, printErr: console.error };
  </script>
  {{{ SCRIPT }}}
</body>
</html>
EOF

echo "==> Building game ($BUILD_TYPE)"
emcmake cmake -S "$GAME_DIR" -B "$BUILD/obj/game" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_PREFIX_PATH="$PREFIX" \
    -DCMAKE_FIND_ROOT_PATH="$PREFIX"
cmake --build "$BUILD/obj/game" -j "$JOBS"

# ---- Package ----------------------------------------------------------------
echo "==> Packaging into dist/"
rm -rf "$DIST"
mkdir -p "$DIST"
for ext in html js wasm data; do
    cp "$BUILD/obj/game/index.$ext" "$DIST/"
done

if [[ $ZIP -eq 1 ]]; then
    command -v zip >/dev/null 2>&1 || { echo "error: 'zip' not found" >&2; exit 1; }
    (cd "$DIST" && zip -qr "$ROOT/dist.zip" .)
    echo "==> Created dist.zip"
fi

echo
echo "Done. Serve with:  python3 -m http.server -d dist 8080"
echo "Then open http://localhost:8080"
