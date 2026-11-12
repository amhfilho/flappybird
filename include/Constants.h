//
// Created by Filho, Antonio on 29/10/2026.
//

#pragma once

namespace Constants {
    // How long the game freezes after the bird hits a pipe
    inline constexpr unsigned int HitFreezeMs = 500;

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
            inline constexpr float StartX = 50.0f;
            inline constexpr float StartY = 50.0f;
            inline constexpr float Size = 30.0f;
            inline constexpr float Width = 35.0f;
            inline constexpr float Height = 25.0f;
            inline constexpr double DownAngle = 55;
            inline constexpr double DownAngleIncrement = 2;
            inline constexpr double UpAngle = -45;
            inline constexpr double UpAngleDecrement = -5;
            inline constexpr float AnimationFPS = 24.0f;
            inline constexpr float Gravity = 500.0f;
            inline constexpr float FlapSpeed = 200.0f;

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
            inline constexpr float Speed = 100.0f;
            inline constexpr float MinimumInitialGap = 90.0f;
        }
        namespace StartScreen {
            inline constexpr float X = 131.0f;
            inline constexpr float Y = 0.0f;
            inline constexpr float Width = 179.0f;
            inline constexpr float Height = 153.0f;
        }
        namespace GameOver {
            inline constexpr float X = 132.0f;
            inline constexpr float Y = 150.0f;
            inline constexpr float Width = 231.0f;
            inline constexpr float Height = 204.0f;
            inline constexpr float CentralX = 162.5f;
            namespace Score {
                inline constexpr float X = 171.25f;
                inline constexpr float Y = 115.0f;
            }
            namespace Best {
                inline constexpr float Y = 135.0f;
            }
            namespace Medal {
                inline constexpr float X = 191.5f;
                inline constexpr float Y = 240.0f;
            }
        }
        namespace Medal {
            namespace Platinum {
                inline constexpr float X = 0.0f;
                inline constexpr float Y = 79.0f;
                inline constexpr float Width = 43.0f;
                inline constexpr float Height = 43.0f;
            }
            namespace Gold {
                inline constexpr float X = 0.0f;
                inline constexpr float Y = 122.0f;
                inline constexpr float Width = 45.0f;
                inline constexpr float Height = 46.0f;
            }
            namespace Silver {
                inline constexpr float X = 47.0f;
                inline constexpr float Y = 79.0f;
                inline constexpr float Width = 45.0f;
                inline constexpr float Height = 46.0f;
            }
            namespace Bronze {
                inline constexpr float X = 47.0f;
                inline constexpr float Y = 122.0f;
                inline constexpr float Width = 45.0f;
                inline constexpr float Height = 46.0f;
            }
        }
    }

    namespace Font {
        inline constexpr const char* Path = "Fonts/PublicPixel.ttf";
        inline constexpr float Size = 40.0f;
    }
}
