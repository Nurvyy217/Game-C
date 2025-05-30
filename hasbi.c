#include "hasbi.h"
#include "supriadi.h"
#include <stdio.h>
#include "fawwaz.h"
#include <stdio.h>
#include "nazwa.h"
#include "suci.h"
#include "raylib.h"
#include <string.h>

/********************************************************* LOADING ANIMATION ******************************************************************/
bool gameStart = false;
bool isLoadingDone = false;
Sound nging, duar;

void DrawLayout()
{
    ClearBackground(BLACK);
    Rectangle source = {0, 0, background.width, background.height}; // Seluruh gambar
    Rectangle dest = {0, 0, GAMEPLAY_WIDTH, SCREEN_HEIGHT};         // Area gameplay
    Vector2 origin = {0, 0};                                        // Titik acuan (pojok kiri atas)

    DrawTexturePro(background, source, dest, origin, 0.0f, WHITE);
    DrawStar();
    // Separator line
    DrawLine(GAMEPLAY_WIDTH, 0, GAMEPLAY_WIDTH, SCREEN_HEIGHT, WHITE);
}

void loadingAnimation()
{
    static float opacity = 1.0f;
    static int section = 0;
    static float timer = 0.0f;

    initLoadScreen();

    if (isLoadingDone)
        return; // Jika sudah selesai, langsung keluar

    timer += GetFrameTime();

    switch (section)
    {
    case 0: // Fade out gambar pertama
        if (timer > 2.0f)
        {
            opacity -= 0.02f;
            if (opacity <= 0)
            {
                opacity = 0;
                section = 1;
                timer = 0;
            }
        }
        break;
    case 1: // gambar kedua
        opacity = 1.0f;
        section = 2;
        timer = 0;
        break;

    case 2: // Fade out gambar kedua, lalu selesai
        if (timer > 2.0f)
        {
            opacity -= 0.02f;
            if (opacity <= 0)
            {
                opacity = 0;
                isLoadingDone = true; // Menandai bahwa loading selesai
                return;
            }
        }
        break;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    if (section == 0)
    {
        DrawTexture(logoDeveloper, SCREEN_WIDTH / 2 - logoDeveloper.width / 2, SCREEN_HEIGHT / 2 - logoDeveloper.height / 2, Fade(WHITE, opacity));
    }
    if (section >= 1)
    {
        DrawTexture(gameNamePhoto, SCREEN_WIDTH / 2 - gameNamePhoto.width / 2, SCREEN_HEIGHT / 2 - gameNamePhoto.height / 2, Fade(WHITE, (section == 1) ? (1.0f - opacity) : opacity));
    }

    EndDrawing();
}

void unloadTextures()
{
    if (texturesLoaded)
    {
        UnloadTexture(logoDeveloper);
        UnloadTexture(gameNamePhoto);
        texturesLoaded = false;
    }
}

/********************************************************* USER PLANE ******************************************************************/
Player player;
// BulletNode *BulletHead = NULL;
// Texture2D bulletTexture;
Sound shootSound;
static float shootCooldown = 0.0f;
static const float SHOOT_INTERVAL = 0.2f;

bool isSoundPlaying = true;

void UpdateShooting(float deltaTime)
{
    // Kurangi cooldown
    if (IsKeyPressed(KEY_F)) {
        isSoundPlaying = !isSoundPlaying;
        
        SetMasterVolume(isSoundPlaying ? 1.0f : 0.0f);
    }

    if (shootCooldown > 0.0f)
    {
        shootCooldown -= deltaTime;
    }

    // Cek apakah SPACE ditekan + cooldown <= 0
    if (IsKeyDown(KEY_SPACE) && shootCooldown <= 0.0f)
    {
        if (!InfoPlayer.DoubleAttack)
        {
            ShootBullet(); // Fungsi menembak
        }
        else
        {
            powerupAttack();
        }

        shootCooldown = SHOOT_INTERVAL; // Reset cooldown
    }
}

void InitPlayer()
{
    player.position = (Vector2){(GAMEPLAY_WIDTH / 2) - 210, SCREEN_HEIGHT - 230};
    player.texture = LoadTexture("assets/userPlane.png");
}


void DrawPlayer()
{
    float scale = 0.6f; // Skala 60% dari ukuran aslinya
    Color playerColor = (playerInvincible > 0) ? (Color){255, 255, 255, 100} : WHITE;
    DrawTextureEx(player.texture, player.position, 0.0f, scale, playerColor);
}

void UpdatePlayer()
{
    if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && player.position.x > -170)
        player.position.x -= PLAYER_SPEED + AddSpeed;
    if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && player.position.x < GAMEPLAY_WIDTH - (((player.texture.width * 0.6) / 2) + 50))
        player.position.x += PLAYER_SPEED + AddSpeed;
    if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && player.position.y > -125)
        player.position.y -= PLAYER_SPEED + AddSpeed;
    if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && player.position.y < SCREEN_HEIGHT - (((player.texture.height * 0.6) / 2) + 15))
        player.position.y += PLAYER_SPEED + AddSpeed;
}

