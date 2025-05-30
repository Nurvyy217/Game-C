#ifndef HASBI_H
#define HASBI_H

#include <stdlib.h>
#include "raylib.h"

/***************************************** DEFINE ************************************************/
// LOADING SCREEN
#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 960
#define GAMEPLAY_WIDTH (SCREEN_WIDTH * 5 / 7)
#define MENU_WIDTH (SCREEN_WIDTH * 2 / 7)
// USER PLANE AND BULLET
#define PLAYER_SPEED 4

// ASTEROIDS
#define MAX_ASTEROIDS 10

extern Sound enemyDestroyed;
// ENEMY
#define SPEED_ENEMY_BULLETS 4
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
// CHARACTERS CHANGE LEVEL

typedef struct CharNode
{
    char character;
    struct CharNode *next;
} CharNode;


// USER PLANE
typedef struct
{
    Vector2 position;
    Texture2D texture;
} Player;



// ASTEROID
typedef struct
{
    Texture2D texture;
    Vector2 position;     // posisi asteroid
    Vector2 speed;        // kecepatan asteroid
    int size;             // 1 = small, 2 = medium, 3 = big
    bool active;          // status asteroid aktif
    int hitEffectFrame;   // 0 atau 1, untuk efek tembakan
    float hitEffectTimer; // Timer untuk durasi efek
} Asteroid;

typedef struct AsteroidNode
{
    Asteroid data;
    struct AsteroidNode *next;
} AsteroidNode;

// EXPLOSIONS EFFECT


// ENEMY
typedef struct Enemy *address;
typedef struct Enemy {
    Vector2 position;
    Vector2 speed;
    bool isActive;
    bool canShoot;
    bool hasShot;
    int health;
    int hitEffectFrame;   // 0 atau 1, untuk efek tembakan
    float hitEffectTimer; // Timer untuk durasi efek
    address next;
} Enemy;

extern address EnemiesHead;

// ENEMY BULLET
typedef struct
{
    Vector2 position;
    bool isActive;
    Vector2 speed;
    int shooterIndex; // Menyimpan indeks musuh yang menembakkan peluru
    bool hasPlayedSound;
    bool hasPlayedDuar;
    float delayTimer;
} EnemyBullet;

typedef struct EnemyBulletNode *PNodeEB;

extern PNodeEB ebHead;

typedef EnemyBullet infotype;

typedef struct EnemyBulletNode
{
    infotype Eb;
    PNodeEB next;
} EnemyBulletNode;
typedef struct
{
    int maxEnemy;
    int enemyBulletSpeed;
    int maxEnemyBullet;
    int enemyTypeShoot;
    int enemySpeed;
    int enemyHealth;
    int healthBroke;
    int enemyDamage;
    AsteroidNode *asteroidHead;
} GameState;

/***************************************** EXTERN **********************************************/

// EXTERN
// extern Bullet bullets[MAX_BULLETS];
extern Sound shootSound;
extern Player player;
extern GameState *state;
extern GameState gamestate;
extern int level;
extern bool isLoadingDone;
extern int playerHealth;
extern Texture2D explosionsTexture;
extern Texture2D hitEffect1, hitEffect2, gameOver;
// extern Explosion explosions[MAX_EXPLOSIONS];
extern Music gameoverMusic;

/***************************************** MODUL MODUL ****************************************/

// LAYOUT
void loadingAnimation();
void DrawLayout();

// USER PLANE
void DrawPlayer();
void InitPlayer();
void UpdatePlayer();


// ASTEROID
void DrawAsteroids(GameState *S);
void AsteroidLoop(GameState *S);
void SpawnAsteroid(GameState *S);
void CheckCollisionAsteroid(GameState *S);
void UpdateAsteroids(GameState *S);
int CountActiveAsteroids(GameState *S);
void RemoveInactiveAsteroids(GameState *S);



// ENEMY
void InitEnemyBullets();
void InitEnemy();
void UpdateEnemies(Texture2D EnemyTexture, int xBounceEnemyRight, int xBounceEnemyLeft, int yPositionBullet, int xPositionBullet, GameState *S);
void UpdateEnemyBullets(Texture2D enemyBulletTexture, GameState *S);
void SpawnEnemies(GameState *S);
void DrawEnemyBullets(Texture2D enemyBulletTexture, float scale, GameState *S);
void CheckEnemyCollisions(int xEnemy, int yEnemy, int radiusPlayer, int radiusBulletEnemy, GameState *S);
void EnemiesLoop(Texture2D EnemyTexture, int xBounceEnemyRight, int xBounceEnemyLeft, int yPositionBullet, int xPositionBullet, GameState *S);
void DrawEnemies(Texture2D EnemyImage, Texture2D EnemyDamaged, float scale, int xEffect, int yEffect, GameState *S);
void EnemyShoot(Texture2D EnemyTexture, int yPositionBullet, int xPositionBullet, GameState *S);
void FreeEnemy();

// GAME
void game();
void DrawLevelTransition(float deltaTime, CharNode *head);
void GameplayWithoutEnemies();
void callAsteroid(GameState *S);
void DrawLvl1();
void level1(float deltaTime);
void DrawLvl3(GameState *S);
void level3(GameState *S, float deltaTime);
void DrawLvl4();
void level4(float deltaTime);
void DrawLvl5();
void level5(float deltaTime);
void DrawBossLevel();
void bossLevel(float deltaTime);

// SETTER
void initGameState(GameState *S);
void setMaxEnemy(GameState *S, int value);
void setMaxEnemyBullet(GameState *S, int value);
void setEnemyBulletSpeed(GameState *S, int value);
void setEnemyTypeShoot(GameState *S, int value);
void setEnemySpeed(GameState *S, int value);
void setEnemyHealth(GameState *S, int value);
void setHealthBroke(GameState *S, int value);
void setEnemyDamage(GameState *S, int value);

// GETTER
int getMaxEnemy(GameState *S);
int getMaxEnemyBullet(GameState *S);
int getEnemyTypeShoot(GameState *S);
int getEnemySpeed(GameState *S);
int getEnemyHealth(GameState *S);
int getHealthBroke(GameState *S);
int getEnemyDamage(GameState *S);

// RESET
void ResetEnemyBullets();
void ResetEnemies();
void ResetAsteroid(GameState *S);

// LOAD & UNLOAD
void LoadAssets();
void unloadTextures();
void UnloadAssets();

// FREE
void FreeEnemyBullets();

#endif
