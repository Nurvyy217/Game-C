#include "raylib.h"
#include "hasbi.h"

#define BOSS_SPEED 2
#define BOSS_MAX_BULLETS 10
#define BOSS_BULLET_SPEED 8
#define MAX_STARS 100

// Bosses
typedef struct {
    Vector2 position;
    Texture2D texture;
    int health;
    int maxHealth;
    int hitEffectFrame; // 0 atau 1, untuk efek tembakan
    float hitEffectTimer; 
    bool aktif;
} Bosses;

typedef struct {
    Vector2 position;
    float speed;
    int size;
} Star;

typedef struct {
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

void InitBosses();
void DrawBosses();
void BossBar();

//
void DrawPositions();
void BossMov();
void ShootBossLaser();
void UpdateBossLaser();
void DrawBossLaser();

void InitStar();
void DrawStar();
void UpdateStar();