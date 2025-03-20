#include "hasbi.h"
#include "supriadi.h"
#include <stdio.h>
#include "fawwaz.h"
#include <stdio.h>
#include "nazwa.h"
#include "suci.h"
#include "raylib.h"


// LOADING SCREEN
void DrawLayout();
void loadingAnimation();
void unloadTextures();
// USER PLANE
void InitPlayer();
void UpdatePlayer();
void DrawPlayer();
// BULLETS
void UpdateShooting(float deltaTime);
void DrawBullets();
void UpdateBullets();
void ShootBullet();
void InitBullets();
// EXPLOSIONS
void CreateExplosion(Vector2 position);
void UpdateExplosions(float deltaTime);
void DrawExplosions(Texture2D explosionTexture);
// ASTEROIDS
void InitAsteroids();
void UpdateAsteroids();
void CheckCollisions();
void SpawnAsteroid();
void AsteroidLoop();
void DrawAsteroids();
// ENEMY
//  Deklarasi fungsi musuh
void InitEnemies();
void DrawEnemies(Texture2D EnemyTexture, Texture2D EnemyDamaged, float scale, int xEffect, int yEffect, GameState *S);
void EnemyShoot();
void UpdateEnemyBullets();
void DrawEnemyBullets(Texture2D EnemyBulletTexture, float scale, GameState *S);
void CheckEnemyCollisions();
// ASSSETS
void LoadAssets();
// GAMEPLAY
// UNLOAD
void UnloadAssets();

Texture2D enemylvl1;
Texture2D enemyBulletlv1;
GameState gamestate;

int level = 0;

// LOADING SCREEN
void DrawLayout()
{
    bool gameStart = false;
    ClearBackground(RAYWHITE);

    // Gameplay area (3/4 of screen, left part)
    // DrawRectangle(0, 0, GAMEPLAY_WIDTH, SCREEN_HEIGHT, DARKBLUE); //posX, posY, width, height, color
    Rectangle source = {0, 0, background.width, background.height}; // Seluruh gambar
    Rectangle dest = {0, 0, GAMEPLAY_WIDTH, SCREEN_HEIGHT};         // Area gameplay
    Vector2 origin = {0, 0};                                        // Titik acuan (pojok kiri atas)

    DrawTexturePro(background, source, dest, origin, 0.0f, WHITE);

    // Menu area (1/4 of screen, right part)
    mainMenu(&gameStart);
    tampilNyawa();
    Tampil_Score();

    // Separator line
    DrawLine(GAMEPLAY_WIDTH, 0, GAMEPLAY_WIDTH, SCREEN_HEIGHT, BLACK); // strat posX, start posY, end posX, end posY
}

bool isLoadingDone = false;

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

// USER PLANE
Player player;
Bullet bullets[MAX_BULLETS];
Texture2D bulletTexture;
Sound shootSound;
static float shootCooldown = 0.0f;
static const float SHOOT_INTERVAL = 0.2f;

void UpdateShooting(float deltaTime)
{
    // Kurangi cooldown
    if (shootCooldown > 0.0f)
    {
        shootCooldown -= deltaTime;
    }

    // Cek apakah SPACE ditekan + cooldown <= 0
    if (IsKeyDown(KEY_SPACE) && shootCooldown <= 0.0f)
    {
        ShootBullet();                  // Fungsi menembak
        shootCooldown = SHOOT_INTERVAL; // Reset cooldown
    }
}

void InitPlayer()
{
    player.position = (Vector2){(GAMEPLAY_WIDTH / 2) - 210, SCREEN_HEIGHT - 230};
    player.texture = LoadTexture("assets/userPlane.png");
}

void InitBullets()
{
    if (bulletTexture.id == 0)
    {
        printf("ERROR: bullet.png gagal dimuat!\n");
    }
    else
    {
        printf("bullet.png berhasil dimuat! Ukuran: %d x %d\n", bulletTexture.width, bulletTexture.height);
    }
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].active = false;
    }
}

