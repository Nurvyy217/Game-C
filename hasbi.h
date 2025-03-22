#ifndef HASBI_H
#define HASBI_H

#include <stdlib.h>
#include "raylib.h"

/***************************************** DEFINE ************************************************/
//LOADING SCREEN
#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 960
#define GAMEPLAY_WIDTH (SCREEN_WIDTH * 5 / 7)
#define MENU_WIDTH (SCREEN_WIDTH * 2 /7)
//USER PLANE AND BULLET
#define BULLET_SPEED 7
#define PLAYER_SPEED 4
#define BULLET_SPEED 7
#define MAX_BULLETS 10
//ASTEROIDS
#define MAX_ASTEROIDS 8
//EXPLOSION
#define MAX_EXPLOSIONS 50
//ENEMY
#define SPEED_ENEMY_BULLETS 3
#define MAX_ENEMIES 10
#define MAX_ENEMY_BULLETS 20
// SETTER GETTER
#define maxEnemy(S) (S)->maxEnemy
#define enemyBulletSpeed(S) (S)->enemyBulletSpeed
#define maxEnemyBullet(S) (S)->maxEnemyBullet
#define enemyTypeShoot(S) (S)->enemyTypeShoot
#define enemySpeed(S) (S)->enemySpeed
#define enemyHealth(S) (S)->enemyHealth
#define healthBroke(S) (S)->healthBroke
#define enemyDamage(S) (S)->enemyDamage


/***************************************** STRUCT ************************************************/
// USER PLANE
typedef struct {
    Vector2 position;
    Texture2D texture;
} Player;
// USER BULLET
typedef struct {
    Vector2 position;
    bool active;
} Bullet;
// ASTEROID
typedef struct {
    Texture2D texture;
    Vector2 position; //posisi asteroid
    Vector2 speed; //kecepatan asteroid
    int size;  // 1 = small, 2 = medium, 3 = big
    bool active; //status asteroid aktif
    int hitEffectFrame; // 0 atau 1, untuk efek tembakan
    float hitEffectTimer; // Timer untuk durasi efek
} Asteroid;
// EXPLOSIONS EFFECT
typedef struct Explosion {
    Vector2 position;
    bool active;
    int frame;   // Frame animasi ledakan
    float timer; // Untuk mengatur kecepatan animasi
} Explosion;
// ENEMY
typedef struct {
    Vector2 position;
    Vector2 speed;
    bool isActive;
    bool canShoot;
    bool hasShot;
    int health; 
    int hitEffectFrame; // 0 atau 1, untuk efek tembakan
    float hitEffectTimer; // Timer untuk durasi efek
} Enemy;
// ENEMY BULLET
typedef struct {
    Vector2 position;
    bool isActive;
    Vector2 speed;
    int shooterIndex; // Menyimpan indeks musuh yang menembakkan peluru
    bool hasPlayedSound;
    bool hasPlayedDuar;
} EnemyBullet;
typedef struct {
    int maxEnemy;
    int enemyBulletSpeed;
    int maxEnemyBullet;
    int enemyTypeShoot;
    int enemySpeed;
    int enemyHealth;
    int healthBroke;
    int enemyDamage;
}GameState;

/***************************************** EXTERN **********************************************/
// EXTERN
extern Bullet bullets[MAX_BULLETS];
extern Sound shootSound;
extern Player player;
extern GameState *state;
extern GameState gamestate;
extern int level;
extern bool isLoadingDone; 
extern int playerHealth;
extern Texture2D explosionsTexture;


/***************************************** MODUL MODUL ****************************************/
// LAYOUT
void unloadTextures();
void loadingAnimation();
void DrawLayout();
// USER PLANE
void DrawPlayer();
void DrawBullets();
void UpdateShooting(float deltaTime);
void InitPlayer(); 
void InitBullets(); 
void UpdatePlayer();
void ShootBullet();
void UpdateBullets();
void UnloadAssets();
void level1(float deltaTime);
// ASTEROID
void InitAsteroids();
void CheckCollisions();
void AsteroidLoop();
// EXPLOSIONS
void DrawExplosions(Texture2D explosionsTexture);
void UpdateExplosions(float deltaTime);
void LoadAssets();
// ENEMY
void initGameState(GameState *S);
void UpdateEnemies(Texture2D EnemyTexture, int xBounceEnemyRight, int xBounceEnemyLeft, int yPositionBullet, int xPositionBullet, GameState *S);
void UpdateEnemyBullets(Texture2D enemyBulletTexture, GameState *S);
void DrawEnemyBullets(Texture2D enemyBulletTexture, float scale, GameState *S);
void CheckEnemyCollisions(int xEnemy, int yEnemy, int radiusPlayer, int radiusBulletEnemy, GameState *S);
void EnemiesLoop(Texture2D EnemyTexture, int xBounceEnemyRight, int xBounceEnemyLeft, int yPositionBullet, int xPositionBullet, GameState *S);
void DrawEnemies(Texture2D EnemyImage, Texture2D EnemyDamaged, float scale, int xEffect, int yEffect, GameState *S);
// GAME
void game();
// SETTER 
void setMaxEnemy(GameState *S, int value);
void setMaxEnemyBullet(GameState *S, int value);
void setEnemyBulletSpeed(GameState *S, int value);
void setEnemyTypeShoot(GameState *S, int value);
void setEnemySpeed(GameState *S, int value);
void setEnemyHealth(GameState *S, int value);
void setHealthBroke(GameState *S, int value);
void setEnemyDamage(GameState *S, int value);
//GETTER
int getEnemyDamage(GameState *S);
#endif
