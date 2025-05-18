#include "suci.h"
#include "hasbi.h"
#include "nazwa.h"
#include "fawwaz.h"
#include "supriadi.h"
#include <stdio.h>
#include <raylib.h>
#include <math.h>

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

void initBG()
{
    background = LoadTexture("assets/background.png");
}

//BARU

void InitAssets(Assets *assets) {
    // Muat background yang sudah ada UFO dan teks "space invaders"
    assets->bg = LoadTexture("assets/bgDisplayUtama.png");

    // assets->title = LoadTexture("title.png");

    // Muat tombol
    assets->btnPlay = LoadTexture("assets/btnPlay.png");
    assets->btnMenu = LoadTexture("assets/btnMenu.png");
    assets->btnExit = LoadTexture("assets/btnExit.png");
    clickMenu = LoadSound("assets/clickMenu.wav");
    selectMenu = LoadSound("assets/selectMenu.wav");

    // Muat musik
    assets->bgMusic = LoadMusicStream("assets/background_music.mp3");
    PlayMusicStream(assets->bgMusic);
    SetMusicVolume(assets->bgMusic, 1.0f);
    assets->isMusicOn = true;

    assets->menuBackground = LoadTexture("aseets/bgMenuScreen.png");  
    assets->settingsBackground = LoadTexture("assets/bgMenuScreen.png"); 
    assets->btnOn = LoadTexture("assets/btnOn.png");
    assets->btnOff = LoadTexture("assets/btnOff.png");
    assets->btnBack = LoadTexture("assets/btnBack.png");
}

void UnloadAssetss(Assets *assets) {
    UnloadTexture(assets->bg);
    // UnloadTexture(assets->title); // Jika tidak lagi dipakai

    UnloadTexture(assets->btnPlay);
    UnloadTexture(assets->btnMenu);
    UnloadTexture(assets->btnExit);

    UnloadTexture(assets->btnOn);
    UnloadTexture(assets->btnOff);
    UnloadTexture(assets->btnBack);

    UnloadTexture(assets->menuBackground);
    UnloadTexture(assets->settingsBackground);

    UnloadMusicStream(assets->bgMusic);
    CloseAudioDevice();
}

