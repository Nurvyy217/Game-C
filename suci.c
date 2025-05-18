#include "suci.h"
#include "hasbi.h"
#include "nazwa.h"
#include "fawwaz.h"
#include "supriadi.h"
#include <stdio.h>
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

// BARU
Sound clickMenu, selectMenu;
void InitAssets(Assets *assets)
{
    // Muat background yang sudah ada UFO dan teks "space invaders"
    assets->bg = LoadTexture("assets/bgDisplayUtama.png");

    // (Jika memang tidak dipakai lagi, boleh dihapus)
    // assets->title = LoadTexture("title.png");

    // Muat tombol
    assets->btnPlay = LoadTexture("assets/btnPlay.png");
    assets->btnMenu = LoadTexture("assets/btnMenu.png");
    assets->btnExit = LoadTexture("assets/btnExit.png");
    clickMenu = LoadSound("assets/clickMenu.wav");
    selectMenu = LoadSound("assets/selectMenu.wav");

    assets->menuBackground = LoadTexture("assets/bgMenuScreen.png");  
    assets->settingsBackground = LoadTexture("assets/bgMenuScreen.png"); 
    assets->btnOn = LoadTexture("assets/btnOn.png");
    assets->btnOff = LoadTexture("assets/btnOff.png");
    assets->btnBack = LoadTexture("assets/btnBack.png");
    assets->bgMusic = LoadMusicStream("assets/background_music.mp3");


    PlayMusicStream(assets->bgMusic);
    SetMusicVolume(assets->bgMusic, 0.5f);
}

void UnloadAssetss(Assets *assets)
{
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
// Inisialisasi menu pengaturan
// void InitSettingsMenu(SettingsMenu *menu) {
//     menu->selectedOption = 0;
//     menu->volume = 1.0f;
//     menu->screenWidth = SCREEN_WIDTH;
//     menu->screenHeight = SCREEN_HEIGHT;
//     menu->options[0] = "On";
//     menu->options[1] = "Off";
//     menu->options[2] = "Back";
//     }

// // Menangani input di menu settings
// void UpdateSettingsMenu(SettingsMenu *menu) {
//     if (IsKeyPressed(KEY_DOWN)) menu->selectedOption = (menu->selectedOption + 1) % 3;
//     if (IsKeyPressed(KEY_UP)) menu->selectedOption = (menu->selectedOption - 1 + 3) % 3;
//     if (menu->selectedOption == 0) {
//         if (IsKeyPressed(KEY_LEFT)) menu->volume = fmax(menu->volume - 0.1f, 0.0f);
//         if (IsKeyPressed(KEY_RIGHT)) menu->volume = fmin(menu->volume + 0.1f, 1.0f);
//     }
//     if (menu->selectedOption == 2 && IsKeyPressed(KEY_ENTER)) {
//         menu->selectedOption = 0;
//     }
//     }
    
    // // Menampilkan menu settings di layar
    // void DrawSettingsMenu(SettingsMenu *menu) {
    // BeginDrawing();
    // ClearBackground(BLUE);
    // DrawText("Pengaturan", SCREEN_WIDTH / 2 - 50, 100, 20, RAYWHITE);
    // for (int i = 0; i < 3; i++) {
    //     Color color = (i == menu->selectedOption) ? YELLOW : WHITE;
    //     DrawText(menu->options[i], SCREEN_WIDTH / 2 - 50, 200 + i * 40, 20, color);
    // }
    // DrawText(TextFormat("Volume: %.1f", menu->volume), SCREEN_WIDTH / 2 - 50, 240, 20, WHITE);
    // EndDrawing();
    // }

void InitSettingsMenu(SettingsMenu *menu) {
    menu->head = NULL;  // Inisialisasi linked list kosong
    menu->selectedOption = 0;
    menu->volume = 1.0f;
    menu->screenWidth = SCREEN_WIDTH;
    menu->screenHeight = SCREEN_HEIGHT;

    // Menambahkan opsi ke dalam linked list
    addOption(menu, "On");
    addOption(menu, "Off");
    addOption(menu, "Back");
}

void addOption(SettingsMenu *menu, const char *option) {
    Node *newNode = (Node*)malloc(sizeof(Node));  // Alokasikan memori untuk node baru
    newNode->option = option;                      // Simpan pilihan dalam node
    newNode->next = NULL;                          // Node terakhir menunjuk ke NULL

    if (menu->head == NULL) {
        menu->head = newNode;  // Jika linked list kosong, node ini menjadi head
    } else {
        Node *current = menu->head;
        while (current->next != NULL) {
            current = current->next;  // Cari node terakhir
        }
        current->next = newNode;  // Tambahkan node baru di akhir
    }
}


void printMenuOptions(SettingsMenu *menu) {
    Node *current = menu->head;
    while (current != NULL) {
        printf("%s\n", current->option);  // Cetak pilihan menu
        current = current->next;          // Pindah ke node berikutnya
    }
}

void clearMenuOptions(SettingsMenu *menu) {
    Node *current = menu->head;
    while (current != NULL) {
        Node *next = current->next;  // Simpan pointer ke node berikutnya
        free(current);               // Hapus node saat ini
        current = next;              // Pindah ke node berikutnya
    }
    menu->head = NULL;  // Set head ke NULL untuk menunjukkan linked list kosong
}

void DrawSettingsMenu(SettingsMenu *menu) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Pengaturan", SCREEN_WIDTH / 2 - 50, 100, 20, RAYWHITE);
    
    // Tampilkan opsi menu dari linked list
    Node *current = menu->head;
    int yOffset = 200;
    while (current != NULL) {
        Color color = (menu->selectedOption == 0) ? YELLOW : WHITE; // Pilihan yang dipilih diberi warna khusus
        DrawText(current->option, SCREEN_WIDTH / 2 - 50, yOffset, 20, color);
        yOffset += 40;
        current = current->next;
    }
    
    EndDrawing();
}

void UpdateSettingsMenu(SettingsMenu *menu) {
    if (IsKeyPressed(KEY_DOWN)) {
        menu->selectedOption = (menu->selectedOption + 1) % 3;
    }
    if (IsKeyPressed(KEY_UP)) {
        menu->selectedOption = (menu->selectedOption - 1 + 3) % 3;
    }

    if (menu->selectedOption == 0) {
        if (IsKeyPressed(KEY_LEFT)) menu->volume = fmax(menu->volume - 0.1f, 0.0f);
        if (IsKeyPressed(KEY_RIGHT)) menu->volume = fmin(menu->volume + 0.1f, 1.0f);
    }

    if (menu->selectedOption == 2 && IsKeyPressed(KEY_ENTER)) {
        // Implement action for "Back"
    }
}


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
    
    while (isRunning) {
        
        UpdateMusicStream(assets.bgMusic);
        
        if (currentScreen == MENU) {
            UpdateMenu(&assets, &currentScreen);
        } else if (currentScreen == PLAY) {
            UpdatePlayScreen(&currentScreen);
        } else if (currentScreen == SETTINGS) {
            UpdateSettingsScreen(&currentScreen, &settingsMenu, &assets);
        }
           if  (currentScreen == EXIT) 
           {  isRunning = false; // Keluar dari game
        }
    }

    UnloadAssetss(&assets);
}