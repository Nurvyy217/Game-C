#include "hasbi.h"
#include "fawwaz.h"
#include <stdlib.h>
#include <stdio.h>

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
    
    while (!WindowShouldClose()) {
        // float deltaTime = GetFrameTime();
        if (!isLoadingDone) {
            loadingAnimation();  // Tampilkan loading lebih dulu
            // isLoadingDone=true;
        } else {
            game();
        }
    }
    CloseAudioDevice();
    UnloadAssets();
    unloadTextures();
    CloseWindow();
    return 0;
}
