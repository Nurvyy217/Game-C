#include "raylib.h"
#include "fawwaz.h"
#include "hasbi.h"
#include "supriadi.h"

Bosses bosses;
BossLaser bossLaser;
Sound laserSound;
Star stars[MAX_STARS];
Texture2D BD1, BD2, BD3, RB, RB1, RB2, RB3, BDef;
Music bossbgm;
float timerExp = 0;
float timerExp2 = 0;
float flashTimer = 0.0f;
bool isFlashing = false;

void InitBosses()
{

    // Load texture dari boss
    bosses.texture = LoadTexture("assets/bossesTest.png");
    BD1 = LoadTexture("assets/bossesBroken1.png");
    BD2 = LoadTexture("assets/bossesBroken2.png");
    BD3 = LoadTexture("assets/bossesBroken3.png");
    RB = LoadTexture("assets/rageBoss.png");
    RB1 = LoadTexture("assets/rageBossBroken1.png");
    RB2 = LoadTexture("assets/rageBossBroken2.png");
    RB3 = LoadTexture("assets/rageBossBroken3.png");
    BDef = LoadTexture("assets/bossesDefeat.png");

    // Load dua gambar laser
    bossLaser.textures[0] = LoadTexture("assets/laser1.png");
    bossLaser.textures[1] = LoadTexture("assets/laser2.png");
    bossLaser.textures[2] = LoadTexture("assets/laser3.png");

    // Load BGM dari boss
    laserSound = LoadSound("assets/bossLaser.wav");
}

static int states;
static float lastStateChange;
void InitialBoss()
{
    if (bosses.aktif == false)
    {
        bosses.position = (Vector2){70, 300};
        bosses.aktif = true;
        bossLaser.active = false;
        bosses.bossRage = false;
        bossLaser.timer = 0.0f;
        bossLaser.cooldown = 3.0f;
        bossLaser.animationTimer = 0.0f;
        bossLaser.currentFrame = 0;
        bosses.health = 300;
        bosses.maxHealth = 300;
        bossLaser.length = 720;
        bosses.hitEffectFrame = 0;
        bosses.hitEffectTimer = 0;
        bosses.theEnd = false;
        states = 0;
        lastStateChange = 0.0f;
    }
}

void InitStar()
{
    for (int i = 0; i < MAX_STARS; i++)
    {
        stars[i].position = (Vector2){GetRandomValue(0, 720), GetRandomValue(0, 960)};
        stars[i].speed = GetRandomValue(50, 200) / 100.0f;
        stars[i].size = GetRandomValue(0.1, 2.2);
    }
}

void UpdateStar()
{
    for (int i = 0; i < MAX_STARS; i++)
    {
        stars[i].position.y += stars[i].speed;
        if (stars[i].position.y > 960)
        {
            stars[i].position.y = 0;
            stars[i].position.x = GetRandomValue(0, 720);
            stars[i].size = GetRandomValue(0.1, 2.2);
        }
    }
}

void DrawStar()
{

    for (int i = 0; i < MAX_STARS; i++)
    {
        DrawCircleV(stars[i].position, stars[i].size, WHITE);
    }
}

void DrawBosses(GameState *S)
{
    if (bosses.aktif)
    {
        float scale = 12.0; // Skala 800% dari ukuran aslinya
        Texture2D currentBossTexture = bosses.texture;

        if (bosses.health <= 250 && bosses.health > 225)
        {

            currentBossTexture = BD1;
        }
        else if (bosses.health <= 225 && bosses.health > 180)
        {
            bosses.bossRage = true;
            currentBossTexture = RB1;
        }
        else if (bosses.health <= (bosses.maxHealth * 0.6) && bosses.health > 150)
        {
            bosses.bossRage = false;
            ResetAsteroid(S);
            currentBossTexture = BD2;
        }
        else if (bosses.health <= (bosses.maxHealth * 0.5) && bosses.health > 120)
        {
            bosses.bossRage = true;
            currentBossTexture = RB2;
        }
        else if (bosses.health <= (bosses.maxHealth * 0.4) && bosses.health > 60)
        {
            bosses.bossRage = false;
            ResetAsteroid(S);
            currentBossTexture = BD3;
        }
        else if (bosses.health <= (bosses.maxHealth * 0.2) && bosses.health > 0)
        {
            bosses.bossRage = true;
            currentBossTexture = RB3;
        }
        else if (bosses.health <= 0)
        {
            bosses.bossRage = false;
            ResetAsteroid(S);
            bosses.defeat = true;
            currentBossTexture = BDef;
        }

        DrawTextureEx(currentBossTexture, bosses.position, 0.0f, scale, WHITE);
        if (bosses.hitEffectTimer > 0)
        {
            Texture2D effect = (bosses.hitEffectFrame == 0) ? hitEffect1 : hitEffect2;

            Vector2 effectPosition;
            effectPosition.x = bosses.position.x - (effect.width / 2) + 160;
            effectPosition.y = bosses.position.y - (effect.height / 2) + 180;
            DrawTextureEx(effect, effectPosition, 0.0f, 2.5f, WHITE);
        }
    }
}

