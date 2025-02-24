#include "hasbi.h"

//LOADING SCREEN
void DrawLayout()
{
    ClearBackground(RAYWHITE);
    
    // Gameplay area (3/4 of screen, left part)
    DrawRectangle(0, 0, GAMEPLAY_WIDTH, SCREEN_HEIGHT, LIGHTGRAY);
    DrawText("Gameplay Area", GAMEPLAY_WIDTH / 2 - 60, SCREEN_HEIGHT / 2, 20, DARKGRAY);
    
    // Menu area (1/4 of screen, right part)
    DrawRectangle(GAMEPLAY_WIDTH, 0, MENU_WIDTH, SCREEN_HEIGHT, DARKGRAY);
    DrawText("Menu Area", GAMEPLAY_WIDTH + MENU_WIDTH / 2 - 40, SCREEN_HEIGHT / 2, 20, RAYWHITE);
    
    // Separator line
    DrawLine(GAMEPLAY_WIDTH, 0, GAMEPLAY_WIDTH, SCREEN_HEIGHT, BLACK);
    
}

Texture2D logoDeveloper;
Texture2D gameNamePhoto;
bool texturesLoaded = false;  // Cek apakah gambar sudah di-load
bool isLoadingDone = false; 

void initTextures() {
    if (!texturesLoaded) {  // Load hanya sekali
        logoDeveloper = LoadTexture("logoDeveloper.png");
        gameNamePhoto = LoadTexture("gameNamePhoto.png");
        texturesLoaded = true;
    }
}

void unloadTextures() {
    if (texturesLoaded) {
        UnloadTexture(logoDeveloper);
        UnloadTexture(gameNamePhoto);
        texturesLoaded = false;
    }
}

void loadingAnimation() {
    static float alpha = 1.0f;
    static int stage = 0;
    static float timer = 0.0f;

    initTextures();

    if (isLoadingDone) return;  // Jika sudah selesai, langsung keluar

    timer += GetFrameTime();

    switch (stage) {
        case 0: // Fade out gambar pertama
            if (timer > 2.0f) {
                alpha -= 0.02f;
                if (alpha <= 0) {
                    alpha = 0;
                    stage = 1;
                    timer = 0;
                }
            }
            break;
        case 1: // gambar kedua
                alpha=1.0f;
                stage = 2;
                timer = 0;
            break;
    
        case 2: // Fade out gambar kedua, lalu selesai
            if (timer > 2.0f) {
                alpha -= 0.02f;
                if (alpha <= 0) {
                    alpha = 0;
                    isLoadingDone = true;  // Menandai bahwa loading selesai
                    return;
                }
            }
            break;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    if (stage == 0) {
        DrawTexture(logoDeveloper, SCREEN_WIDTH / 2 - logoDeveloper.width / 2, SCREEN_HEIGHT / 2 - logoDeveloper.height / 2, Fade(WHITE, alpha));
    }
    if (stage >= 1) {
        DrawTexture(gameNamePhoto, SCREEN_WIDTH / 2 - gameNamePhoto.width / 2, SCREEN_HEIGHT / 2 - gameNamePhoto.height / 2, Fade(WHITE, (stage == 1) ? (1.0f - alpha) : alpha));
    }

    EndDrawing();
}



//USER PLANE
Player player;
Bullet bullets[MAX_BULLETS];

void InitPlayer() {
    player.position = (Vector2){GAMEPLAY_WIDTH / 2, SCREEN_HEIGHT - 115};
    player.texture = LoadTexture("userPlane.png");
}

void InitBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
}

void UpdatePlayer() {
    if (IsKeyDown(KEY_A)||IsKeyDown(KEY_LEFT) && player.position.x > 0) 
    player.position.x -= PLAYER_SPEED;
    if (IsKeyDown(KEY_D)||IsKeyDown(KEY_RIGHT) && player.position.x < GAMEPLAY_WIDTH - (player.texture.width * 0.21)) 
        player.position.x += PLAYER_SPEED;
    if (IsKeyDown(KEY_W)||IsKeyDown(KEY_UP) && player.position.y > 0) 
        player.position.y -= PLAYER_SPEED;
    if (IsKeyDown(KEY_S)||IsKeyDown(KEY_DOWN) && player.position.y < SCREEN_HEIGHT - (player.texture.height * 0.2)) 
        player.position.y += PLAYER_SPEED;

}

void ShootBullet() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].position = (Vector2){player.position.x + player.texture.width *0.2 / 2, player.position.y};
            bullets[i].active = true;
            break;
        }
    }
}

void UpdateBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].position.y -= BULLET_SPEED;
            if (bullets[i].position.y < 0) bullets[i].active = false;
        }
    }
}

void DrawPlayer() {
    float scale = 0.2; // Skala 20% dari ukuran aslinya
    DrawTextureEx(player.texture, player.position, 0.0f, scale, WHITE);
    // DrawTexture(player.texture, player.position.x, player.position.y, WHITE);
}

void DrawBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            DrawCircleV(bullets[i].position, 5, RED);
        }
    }
}

void DrawGameplay() {
    DrawLayout();
    DrawPlayer();
    DrawBullets();
}

void UnloadPlayer() {
    UnloadTexture(player.texture);
}



//ASTEROIDS
Asteroid asteroids[MAX_ASTEROIDS];
int playerHealth=15;

void InitAsteroids() {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        asteroids[i].position = (Vector2){GetRandomValue(0, GAMEPLAY_WIDTH - 50), GetRandomValue(-300, -50)};
        asteroids[i].size = GetRandomValue(1, 3);
        asteroids[i].health = asteroids[i].size;
        asteroids[i].speed = (Vector2){GetRandomValue(-2, 2), GetRandomValue(2, 5)};
        asteroids[i].active = true;
    }
}

void UpdateAsteroids() {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            asteroids[i].position.x += asteroids[i].speed.x;
            asteroids[i].position.y += asteroids[i].speed.y;
            
            if (asteroids[i].position.x <= 0 || asteroids[i].position.x >= GAMEPLAY_WIDTH - 50) {
                asteroids[i].speed.x *= -1;  // Pantulan jika mencapai batas
            }
            
            if (asteroids[i].position.y > SCREEN_HEIGHT) {
                asteroids[i].position = (Vector2){GetRandomValue(0, GAMEPLAY_WIDTH - 50), GetRandomValue(-300, -50)};
                asteroids[i].health = asteroids[i].size;
            }
        }
    }
}

void CheckCollisions() {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            for (int j = 0; j < MAX_BULLETS; j++) {
                if (bullets[j].active &&
                    CheckCollisionCircles(asteroids[i].position, asteroids[i].size * 15, bullets[j].position, 5)) {
                    bullets[j].active = false;
                    asteroids[i].health--;
                    if (asteroids[i].health <= 0) {
                        asteroids[i].active = false;
                    }
                }
                if (CheckCollisionCircles(player.position, 20, asteroids[i].position, asteroids[i].size * 15)) {
                    playerHealth -= asteroids[i].size;
                    asteroids[i].active = false;
                }
            }
        }
    }
}

