#ifndef HASBI_H
#define HASBI_H

#include <stdlib.h>
#include "raylib.h"


//LOADING SCREEN
#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 960
#define GAMEPLAY_WIDTH (SCREEN_WIDTH * 5 / 7)
#define MENU_WIDTH (SCREEN_WIDTH * 2 /7)

extern bool isLoadingDone; 
void initTextures();
void unloadTextures();
void loadingAnimation();
void DrawLayout();


//USER PLANE AND BULLET
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

void InitPlayer(); //posisi userplane pertama kali muncul
void InitBullets(); 
void UpdatePlayer();
void ShootBullet();
void UpdateBullets();
void DrawGameplay();
void UnloadPlayer();


//ASTEROIDS
#define MAX_ASTEROIDS 4
extern int playerHealth;
typedef struct {
    Texture2D texture;
    Vector2 position; //posisi asteroid
    Vector2 speed; //kecepatan asteroid
    int size;  // 1 = small, 2 = medium, 3 = big
    int health; //jumlah hit yang diperlukan untuk menghancurkan asteroid
    bool active; //status asteroid aktif
} Asteroid;
void InitAsteroids();
void CheckCollisions();
void GameLoop();

//EXPLOSION
#define MAX_EXPLOSIONS 50

typedef struct Explosion {
    Vector2 position;
    bool active;
    int frame;   // Frame animasi ledakan
    float timer; // Untuk mengatur kecepatan animasi
} Explosion;
void UpdateExplosions(float deltaTime);
void LoadAssets();

#endif
