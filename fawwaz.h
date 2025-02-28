#include "raylib.h"

#define BOSS_SPEED 2
#define BOSS_MAX_BULLETS 10
#define BOSS_BULLET_SPEED 8


// Bosses
typedef struct {
    Vector2 position;
    Texture2D texture;
} Bosses;

// Boss Shoot
typedef struct {
    Vector2 position;
    bool active;
} Bullet;

Bullet bullets[BOSS_MAX_BULLETS]; 



void InitBosses();
void DrawBosses();

//
void DrawPositions();
void BossMov();