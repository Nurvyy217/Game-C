#include "hasbi.h"
#include <stdlib.h>
#include <stdio.h>
#include "supriadi.h"

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(60);
    InitAudioDevice(); // Inisialisasi perangkat audio
    InitPlayer();
    InitBullets();
    InitAsteroids();
    InitEnemies();
    LoadAssets();
    BuatNyawa();
    
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        if (!isLoadingDone) {
            // loadingAnimation();  // Tampilkan loading lebih dulu
            isLoadingDone=true;
        } else {
            UpdatePlayer();
            // if (IsKeyPressed(KEY_SPACE)) ShootBullet();
            UpdateShooting(deltaTime);
            UpdateBullets();
            UpdateExplosions(deltaTime);
            CheckCollisions();
            UpdateEnemies();
            UpdateEnemyBullets();
            CheckEnemyCollisions();
            AsteroidLoop();
            EnemiesLoop();
            BeginDrawing();
            ClearBackground(BLACK);
            DrawGameplay();  // Menampilkan layout + player + bullet + asteroids
            EndDrawing();
            
            if (gameover()){
                UpdatePlayer();
                if (IsKeyPressed(KEY_SPACE)) ShootBullet();
                UpdateBullets();
                
                BeginDrawing();
                ClearBackground(BLACK);
                DrawGameplay();  // Menampilkan layout + player + bullet
        
                Tampil_Nyawa();
                Tampil_Score();
                EndDrawing();
            }
            else{
                restart();
                EndDrawing();
            }
        }
    }
    CloseAudioDevice();
    UnloadAssets();
    UnloadPlayer();
    unloadTextures();
    CloseWindow();
    unloadNyawa();
    WindowShouldClose();

    return 0;
}
