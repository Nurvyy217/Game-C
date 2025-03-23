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
    SETTINGS
} GameScreen;

typedef enum MenuOption
{
    MENU_PLAY,
    MENU_SETTINGS,
    MENU_EXIT,
    MENU_TOTAL // Jumlah total opsi menu
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
} Assets;

// Deklarasi fungsi
void menuSuci();

#endif
