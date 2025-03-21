#include "supriadi.h"
#include "hasbi.h"
#include "stdlib.h"
#include <stdio.h>

infoPlayer InfoPlayer;
PowerUp powerup;
sparkle Sparkles;


int AddSpeed;
bool ambilpowerup = false;

void infokanPlayer() {
    InfoPlayer.shieldActive = false;
    InfoPlayer.nyawa = NYAWA_AWAL;
    InfoPlayer.score = 0;
    InfoPlayer.nyawaIMG = LoadTexture("asset-menu/1.png");
}

void updateNyawa(int tambah, int kurang) {

    if (InfoPlayer.nyawa < NYAWA_AWAL)
    {
        InfoPlayer.nyawa += tambah;
        if (InfoPlayer.nyawa > NYAWA_AWAL)
        {
            InfoPlayer.nyawa = NYAWA_AWAL;
        }
        
    }

    if (InfoPlayer.shieldActive == false)
    {
        InfoPlayer.nyawa -= kurang ;    
    }
}

void updateScore(int berapa) {
    if (InfoPlayer.score % 10 != 0) {
        ambilpowerup = false;
    }
    InfoPlayer.score += berapa;
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
    DrawText("Press R to Restart", (GAMEPLAY_WIDTH+MENU_WIDTH) / 2 - 120, 600, 30, RAYWHITE);
    if (IsKeyPressed(KEY_R)){
        InfoPlayer.nyawa = NYAWA_AWAL;
        InfoPlayer.score = 0;
        InitPlayer();
        InitEnemies();
    }
}

void inipowerup(){
    spawnPowerUp();
    tampilPowerUp();
    checkPowerUpCollision();
    UpdateSpark();
    updatePowerupTime();
}

void infoPowerUp() {
    powerup.powerupIMG = LoadTexture("assets/powerup.png");
    powerup.active = false;
    Sparkles.sparkIMG = LoadTexture("assets/sparkel1.png");
    Sparkles.aktif = false;
}

void spawnPowerUp() {
    if (!powerup.active && InfoPlayer.score > 0 && InfoPlayer.score % 10 == 0 && !ambilpowerup) {
        powerup.active = true;
        powerup.posisi.x = GetRandomValue(20, GAMEPLAY_WIDTH - 100);
        powerup.posisi.y = 0;
        powerup.type = GetRandomValue(0 , 3);
    }

    if (powerup.active) {
        powerup.posisi.y += 3.0f;

        if (powerup.posisi.y > SCREEN_HEIGHT) {
            powerup.active = false;
            ambilpowerup = true;
        }
    }
}
    
void tampilPowerUp() {
    if (powerup.active) {
        DrawTextureEx(powerup.powerupIMG, powerup.posisi, 0.0f, 0.1f, WHITE);
    }
}

void checkPowerUpCollision(){
    Vector2 playerPosition = (Vector2){player.position.x + 185, player.position.y + 150};
    if (powerup.active && CheckCollisionCircles(playerPosition, 30, powerup.posisi, 30)){
        powerup.active = false;
        ambilpowerup = true;
        ShowSpark(powerup.posisi);
        
        switch (powerup.type) {

            case POWERUP_LIFE:
                updateNyawa(3,0);
                printf("nyawa ");
            break;
            
            case POWERUP_FASTFIRE:
                printf("FIRE ");
                InfoPlayer.DoubleAttack = true ;
                InfoPlayer.AttackTimer = 3.0f  ;
                printf("%d", InfoPlayer.AttackTimer);
            break;

            case POWERUP_SPEED:                
                printf("speed ");
                InfoPlayer.speedActive = true ;
                InfoPlayer.SpeedTimer = 5.0f;
                printf("%d", InfoPlayer.SpeedTimer);
            break;
            
            case POWERUP_SHIELD:
                printf("shield ");
                InfoPlayer.shieldActive = true ;
                InfoPlayer.shieldTimer = 5.0f  ;
                printf("%d", InfoPlayer.shieldTimer);
            break;
        }
    }
}
    

void updatePowerupTime() {

    // Timer Shield
    if (InfoPlayer.shieldActive) {
        InfoPlayer.shieldTimer -= GetFrameTime();
        
        if (InfoPlayer.shieldTimer <= 0) {
            InfoPlayer.shieldActive = false;
            InfoPlayer.shieldTimer = 0;
            printf("Shield habis!\n");
        }
    }

    // Timer MovementSpeed
    if (InfoPlayer.speedActive) {
        InfoPlayer.SpeedTimer -= GetFrameTime();
        
        if (InfoPlayer.SpeedTimer <= 0) {
            InfoPlayer.speedActive = false;
            InfoPlayer.SpeedTimer = 0;
            printf("speed habis!\n");
        }
        AddSpeed = 5;
    }

    else {
        AddSpeed = 0;
    }

    // Timer powerupAttack
    if (InfoPlayer.DoubleAttack) {
        InfoPlayer.AttackTimer -= GetFrameTime();
        
        if (InfoPlayer.AttackTimer <= 0) {
            InfoPlayer.DoubleAttack = false;
            InfoPlayer.AttackTimer = 0;
            printf("speed habis!\n");
        }
    }
}

void powerupAttack() {
    int bulletmuncul = 0;
    
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullets[i].active)
        {
            if (bulletmuncul == 0) {
                bullets[i].position = (Vector2){(player.position.x - 50) + player.texture.width * 0.6 / 2, (player.position.y + player.texture.width * 0.6 / 2) - 110};
            }
            else if (bulletmuncul == 1) {
                bullets[i].position = (Vector2){(player.position.x + 25) + player.texture.width * 0.6 / 2, (player.position.y + player.texture.width * 0.6 / 2) - 110};
            }
            
            bullets[i].active = true;
            bulletmuncul++;
            
            if (bulletmuncul >= 2) {
                break;  
            }
            PlaySound(shootSound);
        }
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
