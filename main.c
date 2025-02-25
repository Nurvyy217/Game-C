#include "hasbi.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(60);
    InitAudioDevice(); // Inisialisasi perangkat audio
    InitPlayer();
    InitBullets();
    InitAsteroids();
    LoadAssets();
    
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        if (!isLoadingDone) {
            // loadingAnimation();  // Tampilkan loading lebih dulu
            isLoadingDone=true;
        } else {
            UpdatePlayer();
            if (IsKeyPressed(KEY_SPACE)) ShootBullet();
            UpdateBullets();
            UpdateExplosions(deltaTime);
            CheckCollisions();
            GameLoop();
            
            BeginDrawing();
            ClearBackground(BLACK);
            DrawGameplay();  // Menampilkan layout + player + bullet + asteroids
            EndDrawing();
        
        }
    }

    UnloadAssets();
    unloadTextures();
    
    CloseWindow();
    return 0;
}
