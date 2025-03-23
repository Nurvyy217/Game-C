#ifndef NAZWA_H
#define NAZWA_H
#define MAX_INPUT_CHARS 20

#include "raylib.h"
extern bool isPaused;
extern bool gameStart;

void mainMenu(bool *gameStart);
void varQuit();
void togglePause();
bool getPauseState();
void gamePaused();
void varRestart();
void loadAssetMenu();
void unloadAssetMenu();
void level2(float deltaTime);




typedef struct {
    char name[MAX_INPUT_CHARS + 1];
} user;

void inputName(user *U);


#endif