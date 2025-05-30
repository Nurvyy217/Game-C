#include "suci.h"
#include "hasbi.h"
#include "nazwa.h"
#include "fawwaz.h"
#include "supriadi.h"
#include <stdio.h>
#include <math.h>
#include <raylib.h>
#include <math.h>

Texture2D logoDeveloper;
Texture2D gameNamePhoto;
Texture2D background;

bool texturesLoaded = false; 

void initLoadScreen()
{
    if (!texturesLoaded)
    { 
        logoDeveloper = LoadTexture("assets/logoG6.png");
        gameNamePhoto = LoadTexture("assets/spaceInviders.png");
        texturesLoaded = true;
    }
}

void initBG()
{
    background = LoadTexture("assets/background.png");
}

Sound clickMenu, selectMenu;

//inisialisasi aset-aset
void InitAssets(Assets *assets)
{
    assets->bg = LoadTexture("assets/bgDisplayUtama.png");

    assets->btnPlay = LoadTexture("assets/btnPlay.png");
    assets->btnMenu = LoadTexture("assets/btnMenu.png");
    assets->btnExit = LoadTexture("assets/btnExit.png");
    clickMenu = LoadSound("assets/clickMenu.wav");
    selectMenu = LoadSound("assets/selectMenu.wav");

    assets->settingsBackground = LoadTexture("assets/bgMenuScreen.png");
    assets->btnOn = LoadTexture("assets/btnOn.png");
    assets->btnOff = LoadTexture("assets/btnOff.png");
    assets->btnBack = LoadTexture("assets/btnBack.png");
    assets->bgMusic = LoadMusicStream("assets/background_music.mp3");

    PlayMusicStream(assets->bgMusic);
    SetMusicVolume(assets->bgMusic, 1.0f);
    assets->isMusicOn = true;

}

void UnloadAssetss(Assets *assets) {
    UnloadTexture(assets->bg);
  

    UnloadTexture(assets->btnPlay);
    UnloadTexture(assets->btnMenu);
    UnloadTexture(assets->btnExit);

    UnloadTexture(assets->btnOn);
    UnloadTexture(assets->btnOff);
    UnloadTexture(assets->btnBack);

    UnloadTexture(assets->settingsBackground);

    UnloadMusicStream(assets->bgMusic);
    CloseAudioDevice();
}

//mengupdate tampilan menu utama, proses input, dan menggambar menu.
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

    BeginDrawing();
    ClearBackground(BLACK);

    DrawTexture(assets->bg, 0, 0, WHITE);

    float scaleBtn = 2.0f;
    int scaledWidth = (int)(assets->btnPlay.width * scaleBtn);
    int scaledHeight = (int)(assets->btnPlay.height * scaleBtn);
    int spacing = 0; 

    int buttonX = SCREEN_WIDTH / 2 - scaledWidth / 2;
    int buttonY = 550; 

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
    varRestart();
    togglePause();
    if (!getPauseState())
    {
        game();
        if (InfoPlayer.nyawa > 0) {
            mainMenu(&gameStart);
        }
    }
    else
    {
        gamePaused();
    }
    
    EndDrawing();
}

//menginisialisasi menu pengaturan, termasuk linked list opsi menu.
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

///Fungsi ini menambahkan opsi ke dalam linked list menu pengaturan.
void addOption(SettingsMenu *menu, const char *option) {
    Node *newNode = (Node*)malloc(sizeof(Node));  // Alokasikan memori untuk node baru
    newNode->option = option;                      // Simpan pilihan dalam node
    newNode->next = NULL;                          // Node terakhir menunjuk ke NULL

    if (menu->head == NULL) {
        menu->head = newNode;  // Jika linked list kosong, node ini jadi head
    } else {
        Node *current = menu->head;
        while (current->next != NULL) {
            current = current->next;  // Cari node terakhir
        }
        current->next = newNode;  // Tambahkan node baru di akhir
    }
}

// Fungsi ini mengprint pilihan menu yang ada dalam linked list.
void printMenuOptions(SettingsMenu *menu) {
    Node *current = menu->head;
    while (current != NULL) {
        printf("%s\n", current->option);  // print pilihan menu
        current = current->next;          // Pindah ke node berikutnya
    }
}

// Fungsi ini menghapus semua opsi yang ada dalam linked list menu pengaturan.
void clearMenuOptions(SettingsMenu *menu) {
    Node *current = menu->head;
    while (current != NULL) {
        Node *next = current->next;  // Simpan pointer ke node berikutnya
        free(current);               // Hapus node saat ini
        current = next;              // Pindah ke node berikutnya
    }
    menu->head = NULL;  // Set head ke NULL
}

void DrawSettingsMenu(SettingsMenu *menu) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    // Tampilkan opsi menu dari linked list
    Node *current = menu->head;
    int yOffset = 200;
    while (current != NULL) {
        Color color = (menu->selectedOption == 0) ? YELLOW : WHITE;
        DrawText(current->option, SCREEN_WIDTH / 2 - 50, yOffset, 20, color);
        yOffset += 40;
        current = current->next;
    }
    
    EndDrawing();
}

void UpdateSettingsMenu(SettingsMenu *menu, Assets *assets) {
    if (IsKeyPressed(KEY_DOWN)) {
        menu->selectedOption = (menu->selectedOption + 1) % 3;
    }
    if (IsKeyPressed(KEY_UP)) {
        menu->selectedOption = (menu->selectedOption - 1 + 3) % 3;
    }

    if (menu->selectedOption == 2 && IsKeyPressed(KEY_ENTER)) {
    
    }
}


void UpdateSettingsScreen(GameScreen *currentScreen, SettingsMenu *menu, Assets *assets) {
    if (IsKeyPressed(KEY_DOWN))
    {
        PlaySound(selectMenu);
        menu->selectedOption = (menu->selectedOption + 1) % 3;
    }
    if (IsKeyPressed(KEY_UP))
    {
        PlaySound(selectMenu);
        menu->selectedOption = (menu->selectedOption - 1 + 3) % 3;
    }

    // Pilihan menu
    if (IsKeyPressed(KEY_ENTER))
    {
        PlaySound(clickMenu);
        if (menu->selectedOption == 0)
        {
            assets->isMusicOn = true;
            PlayMusicStream(assets->bgMusic);
        }
        else if (menu->selectedOption == 1)
        {
            assets->isMusicOn = false;
            StopMusicStream(assets->bgMusic);
        }
        else if (menu->selectedOption == 2)
        {
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

    //  tombol ON dan OFF sejajar
    int btnSpacing = 20; // Jarak antara ON dan OFF
    int btnOnX = (SCREEN_WIDTH / 2) - (assets->btnOn.width * 1.5) - (btnSpacing / 2);
    int btnOffX = (SCREEN_WIDTH / 2) + (btnSpacing / 2);

    DrawTextureEx(assets->btnOn, (Vector2){btnOnX, 180}, 0.0f, 1.5f, onColor);
    DrawTextureEx(assets->btnOff, (Vector2){btnOffX, 180}, 0.0f, 1.5f, offColor);

    DrawTextureEx(assets->btnBack, (Vector2){centerX, 240}, 0.0f, 1.5f, backColor);

    EndDrawing();
}

void menuSuci(){
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
        }
        else if (currentScreen == SETTINGS)
        {
            UpdateSettingsScreen(&currentScreen, &settingsMenu, &assets);
        }
        else if (currentScreen == EXIT)
        {
            isRunning = false;
            UpdateSettingsScreen(&currentScreen, &settingsMenu, &assets);
        }
    }
    UnloadAssetss(&assets);
}
