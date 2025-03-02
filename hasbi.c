#include "hasbi.h"
#include "supriadi.h"
#include <stdio.h>

// LOADING SCREEN
void DrawLayout();
void initTextures();
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
void UpdateEnemies();
void DrawEnemies();
void EnemyShoot();
void UpdateEnemyBullets();
void DrawEnemyBullets();
void CheckEnemyCollisions();

// ASSSETS
void LoadAssets();

// GAMEPLAY
void DrawGameplay();

// UNLOAD
void UnloadAssets();

// LOADING SCREEN
void DrawLayout()
{
    ClearBackground(RAYWHITE);

    // Gameplay area (3/4 of screen, left part)
    DrawRectangle(0, 0, GAMEPLAY_WIDTH, SCREEN_HEIGHT, DARKBLUE);
    DrawText("Gameplay Area", GAMEPLAY_WIDTH / 2 - 60, SCREEN_HEIGHT / 2, 20, RAYWHITE);

    // Menu area (1/4 of screen, right part)
    DrawRectangle(GAMEPLAY_WIDTH, 0, MENU_WIDTH, SCREEN_HEIGHT, DARKGRAY);
    DrawText("Menu Area", GAMEPLAY_WIDTH + MENU_WIDTH / 2 - 40, SCREEN_HEIGHT / 2, 20, RAYWHITE);

    // Separator line
    DrawLine(GAMEPLAY_WIDTH, 0, GAMEPLAY_WIDTH, SCREEN_HEIGHT, BLACK);
}

Texture2D logoDeveloper;
Texture2D gameNamePhoto;
bool texturesLoaded = false; // Cek apakah gambar sudah di-load
bool isLoadingDone = false;

void initTextures()
{
    if (!texturesLoaded)
    { // Load hanya sekali
        logoDeveloper = LoadTexture("assets/logoDeveloper.png");
        gameNamePhoto = LoadTexture("assets/gameNamePhoto.png");
        texturesLoaded = true;
    }
}

void loadingAnimation()
{
    static float opacity = 1.0f;
    static int section = 0;
    static float timer = 0.0f;

    initTextures();

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
        asteroids[i].health = asteroids[i].size;
        asteroids[i].speed = (Vector2){GetRandomValue(-2, 2), GetRandomValue(2, 4)};
        asteroids[i].active = true;
        asteroids[i].hitEffectTimer = 0;
        asteroids[i].hitEffectFrame = 0;
    }
}

void UpdateAsteroids()
{
    float deltaTime = GetFrameTime();
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (asteroids[i].active)
        {
            if (asteroids[i].hitEffectTimer > 0)
            {
                asteroids[i].hitEffectTimer -= deltaTime;
                if (asteroids[i].hitEffectTimer <= 0)
                {
                    asteroids[i].hitEffectTimer = 0;
                }
            } // Hilangkan efek setelah waktu habis
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
                asteroids[i].health = asteroids[i].size;
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
                if (bullets[j].active &&
                    CheckCollisionCircles(asteroids[i].position, asteroids[i].size * 25, bullets[j].position, 5))
                { // asteroid ditembak
                    bullets[j].active = false;
                    asteroids[i].health--;
                    asteroids[i].hitEffectTimer = 0.15f; // Waktu efek aktif
                    asteroids[i].hitEffectFrame = (asteroids[i].health % 2);
                    if (asteroids[i].health <= 0)
                    {
                        PlaySound(asteroidDestroyed);
                        CreateExplosion(asteroids[i].position);
                        asteroids[i].active = false;
                        asteroids[i].hitEffectTimer = 0;
                        asteroids[i].hitEffectFrame = 0;
                    }
                }
                if (CheckCollisionCircles(playerPosition, 35, asteroids[i].position, asteroids[i].size * 25))
                { // user menabrak asteroid
                    updateNyawa();
                    PlaySound(userPlaneExplosions);
                    CreateExplosion(playerPosition);
                    asteroids[i].active = false;
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
            asteroids[i].health = asteroids[i].size;
            asteroids[i].speed = (Vector2){GetRandomValue(-2, 2), GetRandomValue(2, 4)};
            asteroids[i].active = true;
            asteroids[i].hitEffectTimer = 0;
            asteroids[i].hitEffectFrame = 0;
            break;
        }
    }
}

