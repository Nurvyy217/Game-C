#include "supriadi.h"
#include "hasbi.h"
#include "suci.h"
#include "stdlib.h"
#include <stdio.h>
#include "fawwaz.h"
#include "nazwa.h"

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
    InfoPlayer.nyawaIMG = LoadTexture("assets/heart.png");
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

/* INIT ENEMY: ALOKASI UNTUK 1 NODE */
void InitEnemy(){
    address currentEnemy;
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        address new_enemy = (address)malloc(sizeof(Enemy));
        if (new_enemy != NULL)
        {
            new_enemy->isActive = false;
            new_enemy->next = NULL;
            /* code */

        }
        if (EnemiesHead == NULL)
        {
            EnemiesHead = new_enemy;
            currentEnemy = EnemiesHead;
            /* code */
        }
        else{
            currentEnemy->next = new_enemy;
            currentEnemy = new_enemy;
        }
    }
}

void UpdateEnemies(Texture2D EnemyTexture, int xBounceEnemyRight, int xBounceEnemyLeft, int yPositionBullet, int xPositionBullet, GameState *S)
{
    address currentEnemy;
    currentEnemy = EnemiesHead;
    float deltaTime = GetFrameTime();
    for (int i = 0; i < getMaxEnemy(S); i++)
    {
        if (currentEnemy->isActive)
        {
            if (currentEnemy->hitEffectTimer > 0)
            {
                currentEnemy->hitEffectTimer -= deltaTime;
                if (currentEnemy->hitEffectTimer <= 0)
                {
                    currentEnemy->hitEffectTimer = 0;
                }
            }
            currentEnemy->position.y += currentEnemy->speed.y;
            currentEnemy->position.x += currentEnemy->speed.x;

            if (currentEnemy->position.x <= xBounceEnemyRight || currentEnemy->position.x >= GAMEPLAY_WIDTH - (EnemyTexture.width * 2.0f) + xBounceEnemyLeft)
            {
                currentEnemy->speed.x *= -1; // Pantulan jika mencapai batas
            }

            if (currentEnemy->position.y > SCREEN_HEIGHT)
            {
                currentEnemy->position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 170), GetRandomValue(-200, -150)};
                currentEnemy->hasShot = false; // Reset tembakan jika musuh respawn dari atas
            }

            // Jika musuh boleh menembak dan belum menembak sebelumnya, maka tembak
            if (currentEnemy->canShoot && !currentEnemy->hasShot && GetRandomValue(0, 100) < 2)
            {
                EnemyShoot(EnemyTexture, yPositionBullet, xPositionBullet, S);
            }
        }
        currentEnemy = currentEnemy->next;
    }
}

void SpawnEnemies(GameState *S)
{
    address currentEnemy = EnemiesHead;
    for (int i = 0; i < getMaxEnemy(S); i++)
    {
        if (!currentEnemy->isActive)
        {
            currentEnemy->speed = (Vector2){GetRandomValue(-2, 2) * 0.5, GetRandomValue(1, getEnemySpeed(S)) * 0.8};
            currentEnemy->position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 170), GetRandomValue(-200, -150)};
            currentEnemy->canShoot = (GetRandomValue(0, 1) == 1);
            currentEnemy->health = getEnemyHealth(S);
            currentEnemy->isActive = true;
            currentEnemy->hasShot = false;
            currentEnemy->hitEffectTimer = 0;
            currentEnemy->hitEffectFrame = 0;
            break;
        }
        currentEnemy = currentEnemy->next;
    }
}

void CheckEnemyCollisions(int xEnemy, int yEnemy, int radiusPlayer, int radiusBulletEnemy, GameState *S)
{
    address currentEnemy = EnemiesHead;
    Vector2 playerPosition = (Vector2){player.position.x + 185, player.position.y + 150};

    PNodeEB current = ebHead;

    for (int i = 0; i < getMaxEnemy(S); i++)
    {
        if (!currentEnemy->isActive)
            continue; // Lewati musuh yang sudah mati

        Vector2 enemiesPosition = (Vector2){currentEnemy->position.x + xEnemy, currentEnemy->position.y + yEnemy};

        // Cek tabrakan dengan peluru pemain (Musuh tertembak)
        BulletNode *currentB = BulletHead;
        while (currentB != NULL)
        {
            if (currentB->data.active && CheckCollisionCircles(enemiesPosition, radiusPlayer, currentB->data.position, radiusBulletEnemy))
            {
                currentEnemy->health--;
                currentEnemy->hitEffectTimer = 0.15f; // Waktu efek aktif
                currentEnemy->hitEffectFrame = (currentEnemy->health % 2);
                currentB->data.active = false;
                if (currentEnemy->health <= 0)
                {
                    currentEnemy->isActive = false;
                    currentEnemy->hitEffectTimer = 0;
                    currentEnemy->hitEffectFrame = 0;
                    updateScore(2); // Matikan musuh
                    PlaySound(enemyDestroyed);
                    CreateExplosion(enemiesPosition);
                    break; // Hindari multiple hits dalam satu frame
                }
            }

            currentB = currentB->next;
        }
        // Cek tabrakan antara musuh dan pemain (Player menabrak musuh)
        if (playerInvincible <= 0)
        {
            if (CheckCollisionCircles(playerPosition, 45, enemiesPosition, 35))
            {
                updateScore(1);
                updateNyawa(S);
                PlaySound(userPlaneExplosions);
                if (!InfoPlayer.shieldActive)
                {
                    CreateExplosion(playerPosition);
                }
                currentEnemy->isActive = false; // Hancurkan musuh
                break;
            }
        }

        // Cek tabrakan antara peluru musuh dan pemain (Player tertembak)
        current = ebHead;
        while (current != NULL)
        {
            if (playerInvincible <= 0)
            {
                if (current->Eb.isActive && CheckCollisionCircles(current->Eb.position, 5, playerPosition, 45))
                {
                    current->Eb.isActive = false;
                    PlaySound(enemyDestroyed);
                    if (!InfoPlayer.shieldActive)
                    {
                        CreateExplosion(playerPosition);
                    }

                    updateNyawa(S);
                    break;
                }
            }
            current = current->next; // Pindah ke peluru musuh berikutnya
        }
        currentEnemy = currentEnemy->next;
    }
}

void ResetEnemies()
{
    address currentEnemy = EnemiesHead;
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        currentEnemy->isActive = false;
        currentEnemy = currentEnemy->next;
    }
}

void FreeEnemy()
{
    address currentEnemy = EnemiesHead;
    while (currentEnemy != NULL)
    {
        address temp = currentEnemy;
        currentEnemy = currentEnemy->next;
        free(temp);
    }
    EnemiesHead = NULL;
}