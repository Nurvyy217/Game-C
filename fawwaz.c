#include "raylib.h"
#include "fawwaz.h"

// Menyesuaikan layar dengan laptop user
Bosses bosses;

void InitBosses() {
    bosses.position = (Vector2){80, 65};
    bosses.texture = LoadTexture("bossesTest.png");
}

void DrawBosses(){
    float scale = 8.0; // Skala 800% dari ukuran aslinya
    DrawTextureEx(bosses.texture, bosses.position, 0.0f, scale, WHITE);
}

void DrawPositions(){
    Vector2 mousePos = GetMousePosition();
    DrawText(TextFormat("Mouse X: %.0f, Mouse Y: %.0f", mousePos.x, mousePos.y), 10, 10, 20, DARKGRAY);
}

bool movingRight = true; 

void BossMov() 
{
    if (movingRight) 
    {
        bosses.position.x += BOSS_SPEED;
        if (bosses.position.x >= 135) {
            movingRight = false; 
        }
    } else 
    {
        bosses.position.x -= BOSS_SPEED;
        if (bosses.position.x <= 0) { 
            movingRight = true; 
        }
    }
}

void InitBossShoot()
{
    for (int i = 0; i < BOSS_MAX_BULLETS; i++)
    {
        bullets[i].active = false;
    }

}

void BossShoot(Vector2 startPos)
{
    for (int i = 0; i < BOSS_MAX_BULLETS; i++)
    {
        if(!bullets[i].active)
        {
            bullets[i].position = startPos;
            bullets[i].active = true;
        }
    }
}

void UpdateBullets() {
    for (int i = 0; i < BOSS_MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].position.y += BOSS_BULLET_SPEED;  // Peluru bergerak ke atas
            
            // Jika peluru keluar layar, nonaktifkan
            if (bullets[i].position.y < 0) {
                bullets[i].active = false;
            }
        }
    }