/********************************************************* SETTER GETTER ******************************************************************/
GameState gamestate;

void setMaxEnemy(GameState *S, int value)
{
    maxEnemy(S) = value;
}
void setMaxEnemyBullet(GameState *S, int value)
{
    maxEnemyBullet(S) = value;
}
void setEnemyBulletSpeed(GameState *S, int value)
{
    enemyBulletSpeed(S) = value;
}
void setEnemyTypeShoot(GameState *S, int value)
{
    enemyTypeShoot(S) = value;
}
void setEnemySpeed(GameState *S, int value)
{
    enemySpeed(S) = value;
}
void setEnemyHealth(GameState *S, int value)
{
    enemyHealth(S) = value;
}
void setHealthBroke(GameState *S, int value)
{
    healthBroke(S) = value;
}
void setEnemyDamage(GameState *S, int value)
{
    enemyDamage(S) = value;
}

int getMaxEnemy(GameState *S)
{
    return maxEnemy(S);
}
int getMaxEnemyBullet(GameState *S)
{
    return maxEnemyBullet(S);
}
int getEnemyBulletSpeed(GameState *S)
{
    return enemyBulletSpeed(S);
}
int getEnemyTypeShoot(GameState *S)
{
    return enemyTypeShoot(S);
}
int getEnemySpeed(GameState *S)
{
    return enemySpeed(S);
}
int getEnemyHealth(GameState *S)
{
    return enemyHealth(S);
}
int getHealthBroke(GameState *S)
{
    return healthBroke(S);
}
int getEnemyDamage(GameState *S)
{
    return enemyDamage(S);
}

/********************************************************* EXPLOSIONS ******************************************************************





/********************************************************* ASTEROIDS ******************************************************************/
Sound userPlaneExplosions;
Texture2D asteroidTexture;

/* REMOVEINACTIVEASTEROID: MENGHAPUS ASTEROID YANG TIDAK AKTIF DARI LIST */
void RemoveInactiveAsteroids(GameState *S)
{
    AsteroidNode **indirect = &S->asteroidHead;
    while (*indirect != NULL)
    {
        AsteroidNode *current = *indirect;
        if (!current->data.active)
        {
            *indirect = current->next;
            free(current);
        }
        else
        {
            indirect = &(*indirect)->next;
        }
    }
}

/* COUNT ACTIVE ASTEROIDS: MENGHITUNG JUMLAH ASTEROID YANG AKTIF */
int CountActiveAsteroids(GameState *S)
{
    int count = 0;
    AsteroidNode *current = S->asteroidHead;
    while (current != NULL)
    {
        if (current->data.active)
            count++;
        current = current->next;
    }
    return count;
}

/* SPAWN ASTEROID: ALOKASI 1 NODE ASTEROID DAN INISIALISASI PROPERTI ASTEROID*/
void SpawnAsteroid(GameState *S)
{

    if (CountActiveAsteroids(S) >= MAX_ASTEROIDS)
        return;

    AsteroidNode *newNode = (AsteroidNode *)malloc(sizeof(AsteroidNode));
    newNode->data.position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 100), GetRandomValue(-50, -100)};
    newNode->data.size = GetRandomValue(1, 3);
    newNode->data.speed = (Vector2){GetRandomValue(-1, 1), GetRandomValue(5, 7)};
    newNode->data.active = true;
    newNode->next = NULL;

    newNode->next = S->asteroidHead;
    S->asteroidHead = newNode;
}

