#include "raylib.h"
#include "fawwaz.h"
#include "hasbi.h"

// Menyesuaikan layar dengan laptop user
Bosses bosses;
BossLaser bossLaser;
Sound laserSound;

void InitBosses()
{
    bosses.position = (Vector2){70, 300};
    bosses.aktif = true;
    bosses.texture = LoadTexture("assets/bossesTest.png");
    bossLaser.active = false;
    bossLaser.timer = 0.0f;
    bossLaser.cooldown = 3.0f;
    bossLaser.animationTimer = 0.0f;
    bossLaser.currentFrame = 0;
    bosses.health = 3;
    bossLaser.length = 720;

    // Load dua gambar laser
    bossLaser.textures[0] = LoadTexture("assets/laser1.png");
    bossLaser.textures[1] = LoadTexture("assets/laser2.png");
    bossLaser.textures[2] = LoadTexture("assets/laser3.png");
    laserSound = LoadSound("assets/bossLaser.wav");
}

void DrawBosses()
{
    if (bosses.aktif)
    {
        float scale = 12.0; // Skala 800% dari ukuran aslinya
        DrawTextureEx(bosses.texture, bosses.position, 0.0f, scale, WHITE);
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
    static int state = 0;
    static float lastStateChange = 0.0f;
    const float stateDuration = 1.6f;
    float speed = 130.0f * GetFrameTime(); // Sesuaikan kecepatan dengan frame rate

    float currentTime = GetTime();
    if (bosses.aktif)
    {
        // Periksa apakah sudah waktunya pindah state
        if (currentTime - lastStateChange >= stateDuration)
        {
            state = (state + 1) % 4; // Looping antar state
            lastStateChange = currentTime;

            // Debugging: Tampilkan perubahan state di terminal
        }

        // Gerakan sesuai state saat ini
        switch (state)
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