void UpdatePlayer()
{
    if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && player.position.x > -170)
        player.position.x -= PLAYER_SPEED;
    if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && player.position.x < GAMEPLAY_WIDTH - (((player.texture.width * 0.6) / 2) + 50))
        player.position.x += PLAYER_SPEED;
    if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && player.position.y > -125)
        player.position.y -= PLAYER_SPEED;
    if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && player.position.y < SCREEN_HEIGHT - (((player.texture.height * 0.6) / 2) + 15))
        player.position.y += PLAYER_SPEED;
}

void ShootBullet()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullets[i].active)
        {
            bullets[i].position = (Vector2){(player.position.x - 25) + player.texture.width * 0.6 / 2, (player.position.y + player.texture.width * 0.6 / 2) - 110};
            PlaySound(shootSound);
            bullets[i].active = true;
            break;
        }
    }
}

void UpdateBullets()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            bullets[i].position.y -= BULLET_SPEED;
            if (bullets[i].position.y < 0)
                bullets[i].active = false;
        }
    }
}

void DrawPlayer()
{
    float scale = 0.6f; // Skala 60% dari ukuran aslinya
    DrawTextureEx(player.texture, player.position, 0.0f, scale, WHITE);
    // DrawTexture(player.texture, player.position.x, player.position.y, WHITE);
}

void DrawBullets()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            DrawTextureEx(bulletTexture, (Vector2){bullets[i].position.x - 5, bullets[i].position.y - 5}, 0.0f, 1.0f, WHITE);
        }
    }
}

// SETTER GETTER

void initGameState(GameState *S)
{
    maxEnemy(S) = 5;
    maxEnemyBullet(S) = 5;
    enemyBulletSpeed(S) = 3;
}

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

// EXPLOSIONS
Explosion explosions[MAX_EXPLOSIONS];
Texture2D explosionsTexture;
Sound asteroidDestroyed, userPlaneExplosions;

void CreateExplosion(Vector2 position)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (!explosions[i].active)
        {
            explosions[i].position = position; // Menyesuaikan agar pusat
            // explosions[i].position.y = position.y + 70;
            explosions[i].active = true;
            explosions[i].frame = 0;
            explosions[i].timer = 0;
            break; // Keluar setelah menemukan slot kosong
        }
    }
}

void UpdateExplosions(float deltaTime)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (explosions[i].active)
        {
            explosions[i].timer += deltaTime;
            if (explosions[i].timer > 0.1f)
            { // Ubah frame setiap 0.1 detik
                explosions[i].frame++;
                explosions[i].timer = 0;
            }
            if (explosions[i].frame >= 5)
            { // Misal animasi punya 5 frame
                explosions[i].active = false;
            }
        }
    }
}

void DrawExplosions(Texture2D explosionTexture)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (explosions[i].active)
        {
            Rectangle source = {explosions[i].frame * 64, 0, 64, 64}; // Misal sprite 64x64
            Rectangle dest = {explosions[i].position.x, explosions[i].position.y, 128, 128};
            DrawTexturePro(explosionTexture, source, dest, (Vector2){18, 0}, 0, WHITE);
        }
    }
}

// ASTEROIDS
Asteroid asteroids[MAX_ASTEROIDS];
Texture2D asteroidTexture;
Texture2D hitEffect1;
Texture2D hitEffect2;

void InitAsteroids()
{
    asteroidTexture = LoadTexture("assets/Asteroid.png");
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        asteroids[i].position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 100), GetRandomValue(-300, -50)};
        asteroids[i].size = GetRandomValue(1, 3);
        asteroids[i].speed = (Vector2){GetRandomValue(-1, 1), GetRandomValue(2, 3)};
        asteroids[i].active = true;
    }
}

