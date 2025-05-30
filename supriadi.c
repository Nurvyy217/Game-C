#include "supriadi.h"
#include "hasbi.h"
#include "suci.h"
#include "stdlib.h"
#include <stdio.h>
#include "fawwaz.h"
#include "suci.h"

infoPlayer InfoPlayer;
PowerUp powerup;
sparkle Sparkles;
Sound powerupSound;
Texture2D shield;
Texture2D Wing;

int playerInvincible = 0; // Timer kebal (dalam frame)
int AddSpeed;
float timer = 0;

void infokanPlayer()
{
    InfoPlayer.shieldActive = false;
    InfoPlayer.nyawa = NYAWA_AWAL;
    InfoPlayer.score = 0;
    InfoPlayer.nyawaIMG = LoadTexture("asset-menu/1.png");
}

void updateNyawa(GameState *S)
{
    if (InfoPlayer.AddNyawa)
    {
        InfoPlayer.nyawa += 3;
    }

    else if (!InfoPlayer.shieldActive && playerInvincible <= 0)
    {
        InfoPlayer.nyawa -= getEnemyDamage(S);
        playerInvincible = 60;
    }

    if (InfoPlayer.nyawa > NYAWA_AWAL)
    {
        InfoPlayer.nyawa = NYAWA_AWAL;
    }
}

void updateScore(int berapa)
{
    InfoPlayer.score += berapa;
}

void tampilNyawa()
{
    ClearBackground(RAYWHITE);
    int nyawanya = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if ((i * 5 + j) < InfoPlayer.nyawa)
            {
                Vector2 posisi = {531 + j * (InfoPlayer.nyawaIMG.width * 0.02f), 220 + i * (InfoPlayer.nyawaIMG.height * 0.02f)};
                DrawTextureEx(InfoPlayer.nyawaIMG, posisi, 0.0f, 0.02f, WHITE);
            }
        }
    }
    DrawText(TextFormat("Health: %d", InfoPlayer.nyawa), GAMEPLAY_WIDTH + MENU_WIDTH / 2 - 85, 190, 30, WHITE);
}

void Tampil_Score()
{
    DrawText(TextFormat("Score: %d", InfoPlayer.score), GAMEPLAY_WIDTH + MENU_WIDTH / 2 - 85, 140, 25, RAYWHITE);
}

void gameover(GameState *S)
{
    ClearBackground(BLACK);
    Rectangle source = {0, 0, gameOver.width, gameOver.height}; // Seluruh gambar
    Rectangle dest = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};         // Area gameplay
    Vector2 origin = {0, 0};                                        // Titik acuan (pojok kiri atas)

    DrawTexturePro(gameOver, source, dest, origin, 0.0f, WHITE);
    // DrawText("Game Over", (GAMEPLAY_WIDTH + MENU_WIDTH) / 2 - 140, SCREEN_HEIGHT / 2, 50, RAYWHITE);
    // DrawText("Press R to Restart", (GAMEPLAY_WIDTH + MENU_WIDTH) / 2 - 160, 600, 30, RAYWHITE);
    if (IsKeyPressed(KEY_ENTER))
    {
        PlaySound(clickMenu);
        InfoPlayer.nyawa = NYAWA_AWAL;
        InfoPlayer.score = 0;

        InitPlayer();
        InitBullets();
        ResetPlayerBulet();
        ResetExplosions();
        ResetEnemyBullets();
        ResetEnemies();
        ResetAsteroid(S);
        bosses.aktif = false;
    }
}



void inipowerup()
{
    if (level != 3)
    {

        SpawnPowerUpTime();
    }
    tampilPowerUp();
    checkPowerUpCollision();
    UpdateSpark();
    updatePowerupTime();
    ShowPowerupShield();
    ShowWingDoubleAttack();
    tampilspark();
}

void infoPowerUp()
{
    powerup.powerupIMG = LoadTexture("assets/powerup.png");
    shield = LoadTexture("assets/shield.png");
    Wing = LoadTexture("assets/wing.png");
    powerupSound = LoadSound("assets/powerup.wav");
    powerup.active = false;
    Sparkles.sparkIMG = LoadTexture("assets/sparkel1.png");
    Sparkles.aktif = false;
    powerup.SpawnTime = 0;
}

void SpawnPowerUpTime()
{
    powerup.SpawnTime += GetFrameTime();
    if (powerup.SpawnTime >= 8)
    {
        spawnPowerUp();
    }
}

void spawnPowerUp()
{
    if (!powerup.active)
    {
        powerup.active = true;
        powerup.posisi.x = GetRandomValue(20, GAMEPLAY_WIDTH - 100);
        powerup.posisi.y = 0;
        powerup.type = GetRandomValue(0, 3);
    }

    if (powerup.active)
    {
        powerup.posisi.y += 3.0f;

        if (powerup.posisi.y > SCREEN_HEIGHT)
        {
            powerup.active = false;
            powerup.SpawnTime = 0;
        }
    }
}

void tampilPowerUp()
{
    if (powerup.active)
    {
        DrawTextureEx(powerup.powerupIMG, powerup.posisi, 0.0f, 0.15f, WHITE);
    }
    GetTime();
}

