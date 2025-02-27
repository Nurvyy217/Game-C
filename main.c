<<<<<<< HEAD
#include "hasbi.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(60);
    InitAudioDevice(); // Inisialisasi perangkat audio
    InitPlayer();
    InitBullets();
    InitAsteroids();
    LoadAssets();
    
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();


        if (!isLoadingDone) {
            // loadingAnimation();  // Tampilkan loading lebih dulu
            isLoadingDone=true;
        } else {
            UpdatePlayer();
            // if (IsKeyPressed(KEY_SPACE)) ShootBullet();
            UpdateShooting(deltaTime);
            UpdateBullets();
            UpdateExplosions(deltaTime);
            CheckCollisions();
            GameLoop();
            
            BeginDrawing();
            ClearBackground(BLACK);
            DrawGameplay();  // Menampilkan layout + player + bullet + asteroids
            EndDrawing();
        
        }
    }
    CloseAudioDevice();
    UnloadAssets();
    unloadTextures();
    
    CloseWindow();
    return 0;
}
=======
#include <stdio.h>

int main(){
    printf("IKAN IKAN apa yang jenisnya laki-laki semua?\n");
    printf("jawaban : ikan mas");
}

>>>>>>> abe55eca8498b8bcc1bc036c27747a3a194f17cc
