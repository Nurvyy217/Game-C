#ifndef NAZWA_H
#define NAZWA_H
#define MAX_INPUT_CHARS 20

#include "raylib.h"
extern bool isPaused;



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



void UpdateExplosion(float deltaTime);
void DrawExplosion();

typedef struct {
    char name[MAX_INPUT_CHARS + 1];
} user;

void inputName(user *U);

#endif