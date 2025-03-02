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
    
    infokanPlayer();
    infoPowerUp();
    
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        if (!isLoadingDone) {
            // loadingAnimation();  // Tampilkan loading lebih dulu
            isLoadingDone=true;
        } else {
            
            if (gameover()){
                
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
                DrawGameplay();
                  // Menampilkan layout + player + bullet + asteroids
                tampilNyawa();
                Tampil_Score();
                spawnPowerUp();
                tampilPowerUp();
                // checkPowerUpCollision();
                EndDrawing();
            }
            else{
                restart();
                EndDrawing();
            }
        }
    }

    EndDrawing();
    CloseAudioDevice();
    UnloadAssets();
    // UnloadPlayer();
    unloadTextures();
    CloseWindow();
    WindowShouldClose();

    return 0;
}
