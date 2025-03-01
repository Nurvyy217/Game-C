#include "hasbi.h"
#include "nazwa.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(60);
    InitAudioDevice(); // Inisialisasi perangkat audio

    InitPlayer();
    InitBullets();
    InitAsteroids();
    InitEnemies();
    LoadAssets();
    
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        if (!isLoadingDone) {
            loadingAnimation();  // Tampilkan loading lebih dulu
            // isLoadingDone=true;
            
        } else {
            // UpdatePlayer();
            // // if (IsKeyPressed(KEY_SPACE)) ShootBullet();
            // UpdateShooting(deltaTime);
            // UpdateBullets();
            // UpdateExplosions(deltaTime);
            // CheckCollisions();
            // UpdateEnemies();
            // UpdateEnemyBullets();
            // CheckEnemyCollisions();
            // AsteroidLoop();
            // EnemiesLoop();
            
            level1();
            drawGameplay();
            bool gameStart = false; //status game
            mainMenu(&gameStart); 
        }
    }
    CloseAudioDevice();
    UnloadAssets();
    unloadTextures();
    CloseWindow();
    return 0;
}
