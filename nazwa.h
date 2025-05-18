#ifndef NAZWA_H
#define NAZWA_H
#define MAX_INPUT_CHARS 20

#include "raylib.h"
extern bool isPaused;
extern bool gameStart;



void mainMenu(bool *gameStart);
void varMenu(bool *soundAssets);
void varQuit();
void togglePause();
bool getPauseState();
void gamePaused();
void loadAssetMenu();
void loadMusic();
void unloadAssetMenu();
void level2(float deltaTime);


#endif