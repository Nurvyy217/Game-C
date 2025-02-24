#include "hasbi.h"
#include "supriadi.h"

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(60);

    InitPlayer();
    InitBullets();
    BuatNyawa();
    
    while (!WindowShouldClose()) {
        if (!isLoadingDone) {
            loadingAnimation();  // Tampilkan loading lebih dulu
        } else {
            
            if (gameover()){
                UpdatePlayer();
                if (IsKeyDown(KEY_SPACE)) ShootBullet();
                UpdateBullets();
                
                BeginDrawing();
                ClearBackground(BLACK);
                DrawGameplay();  // Menampilkan layout + player + bullet
        
                rubahNyawa();
                Tampil_Nyawa();
                EndDrawing();
            }
            else{
                DrawText("Press R to Restart", 300, 300, 20, BLACK);
                restart();
                EndDrawing();
            }
        }
    }
    UnloadPlayer();
    unloadTextures();
    unloadNyawa();
    WindowShouldClose();

    return 0;
}