/* UPDATE ASTEROIDS: PERBARUI POSISI (PERGERAKAN ASTEROID DI LAYAR)*/
void UpdateAsteroids(GameState *S)
{
    AsteroidNode *current = S->asteroidHead;

    while (current != NULL)
    {
        if (current->data.active)
        {
            current->data.position.x += current->data.speed.x;
            current->data.position.y += current->data.speed.y;

            float forAsteroidSize;
            if (current->data.size == 1)
                forAsteroidSize = 0.05f;
            else if (current->data.size == 2)
                forAsteroidSize = 0.07f;
            else
                forAsteroidSize = 0.09f;
            
            float limitX = GAMEPLAY_WIDTH - (asteroidTexture.width * forAsteroidSize);

            if (current->data.position.x <= 0 || current->data.position.x >= limitX)
            {
                current->data.speed.x *= -1;
            }
            if (current->data.position.y > SCREEN_HEIGHT)
            {
                current->data.active = false;
            }
        }
        current = current->next;
    }
}

/*CHECKCOLLISIONASTEROID: CEK APAKAH ASTEROID MENYENTUH PEMAIN*/
void CheckCollisionAsteroid(GameState *S)
{
    Vector2 playerPosition = (Vector2){player.position.x + 175, player.position.y + 140};

    AsteroidNode *current = S->asteroidHead;
    while (current != NULL)
    {
        if (current->data.active && playerInvincible <= 0)
        {
            if (CheckCollisionCircles(playerPosition, 35, current->data.position, current->data.size * 20))
            {
                updateNyawa(S);
                PlaySound(userPlaneExplosions);
                if (!InfoPlayer.shieldActive)
                {
                    CreateExplosion(playerPosition);
                }
                current->data.active = false;
                break;
            }
        }
        current = current->next;
    }
}

/* ASTEROID LOOP: MENGGUNAKAN TIMER UNTUK MENGATUR SPAWN ASTEROID */
void AsteroidLoop(GameState *S)
{
    static float asteroidSpawnTimer = 0.0f;
    asteroidSpawnTimer += GetFrameTime();

    if (asteroidSpawnTimer >= 0.2f)
    {
        SpawnAsteroid(S);
        asteroidSpawnTimer = 0.0f;
    }

    UpdateAsteroids(S);
    RemoveInactiveAsteroids(S);
}

/* DRAW ASTEROIDS: MENGGAMBAR ASTEROID DI LAYAR */
void DrawAsteroids(GameState *S)
{
    AsteroidNode *current = S->asteroidHead;
    while (current != NULL)
    {
        if (current->data.active)
        {
            float scale = current->data.size == 1 ? 0.05f : current->data.size == 2 ? 0.07f
                                                                                    : 0.09f;

            DrawTextureEx(asteroidTexture, current->data.position, 0.0f, scale, WHITE);
        }
        current = current->next;
    }
}

/********************************************************* ENEMIES ******************************************************************/
// Enemy enemies[MAX_ENEMIES];
address EnemiesHead = NULL;
PNodeEB ebHead = NULL;
Sound nging, duar, enemyDestroyed;
Texture2D  hitEffect1, hitEffect2;

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

// void UpdateEnemies(Texture2D EnemyTexture, int xBounceEnemyRight, int xBounceEnemyLeft, int yPositionBullet, int xPositionBullet, GameState *S)
// {
//     float deltaTime = GetFrameTime();
//     for (int i = 0; i < getMaxEnemy(S); i++)
//     {
//         if (enemies[i].isActive)
//         {
//             if (enemies[i].hitEffectTimer > 0)
//             {
//                 enemies[i].hitEffectTimer -= deltaTime;
//                 if (enemies[i].hitEffectTimer <= 0)
//                 {
//                     enemies[i].hitEffectTimer = 0;
//                 }
//             }
//             enemies[i].position.y += enemies[i].speed.y;
//             enemies[i].position.x += enemies[i].speed.x;

//             if (enemies[i].position.x <= xBounceEnemyRight || enemies[i].position.x >= GAMEPLAY_WIDTH - (EnemyTexture.width * 2.0f) + xBounceEnemyLeft)
//             {
//                 enemies[i].speed.x *= -1; // Pantulan jika mencapai batas
//             }

//             if (enemies[i].position.y > SCREEN_HEIGHT)
//             {
//                 enemies[i].position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 170), GetRandomValue(-200, -150)};
//                 enemies[i].hasShot = false; // Reset tembakan jika musuh respawn dari atas
//             }

