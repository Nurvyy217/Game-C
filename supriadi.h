#ifndef SUPRIADI_H
#define SUPRIADI_H

#include "raylib.h"
#include "hasbi.h"

#define NYAWA_AWAL 15
#define MAX_POWERUPS 5

// Struktur untuk menyimpan data pemain
typedef struct
{
    int nyawa;
    int score;
    Texture2D nyawaIMG;

    bool shieldActive;
    float shieldTimer;

    bool speedActive;
    float SpeedTimer;

    bool DoubleAttack;
    float AttackTimer;

    bool AddNyawa;

} infoPlayer;

typedef enum
{
    POWERUP_LIFE,
    POWERUP_FASTFIRE,
    // POWERUP_SCORE,
    POWERUP_SPEED,
    POWERUP_SHIELD,
} PowerUpType;

typedef struct
{
    Vector2 posisi;
    Texture2D powerupIMG;
    bool active;
    PowerUpType type;
    float SpawnTime;

} PowerUp;

typedef struct
{
    Vector2 PosisiSpark;
    bool aktif;
    float timer;
    Texture2D sparkIMG;
} sparkle;

extern sparkle Sparkles;
extern infoPlayer InfoPlayer;
extern PowerUp powerup;
extern int AddSpeed;
extern int playerInvincible;

void tampilNyawa();
void infokanPlayer();
void Tampil_Score();
void gameover();
void infoPowerUp();
void spawnPowerUp();
void SpawnPowerUpTime();
void tampilPowerUp();
void updateScore(int berapa);
void updateNyawa(GameState *S);
void checkPowerUpCollision();
void inipowerup();
void UpdateSpark();
void ShowSpark(Vector2 position);
void tampilspark();
void updatePowerupTime();
void powerupAttack();
void ResetSpark();
void TampilInfoPowerup();
void ShowPowerupShield();
void ShowWingDoubleAttack();

#endif