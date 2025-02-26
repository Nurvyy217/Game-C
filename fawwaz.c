#include "raylib.h"
#include "fawwaz.h"
#include "hasbi.h"

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

void BossMov(){
    Vector2 
}