void UpdateAsteroids()
{
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (asteroids[i].active)
        {
            asteroids[i].position.x += asteroids[i].speed.x;
            asteroids[i].position.y += asteroids[i].speed.y;

            if (asteroids[i].size == 1)
            {
                if (asteroids[i].position.x <= 0 || asteroids[i].position.x >= GAMEPLAY_WIDTH - (asteroidTexture.width * 0.05f))
                {
                    asteroids[i].speed.x *= -1; // Pantulan jika mencapai batas
                }
            }

            if (asteroids[i].size == 2)
            {
                if (asteroids[i].position.x <= 0 || asteroids[i].position.x >= GAMEPLAY_WIDTH - (asteroidTexture.width * 0.07f))
                {
                    asteroids[i].speed.x *= -1; // Pantulan jika mencapai batas
                }
            }

            if (asteroids[i].size == 3)
            {
                if (asteroids[i].position.x <= 0 || asteroids[i].position.x >= GAMEPLAY_WIDTH - (asteroidTexture.width * 0.09f))
                {
                    asteroids[i].speed.x *= -1; // Pantulan jika mencapai batas
                }
            }

            if (asteroids[i].position.y > SCREEN_HEIGHT)
            {
                asteroids[i].position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 100), GetRandomValue(-300, -50)};
            }
        }
    }
}

void CheckCollisions()
{
    Vector2 playerPosition = (Vector2){player.position.x + 175, player.position.y + 140};
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (asteroids[i].active)
        {
            for (int j = 0; j < MAX_BULLETS; j++)
            {
                if (CheckCollisionCircles(playerPosition, 35, asteroids[i].position, asteroids[i].size * 25))
                { // user menabrak asteroid
                    updateNyawa();
                    PlaySound(userPlaneExplosions);
                    CreateExplosion(playerPosition);
                    asteroids[i].active = false;
                    break;
                }
            }
        }
    }
}

void SpawnAsteroid()
{
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (!asteroids[i].active)
        {
            asteroids[i].position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 100), GetRandomValue(-1, -50)};
            asteroids[i].size = GetRandomValue(1, 3);
            asteroids[i].speed = (Vector2){GetRandomValue(-1, 1), GetRandomValue(2, 3)};
            asteroids[i].active = true;
            break;
        }
    }
}

void AsteroidLoop()
{
    static float asteroidSpawnTimer = 0.0f;
    asteroidSpawnTimer += GetFrameTime();

    if (asteroidSpawnTimer >= 0.2f)
    { // Setiap 2 detik, spawn asteroid baru
        SpawnAsteroid();
        asteroidSpawnTimer = 0.0f;
    }
    UpdateAsteroids();
}

void DrawAsteroids()
{
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (asteroids[i].active)
        {
            float scale; // Sesuaikan skala asteroid
            if (asteroids[i].size == 1)
            {
                scale = asteroids[i].size * 0.05f;
            }
            else if (asteroids[i].size == 2)
            {
                scale = asteroids[i].size * 0.035f;
            }
            else if (asteroids[i].size == 3)
            {
                scale = asteroids[i].size * 0.03f;
            }
            DrawTextureEx(asteroidTexture, asteroids[i].position, 0.0f, scale, WHITE);
        }
    }
}

// ENEMY
Enemy enemies[MAX_ENEMIES];
EnemyBullet enemyBullets[MAX_ENEMY_BULLETS];
Sound nging, duar;


