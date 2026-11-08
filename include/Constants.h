//
// Created by Filho, Antonio on 29/10/2026.
//

#pragma once

namespace Constants {
    namespace Window {
        inline constexpr const char* Title = "SDL3 Flappy Bird";
        inline constexpr int Width = 556;
        inline constexpr int Height = 700;
    }

    namespace Sprites {
        inline constexpr const char* TileSetPath = "Graphics/sprites.png";
        namespace Background {
            inline constexpr float X = 389.0f;
            inline constexpr float Y = 3.0f;
            inline constexpr float Width = 277.0f;
            inline constexpr float Height = 202.0f;
        }
        namespace Bird {
            inline constexpr float Size = 30.0f;
            inline constexpr float Width = 35.0f;
            inline constexpr float Height = 25.0f;
            inline constexpr double DownAngle = 55;
            inline constexpr double UpAngle = -45;
            inline constexpr float AnimationFPS = 24.0f;
            namespace Sprite1 {
                inline constexpr float X = 0.0f;
                inline constexpr float Y = 0.0f;
            }
            namespace Sprite2 {
                inline constexpr float X = 0.0f;
                inline constexpr float Y = 25.0f;
            }
            namespace Sprite3 {
                inline constexpr float X = 0.0f;
                inline constexpr float Y = 50.0f;
            }
        }
        namespace Floor {
            inline constexpr float X = 0.0f;
            inline constexpr float Y = 612.0f;
            inline constexpr float Width = 225.0f;
            inline constexpr float Height = 111.0f;
        }
        namespace DownPipe {
            inline constexpr float X = 0.0f;
            inline constexpr float Y = 168.0f;
            inline constexpr float Width = 54.0f;
            inline constexpr float Height = 404.0f;
            inline constexpr float MinimumY = 208.0f;
            inline constexpr float MaximumY = 549.5f;
        }
        namespace UpperPipe {
            inline constexpr float X = 52.0f;
            inline constexpr float Y = 168.0f;
            inline constexpr float Width = 54.0f;
            inline constexpr float Height = 404.0f;
            inline constexpr float MinimumY = -341.5f;
            inline constexpr float MaximumY = 0.0f;
        }
        namespace Pipes {
            inline constexpr float MinimumVisiblePipeHeight = 62.5f;
            inline constexpr float Speed = 100.0f;
        }
    }
}
