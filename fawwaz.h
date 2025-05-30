#include "raylib.h"
#include "hasbi.h"

#define BOSS_SPEED 2
#define BOSS_MAX_BULLETS 10
#define BOSS_BULLET_SPEED 8
#define MAX_STARS 400
#define MAX_ENEMY_BULLETS 20

// Bosses
typedef struct
{
    Vector2 position;
    Texture2D texture;
    int health;
    int maxHealth;
    int hitEffectFrame; // 0 atau 1, untuk efek tembakan
    float hitEffectTimer;
    float destroyTime;
    bool aktif;
    bool bossRage;
    bool defeat;
    bool destroyed;
    bool theEnd;
} Bosses;

typedef struct
{
    Vector2 position;
    float speed;
    int size;
} Star;

typedef struct StarNode {
    Star data;
    struct StarNode* next;
} StarNode;

extern StarNode* starHead;

typedef struct EnemyBulletNode* PNodeEB;


typedef struct
{
    Vector2 position;
    bool active;
    float timer;
    float cooldown;
    float animationTimer;
    int currentFrame; // Untuk menyimpan frame animasi laser
    float length;
    Texture2D textures[3];
} BossLaser;

extern Bosses bosses;
extern BossLaser bossLaser;
extern Sound laserSound;
extern Music bossbgm;
extern Sound nging, duar;

void InitBosses();
void DrawBosses(GameState *S);
void BossBar();
void BossRage(GameState *S);
void BossMov();
void ShootBossLaser();
void UpdateBossLaser();
void DrawBossLaser();
void BossExplosions();
void CheckBossCollisions(GameState *S);
void InitStar();
void DrawStar();
void UpdateStar();
void InitBGM();
void UpdateBGM();
void UnloadBGM();
void InitialBoss();
void FreeStars();

void InitEnemyBullets();
void FreeEnemyBullets();