void AsteroidLoop()
{
    static float asteroidSpawnTimer = 0.0f;
    asteroidSpawnTimer += GetFrameTime();

    if (asteroidSpawnTimer >= 2.0f)
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
            if (asteroids[i].hitEffectTimer > 0)
            {
                Texture2D effect = (asteroids[i].hitEffectFrame == 0) ? hitEffect1 : hitEffect2;

                Vector2 effectPosition;
                effectPosition.x = asteroids[i].position.x - (effect.width / 2) + (asteroids[i].size * 25) - 20;
                effectPosition.y = asteroids[i].position.y - (effect.height / 2) + (asteroids[i].size * 25) + 10;

                DrawTexture(effect, effectPosition.x, effectPosition.y, WHITE);
            }
        }
    }
}



// ENEMY
Texture2D enemyTexture, enemyBulletTexture;
Enemy enemies[MAX_ENEMIES];
EnemyBullet enemyBullets[MAX_ENEMY_BULLETS];

void InitEnemies()
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i].position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 150), GetRandomValue(-400, -100)};
        enemies[i].speed = (Vector2){GetRandomValue(-2, 2) * 0.5, GetRandomValue(1, 2) * 0.4};
        enemies[i].isActive = true;
        enemies[i].canShoot = (GetRandomValue(0, 1) == 1);
        enemies[i].hasShot = false;
    }
}

void UpdateEnemies()
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].isActive)
        {
            enemies[i].position.y += enemies[i].speed.y;
            enemies[i].position.x += enemies[i].speed.x;

            if (enemies[i].position.x <= -85 || enemies[i].position.x >= GAMEPLAY_WIDTH - (enemyTexture.width * 2.0f) + 85)
            {
                enemies[i].speed.x *= -1; // Pantulan jika mencapai batas
            }

            if (enemies[i].position.y > SCREEN_HEIGHT)
            {
                enemies[i].position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 170), GetRandomValue(-300, -50)};
                enemies[i].hasShot = false; // Reset tembakan jika musuh respawn dari atas
            }

            // Jika musuh boleh menembak dan belum menembak sebelumnya, maka tembak
            if (enemies[i].canShoot && !enemies[i].hasShot && GetRandomValue(0, 100) < 2)
            {
                EnemyShoot();
            }
        }
    }
}


void SpawnEnemies()
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemies[i].isActive)
        {
            enemies[i].position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 170), GetRandomValue(-150, -100)};
            enemies[i].speed = (Vector2){GetRandomValue(-2, 2) * 0.5, GetRandomValue(1, 2) * 0.4};
            enemies[i].canShoot = (GetRandomValue(0, 1) == 1);
            enemies[i].isActive = true;
            enemies[i].hasShot = false;
            break;
        }
    }
}

void EnemiesLoop()
{
    static float enemiesSpawnTimer = 0.0f;
    enemiesSpawnTimer += GetFrameTime();

    if (enemiesSpawnTimer >= 0.5f)
    { // Setiap 2 detik, spawn asteroid baru
        SpawnEnemies();
        enemiesSpawnTimer = 0.0f;
    }
    UpdateEnemies();
}

void DrawEnemies()
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].isActive)
        {
            DrawTextureEx(enemyTexture, enemies[i].position, 0.0f, 2.0f, WHITE);
        }
    }
}

