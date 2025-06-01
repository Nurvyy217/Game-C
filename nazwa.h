#ifndef NAZWA_H
#define NAZWA_H
#define MAX_INPUT_CHARS 20
#define BULLET_SPEED 7
#define MAX_BULLETS 10
// EXPLOSION
#define MAX_EXPLOSIONS 50

#include "raylib.h"
extern bool isPaused;
extern bool gameStart;

// USER BULLET
typedef struct
{
    Vector2 position;
    bool active;
} Bullet;

typedef struct BulletNode
{
    Bullet data;
    struct BulletNode *next;
} BulletNode;

extern BulletNode *BulletHead;

typedef struct Explosion
{
    Vector2 position;
    bool active;
    int frame;   // Frame animasi ledakan
    float timer; // Untuk mengatur kecepatan animasi
} Explosion;

typedef struct ExplosionNode {
    Explosion data;
    struct ExplosionNode* next;
} ExplosionNode;

extern ExplosionNode* ExplosionHead;

//BULLETS
void DrawBullets();
void UpdateShooting(float deltaTime);
void InitBullets();
void ShootBullet();
void UpdateBullets();
void freeBullets();

// EXPLOSIONS
void DrawExplosions(Texture2D explosionsTexture);
void UpdateExplosions(float deltaTime);
void CreateExplosion(Vector2 position);
void freeExplosions();

// RESET
void ResetPlayerBulet();
void ResetExplosions();

void mainMenu(bool *gameStart);
void varMenu(bool *soundAssets);
void varQuit();
void varRestart();
void varSound(bool *gameStart);
void togglePause();
bool getPauseState();
void gamePaused();
void loadAssetMenu();
void loadMusic();
void unloadAssetMenu();
void level2(float deltaTime);


#endif