void UpdateEnemies(Texture2D EnemyTexture, int xRight, int xLeft, int yPosition, int xPosition, int shootpattern, GameState *S)
{
    float deltaTime = GetFrameTime();
    for (int i = 0; i < getMaxEnemy(S); i++)
    {
        if (enemies[i].isActive)
        {
            if (enemies[i].hitEffectTimer > 0)
            {
                enemies[i].hitEffectTimer -= deltaTime;
                if (enemies[i].hitEffectTimer <= 0)
                {
                    enemies[i].hitEffectTimer = 0;
                }
            }
            enemies[i].position.y += enemies[i].speed.y;
            enemies[i].position.x += enemies[i].speed.x;

            if (enemies[i].position.x <= xRight || enemies[i].position.x >= GAMEPLAY_WIDTH - (EnemyTexture.width * 2.0f) + xLeft)
            {
                enemies[i].speed.x *= -1; // Pantulan jika mencapai batas
            }

            if (enemies[i].position.y > SCREEN_HEIGHT)
            {
                enemies[i].position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 170), GetRandomValue(-200, -150)};
                enemies[i].hasShot = false; // Reset tembakan jika musuh respawn dari atas
            }

            // Jika musuh boleh menembak dan belum menembak sebelumnya, maka tembak
            if (enemies[i].canShoot && !enemies[i].hasShot && GetRandomValue(0, 100) < 2)
            {
                EnemyShoot(EnemyTexture, yPosition, xPosition, shootpattern, S);
            }
        }
    }
}

void SpawnEnemies(int type, int eneLife, GameState *S)
{
    for (int i = 0; i < getMaxEnemy(S); i++)
    {
        if (!enemies[i].isActive)
        {
            if (type == 0)
                enemies[i].speed = (Vector2){GetRandomValue(-2, 2) * 0.5, GetRandomValue(1, 2) * 0.8};
            else if (type == 1)
                enemies[i].speed = (Vector2){1, 1};
            enemies[i].position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 170), GetRandomValue(-200, -150)};
            enemies[i].canShoot = (GetRandomValue(0, 1) == 1);
            enemies[i].health = eneLife;
            enemies[i].isActive = true;
            enemies[i].hasShot = false;
            enemies[i].hitEffectTimer = 0;
            enemies[i].hitEffectFrame = 0;
            break;
        }
    }
}

void EnemiesLoop(int type, int shootpattern, int eneLife, Texture2D EnemyTexture, int xRight, int xLeft, int yPosition, int xPosition, GameState *S)
{
    static float enemiesSpawnTimer = 0.0f;
    enemiesSpawnTimer += GetFrameTime();

    if (enemiesSpawnTimer >= 0.4f)
    { // Setiap 2 detik, spawn asteroid baru
        SpawnEnemies(type, eneLife, S);
        enemiesSpawnTimer = 0.0f;
    }
    UpdateEnemies(EnemyTexture, xRight, xLeft, yPosition, xPosition, shootpattern, S);
}

void DrawEnemies(Texture2D EnemyTexture, Texture2D EnemyDamaged, float scale, int xEffect, int yEffect, GameState *S)
{
    for (int i = 0; i < getMaxEnemy(S); i++)
    {
        if (enemies[i].isActive)
        {
            Texture2D currentTexture = (enemies[i].health <= 2) ? EnemyDamaged : EnemyTexture;
            DrawTextureEx(currentTexture, enemies[i].position, 0.0f, scale, WHITE);
            if (enemies[i].hitEffectTimer > 0)
            {
                Texture2D effect = (enemies[i].hitEffectFrame == 0) ? hitEffect1 : hitEffect2;

                Vector2 effectPosition;
                effectPosition.x = enemies[i].position.x - (effect.width / 2) + xEffect;
                effectPosition.y = enemies[i].position.y - (effect.height / 2) + yEffect;
                DrawTexture(effect, effectPosition.x, effectPosition.y, WHITE);
            }
        }
    }
}