void EnemyShoot()
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemies[i].isActive)
            continue; // Musuh mati tidak boleh menembak

        for (int j = 0; j < MAX_ENEMY_BULLETS; j++)
        {
            if (!enemyBullets[j].isActive)
            {
                enemyBullets[j].position = (Vector2){(enemies[i].position.x - 30) + enemyTexture.width,
                                                     (enemies[i].position.y + enemyTexture.width + 20)};
                enemyBullets[j].isActive = true;
                enemyBullets[j].shooterIndex = i; // Tandai peluru ini ditembak oleh musuh ke-i
                if (GetRandomValue(0, 1) == 0)
                {
                    enemyBullets[j].speed = (Vector2){0, SPEED_ENEMY_BULLETS}; // Vertikal (ke bawah)
                }
                else
                {
                    int dirX = GetRandomValue(0, 1) == 0 ? -1 : 1; // Bisa kiri atau kanan
                    enemyBullets[j].speed = (Vector2){dirX * 2.0f, SPEED_ENEMY_BULLETS}; // Diagonal
                }
                enemies[i].hasShot = true;
                break;
            }
        }
    }
}

void UpdateEnemyBullets()
{
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++)
    {
        if (enemyBullets[i].isActive)
        {
            enemyBullets[i].position.x += enemyBullets[i].speed.x;
            enemyBullets[i].position.y += enemyBullets[i].speed.y;
            if (enemyBullets[i].position.x <= -15 || enemyBullets[i].position.x >= GAMEPLAY_WIDTH - (enemyBulletTexture.width * 0.8f) + 15)
            {
                enemyBullets[i].speed.x *= -1; // Pantulan jika mencapai batas
            }
            enemyBullets[i].shooterIndex = -1;
            if (enemyBullets[i].position.y > 960)
            {
                enemyBullets[i].isActive = false;
            }
        }
    }
}

void DrawEnemyBullets()
{
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++)
    {
        if (enemyBullets[i].isActive)
        {
            DrawTextureEx(enemyBulletTexture, enemyBullets[i].position, 0.0f, 0.8f, WHITE);
        }
    }
}

void CheckEnemyCollisions()
{
    Vector2 playerPosition = (Vector2){player.position.x + 185, player.position.y + 150};

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemies[i].isActive)
            continue; // Lewati musuh yang sudah mati

        Vector2 enemiesPosition = (Vector2){enemies[i].position.x + 95, enemies[i].position.y + 70};

        // Cek tabrakan dengan peluru pemain (Musuh tertembak)
        for (int j = 0; j < MAX_BULLETS; j++)
        {
            if (bullets[j].active && CheckCollisionCircles(enemiesPosition, 35, bullets[j].position, 5))
            {
                bullets[j].active = false;
                enemies[i].isActive = false; 
                updateScore();// Matikan musuh
                PlaySound(asteroidDestroyed);
                CreateExplosion(enemiesPosition);

                // Hapus semua peluru yang ditembak musuh ini
                for (int k = 0; k < MAX_ENEMY_BULLETS; k++)
                {
                    if (enemyBullets[k].shooterIndex == i)
                    {
                        enemyBullets[k].isActive = false;
                    }
                }

                break; // Hindari multiple hits dalam satu frame
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
        if (CheckCollisionCircles(playerPosition, 30, powerup.posisi, 30)){
            powerup.active = false;
            InfoPlayer.nyawa = InfoPlayer.nyawa + 10;
        }

        // Cek tabrakan antara peluru musuh dan pemain (Player tertembak)
        for (int k = 0; k < MAX_ENEMY_BULLETS; k++)
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

// ASSETS
void LoadAssets()
{
    shootSound = LoadSound("assets/shoot.wav");
    bulletTexture = LoadTexture("assets/bullet.png");
    explosionsTexture = LoadTexture("assets/Explosions.png");
    asteroidDestroyed = LoadSound("assets/asteroidDestroyed.wav");
    userPlaneExplosions = LoadSound("assets/userPlaneExplosion.wav");
    hitEffect1 = LoadTexture("assets/efekTembakan1.png");
    hitEffect2 = LoadTexture("assets/efekTembakan2.png");
    enemyTexture = LoadTexture("assets/musuh.png");
    enemyBulletTexture = LoadTexture("assets/enemyBullet.png");
}

// GAMEPLAY
void DrawGameplay()
{
    DrawLayout();
    DrawPlayer();
    DrawBullets();
    DrawAsteroids();
    DrawExplosions(explosionsTexture);
    DrawEnemies();
    DrawEnemyBullets();
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