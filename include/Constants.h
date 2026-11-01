//
// Created by Filho, Antonio on 29/10/2026.
//

#pragma once

namespace Constants {
    namespace Window {
        inline constexpr const char* Title = "SDL3 Flappy Bird";
        inline constexpr int Width = 556;
        inline constexpr int Height = 900;
    }

    namespace Bird {
        inline constexpr float Size = 30.0f;
    }

    namespace Sprites {
        inline constexpr const char* TileSetPath = "Graphics/sprites.png";
        namespace Background {
            inline constexpr float X = 389.0f;
            inline constexpr float Y = 3.0f;
            inline constexpr float Width = 277.0f;
            inline constexpr float Height = 202.0f;
        }
        namespace Floor {
            inline constexpr float X = 0.0f;
            inline constexpr float Y = 612.0f;
            inline constexpr float Width = 225.0f;
            inline constexpr float Height = 111.0f;
        }
    }
}
