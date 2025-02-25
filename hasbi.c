#include "hasbi.h"
#include<stdio.h>

//LOADING SCREEN
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
    if ((IsKeyDown(KEY_A)||IsKeyDown(KEY_LEFT)) && player.position.x > 0) 
    player.position.x -= PLAYER_SPEED;
    if ((IsKeyDown(KEY_D)||IsKeyDown(KEY_RIGHT))&& player.position.x < GAMEPLAY_WIDTH - (player.texture.width * 0.21)) 
        player.position.x += PLAYER_SPEED;
    if ((IsKeyDown(KEY_W)||IsKeyDown(KEY_UP)) && player.position.y > 0) 
        player.position.y -= PLAYER_SPEED;
    if ((IsKeyDown(KEY_S)||IsKeyDown(KEY_LEFT)) && player.position.y < SCREEN_HEIGHT - (player.texture.height * 0.2)) 
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


void UnloadPlayer() {
    UnloadTexture(player.texture);
}

//EXPLOSIONS
Explosion explosions[MAX_EXPLOSIONS];
Texture2D explosionsTexture;
Sound asteroidDestroyed, userPlaneExplosions;

void LoadAssets() {
    explosionsTexture = LoadTexture("Explosions.png");
    asteroidDestroyed= LoadSound("asteroidDestroyed.wav");
    userPlaneExplosions= LoadSound("userPlaneExplosion.wav");
}

void CreateExplosion(Vector2 position) {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (!explosions[i].active) {
            explosions[i].position = position; // Menyesuaikan agar pusat
            // explosions[i].position.y = position.y + 70;
            explosions[i].active = true;
            explosions[i].frame = 0;
            explosions[i].timer = 0;
            break;  // Keluar setelah menemukan slot kosong
        }
    }
}

void UpdateExplosions(float deltaTime) {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (explosions[i].active) {
            explosions[i].timer += deltaTime;
            if (explosions[i].timer > 0.1f) { // Ubah frame setiap 0.1 detik
                explosions[i].frame++;
                explosions[i].timer = 0;
            }
            if (explosions[i].frame >= 5) { // Misal animasi punya 5 frame
                explosions[i].active = false;
            }
        }
    }
}

void DrawExplosions(Texture2D explosionTexture) {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (explosions[i].active) {
            Rectangle source = { explosions[i].frame * 64, 0, 64, 64 }; // Misal sprite 64x64
            Rectangle dest = { explosions[i].position.x, explosions[i].position.y, 128, 128 };
            DrawTexturePro(explosionTexture, source, dest, (Vector2){ 18, 0 }, 0, WHITE);
        }
    }
}







//ASTEROIDS
Asteroid asteroids[MAX_ASTEROIDS];
int playerHealth=15;
Texture2D asteroidTexture;


void InitAsteroids() {
    asteroidTexture = LoadTexture("Asteroid.png");
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        asteroids[i].position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 100), GetRandomValue(-300, -50)};
        asteroids[i].size = GetRandomValue(1, 3);
        asteroids[i].health = asteroids[i].size;
        asteroids[i].speed = (Vector2){GetRandomValue(-2, 2), GetRandomValue(2, 4)};
        asteroids[i].active = true;
    }
}


void UpdateAsteroids() {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            asteroids[i].position.x += asteroids[i].speed.x;
            asteroids[i].position.y += asteroids[i].speed.y;
            
            if(asteroids[i].size==1){
                if (asteroids[i].position.x <= 0 || asteroids[i].position.x >= GAMEPLAY_WIDTH - (asteroidTexture.width*0.05f)) {
                    asteroids[i].speed.x *= -1;  // Pantulan jika mencapai batas
                }
            }

            if(asteroids[i].size==2){
                if (asteroids[i].position.x <= 0 || asteroids[i].position.x >= GAMEPLAY_WIDTH - (asteroidTexture.width*0.07f)) {
                    asteroids[i].speed.x *= -1;  // Pantulan jika mencapai batas
                }
            }

            if(asteroids[i].size==3){
                if (asteroids[i].position.x <= 0 || asteroids[i].position.x >= GAMEPLAY_WIDTH - (asteroidTexture.width*0.09f)) {
                    asteroids[i].speed.x *= -1;  // Pantulan jika mencapai batas
                }
            }


            if (asteroids[i].position.y > SCREEN_HEIGHT) {
                asteroids[i].position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 100), GetRandomValue(-300, -50)};
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
                    CheckCollisionCircles(asteroids[i].position, asteroids[i].size * 25, bullets[j].position, 5)) {
                    bullets[j].active = false;
                    asteroids[i].health--;
                    if (asteroids[i].health <= 0) {
                        PlaySound(asteroidDestroyed);
                        CreateExplosion(asteroids[i].position);
                        asteroids[i].active = false;
                    }
                }
                if (CheckCollisionCircles(player.position, 35, asteroids[i].position, asteroids[i].size * 25)) {
                    playerHealth -= asteroids[i].size;
                    PlaySound(userPlaneExplosions);
                    CreateExplosion(player.position);
                    asteroids[i].active = false;
                }
            }
        }
    }
}


void SpawnAsteroid() {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active){
            asteroids[i].position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 100), GetRandomValue(-1, -50)};
            asteroids[i].size = GetRandomValue(1, 3);
            asteroids[i].health = asteroids[i].size;
            asteroids[i].speed = (Vector2){GetRandomValue(-2, 2), GetRandomValue(2, 4)};
            asteroids[i].active = true;
            break;
        }
        
    }
}


void GameLoop() {
    static float asteroidSpawnTimer = 0.0f;
    asteroidSpawnTimer += GetFrameTime();

    if (asteroidSpawnTimer >= 2.0f) { // Setiap 2 detik, spawn asteroid baru
        SpawnAsteroid();
        asteroidSpawnTimer = 0.0f;
    }

    UpdateAsteroids();
}

void DrawAsteroids() {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            float scale; // Sesuaikan skala asteroid
            if(asteroids[i].size==1){
                scale=asteroids[i].size * 0.05f;
            }else if(asteroids[i].size==2){
                scale=asteroids[i].size * 0.035f;
            }else if(asteroids[i].size==3){
                scale=asteroids[i].size * 0.03f;
            }
            
            DrawTextureEx(asteroidTexture, asteroids[i].position, 0.0f, scale, WHITE);
            // float rotation = 0.0f; // Bisa dibuat berputar kalau mau

            // Jika asteroid bergerak ke kiri, balikkan gambar
        //     Vector2 drawPosition = {
        //         asteroids[i].position.x - (asteroidTexture.width * scale / 2),
        //         asteroids[i].position.y - (asteroidTexture.height * scale / 2)
        //     };

        //     Vector2 origin = { asteroidTexture.width / 2, asteroidTexture.height / 2 };

        //     DrawTexturePro(
        //         asteroidTexture,
        //         (Rectangle){0, 0, (asteroids[i].speed.x >= 0) ? asteroidTexture.width : -asteroidTexture.width, asteroidTexture.height},
        //         (Rectangle){drawPosition.x, drawPosition.y, asteroidTexture.width * scale, asteroidTexture.height * scale},
        //         origin,
        //         rotation,
        //         WHITE
        //     );
        // }
    }
}
}


void DrawHealth() {
    DrawText(TextFormat("Health: %d", playerHealth), 20, 20, 20, RED);
}







//GAMEPLAY
void DrawGameplay() {
    DrawLayout();
    DrawPlayer();
    DrawBullets();
    DrawAsteroids();
    DrawHealth();
    DrawExplosions(explosionsTexture);
}
