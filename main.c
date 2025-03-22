#include "hasbi.h"
#include <stdlib.h>
#include <stdio.h>
#include "nazwa.h"
#include "fawwaz.h"
#include "supriadi.h"
#include "suci.h"

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(60);
    InitAudioDevice(); // Inisialisasi perangkat audio
    InitPlayer();
    InitBossesShoot();
    InitBosses();
    InitBullets();
    InitAsteroids();
    InitEnemies();
    LoadAssets();
    loadAssetMenu();
    infokanPlayer();
    infoPowerUp();
    
    while (!WindowShouldClose()) {
        if (!isLoadingDone) {
            loadingAnimation();}  // Tampilkan loading lebih dulu
        else {
                menuSuci();
            }
    }

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

