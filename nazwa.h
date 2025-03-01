#ifndef NAZWA_H
#define NAZWA_H

#include "raylib.h"

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 960
#define GAMEPLAY_WIDTH (SCREEN_WIDTH * 5 / 7)
#define MENU_WIDTH (SCREEN_WIDTH * 2 /7)

void mainMenu(bool *gameStart);
void varSound(bool *isSoundOn, Texture2D soundOnTexture, Texture2D soundOffTexture);
void varHeart(Texture2D heartTexture);
void varMenu(Texture2D menuTexture, bool *isSoundOn, Texture2D soundOnTexture, Texture2D soundOffTexture, 
    Texture2D restartTexture, Texture2D pauseTexture, Texture2D quitTexture);
void varQuit();
void varPause();
void varRestart();

#endif