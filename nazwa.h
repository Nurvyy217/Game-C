#ifndef NAZWA_H
#define NAZWA_H

#include "raylib.h"
extern bool isPaused;

void mainMenu(bool *gameStart);
void varQuit();
void togglePause();
bool getPauseState();
void gamePaused();
void varRestart();
void loadAssetMenu();
void unloadAssetMenu();


#endif