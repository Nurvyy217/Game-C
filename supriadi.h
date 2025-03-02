#ifndef SUPRIADI_H
#define SUPRIADI_H

#include "raylib.h"

#define NYAWA_AWAL 16
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

extern infoPlayer InfoPlayer;
extern PowerUp powerup;

void tampilNyawa();
void infokanPlayer();
void Tampil_Score();
bool gameover();
void restart();
void infoPowerUp();
void spawnPowerUp();
void tampilPowerUp();
void unloadResources();
void updateScore();
void updateNyawa();
void checkPowerUpCollision();



#endif