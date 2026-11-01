cmake_minimum_required(VERSION 4.3)
project(flappybird)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew" "/usr/local")

# Avoid picking up a separately-installed SDL3.framework (e.g. in
# ~/Library/Frameworks) while SDL3_image resolves to the Homebrew dylib.
# Loading both SDL3 binaries into the same process causes duplicate
# Objective-C class registrations and crashes at runtime.
set(CMAKE_FIND_FRAMEWORK NEVER)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)
find_package(SDL3_mixer REQUIRED CONFIG)

include_directories(include)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Game.cpp
        src/Colors.cpp
        include/Colors.h
        include/Game.h
        include/Constants.h
        src/Bird.cpp
        include/Bird.h
        include/Vector2.h
        src/TextureLoader.cpp
        include/TextureLoader.h
        src/AssetLoader.cpp
        src/Sprite.cpp
        include/Sprite.h
        src/Background.cpp
        include/Background.h
        include/Background.h

)

target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        SDL3_mixer::SDL3_mixer
)
# Copy fonts next to the executable so they are found via SDL_GetBasePath()
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Fonts $<TARGET_FILE_DIR:${PROJECT_NAME}>/Fonts
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Sounds $<TARGET_FILE_DIR:${PROJECT_NAME}>/Sounds
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Graphics $<TARGET_FILE_DIR:${PROJECT_NAME}>/Graphics
)