void EnemyShoot(Texture2D EnemyTexture, int yPosition, int xPosition, int shootpattern, GameState *S)
{
    for (int i = 0; i < getMaxEnemy(S); i++)
    {
        if (!enemies[i].isActive || enemies[i].hasShot)
            continue; // Musuh mati tidak boleh menembak
        if (shootpattern == 0)
        {
            for (int j = 0; j < getMaxEnemyBullet(S); j++)
            {
                if (!enemyBullets[j].isActive)
                {
                    enemyBullets[j].position = (Vector2){(enemies[i].position.x - xPosition) + EnemyTexture.width,
                                                         (enemies[i].position.y + EnemyTexture.width + yPosition)};
                    enemyBullets[j].isActive = true;
                    enemyBullets[j].shooterIndex = i; // Tandai peluru ini ditembak oleh musuh ke-i
                    if (GetRandomValue(0, 1) == 0)
                    {
                        enemyBullets[j].speed = (Vector2){0, SPEED_ENEMY_BULLETS}; // Vertikal (ke bawah)
                    }
                    else
                    {
                        int dirX = GetRandomValue(0, 1) == 0 ? -1 : 1;                       // Bisa kiri atau kanan
                        enemyBullets[j].speed = (Vector2){dirX * 2.0f, SPEED_ENEMY_BULLETS}; // Diagonal
                    }
                    enemies[i].hasShot = true;
                    break;
                }
            }
        }
        else if (shootpattern = 1)
        {
            int bulletsSpawned = 0;
            for (int j = 0; j < getMaxEnemyBullet(S) && bulletsSpawned < 4; j++)
            {
                if (!enemyBullets[j].isActive)
                {
                    float offsetX = (bulletsSpawned % 2 == 0) ? -10 : 10; // Geser kiri & kanan
                    float offsetY = (bulletsSpawned < 2) ? 0 : -35;       // Peluru belakang lebih mundur sedikit
                    enemyBullets[j].position = (Vector2){
                        (enemies[i].position.x - xPosition) + EnemyTexture.width + offsetX,
                        (enemies[i].position.y + EnemyTexture.width + yPosition + offsetY)};
                    enemyBullets[j].isActive = true;
                    enemyBullets[j].shooterIndex = i;
                    enemyBullets[j].speed = (Vector2){0, SPEED_ENEMY_BULLETS + 5}; // Lurus ke bawah
                    bulletsSpawned++;
                }
            }
            enemies[i].hasShot = true;
        }
        else if (shootpattern = 2)
        {
            int bulletsSpawned = 0;
            for (int j = 0; j < getMaxEnemyBullet(S) && bulletsSpawned < 3; j++)
            {
                if (!enemyBullets[j].isActive)
                {
                    float offsetX = (bulletsSpawned == 0) ? -15 : (bulletsSpawned == 1) ? 0
                                                                                        : 15;
                    float speedX = (bulletsSpawned == 0) ? -2.0f : (bulletsSpawned == 1) ? 0
                                                                                         : 2.0f;
                    enemyBullets[j].position = (Vector2){(enemies[i].position.x - xPosition) + EnemyTexture.width + offsetX,
                                                         (enemies[i].position.y + EnemyTexture.width + yPosition)};
                    enemyBullets[j].isActive = true;
                    enemyBullets[j].shooterIndex = i;
                    enemyBullets[j].speed = (Vector2){speedX, SPEED_ENEMY_BULLETS}; // Kiri, tengah, kanan
                    bulletsSpawned++;
                }
            }
            enemies[i].hasShot = true;
        }
    }
}

