#include "hasbi.h"
#include "fawwaz.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(60);
    InitAudioDevice(); // Inisialisasi perangkat audio
    InitPlayer();
    InitBullets();
    
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();


        if (!isLoadingDone) {
            loadingAnimation();  // Tampilkan loading lebih dulu
        } else {
            
            UpdatePlayer();
            // if (IsKeyPressed(KEY_SPACE)) ShootBullet();
            UpdateShooting(deltaTime);
            UpdateBullets();
            UpdateExplosions(deltaTime);
            CheckCollisions();
            GameLoop();
            
            BeginDrawing();
            ClearBackground(BLACK);
            DrawGameplay();  // Menampilkan layout + player + bullet
            EndDrawing();
        
        }
    }
    CloseAudioDevice();
    UnloadAssets();
    unloadTextures();
    
    CloseWindow();
    return 0;
}
