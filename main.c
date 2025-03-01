#include "hasbi.h"
#include "nazwa.h"

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(60);

    InitPlayer();
    InitBullets();
    
    while (!WindowShouldClose()) {
        if (!isLoadingDone) {
            loadingAnimation();  // Tampilkan loading lebih dulu
        } else {
            UpdatePlayer();
            if (IsKeyPressed(KEY_SPACE)) ShootBullet();
            UpdateBullets();
            
            BeginDrawing();
            ClearBackground(BLACK);
            DrawGameplay();  // Menampilkan layout + player + bullet
            EndDrawing();
        }
    }

    UnloadPlayer();
    unloadTextures();
    
    CloseWindow();
    return 0;
}
