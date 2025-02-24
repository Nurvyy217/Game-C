#include "supriadi.h"

Texture2D nyawaIMG;
int score = 0;
int Nyawa = NYAWA_AWAL;

void BuatNyawa() {
    nyawaIMG = LoadTexture("nyawa.png");
}

void rubahNyawa(){
    if (IsKeyPressed(KEY_K)) Nyawa--;
    // if (IsKeyPressed(KEY_ENTER)) score += 10;    
}

void Tampil_Nyawa(){
    ClearBackground(RAYWHITE);
    for (int i = 0; i < Nyawa; i++) {
        Vector2 posisi = { 550 + i * (nyawaIMG.width * 0.02f + 10 ), 800 };
        DrawTextureEx(nyawaIMG, posisi, 0.0f, 0.02f, WHITE);

    }
    DrawText(TextFormat("Score: %d", score), 560, 575, 30, RAYWHITE);
    if (Nyawa == 0) {
        DrawText("GAME OVER", 300, 200, 40, RED);
    }
}

bool gameover(){
    return (Nyawa == 0)? false : true;
}

void restart(){
    if (IsKeyPressed(KEY_R)){
        Nyawa = NYAWA_AWAL;
    }    
}

void unloadNyawa() {
    UnloadTexture(nyawaIMG);
}