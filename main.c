#include "hasbi.h"
#include <stdlib.h>
#include <stdio.h>
#include "nazwa.h"
#include "fawwaz.h"
#include "supriadi.h"

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(60);
    InitAudioDevice(); // Inisialisasi perangkat audio
    InitPlayer();
    InitBosses();
    InitBullets();
    InitAsteroids();
    InitEnemies();
    LoadAssets();
    loadAssetMenu();
    infokanPlayer();
    infoPowerUp();
    
    while (!WindowShouldClose()) {
        // float deltaTime = GetFrameTime();

        if (!isLoadingDone) {
            loadingAnimation();  // Tampilkan loading lebih dulu
            // isLoadingDone=true;
        } else {
            varQuit();
            BeginDrawing();
            ClearBackground(BLACK);
            togglePause(); 
            if (!getPauseState()) {
                game();  
            } else {
                gamePaused();
            }
            EndDrawing();
        }
    }

    EndDrawing();
    CloseAudioDevice();
    UnloadAssets();
    // UnloadPlayer();
    unloadTextures();
    unloadAssetMenu();
    unloadResources();
    CloseWindow();
    WindowShouldClose();

    return 0;
}