void UpdateEnemyBullets(Texture2D enemyBulletTexture, int type, GameState *S)
{
    int n=getMaxEnemyBullet(S);
    static float delayTimer[MAX_ENEMY_BULLETS] = {0};

    for (int i = 0; i < getMaxEnemyBullet(S); i++)
    {
        if (enemyBullets[i].isActive)
        {
            if (type == 0)
            {
                enemyBullets[i].position.x += enemyBullets[i].speed.x;
                enemyBullets[i].position.y += enemyBullets[i].speed.y;
            }
            else if (type == 1)
            {
                int shooterIdx = enemyBullets[i].shooterIndex; // Ambil index musuh yang menembak

                if (shooterIdx >= 0 && shooterIdx < getMaxEnemy(S))
                {
                    Vector2 shooterPos = enemies[shooterIdx].position; // Posisi musuh

                    if (delayTimer[i] < 2.0f)
                    {
                        // ✅ Selama 2 detik pertama, peluru mengikuti musuh
                        enemyBullets[i].position.x = shooterPos.x + (enemyBulletTexture.width / 2) + 71;
                        enemyBullets[i].position.y = shooterPos.y + enemyBulletTexture.height + 185;

                        delayTimer[i] += GetFrameTime();

                        if (delayTimer[i] < 2.0f && !enemyBullets[i].hasPlayedSound)
                        {
                            SetSoundVolume(nging, 3.0f);
                            PlaySound(nging);                      // Suara "nging"
                            enemyBullets[i].hasPlayedSound = true; // Tandai agar tidak diulang
                        }
                    }

                    // ✅ Setelah 2 detik, mulai tembakan cepat dan mainkan suara "duar"
                    if (delayTimer[i] >= 2.0f)
                    {
                        if (!enemyBullets[i].hasPlayedDuar) // ✅ Pastikan "duar" hanya dimainkan sekali
                        {
                            PlaySound(duar);
                            enemyBullets[i].hasPlayedDuar = true; // Tandai sudah dimainkan
                        }
                        enemyBullets[i].speed.y = SPEED_ENEMY_BULLETS * 10.0f;
                        enemyBullets[i].position.y += enemyBullets[i].speed.y;

                        if (enemyBullets[i].position.y > 960)
                        {
                            delayTimer[i] = 0;
                            enemyBullets[i].hasPlayedSound = false; // Reset untuk peluru berikutnya
                        }
                    }
                }
            }

            if (enemyBullets[i].position.x <= -15 || enemyBullets[i].position.x >= GAMEPLAY_WIDTH - (enemyBulletTexture.width * 0.8f) + 15)
            {
                enemyBullets[i].speed.x *= -1;
            }

            if (enemyBullets[i].position.y > 960)
            {
                enemyBullets[i].isActive = false;
                enemyBullets[i].hasPlayedSound = false;
                enemyBullets[i].hasPlayedDuar = false;
                delayTimer[i] = 0;
                enemyBullets[i].shooterIndex = -1;
            }
        }
    }
}

void DrawEnemyBullets(Texture2D enemyBulletTexture, float scale, GameState *S)
{

    for (int i = 0; i < getMaxEnemyBullet(S); i++)
    {
        if (enemyBullets[i].isActive)
        {
            DrawTextureEx(enemyBulletTexture, enemyBullets[i].position, 0.0f, scale, WHITE);
        }
    }
}

void CheckEnemyCollisions(int xEnemy, int yEnemy, int radiusPlayer, int radiusBulletEnemy, GameState *S)
{
    Vector2 playerPosition = (Vector2){player.position.x + 185, player.position.y + 150};

    for (int i = 0; i < getMaxEnemy(S); i++)
    {
        if (!enemies[i].isActive)
            continue; // Lewati musuh yang sudah mati

        Vector2 enemiesPosition = (Vector2){enemies[i].position.x + xEnemy, enemies[i].position.y + yEnemy};

        // Cek tabrakan dengan peluru pemain (Musuh tertembak)
        for (int j = 0; j < MAX_BULLETS; j++)
        {
            if (bullets[j].active && CheckCollisionCircles(enemiesPosition, radiusPlayer, bullets[j].position, radiusBulletEnemy))
            {
                enemies[i].health--;
                enemies[i].hitEffectTimer = 0.15f; // Waktu efek aktif
                enemies[i].hitEffectFrame = (enemies[i].health % 2);
                bullets[j].active = false;
                if (enemies[i].health <= 0)
                {
                    enemies[i].isActive = false;
                    enemies[i].hitEffectTimer = 0;
                    enemies[i].hitEffectFrame = 0;
                    updateScore(); // Matikan musuh
                    PlaySound(asteroidDestroyed);
                    CreateExplosion(enemiesPosition);
                    break; // Hindari multiple hits dalam satu frame
                }
            }
        }

        // Cek tabrakan antara musuh dan pemain (Player menabrak musuh)
        if (CheckCollisionCircles(playerPosition, 45, enemiesPosition, 35))
        {
            updateNyawa();
            PlaySound(userPlaneExplosions);
            CreateExplosion(playerPosition);
            enemies[i].isActive = false; // Hancurkan musuh
            break;
        }

        // Cek tabrakan antara peluru musuh dan pemain (Player tertembak)
        for (int k = 0; k < getMaxEnemyBullet(S); k++)
        {
            if (enemyBullets[k].isActive && CheckCollisionCircles(enemyBullets[k].position, 5, playerPosition, 45))
            {
                enemyBullets[k].isActive = false;
                PlaySound(asteroidDestroyed);
                CreateExplosion(playerPosition);
                updateNyawa();
                break;
            }
        }
    }
}