void UpdateMenu(Assets *assets, GameScreen *currentScreen)
{
    static MenuOption menuIndex = MENU_PLAY;

    // --- Input Navigasi ---
    if (IsKeyPressed(KEY_DOWN))
    {
        PlaySound(selectMenu);
        menuIndex = (menuIndex + 1) % MENU_TOTAL;
    }
    if (IsKeyPressed(KEY_UP))
    {
        PlaySound(selectMenu);
        menuIndex = (menuIndex - 1 + MENU_TOTAL) % MENU_TOTAL;
    }
    if (IsKeyPressed(KEY_ENTER))
    {
        PlaySound(clickMenu);
        if (menuIndex == MENU_PLAY)
        {
            *currentScreen = PLAY;
            StopMusicStream(assets->bgMusic);
        }
        else if (menuIndex == MENU_SETTINGS)
        {
            *currentScreen = SETTINGS;
        }
        else if (menuIndex == MENU_EXIT)
        {
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
    int buttonY = 550; // Posisi awal tombol PLAY

    // Tombol PLAY
    if (assets->btnPlay.id > 0)
    {
        Color tintPlay = (menuIndex == MENU_PLAY) ? PURPLE : WHITE;
        DrawTextureEx(assets->btnPlay, (Vector2){buttonX, buttonY}, 0.0f, scaleBtn, tintPlay);
    }

    // Tombol MENU
    buttonY += 70;
    if (assets->btnMenu.id > 0)
    {
        Color tintMenu = (menuIndex == MENU_SETTINGS) ? PURPLE : WHITE;
        DrawTextureEx(assets->btnMenu, (Vector2){buttonX, buttonY}, 0.0f, scaleBtn, tintMenu);
    }

    // Tombol EXIT
    buttonY += 90;
    if (assets->btnExit.id > 0)
    {
        Color tintExit = (menuIndex == MENU_EXIT) ? PURPLE : WHITE;
        DrawTextureEx(assets->btnExit, (Vector2){buttonX, buttonY}, 0.0f, scaleBtn, tintExit);
    }
    EndDrawing();
}

void UpdatePlayScreen(GameScreen *currentScreen)
{

    BeginDrawing();

    ClearBackground(DARKGRAY);
    varQuit();
    togglePause();
    if (!getPauseState())
    {
        game();
    }
    else
    {
        gamePaused();
    }
    EndDrawing();
}

// --- Gambar ---
// BeginDrawing();
// ClearBackground(BLACK);

// // Gambar background
// DrawTexture(assets->bg, 0, 0, WHITE);

// // --- Pengaturan Tombol ---
// float scaleBtn = 2.0f;  
// int scaledWidth = (int)(assets->btnPlay.width * scaleBtn);
// int scaledHeight = (int)(assets->btnPlay.height * scaleBtn);
// int spacing = 0; // spacing 0, tapi kita atur offset secara manual

// int buttonX = SCREEN_WIDTH / 2 - scaledWidth / 2;
// int buttonY = 500;  // Posisi awal tombol PLAY

// // Tombol PLAY
// if (assets->btnPlay.id > 0) {
//     Color tintPlay = (menuIndex == MENU_PLAY) ? PURPLE : WHITE;
//     DrawTextureEx(assets->btnPlay, (Vector2){buttonX, buttonY}, 0.0f, scaleBtn, tintPlay);
// }

// // Hitung offset untuk mengurangi jarak antar tombol
// int offset = (int)(scaledHeight * 0.3f);  // Misalnya, kurangi 30% dari tinggi tombol

// // Tombol MENU
// buttonY += scaledHeight - offset;
// if (assets->btnMenu.id > 0) {
//     Color tintMenu = (menuIndex == MENU_SETTINGS) ? PURPLE : WHITE;
//     DrawTextureEx(assets->btnMenu, (Vector2){buttonX, buttonY}, 0.0f, scaleBtn, tintMenu);
// }

// // Tombol EXIT
// buttonY += scaledHeight - offset;
// if (assets->btnExit.id > 0) {
//     Color tintExit = (menuIndex == MENU_EXIT) ? PURPLE : WHITE;
//     DrawTextureEx(assets->btnExit, (Vector2){buttonX, buttonY}, 0.0f, scaleBtn, tintExit);
// EndDrawing();
// }

// void UpdatePlayScreen(GameScreen *currentScreen) {
// BeginDrawing();
// ClearBackground(DARKGRAY);
// DrawText("Game Sedang Dimainkan!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 20, RAYWHITE);
// if (IsKeyPressed(KEY_ESCAPE)) *currentScreen = MENU;
// EndDrawing();
// }

// Inisialisasi menu pengaturan
void InitSettingsMenu(SettingsMenu *menu) {
menu->selectedOption = 0;
menu->volume = 1.0f;
menu->screenWidth = SCREEN_WIDTH;
menu->screenHeight = SCREEN_HEIGHT;
menu->options[0] = "Volume";
menu->options[1] = "Resolusi";
menu->options[2] = "Kembali";
}

// Menangani input di menu settings
void UpdateSettingsMenu(SettingsMenu *menu) {
if (IsKeyPressed(KEY_DOWN)) menu->selectedOption = (menu->selectedOption + 1) % 3;
if (IsKeyPressed(KEY_UP)) menu->selectedOption = (menu->selectedOption - 1 + 3) % 3;
if (menu->selectedOption == 0) {
    if (IsKeyPressed(KEY_LEFT)) menu->volume = fmax(menu->volume - 0.1f, 0.0f);
    if (IsKeyPressed(KEY_RIGHT)) menu->volume = fmin(menu->volume + 0.1f, 1.0f);
}
if (menu->selectedOption == 2 && IsKeyPressed(KEY_ENTER)) {
    menu->selectedOption = 0;
}
}

// Menampilkan menu settings di layar
void DrawSettingsMenu(SettingsMenu *menu) {
BeginDrawing();
ClearBackground(BLUE);
DrawText("Pengaturan", SCREEN_WIDTH / 2 - 50, 100, 20, RAYWHITE);
for (int i = 0; i < 3; i++) {
    Color color = (i == menu->selectedOption) ? YELLOW : WHITE;
    DrawText(menu->options[i], SCREEN_WIDTH / 2 - 50, 200 + i * 40, 20, color);
}
DrawText(TextFormat("Volume: %.1f", menu->volume), SCREEN_WIDTH / 2 - 50, 240, 20, WHITE);
EndDrawing();
}

void menuSuci()
{
// Hubungkan menu pengaturan ke game loop
void UpdateSettingsScreen(GameScreen *currentScreen, SettingsMenu *menu, Assets *assets) {
// Navigasi dengan tombol panah
if (IsKeyPressed(KEY_DOWN)) {
    menu->selectedOption = (menu->selectedOption + 1) % 3;
}
if (IsKeyPressed(KEY_UP)) {
    menu->selectedOption = (menu->selectedOption - 1 + 3) % 3;
}

// Pilihan menu
if (IsKeyPressed(KEY_ENTER)) {
    if (menu->selectedOption == 0) {
        assets->isMusicOn = true;
        PlayMusicStream(assets->bgMusic);
    } else if (menu->selectedOption == 1) {
        assets->isMusicOn = false;
        StopMusicStream(assets->bgMusic);
    } else if (menu->selectedOption == 2) {
        *currentScreen = MENU;
    }
}

// Gambar tampilan pengaturan
BeginDrawing();
ClearBackground(RAYWHITE);
DrawTexture(assets->settingsBackground, 0, 0, WHITE);

Color onColor = (menu->selectedOption == 0) ? BLUE : WHITE;
Color offColor = (menu->selectedOption == 1) ? BLUE : WHITE;
Color backColor = (menu->selectedOption == 2) ? BLUE : WHITE;
int centerX = (SCREEN_WIDTH / 2) - ((assets->btnOn.width * 1.5) / 2);

// Posisi tombol ON dan OFF sejajar
int btnSpacing = 20; // Jarak antara ON dan OFF
int btnOnX = (SCREEN_WIDTH / 2) - (assets->btnOn.width * 1.5) - (btnSpacing / 2);
int btnOffX = (SCREEN_WIDTH / 2) + (btnSpacing / 2);

DrawTextureEx(assets->btnOn, (Vector2){btnOnX, 180}, 0.0f, 1.5f, onColor);
DrawTextureEx(assets->btnOff, (Vector2){btnOffX, 180}, 0.0f, 1.5f, offColor);

// Tombol BACK tetap di tengah
DrawTextureEx(assets->btnBack, (Vector2){centerX, 240}, 0.0f, 1.5f, backColor);

EndDrawing();
}

void menuSuci() {
    Assets assets;
    InitAssets(&assets);


    SettingsMenu settingsMenu;
    InitSettingsMenu(&settingsMenu);
    
    GameScreen currentScreen = MENU;
    bool isRunning = true;

    while (isRunning)
    {

        UpdateMusicStream(assets.bgMusic);

        if (currentScreen == MENU)
        {
            UpdateMenu(&assets, &currentScreen);
        }
        else if (currentScreen == PLAY)
        {

            UpdatePlayScreen(&currentScreen);
        } else if (currentScreen == SETTINGS) {
            UpdateSettingsScreen(&currentScreen, &settingsMenu, &assets);
        }
           if  (currentScreen == EXIT) 
           {  isRunning = false; // Keluar dari game
        }
    }
   
}


Texture2D gameoverImage;  // Definisi variabel global

void InitGameoverImage() {
    gameoverImage = LoadTexture("assets/gameover.png");  // Pastikan path benar
}
void UnloadGameoverImage() {
    UnloadTexture(gameoverImage);
}

