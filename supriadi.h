#ifndef SUPRIADI_H
#define SUPRIADI_H

#include "raylib.h"
#include "hasbi.h"

#define NYAWA_AWAL 15
#define MAX_POWERUPS 5

// Struktur untuk menyimpan data pemain
typedef struct {
    int nyawa;
    int score;
    Texture2D nyawaIMG;
}infoPlayer;

// Struktur untuk menyimpan data power-up
typedef struct {
    Vector2 posisi;
    Texture2D powerupIMG;
    bool active;
} PowerUp;

// struktur untuk memunculkan animasi ketika mendapat power up
typedef struct{
    Vector2 PosisiSpark;
    bool aktif;
    float timer;
    Texture2D sparkIMG;
} sparkle;

extern sparkle Sparkles;
extern infoPlayer InfoPlayer;
extern PowerUp powerup;
extern int playerInvincible;

void tampilNyawa();
void infokanPlayer();
void Tampil_Score();
void gameover();
void infoPowerUp();
void spawnPowerUp();
void tampilPowerUp();
void unloadResources();
void updateScore();
void updateNyawa(GameState *S);
void checkPowerUpCollision();
void inipowerup();
void UpdateSpark();
void ShowSpark(Vector2 position);
void tampilspark();
void ResetSpark();

#endif