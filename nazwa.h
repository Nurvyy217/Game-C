#ifndef NAZWA_H
#define NAZWA_H
#define MAX_INPUT_CHARS 20

#include "raylib.h"
extern bool isPaused;

void mainMenu(bool *gameStart);
void varQuit();
void togglePause();
bool getPauseState();
void gamePaused();
void loadAssetMenu();
void unloadAssetMenu();
void DrawPlayerLvl6();
void DrawLvl6();
void level6(float deltaTime);
void DrawUfo();
void AsteroidLooping();
void SpawnAsteroid2();
void UpdateAsteroids2();
void CheckEnemyCollision();
void EnemyShooting();
void SpawnEnemy();
void UpdateEnemy();
void EnemiesLooping();

void UpdateExplosion(float deltaTime);
void DrawExplosion();

typedef struct {
    char name[MAX_INPUT_CHARS + 1];
} user;

void inputName(user *U);

#endif