//             // Jika musuh boleh menembak dan belum menembak sebelumnya, maka tembak
//             if (enemies[i].canShoot && !enemies[i].hasShot && GetRandomValue(0, 100) < 2)
//             {
//                 EnemyShoot(EnemyTexture, yPositionBullet, xPositionBullet, S);
//             }
//         }
//     }
// }

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

// void SpawnEnemies(GameState *S)
// {
//     for (int i = 0; i < getMaxEnemy(S); i++)
//     {
//         if (!enemies[i].isActive)
//         {
//             enemies[i].speed = (Vector2){GetRandomValue(-2, 2) * 0.5, GetRandomValue(1, getEnemySpeed(S)) * 0.8};
//             enemies[i].position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 170), GetRandomValue(-200, -150)};
//             enemies[i].canShoot = (GetRandomValue(0, 1) == 1);
//             enemies[i].health = getEnemyHealth(S);
//             enemies[i].isActive = true;
//             enemies[i].hasShot = false;
//             enemies[i].hitEffectTimer = 0;
//             enemies[i].hitEffectFrame = 0;
//             break;
//         }
//     }
// }

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

void EnemiesLoop(Texture2D EnemyTexture, int xBounceEnemyRight, int xBounceEnemyLeft, int yPositionBullet, int xPositionBullet, GameState *S)
{
    static float enemiesSpawnTimer = 0.0f;
    enemiesSpawnTimer += GetFrameTime();

    if (enemiesSpawnTimer >= 0.4f)
    { // Setiap 2 detik, spawn asteroid baru
        SpawnEnemies(S);
        enemiesSpawnTimer = 0.0f;
    }
    UpdateEnemies(EnemyTexture, xBounceEnemyRight, xBounceEnemyLeft, yPositionBullet, xPositionBullet, S);
}

void DrawEnemies(Texture2D EnemyTexture, Texture2D EnemyDamaged, float scale, int xEffect, int yEffect, GameState *S)
{
    address currentEnemy = EnemiesHead;
    for (int i = 0; i < getMaxEnemy(S); i++)
    {
        if (currentEnemy->isActive)
        {
            Texture2D currentTexture = (currentEnemy->health <= getHealthBroke(S)) ? EnemyDamaged : EnemyTexture;
            DrawTextureEx(currentTexture, currentEnemy->position, 0.0f, scale, WHITE);
            if (currentEnemy->hitEffectTimer > 0)
            {
                Texture2D effect = (currentEnemy->hitEffectFrame == 0) ? hitEffect1 : hitEffect2;

                Vector2 effectPosition;
                effectPosition.x = currentEnemy->position.x - (effect.width / 2) + xEffect;
                effectPosition.y = currentEnemy->position.y - (effect.height / 2) + yEffect;
                DrawTexture(effect, effectPosition.x, effectPosition.y, WHITE);
            }
        }
        currentEnemy = currentEnemy->next;
    }
}

