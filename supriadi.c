#include "supriadi.h"
#include "hasbi.h"
#include "stdlib.h"
#include <stdio.h>

infoPlayer InfoPlayer;
PowerUp powerup;
sparkle Sparkles;


bool ambilpowerup;

void infokanPlayer() {
    InfoPlayer.nyawa = NYAWA_AWAL;
    InfoPlayer.score = 0;
    InfoPlayer.nyawaIMG = LoadTexture("asset-menu/1.png");
}

void updateNyawa() {
    InfoPlayer.nyawa -= 1;
}

void updateScore() {
    InfoPlayer.score++;
}

void tampilNyawa() {
    ClearBackground(RAYWHITE);
    int nyawanya = 0 ;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++){
            if ((i * 5 + j) < InfoPlayer.nyawa) {
            Vector2 posisi = {531 + j * (InfoPlayer.nyawaIMG.width * 0.02f), 800 + i * (InfoPlayer.nyawaIMG.height * 0.02f)};
            DrawTextureEx(InfoPlayer.nyawaIMG, posisi, 0.0f, 0.02f, WHITE);
            }
        }
    }
    DrawText(TextFormat("Nyawa: %d", InfoPlayer.nyawa), GAMEPLAY_WIDTH + MENU_WIDTH / 2 - 85, 770, 25, WHITE);

}

void Tampil_Score(){
    DrawText(TextFormat("Score: %d", InfoPlayer.score), GAMEPLAY_WIDTH + MENU_WIDTH / 2 - 85, 140, 25, RAYWHITE);
}

void gameover(){
    ClearBackground(BLACK);
    DrawText("Game Over", (GAMEPLAY_WIDTH+MENU_WIDTH) / 2 - 120, SCREEN_HEIGHT / 2, 50, RAYWHITE);
    DrawText("Press R to Restart", (GAMEPLAY_WIDTH+MENU_WIDTH) / 2 - 150, 600, 30, RAYWHITE);
    if (IsKeyPressed(KEY_R)){
        InfoPlayer.nyawa = NYAWA_AWAL;
        InfoPlayer.score = 0;
        InitPlayer();
        InitEnemies();
        InitBullets();

    }
}

void inipowerup(){
    spawnPowerUp();
    tampilPowerUp();
    checkPowerUpCollision();
}

void infoPowerUp() {
    powerup.powerupIMG = LoadTexture("assets/powerup.png");
    powerup.active = false;
    Sparkles.sparkIMG = LoadTexture("assets/sparkel1.png");
    Sparkles.aktif = false;
}

void spawnPowerUp() {
    if (InfoPlayer.score % 10 == 0) {
        if (!powerup.active && InfoPlayer.score > 0 && !ambilpowerup) {
            powerup.active = true;
            powerup.posisi.x = GetRandomValue(20, GAMEPLAY_WIDTH - 100);
            powerup.posisi.y = GetRandomValue(400, SCREEN_HEIGHT - 100);
        }
    }else{
        ambilpowerup = false;
    }
}

void tampilPowerUp() {
    if (powerup.active) {
        DrawTextureEx(powerup.powerupIMG, powerup.posisi, 0.0f, 0.1f, WHITE);
    }
}

void ShowSpark(Vector2 position){
    Sparkles.aktif = true;
    Sparkles.timer = 0.0f;    
    Sparkles.PosisiSpark = position;
}

void UpdateSpark() {
    if (Sparkles.aktif) {
        Sparkles.timer += GetFrameTime();
        if (Sparkles.timer >= 1.0f) {
            Sparkles.aktif = false;
        }
    }
}

void tampilspark() {
    if (Sparkles.aktif) {
        DrawTextureEx(Sparkles.sparkIMG, Sparkles.PosisiSpark, 0.0f, 1.0f, WHITE);
    }
}

void checkPowerUpCollision(){
    Vector2 playerPosition = (Vector2){player.position.x + 185, player.position.y + 150};
    if (powerup.active && CheckCollisionCircles(playerPosition, 30, powerup.posisi, 30)){
        InfoPlayer.nyawa = InfoPlayer.nyawa + 3;
        powerup.active = false;
        ambilpowerup = true;
        ShowSpark(powerup.posisi);
    }
}

void unloadResources() {
    UnloadTexture(InfoPlayer.nyawaIMG);
    UnloadTexture(powerup.powerupIMG);
    UnloadTexture(Sparkles.sparkIMG);
}