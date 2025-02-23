#ifndef HASBI_H
#define HASBI_H

#include "raylib.h"

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 960
#define GAMEPLAY_WIDTH (SCREEN_WIDTH * 5 / 7)
#define MENU_WIDTH (SCREEN_WIDTH * 2 /7)

extern bool isLoadingDone; 
void initTextures();
void unloadTextures();
void loadingAnimation();
void DrawLayout();


#define PLAYER_SPEED 5
#define BULLET_SPEED 7
#define MAX_BULLETS 10

typedef struct {
    Vector2 position;
    Texture2D texture;
} Player;

typedef struct {
    Vector2 position;
    bool active;
} Bullet;

void InitPlayer();
void InitBullets();
void UpdatePlayer();
void ShootBullet();
void UpdateBullets();
void DrawGameplay();
void UnloadPlayer();
#endif
