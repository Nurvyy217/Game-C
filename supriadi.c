#include "supriadi.h"
#include "hasbi.h"
#include "stdlib.h"
#include <stdio.h>

infoPlayer InfoPlayer;
Player pemain;
PowerUp powerup;



void infokanPlayer() {
    InfoPlayer.nyawa = NYAWA_AWAL;
    InfoPlayer.score = 0;
    InfoPlayer.nyawaIMG = LoadTexture("nyawa.png");
}

void updateNyawa() {
    InfoPlayer.nyawa -= 1;
}

void updateScore() {
    InfoPlayer.score += 10;
}

void tampilNyawa() {
    ClearBackground(RAYWHITE);
    for (int i = 0; i < InfoPlayer.nyawa; i++) {
        Vector2 posisi = {20 + i * (InfoPlayer.nyawaIMG.width * 0.02f), 20};
        DrawTextureEx(InfoPlayer.nyawaIMG, posisi, 0.0f, 0.02f, WHITE);
    }
    DrawText(TextFormat("Nyawa: %d", InfoPlayer.nyawa), 20, 50, 20, RED);
}

void Tampil_Score(){
    DrawText(TextFormat("Score: %d", InfoPlayer.score), 560, 575, 30, RAYWHITE);
}

bool gameover(){
    return (InfoPlayer.nyawa == 0)? false : true;
}

void restart(){
    DrawText("Press R to Restart", 300, 300, 20, BLACK);
    if (IsKeyPressed(KEY_R)){
        InfoPlayer.nyawa = NYAWA_AWAL;
        InfoPlayer.score = 0;
    }    
}

void infoPowerUp() {
    powerup.powerupIMG = LoadTexture("assets/powerup.png");
    powerup.active = false;
}

void spawnPowerUp() {
    if (InfoPlayer.score % 100 == 0 && !powerup.active && InfoPlayer.score > 0) {
        powerup.active = true;
        powerup.posisi.x = GetRandomValue(20, GAMEPLAY_WIDTH - 100);
        powerup.posisi.y = GetRandomValue(20, SCREEN_HEIGHT - 100);
    }
}

void tampilPowerUp() {
    if (powerup.active) {
        DrawTextureEx(powerup.powerupIMG, powerup.posisi, 0.0f, 0.1f, WHITE);
    }
}

void checkPowerUpCollision(){
    Vector2 playerPosition = (Vector2){pemain.position.x + 185, pemain.position.y + 150};
    if (CheckCollisionCircles(playerPosition, 30, powerup.posisi, 30)){
        powerup.active = false;
        InfoPlayer.nyawa = InfoPlayer.nyawa + 10;
    }
}

void unloadResources() {
    UnloadTexture(InfoPlayer.nyawaIMG);
    UnloadTexture(powerup.powerupIMG);
}