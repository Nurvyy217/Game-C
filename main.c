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
    Image IconNa = LoadImage("assets/IconNa.png");
    SetWindowIcon(IconNa);
    SetTargetFPS(60);
    InitAudioDevice(); // Inisialisasi perangkat audio
    InitBGM();
    InitStar();
    InitPlayer();
    InitEnemy();
    InitBosses();
    InitEnemyBullets();
    InitBullets();
    
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

    UnloadImage(IconNa);
    FreeStars();
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
