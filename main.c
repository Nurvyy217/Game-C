#include "hasbi.h"
#include <stdlib.h>
#include <stdio.h>
#include "nazwa.h"
#include "fawwaz.h"
#include "supriadi.h"
#include "suci.h"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(60);
    InitAudioDevice(); // Inisialisasi perangkat audio
    InitBGM();
    InitStar();
    InitPlayer();
    InitEnemy();
    InitBosses();
    InitEnemyBullets();
    InitBullets();
    
    InitEnemyBullets();
    LoadAssets();
    loadAssetMenu();
    infokanPlayer();
    infoPowerUp();

    while (!WindowShouldClose())
    {
        if (!isLoadingDone)
        {
            loadingAnimation();
        } // Tampilkan loading lebih dulu
        else
        {
            menuSuci();
        }
    }

    FreeStars();
    FreeEnemyBullets();
    FreeEnemy();
    CloseAudioDevice();
    UnloadAssets();
    FreeEnemyBullets();
    freeBullets();
    freeExplosions();
    // UnloadPlayer();
    unloadTextures();
    unloadAssetMenu();
    CloseWindow();
    WindowShouldClose();

    return 0;
}