void ResetEnemies(GameState *S)
{
    for (int i = 0; i < getMaxEnemy(S); i++)
    {
        enemies[i].isActive = false;
    }
}

void ResetEnemyBullets(GameState *S)
{
    for (int i = 0; i < getMaxEnemyBullet(S); i++)
    {
        enemyBullets[i].isActive = false;
    }
}

Texture2D enemyLvl5, enemyLvl6, enemyBulletLv3, enePurpleDamaged;
// ASSETS
void LoadAssets()
{
    initBG();
    shootSound = LoadSound("assets/shoot.wav");
    nging = LoadSound("assets/nging.wav");
    duar = LoadSound("assets/duar.wav");
    bulletTexture = LoadTexture("assets/bullet.png");
    explosionsTexture = LoadTexture("assets/Explosions.png");
    asteroidDestroyed = LoadSound("assets/asteroidDestroyed.wav");
    userPlaneExplosions = LoadSound("assets/userPlaneExplosion.wav");
    hitEffect1 = LoadTexture("assets/efekTembakan1.png");
    hitEffect2 = LoadTexture("assets/efekTembakan2.png");
    enemylvl1 = LoadTexture("assets/musuh.png");
    enemyBulletlv1 = LoadTexture("assets/enemyBullet.png");
    enemyLvl5 = LoadTexture("assets/enemyLvl5.png");
    enemyLvl6 = LoadTexture("assets/enePurple.png");
    enemyBulletLv3 = LoadTexture("assets/laserUfo.png");
    enePurpleDamaged = LoadTexture("assets/enePurpleDamaged.png");
}

// GAMEPLAY

void DrawLvl1()
{
    DrawLayout();
    DrawPlayer();
    DrawBullets();
    DrawExplosions(explosionsTexture);
    DrawEnemies(enemylvl1, enemylvl1, 2.0f, 80, 110, &gamestate);
    DrawEnemyBullets(enemyBulletlv1, 0.8f, &gamestate);
    tampilspark();
}



void DrawLvl5()
{
    DrawLayout();
    DrawPlayer();
    DrawBullets();
    DrawExplosions(explosionsTexture);
    DrawEnemies(enemyLvl5, enemyLvl5, 1.2f, 80, 110, &gamestate);
    DrawEnemyBullets(enemyBulletlv1, 0.8f, &gamestate);
    tampilspark();
}

void DrawLvl6()
{
    DrawLayout();
    DrawPlayer();
    DrawBullets();
    DrawExplosions(explosionsTexture);
    DrawEnemies(enemyLvl6, enePurpleDamaged, 3.0f, 150, 200, &gamestate);
    DrawEnemyBullets(enemyBulletLv3, 1.3, &gamestate);
    tampilspark();
}
void level6(float deltaTime)
{
    UpdatePlayer();
    UpdateShooting(deltaTime);
    UpdateBullets();
    UpdateExplosions(deltaTime);
    UpdateEnemies(enemyLvl6, -90, 10, 150, -5, 0, &gamestate);
    UpdateEnemyBullets(enemyBulletLv3, 1, &gamestate);
    CheckEnemyCollisions(100, 140, 40, 10, &gamestate);
    EnemiesLoop(1, 0, 4, enemyLvl6, -90, 10, 150, -5, &gamestate);
    DrawLvl6();
    inipowerup();
    UpdateSpark();
}

