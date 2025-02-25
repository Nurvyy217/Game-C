#include "supriadi.h"
#include "hasbi.h"

Texture2D nyawaIMG;
int score = 0;
int Nyawa = NYAWA_AWAL;

void BuatNyawa() {
    nyawaIMG = LoadTexture("nyawa.png");
}

void rubahNyawa(){

    // kurangi nyawa jika terkena tembakan musuh
    // kurangi nyawa jika terkena meteor
    // dan lain lain

    if (IsKeyPressed(KEY_K)) Nyawa--; //hanya sementara   
}

void Tampil_Nyawa(){
    int n = 0,m = 0;
    ClearBackground(RAYWHITE);
    for (int i = 0; i < Nyawa; i++) {
        if (i == 8){
            n = 20;
            m = 130;
        }
        Vector2 posisi = {GAMEPLAY_WIDTH + 40 - m + i * (nyawaIMG.width * 0.02f), 800 + n};
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