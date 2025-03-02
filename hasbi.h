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
#define MAX_BULLETS 5


typedef struct {
    Vector2 position;
    Texture2D texture;
} Player;


typedef struct {
    Vector2 position;
    bool active;
} Bullet;

void UpdateShooting(float deltaTime);
void InitPlayer(); //posisi userplane pertama kali muncul
void InitBullets(); 
void UpdatePlayer();
void ShootBullet();
void UpdateBullets();
void DrawGameplay();
void UnloadAssets();


//ASTEROIDS
#define MAX_ASTEROIDS 0
typedef struct {
    Texture2D texture;
    Vector2 position; //posisi asteroid
    Vector2 speed; //kecepatan asteroid
    int size;  // 1 = small, 2 = medium, 3 = big
    int health; //jumlah hit yang diperlukan untuk menghancurkan asteroid
    bool active; //status asteroid aktif
    int hitEffectFrame; // 0 atau 1, untuk efek tembakan
    float hitEffectTimer; // Timer untuk durasi efek
} Asteroid;
void InitAsteroids();
void CheckCollisions();
void AsteroidLoop();

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


//ENEMY
#define SPEED_ENEMY_BULLETS 2
#define MAX_ENEMIES 8
#define MAX_ENEMY_BULLETS 3


// Struktur untuk musuh
typedef struct {
    Vector2 position;
    Vector2 speed;
    bool isActive;
    bool canShoot;
    bool hasShot;
} Enemy;

// Struktur untuk peluru musuh
typedef struct {
    Vector2 position;
    bool isActive;
    Vector2 speed;
    int shooterIndex; // Menyimpan indeks musuh yang menembakkan peluru
} EnemyBullet;

void InitEnemies();
void UpdateEnemies();
void UpdateEnemyBullets();
void CheckEnemyCollisions();
void EnemiesLoop();


#endif