void ShootBossLaser()
{
    if (!bossLaser.active && bossLaser.cooldown <= 0)
    { // Cek cooldown sebelum bisa nembak
        bossLaser.active = true;
        PlaySound(laserSound);
        bossLaser.timer = 2.0f;    // Laser aktif selama 3 detik
        bossLaser.cooldown = 5.0f; // Setelah tembakan, cooldown 5 detik
    }
}

void UpdateBossLaser()
{
    if (bossLaser.active)
    {
        // Laser tetap berada di depan mata boss
        bossLaser.position.x = bosses.position.x + (bosses.texture.width * 6.0f) - 8;
        bossLaser.position.y = bosses.position.y + (bosses.texture.height * 6.0f) - 22;

        // Panjang laser agar selalu menyentuh batas bawah layar
        bossLaser.length = 960 - bossLaser.position.y;

        // Kurangi timer, jika habis maka laser mati
        bossLaser.timer -= GetFrameTime();
        if (bossLaser.timer <= 0)
        {
            bossLaser.active = false;
            StopSound(laserSound);
        }

        // Animasi laser (ganti sprite setiap 0.1 detik)
        bossLaser.animationTimer += GetFrameTime();
        if (bossLaser.animationTimer >= 0.1f)
        {
            bossLaser.currentFrame = (bossLaser.currentFrame + 1) % 3;
            bossLaser.animationTimer = 0.0f;
        }
    }

    // Cooldown setelah menembak
    if (bossLaser.cooldown > 0)
    {
        bossLaser.cooldown -= GetFrameTime();
    }
}

void DrawBossLaser()
{
    if (bossLaser.active)
    {
        DrawTexturePro(
            bossLaser.textures[bossLaser.currentFrame],
            (Rectangle){0, 0, bossLaser.textures[bossLaser.currentFrame].width, bossLaser.textures[bossLaser.currentFrame].height},
            (Rectangle){bossLaser.position.x, bossLaser.position.y, bossLaser.textures[bossLaser.currentFrame].width * 1.2f, bossLaser.length},
            (Vector2){0, 0},
            0.0f,
            WHITE);
    }
}

void BossMov()
{
    const float stateDuration = 1.6f;
    float deltaTime = GetFrameTime();
    float speed = 130.0f * GetFrameTime(); // Sesuaikan kecepatan dengan frame rate

    float currentTime = GetTime();

    if (bosses.aktif)
    {
        if (bosses.hitEffectTimer > 0)
        {
            bosses.hitEffectTimer -= deltaTime;
            if (bosses.hitEffectTimer <= 0)
            {
                bosses.hitEffectTimer = 0;
            }
        }
        // Periksa apakah sudah waktunya pindah state
        if (currentTime - lastStateChange >= stateDuration)
        {
            states = (states + 1) % 4; // Looping antar state
            lastStateChange = currentTime;

            // Debugging: Tampilkan perubahan state di terminal
        }

        // Gerakan sesuai state saat ini
        if (bosses.health > 0)
        {
            switch (states)
            {
            case 0: // Maju kanan-bawah
                bosses.position.x += speed;
                bosses.position.y += speed;
                break;

            case 1: // Mundur kanan-atas
                bosses.position.x += speed;
                bosses.position.y -= speed;
                break;

            case 2: // Mundur kiri-atas
                bosses.position.x -= speed;
                bosses.position.y -= speed;
                break;

            case 3: // Maju kiri-bawah
                bosses.position.x -= speed;
                bosses.position.y += speed;
                break;
            }
        }
    }
}

