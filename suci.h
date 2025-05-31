#ifndef SUCI_H
#define SUCI_H

#include "raylib.h"

extern bool texturesLoaded;
void initLoadScreen();
extern Texture2D logoDeveloper;
extern Texture2D gameNamePhoto;
extern Texture2D background;
void initBG();
extern Sound clickMenu;

typedef enum GameScreen
{
    MENU,
    PLAY,
    SETTINGS,
    EXIT
} GameScreen;

typedef enum MenuOption
{
    MENU_PLAY,
    MENU_SETTINGS,
    MENU_EXIT,
    MENU_TOTAL
} MenuOption;

// Struktur untuk menyimpan aset
typedef struct Assets
{
    Texture2D bg;
    Texture2D title;
    Texture2D btnPlay;
    Texture2D btnMenu;
    Texture2D btnExit;

    Music bgMusic;
    Texture2D settingsBackground;
    Texture2D btnOn;
    Texture2D btnOff;
    Texture2D btnBack;
    bool isMusicOn;
} Assets;


typedef struct Node {
    const char *option;   // Menyimpan pilihan menu
    struct Node *next;    // Pointer ke node berikutnya
} Node;

// Struktur  settings menu  linked list
typedef struct {
    Node *head;           // Pointer ke node pertama dalam linked list
    int selectedOption;   // Pilihan menu yang dipilih
    float volume;
    int screenWidth;
    int screenHeight;
} SettingsMenu;

// Deklarasi fungsi
void menuSuci();
void InitAssets(Assets *assets);
void UnloadAssetss(Assets *assets);
void UpdateMenu(Assets *assets, GameScreen *currentScreen);
void UpdatePlayScreen(GameScreen *currentScreen);
void UpdateSettingsScreen(GameScreen *currentScreen, SettingsMenu *menu, Assets *assets);
void InitSettingsMenu(SettingsMenu *menu);
void InitSettingsMenu(SettingsMenu *menu);
void addOption(SettingsMenu *menu, const char *option);  // Fungsi menambah opsi menu ke linked list
void printMenuOptions(SettingsMenu *menu);  // Fungsi menampilkan pilihan menu dari linked list
void clearMenuOptions(SettingsMenu *menu);   // Fungsi menghapus linked list
void UpdateSettingsMenu(SettingsMenu *menu, Assets *assets);
void DrawSettingsMenu(SettingsMenu *menu);


#endif
