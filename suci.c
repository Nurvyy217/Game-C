#include "suci.h"
#include "hasbi.h"
#include "nazwa.h"
#include "fawwaz.h"
#include "supriadi.h"
#include <stdio.h>
Texture2D logoDeveloper;
Texture2D gameNamePhoto;
Texture2D background;
bool texturesLoaded = false; // Cek apakah gambar sudah di-load

void initLoadScreen()
{
    if (!texturesLoaded)
    { // Load hanya sekali
        logoDeveloper = LoadTexture("assets/logoG6.png");
        gameNamePhoto = LoadTexture("assets/spaceInviders.png");
        texturesLoaded = true;
    }
}

void initBG(){
    background= LoadTexture("assets/background.png");
}










//BARU

void InitAssets(Assets *assets) {
    // Muat background yang sudah ada UFO dan teks "space invaders"
    assets->bg = LoadTexture("bgDisplayUtama.png");

    // (Jika memang tidak dipakai lagi, boleh dihapus)
    // assets->title = LoadTexture("title.png");

    // Muat tombol
    assets->btnPlay = LoadTexture("btnPlay.png");
    assets->btnMenu = LoadTexture("btnMenu.png");
    assets->btnExit = LoadTexture("btnExit.png");

    // Muat musik
    assets->bgMusic = LoadMusicStream("background_music.mp3");
    PlayMusicStream(assets->bgMusic);
    SetMusicVolume(assets->bgMusic, 1.0f);
}

void UnloadAssetss(Assets *assets) {
    UnloadTexture(assets->bg);
    // UnloadTexture(assets->title); // Jika tidak lagi dipakai

    UnloadTexture(assets->btnPlay);
    UnloadTexture(assets->btnMenu);
    UnloadTexture(assets->btnExit);

    UnloadMusicStream(assets->bgMusic);
    CloseAudioDevice();
}

void UpdateMenu(Assets *assets, GameScreen *currentScreen) {
    static MenuOption menuIndex = MENU_PLAY;  

    // --- Input Navigasi ---
    if (IsKeyPressed(KEY_DOWN)) {
        menuIndex = (menuIndex + 1) % MENU_TOTAL; 
    }
    if (IsKeyPressed(KEY_UP)) {
        menuIndex = (menuIndex - 1 + MENU_TOTAL) % MENU_TOTAL; 
    }
    if (IsKeyPressed(KEY_ENTER)) {
        if (menuIndex == MENU_PLAY) {
            *currentScreen = PLAY;
        } else if (menuIndex == MENU_SETTINGS) {
            *currentScreen = SETTINGS;
        } else if (menuIndex == MENU_EXIT) {
            CloseWindow();
        }
    }
    
    // --- Gambar ---
    BeginDrawing();
    ClearBackground(BLACK);

    // Gambar background
    DrawTexture(assets->bg, 0, 0, WHITE);

    // --- Pengaturan Tombol ---
    float scaleBtn = 2.0f;  
    int scaledWidth = (int)(assets->btnPlay.width * scaleBtn);
    int scaledHeight = (int)(assets->btnPlay.height * scaleBtn);
    int spacing = 0; // spacing 0, tapi kita atur offset secara manual

    int buttonX = SCREEN_WIDTH / 2 - scaledWidth / 2;
    int buttonY = 500;  // Posisi awal tombol PLAY

    // Tombol PLAY
    if (assets->btnPlay.id > 0) {
        Color tintPlay = (menuIndex == MENU_PLAY) ? PURPLE : WHITE;
        DrawTextureEx(assets->btnPlay, (Vector2){buttonX, buttonY}, 0.0f, scaleBtn, tintPlay);
    }

    // Hitung offset untuk mengurangi jarak antar tombol
    int offset = (int)(scaledHeight * 0.3f);  // Misalnya, kurangi 30% dari tinggi tombol

    // Tombol MENU
    buttonY += scaledHeight - offset;
    if (assets->btnMenu.id > 0) {
        Color tintMenu = (menuIndex == MENU_SETTINGS) ? PURPLE : WHITE;
        DrawTextureEx(assets->btnMenu, (Vector2){buttonX, buttonY}, 0.0f, scaleBtn, tintMenu);
    }

    // Tombol EXIT
    buttonY += scaledHeight - offset;
    if (assets->btnExit.id > 0) {
        Color tintExit = (menuIndex == MENU_EXIT) ? PURPLE : WHITE;
        DrawTextureEx(assets->btnExit, (Vector2){buttonX, buttonY}, 0.0f, scaleBtn, tintExit);
    }
    EndDrawing();
}

void UpdatePlayScreen(GameScreen *currentScreen) {
    BeginDrawing();
    ClearBackground(DARKGRAY);
    varQuit();
    togglePause(); 
    if (!getPauseState()) {
        game();  
    } else {
        gamePaused();
    }
    EndDrawing();
}

void UpdateSettingsScreen(GameScreen *currentScreen) {
    BeginDrawing();
    ClearBackground(BLUE);
    DrawText("Pengaturan", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, 20, RAYWHITE);
    DrawText("Tekan ESC untuk kembali", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 30, 20, RAYWHITE);
    if (IsKeyPressed(KEY_ESCAPE)) *currentScreen = MENU;
    EndDrawing();
}


void menuSuci(){
    Assets assets;
    InitAssets(&assets);
    
    GameScreen currentScreen = MENU;
    bool isRunning = true;
    
    while (isRunning) {
        
        UpdateMusicStream(assets.bgMusic);
        
        if (currentScreen == MENU) {
            UpdateMenu(&assets, &currentScreen);
        } else if (currentScreen == PLAY) {
            UpdatePlayScreen(&currentScreen);
        } else if (currentScreen == SETTINGS) {
            UpdateSettingsScreen(&currentScreen);
        } else {
            isRunning = false; // Keluar dari game
        }
    }
   
}