void EnemyShoot(Texture2D EnemyTexture, int yPositionBullet, int xPositionBullet, GameState *S)
{
    address currentEnemy = EnemiesHead;
    PNodeEB current = ebHead;
    for (int i = 0; i < getMaxEnemy(S); i++)
    {
        if (!currentEnemy->isActive || currentEnemy->hasShot)
            continue; // Musuh mati tidak boleh menembak
        if (getEnemyTypeShoot(S) == 0 || getEnemyTypeShoot(S) == 3)
        {
            current = ebHead;
            for (int j = 0; j < getMaxEnemyBullet(S); j++)
            {
                // Create Node
                if (current->Eb.isActive == false)
                {
                    current->Eb.position = (Vector2){(currentEnemy->position.x - xPositionBullet) + EnemyTexture.width,
                                                     (currentEnemy->position.y + EnemyTexture.width + yPositionBullet)};
                    current->Eb.isActive = true;
                    current->Eb.shooterIndex = i; // Tandai peluru ini ditembak oleh musuh ke-i
                    if (GetRandomValue(0, 1) == 0)
                    {
                        current->Eb.speed = (Vector2){0, SPEED_ENEMY_BULLETS}; // Vertikal (ke bawah)
                    }
                    else
                    {
                        int dirX = GetRandomValue(0, 1) == 0 ? -1 : 1;                   // Bisa kiri atau kanan
                        current->Eb.speed = (Vector2){dirX * 2.0f, SPEED_ENEMY_BULLETS}; // Diagonal
                    }
                    currentEnemy->hasShot = true;
                    break;
                }
                current = current->next;
            }
        }
        else if (getEnemyTypeShoot(S) == 1)
        {   
            int bulletsSpawned = 0;
            current = ebHead;
            for (int j = 0; j < getMaxEnemyBullet(S) && bulletsSpawned < 4; j++)
            {
                if (current->Eb.isActive == false)
                {
                    float offsetX = (bulletsSpawned % 2 == 0) ? -10 : 10; // Geser kiri & kanan
                    float offsetY = (bulletsSpawned < 2) ? 0 : -35;       // Peluru belakang lebih mundur sedikit
                    current->Eb.position = (Vector2){
                        (currentEnemy->position.x - xPositionBullet) + EnemyTexture.width + offsetX,
                        (currentEnemy->position.y + EnemyTexture.width + yPositionBullet + offsetY)};
                    current->Eb.isActive = true;
                    current->Eb.shooterIndex = i;
                    current->Eb.speed = (Vector2){0, SPEED_ENEMY_BULLETS + 5}; // Lurus ke bawah
                    bulletsSpawned++;
                }
                current = current->next;
            }
            currentEnemy->hasShot = true;
        }
        else if (getEnemyTypeShoot(S) == 2)
        {
            int bulletsSpawned = 0;
            current = ebHead;
            for (int j = 0; j < getMaxEnemyBullet(S) && bulletsSpawned < 3; j++)
            {
                if (current->Eb.isActive == false)
                {
                    float offsetX = (bulletsSpawned == 0) ? -15 : (bulletsSpawned == 1) ? 0
                                                                                        : 15;
                    float speedX = (bulletsSpawned == 0) ? -2.0f : (bulletsSpawned == 1) ? 0
                                                                                         : 2.0f;
                    current->Eb.position = (Vector2){(currentEnemy->position.x - xPositionBullet) + EnemyTexture.width + offsetX,
                                                     (currentEnemy->position.y + EnemyTexture.width + yPositionBullet)};
                    current->Eb.isActive = true;
                    current->Eb.shooterIndex = i;
                    current->Eb.speed = (Vector2){speedX, 4}; // Kiri, tengah, kanan
                    bulletsSpawned++;
                }
                current = current->next;
            }
            currentEnemy->hasShot = true;
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

/********************************************************* RESET ******************************************************************/
void ResetEnemies()
{
    address currentEnemy = EnemiesHead;
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        currentEnemy->isActive = false;
        currentEnemy = currentEnemy->next;
    }
}

void ResetAsteroid(GameState *S)
{
    AsteroidNode *current = S->asteroidHead;
    while (current != NULL)
    {
        current->data.active = false;
        current = current->next;
    }
}

/********************************************************* LOAD AND UNLOAD ******************************************************************/
Texture2D enemyLvl5, enemyLvl6, enemyBulletLv3, enePurpleDamaged, enemyLvl5Broken, enemylvl1, enemyBulletlv1, gameOver;
Sound typing;
Music gameplayMusic;
void LoadAssets()
{
    initBG();
    asteroidTexture = LoadTexture("assets/Asteroid.png");
    enemyDestroyed = LoadSound("assets/enemyDestroyed.wav");
    typing = LoadSound("assets/typing.wav");
    shootSound = LoadSound("assets/shoot.wav");
    nging = LoadSound("assets/nging.wav");
    duar = LoadSound("assets/duar.wav");
    userPlaneExplosions = LoadSound("assets/userPlaneExplosion.wav");
    hitEffect1 = LoadTexture("assets/efekTembakan1.png");
    hitEffect2 = LoadTexture("assets/efekTembakan2.png");
    enemylvl1 = LoadTexture("assets/musuh.png");
    enemyBulletlv1 = LoadTexture("assets/enemyBullet.png");
    enemyLvl5 = LoadTexture("assets/enemyLvl5.png");
    enemyLvl6 = LoadTexture("assets/enePurple.png");
    enemyBulletLv3 = LoadTexture("assets/laserUfo.png");
    enePurpleDamaged = LoadTexture("assets/enePurpleDamaged.png");
    enemyLvl5Broken = LoadTexture("assets/enemyLvl5Broken.png");
    gameplayMusic = LoadMusicStream("assets/gameplay.mp3");
    gameOver = LoadTexture("assets/gameover.png");
}
// UNLOAD
void UnloadAssets()
{
    UnloadSound(userPlaneExplosions);
    UnloadSound(shootSound);
    UnloadSound(nging);
    UnloadSound(duar);
    UnloadSound(enemyDestroyed);
    UnloadTexture(explosionsTexture);
    UnloadTexture(asteroidTexture);
    UnloadTexture(hitEffect1);
    UnloadTexture(hitEffect2);
    UnloadTexture(enemylvl1);
    UnloadTexture(enemyBulletlv1);
    UnloadTexture(enemyLvl5);
    UnloadTexture(enemyLvl6);
    UnloadTexture(enemyBulletLv3);
    UnloadTexture(enePurpleDamaged);
}

/********************************************************* GAMEPLAY ******************************************************************/
int level = 0;
int previousLevel = 0;
bool isLevelTransition = false;
static CharNode *levelTextHead = NULL;
char buffer[12];
int letterIndex = 0;  // Indeks huruf yang sudah muncul
float letterTimer = 0;

bool IsSameText(CharNode *a, CharNode *b) {
    while (a && b) {
        if (a->character != b->character) return false;
        a = a->next;
        b = b->next;
    }
    return (a == NULL && b == NULL);
}


CharNode *SetLevelText(const char *text)
{
    CharNode *head = NULL;
    CharNode *curr = NULL;

    for (int i = 0; text[i] != '\0'; i++)
    {
        CharNode *newNode = (CharNode *)malloc(sizeof(CharNode));
        newNode->character = text[i];
        newNode->next = NULL;

        if (head == NULL)
        {
            head = newNode;
        }
        else
        {
            curr->next = newNode;
        }
        curr = newNode;
    }

    return head; // Return linked list head
}
void FreeLevelText(CharNode *head)
{
    CharNode *curr = head;
    while (curr != NULL)
    {
        CharNode *next = curr->next;
        free(curr);
        curr = next;
    }
}

void DrawLevelTransition(float deltaTime, CharNode *head)
{
    static float letterTimer = 0.0f;
    static float removeTimer = 0.0f;
    static int letterIndex = 0;
    static bool removing = false;
    static CharNode *previousHead = NULL;

    // Reset semua variabel jika teks berubah
    if (!IsSameText(head, previousHead)) {
        letterTimer = 0.0f;
        removeTimer = 0.0f;
        letterIndex = 0;
        removing = false;
        previousHead = head;
    }

    // Hitung jumlah huruf
    int totalLetters = 0;
    for (CharNode *temp = head; temp != NULL; temp = temp->next)
        totalLetters++;
    
    int textX = GAMEPLAY_WIDTH / 2 - (totalLetters * 20) / 2;
    int textY = SCREEN_HEIGHT / 2 - 20;

    letterTimer += deltaTime;

    if (!removing && letterIndex < totalLetters && letterTimer > 0.2f)
    {
        SetSoundVolume(typing, 3.0f);
        PlaySound(typing);
        letterIndex++;
        letterTimer = 0.0f;
    }

    if (letterIndex == totalLetters)
    {
        removeTimer += deltaTime;
        if (removeTimer > 2.0f)
        {
            removing = true;
            removeTimer = 0.0f;
        }
    }

    if (removing && letterIndex > 0 && letterTimer > 0.2f &&!bosses.theEnd)
    {
        PlaySound(typing);
        letterIndex--;
        letterTimer = 0.0f;
    }

    // Gambar karakter satu-satu
    CharNode *curr = head;
    int x = textX;
    

    for (int i = 0; i < letterIndex && curr != NULL; i++)
    {
        DrawText(TextFormat("%c", curr->character), x, textY, 40, WHITE);
        x += 25;
        curr = curr->next;
    }

    if (removing && letterIndex == 0)
    {
        isLevelTransition = false;
        removing = false;
    }
}


void GameplayWithoutEnemies()
{
    UpdatePlayer();
    DrawLayout();
    DrawPlayer();
}

void callAsteroid(GameState *S)
{
    DrawAsteroids(S);
    CheckCollisionAsteroid(S);
    AsteroidLoop(S);
}

void initGameState(GameState *S)
{
    maxEnemy(S) = 6;
    maxEnemyBullet(S) = 5;
    enemyBulletSpeed(S) = 3;
    enemyTypeShoot(S) = 0;
    enemySpeed(S) = 1;
    enemyHealth(S) = 3;
    healthBroke(S) = 2;
    enemyDamage(S) = 1;
}

void DrawLvl1()
{
    DrawLayout();
    DrawPlayer();
    DrawBullets();
    DrawExplosions(explosionsTexture);
    DrawEnemies(enemylvl1, enemylvl1, 2.0f, 120, 150, &gamestate);
    DrawEnemyBullets(enemyBulletlv1, 0.8f, &gamestate);
    tampilspark();
    
}
void level1(float deltaTime)
{
    setEnemyHealth(&gamestate, 1);
    UpdatePlayer();
    UpdateShooting(deltaTime);
    UpdateBullets();
    UpdateExplosions(deltaTime);
    UpdateEnemies(enemylvl1, -85, 85, 20, 30, &gamestate);
    UpdateEnemyBullets(enemyBulletlv1, &gamestate);
    CheckEnemyCollisions(95, 70, 35, 5, &gamestate);
    EnemiesLoop(enemylvl1, -85, 85, 20, 30, &gamestate);
    DrawLvl1();
    inipowerup();
    UpdateSpark();
}
void DrawLvl3(GameState *S)
{
    DrawLayout();
    DrawPlayer();
    DrawBullets();
    DrawExplosions(explosionsTexture);
    DrawAsteroids(S);
    
}
void level3(GameState *S, float deltaTime)
{
    UpdatePlayer();
    UpdateShooting(deltaTime);
    UpdateBullets();
    UpdateExplosions(deltaTime);
    CheckCollisionAsteroid(S);
    DrawLvl3(S);
    AsteroidLoop(S);
    inipowerup();
}

void DrawLvl4()
{
    DrawLayout();
    DrawPlayer();
    DrawBullets();
    DrawExplosions(explosionsTexture);
    DrawEnemies(enemyLvl5, enemyLvl5Broken, 1.2f, 80, 110, &gamestate);
    DrawEnemyBullets(enemyBulletlv1, 0.8f, &gamestate);
    tampilspark();
    
}
void level4(float deltaTime)
{
    setEnemyDamage(&gamestate, 4);
    setEnemyHealth(&gamestate, 3);
    setMaxEnemy(&gamestate, 7);
    setHealthBroke(&gamestate, 1);
    setEnemyTypeShoot(&gamestate, 1);
    setMaxEnemyBullet(&gamestate, 20);
    UpdatePlayer();
    UpdateShooting(deltaTime);
    UpdateBullets();
    UpdateExplosions(deltaTime);
    UpdateEnemies(enemyLvl5, -35, 125, 20, 30, &gamestate);
    UpdateEnemyBullets(enemyBulletlv1, &gamestate);
    CheckEnemyCollisions(60, 50, 40, 10, &gamestate);
    EnemiesLoop(enemyLvl5, -35, 125, 20, 30, &gamestate);
    DrawLvl4();
    inipowerup();
}

void DrawLvl5()
{
    DrawLayout();
    DrawPlayer();
    DrawBullets();
    DrawExplosions(explosionsTexture);
    DrawEnemies(enemyLvl6, enePurpleDamaged, 3.0f, 150, 200, &gamestate);
    DrawEnemyBullets(enemyBulletLv3, 1.3, &gamestate);
    tampilspark();

}
void level5(float deltaTime)
{
    setMaxEnemy(&gamestate, 6);
    setEnemyDamage(&gamestate, 2);
    setEnemyTypeShoot(&gamestate, 3);
    setMaxEnemyBullet(&gamestate, 5);
    setEnemyHealth(&gamestate, 4);
    setHealthBroke(&gamestate, 2);
    setMaxEnemyBullet(&gamestate, 4);
    UpdatePlayer();
    UpdateShooting(deltaTime);
    UpdateBullets();
    UpdateExplosions(deltaTime);
    UpdateEnemies(enemyLvl6, -90, 10, 150, -5, &gamestate);
    UpdateEnemyBullets(enemyBulletLv3, &gamestate);
    CheckEnemyCollisions(100, 140, 40, 10, &gamestate);
    EnemiesLoop(enemyLvl6, -90, 10, 150, -5, &gamestate);
    DrawLvl5();
    inipowerup();
}

void DrawBossLevel()
{
    DrawLayout();
    DrawPlayer();
    DrawBullets();
    DrawBosses(&gamestate);
    DrawExplosions(explosionsTexture);
    DrawBossLaser();
}
void bossLevel(float deltaTime)
{
    if (bosses.theEnd) // Jika boss mati
    {
        snprintf(buffer, sizeof(buffer), "The End");
        levelTextHead = SetLevelText(buffer);
        letterIndex = 0;
        letterTimer = 0;
        isLevelTransition = true;
    }
    StopMusicStream(gameplayMusic);
    InitialBoss();
    BossMov();
    ShootBossLaser();
    UpdateBossLaser();
    UpdatePlayer();
    UpdateShooting(deltaTime);
    UpdateExplosions(deltaTime);
    CheckBossCollisions(&gamestate);
    UpdateBullets();
    DrawBossLevel(&gamestate);
    BossBar();
    inipowerup();
    UpdateSpark();
    BossRage(&gamestate);
    BossExplosions();
    UpdateBGM();
}

void game()
{
   
    if (!IsMusicStreamPlaying(gameplayMusic)&&!bosses.theEnd)
    {
        PlayMusicStream(gameplayMusic);
    }
    SetMusicVolume(gameplayMusic, 1.0f);
    UpdateMusicStream(gameplayMusic);
    if (playerInvincible > 0)
    {
        playerInvincible--;
    }
    initGameState(&gamestate);
    float deltaTime = GetFrameTime();
    static float levelTimer = 0.0f;
    bool isGameOver = false;

    if (InfoPlayer.nyawa <= 0)
    {
        isGameOver = true;
    }

    if (!isGameOver)
    { 
        UpdateStar();
        // Tentukan level berdasarkan skor
        if (InfoPlayer.score < 25)
        {
            level = 1;
        }
        else if (InfoPlayer.score >= 25 && InfoPlayer.score < 50)
        {
            level = 2;
        }
        else if (InfoPlayer.score >= 50 && InfoPlayer.score < 75)
        {
            level = 3;
        }
        else if (InfoPlayer.score >= 75 && InfoPlayer.score < 100)
        {
            level = 4;
        }
        else if (InfoPlayer.score >= 100 && InfoPlayer.score < 125)
        {
            level = 5;
        }
        else if (InfoPlayer.score >= 125)
        {
            level = 6;
        }

        // Jika level berubah, reset musuh dan peluru
        if (level != previousLevel)
        {
            ResetEnemies();
            ResetEnemyBullets();
            ResetExplosions();
            ResetPlayerBulet();
            ResetAsteroid(&gamestate);
            ResetSpark();
            previousLevel = level; // Simpan level baru sebagai level sebelumnya
            FreeLevelText(levelTextHead);
            if (level < 6)
            {
                snprintf(buffer, sizeof(buffer), "Level%d", level);
                levelTextHead = SetLevelText(buffer);
            }
            else
            {
                StopMusicStream(gameplayMusic);
                snprintf(buffer, sizeof(buffer), "Boss Level");
                levelTextHead = SetLevelText(buffer);
            }

            isLevelTransition = true;
        }

        // Jalankan level yang sesuai
        if (isLevelTransition)
        {
            GameplayWithoutEnemies();
            DrawLevelTransition(GetFrameTime(), levelTextHead);
        }
        else
        {
            
            switch (level)
            {
            case 1:
                level1(deltaTime);
                break;
            case 2:
                level2(deltaTime);
                break;
            case 3:
                level3(&gamestate, deltaTime); // Panggil asteroid
                levelTimer += deltaTime;       // Tambahkan waktu saat level 2

                // Jika sudah mencapai 20 detik, naik ke level 3
                if (levelTimer >= 20.0f)
                {
                    InfoPlayer.score = 75;
                }
                break;
            case 4:
                level4(deltaTime);
                break;
            case 5:
                level5(deltaTime);
                break;
            case 6:
                bossLevel(deltaTime);
                break;
            }
        }
    }
    else
    {
        levelTimer = 0.0f;
        gameover(&gamestate);
    }
}
