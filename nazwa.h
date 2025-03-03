#ifndef NAZWA_H
#define NAZWA_H

#include "raylib.h"
extern bool isPaused;

void mainMenu(bool *gameStart);
void varQuit();
void togglePause();
bool getPauseState();
void gamePaused();
void loadAssetMenu();
void unloadAssetMenu();

// typedef struct {
//     int score;
//     Vector2 playerPos;
// } GameState;

// void varRestart(GameState *game);

#endif