void checkPowerUpCollision()
{
    GameState *S = &gamestate;
    Vector2 playerPosition = (Vector2){player.position.x + 185, player.position.y + 150};
    if (powerup.active && CheckCollisionCircles(playerPosition, 30, powerup.posisi, 30))
    {
        updateScore(5);
        PlaySound(powerupSound);
        powerup.active = false;
        powerup.SpawnTime = 0;
        ShowSpark(powerup.posisi);

        switch (powerup.type)
        {
        case POWERUP_LIFE:
            InfoPlayer.AddNyawa = true;
            updateNyawa(S);
            break;

        case POWERUP_FASTFIRE:
            InfoPlayer.DoubleAttack = true;
            InfoPlayer.AttackTimer = 5.0f;
            break;

        case POWERUP_SPEED:
            InfoPlayer.speedActive = true;
            InfoPlayer.SpeedTimer = 5.0f;
            break;

        case POWERUP_SHIELD:
            InfoPlayer.shieldActive = true;
            InfoPlayer.shieldTimer = 5.0f;
            break;
        }
    }
}

void updatePowerupTime()
{
    // Timer Shield
    if (InfoPlayer.shieldActive)
    {
        InfoPlayer.shieldTimer -= GetFrameTime();

        if (InfoPlayer.shieldTimer <= 0)
        {
            InfoPlayer.shieldActive = false;
            InfoPlayer.shieldTimer = 0;
        }
    }

    if (InfoPlayer.AddNyawa)
    {
        timer += GetFrameTime();

        if (timer >= 1)
        {
            InfoPlayer.AddNyawa = false;
            timer = 0;
        }
    }

    // Timer MovementSpeed
    if (InfoPlayer.speedActive)
    {
        InfoPlayer.SpeedTimer -= GetFrameTime();

        if (InfoPlayer.SpeedTimer <= 0)
        {
            InfoPlayer.speedActive = false;
            InfoPlayer.SpeedTimer = 0;
        }
        AddSpeed = 5;
    }

    else
    {
        AddSpeed = 0;
    }

    // Timer powerupAttack
    if (InfoPlayer.DoubleAttack)
    {
        InfoPlayer.AttackTimer -= GetFrameTime();

        if (InfoPlayer.AttackTimer <= 0)
        {
            InfoPlayer.DoubleAttack = false;
            InfoPlayer.AttackTimer = 0;
        }
    }
}

void powerupAttack()
{
    int bulletmuncul = 0;
    int maxBullets = InfoPlayer.DoubleAttack ? 10 : 5;
    int currentActiveBullets = 0;

    BulletNode* current = BulletHead;

    // Hitung bullet yang aktif
    while (current != NULL)
    {
        if (current->data.active)
            currentActiveBullets++;
        current = current->next;
    }

    // Jika sudah mencapai batas, keluar
    if (currentActiveBullets >= maxBullets)
        return;

    current = BulletHead;

    while (current != NULL)
    {
        if (!current->data.active)
        {
            if (bulletmuncul == 0)
            {
                current->data.position = (Vector2){(player.position.x - 65) + player.texture.width * 0.6f / 2, (player.position.y + player.texture.width * 0.6f / 2) - 110};
            }
            else if (bulletmuncul == 1)
            {
                current->data.position = (Vector2){(player.position.x + 20) + player.texture.width * 0.6f / 2, (player.position.y + player.texture.width * 0.6f / 2) - 110};
            }

            current->data.active = true;
            PlaySound(shootSound);
            bulletmuncul++;
            currentActiveBullets++;

            if (bulletmuncul >= 2 || currentActiveBullets >= maxBullets)
                break;
        }
        current = current->next;
    }
}


void ShowPowerupShield()
{
    Vector2 playerPosition = (Vector2){player.position.x + 153, player.position.y + 130};
    if (InfoPlayer.shieldActive)
    {
        DrawTextureEx(shield, playerPosition, 0.0f, 0.4f, WHITE);
    }
}

void ShowWingDoubleAttack()
{
    Vector2 playerPosition = (Vector2){player.position.x + 153, player.position.y + 130};
    if (InfoPlayer.DoubleAttack)
    {
        DrawTextureEx(Wing, playerPosition, 0.0f, 0.4f, WHITE);
    }
}

void TampilInfoPowerup()
{
    DrawText(TextFormat("Powerup: "), GAMEPLAY_WIDTH + MENU_WIDTH / 2 - 85, 350, 30, WHITE);

    if (InfoPlayer.AddNyawa)
    {
        DrawText(TextFormat("GetHealth! "), GAMEPLAY_WIDTH + MENU_WIDTH / 2 - 85, 390, 25, WHITE);
    }
    if (InfoPlayer.DoubleAttack)
    {
        DrawText(TextFormat("DoubleAttack! "), GAMEPLAY_WIDTH + MENU_WIDTH / 2 - 85, 390, 25, WHITE);
    }
    if (InfoPlayer.shieldActive)
    {
        DrawText(TextFormat("ShieldActive! "), GAMEPLAY_WIDTH + MENU_WIDTH / 2 - 85, 390, 25, WHITE);
    }
    if (InfoPlayer.speedActive)
    {
        DrawText(TextFormat("SpeedActive! "), GAMEPLAY_WIDTH + MENU_WIDTH / 2 - 85, 390, 25, WHITE);
    }
}

void ShowSpark(Vector2 position)
{
    Sparkles.aktif = true;
    Sparkles.timer = 0.0f;
    Sparkles.PosisiSpark = position;
}

void UpdateSpark()
{
    if (Sparkles.aktif)
    {
        Sparkles.timer += GetFrameTime();
        if (Sparkles.timer >= 2.0f)
        {
            Sparkles.aktif = false;
        }
    }
}

void tampilspark()
{
    if (Sparkles.aktif)
    {
        DrawTextureEx(Sparkles.sparkIMG, Sparkles.PosisiSpark, 0.0f, 1.0f, WHITE);
    }
}

void ResetSpark()
{
    Sparkles.aktif = false;
}