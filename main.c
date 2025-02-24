#include "hasbi.h"
#include <stdlib.h>

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(60);

    InitPlayer();
    InitBullets();
    InitAsteroids();
    
    while (!WindowShouldClose()) {
        if (!isLoadingDone) {
            loadingAnimation();  // Tampilkan loading lebih dulu
        } else {
            UpdatePlayer();
            if (IsKeyPressed(KEY_SPACE)) ShootBullet();
            UpdateBullets();
            CheckCollisions();
            GameLoop();
            
            BeginDrawing();
            ClearBackground(BLACK);
            DrawGameplay();  // Menampilkan layout + player + bullet + asteroids
            EndDrawing();
        
        }
    }

    UnloadPlayer();
    unloadTextures();
    
    CloseWindow();
    return 0;
}