void CheckBossCollisions(GameState *S)
{
    // Player terkena laser boss
    if (bossLaser.active)
    {
        Rectangle laserHitbox = {
            bossLaser.position.x - 330,
            bossLaser.position.y,
            // bossLaser.textures[bossLaser.currentFrame].width * 1.2f,
            5,
            bossLaser.length};

        Rectangle playerHitbox = {
            player.position.x - 140,
            player.position.y - 100,
            40,
            30};

        if (CheckCollisionRecs(laserHitbox, playerHitbox))
        {
            updateNyawa(S);
        }
    }

    // Player menabrak boss
    Rectangle bossHitbox = {
        bosses.position.x + 50,
        bosses.position.y,
        bosses.texture.width * 11.0f,
        bosses.texture.height * 12.0f};

    Rectangle playerHitbox = {
        player.position.x + 250,
        player.position.y + 150,
        30,
        30};

    if (CheckCollisionRecs(bossHitbox, playerHitbox))
    {
        updateNyawa(S);
    }

    // Peluru player mengenai boss
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            Rectangle bulletHitbox = {
                bullets[i].position.x,
                bullets[i].position.y,
                20,
                10};

            if (CheckCollisionRecs(bulletHitbox, bossHitbox))
            {
                bosses.health -= 1;        // Boss kehilangan 1 HP per tembakan
                bullets[i].active = false; // Nonaktifkan peluru setelah kena
                bosses.hitEffectTimer = 0.15f;
                bosses.hitEffectFrame = (bosses.health % 2);
                if (bosses.health <= 0)
                {
                    bossLaser.active = false;
                    bossLaser.cooldown = 10000;
                    bosses.hitEffectTimer = 0;
                    bosses.hitEffectFrame = 0;
                }
            }
        }
    }
}

void BossBar()
{
    float healthBarWidth = 400 * ((float)bosses.health / bosses.maxHealth);
    if (bosses.health > 0)
    {
        DrawRectangle(60, 50, 400, 20, RED);
        DrawRectangle(60, 50, healthBarWidth, 20, GREEN);
        DrawText(TextFormat("Xaeph, The Last Alien"), 150, 80, 20, WHITE);
    }
}

void BossRage(GameState *S)
{
    if (bosses.bossRage == true)
    {
        callAsteroid(S);
    }
}

// void BossExplosions()
// {
//     if (bosses.defeat == true)
//     {
//         timerExp += GetFrameTime();
//         if (timerExp >= GetRandomValue(1,3) * 0.1f)
//         {
//                 Vector2 posisi = {explosions->position.x = GetRandomValue( 0, 540), explosions->position.y = GetRandomValue(0, 240)};
//                 CreateExplosion(posisi);
//                 timerExp = 0;
//             }
//     }
// }

void BossExplosions()
{
    float dt = GetFrameTime();
    int jmlledakan = 2;
    if (bosses.defeat)
    {
        timerExp += GetFrameTime();
        timerExp2 += GetFrameTime();
        if (timerExp >= 0.15f && timerExp2 < 5.0f)
        {
            for (int i = 0; i < jmlledakan; i++)
            {
                Vector2 posisi1 = {explosions->position.x = GetRandomValue(bosses.position.x - 100, bosses.position.x + 350), explosions->position.y = GetRandomValue(bosses.position.y - 100, bosses.position.y + 250)};
                CreateExplosion(posisi1);
                PlaySound(asteroidDestroyed);
            }
            timerExp = 0;
        }
        if (bosses.destroyTime >= 0.0f)
        {
            bosses.destroyTime += dt; // Tambah waktu berjalan

            if (bosses.destroyTime >= 5.0f)
            {                         // Jika sudah 5 detik
                bosses.aktif = false; // Matikan musuh
                StopMusicStream(bossbgm);
                bosses.theEnd = true;
            }
        }
    }
}

void InitBGM()
{
    bossbgm = LoadMusicStream("assets/vordt.mp3"); // Load BGM
    PlayMusicStream(bossbgm);                      // Mulai memutar musik
}

void UpdateBGM()
{
    UpdateMusicStream(bossbgm);
}

void UnloadBGM()
{
    UnloadMusicStream(bossbgm);
}