void DrawLvl4()
{
    DrawLayout();
    DrawPlayer();
    DrawBullets();
    DrawBosses();
    DrawBossShoot();
}

void level1(float deltaTime)
{
    UpdatePlayer();
    UpdateShooting(deltaTime);
    UpdateBullets();
    UpdateExplosions(deltaTime);
    UpdateEnemies(enemylvl1, -85, 85, 20, 30, 0, &gamestate);
    UpdateEnemyBullets(enemyBulletlv1, 0, &gamestate);
    CheckEnemyCollisions(95, 70, 35, 5, &gamestate);
    EnemiesLoop(0, 0, 1, enemylvl1, -85, 85, 20, 30, &gamestate);
    DrawLvl1();
    inipowerup();
    UpdateSpark();
}
void level2()
{
    DrawAsteroids();
    CheckCollisions();
    AsteroidLoop();
}
void level4(float deltaTime)
{
    BossMov();
    UpdatePlayer();
    UpdateBulletBoss();
    UpdateShooting(deltaTime);
    UpdateBullets();
    DrawLvl4();
    inipowerup();
    UpdateSpark();
}
void level5(float deltaTime)
{
    UpdatePlayer();
    UpdateShooting(deltaTime);
    UpdateBullets();
    UpdateExplosions(deltaTime);
    UpdateEnemies(enemyLvl5, -35, 125, 20, 30, 1, &gamestate);
    UpdateEnemyBullets(enemyBulletlv1, 0, &gamestate);
    CheckEnemyCollisions(60, 50, 40, 10, &gamestate);
    EnemiesLoop(0, 1, 3, enemyLvl5, -35, 125, 20, 30, &gamestate);
    DrawLvl5();
    inipowerup();
    UpdateSpark();
}

int previousLevel = 0;
void game()
{
    initGameState(&gamestate);
    float deltaTime = GetFrameTime();
    bool isGameOver = false; // Pastikan inisialisasi benar

    if (InfoPlayer.nyawa <= 0)
    {
        isGameOver = true;
    }

    if (!isGameOver)
    {
        // Tentukan level berdasarkan skor
        if (InfoPlayer.score < 30)
        {
            level = 1;
        }
        else if (InfoPlayer.score >= 30 && InfoPlayer.score < 40)
        {
            level = 2;
        }
        else if (InfoPlayer.score >= 40 && InfoPlayer.score < 60)
        {
            level = 3;
        }
        else if (InfoPlayer.score >= 60 && InfoPlayer.score < 80)
        {
            level = 4;
        }
        else if (InfoPlayer.score >= 80 && InfoPlayer.score < 100)
        {
            level = 5;
        }
        else if (InfoPlayer.score >= 100 && InfoPlayer.score < 120)
        {
            level = 6;
        }

        // Jika level berubah, reset musuh dan peluru
        if (level != previousLevel)
        {
            ResetEnemies(&gamestate);
            ResetEnemyBullets(&gamestate);
            previousLevel = level; // Simpan level baru sebagai level sebelumnya
        }

        // Jalankan level yang sesuai
        switch (level)
        {
        case 1:
            level6(deltaTime);
            level2();
            break;
        case 2:
            level3(deltaTime);
            break;
        case 3:
            level3(deltaTime);
            break;
        case 4:
            level1(deltaTime);
            break;
        case 5:
            level4(deltaTime);
            break;
        }
    }
    else
    {
        gameover();
    }
}

// UNLOAD
void UnloadAssets()
{
    UnloadTexture(player.texture);
    UnloadTexture(explosionsTexture);
    UnloadTexture(asteroidTexture);
    UnloadSound(asteroidDestroyed);
    UnloadSound(userPlaneExplosions);
    UnloadSound(shootSound);
    UnloadTexture(hitEffect1);
    UnloadTexture(hitEffect2);
}