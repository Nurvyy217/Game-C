#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LIVES 3
#define INVINCIBILITY_TIME 1.0f  // 1 detik
#define POWERUP_DURATION 5.0f   // 5 detik untuk power-up sementara

typedef enum { NONE, EXTRA_LIFE, DOUBLE_SCORE, SHIELD, SPEED_BOOST } PowerUpType;

typedef struct {
    int lives;
    float invincibleTimer;
    int score;
    float speed;
    int doubleScoreActive;
    float powerUpTimer;
} Player;

typedef struct {
    Vector2 position;
    PowerUpType type;
    bool active;
} PowerUp;

void TakeDamage(Player *player) {
    if (player->invincibleTimer <= 0) {
        if (player->lives > 0) {
            player->lives--;
            player->invincibleTimer = INVINCIBILITY_TIME; // Aktifkan invincibility
        }
    }
}

void ApplyPowerUp(Player *player, PowerUp *powerUp) {
    if (!powerUp->active) return;
    
    switch (powerUp->type) {
        case EXTRA_LIFE:
            player->lives++;
            break;
        case DOUBLE_SCORE:
            player->doubleScoreActive = 1;
            player->powerUpTimer = POWERUP_DURATION;
            break;
        case SHIELD:
            player->invincibleTimer = POWERUP_DURATION;
            break;
        case SPEED_BOOST:
            player->speed *= 1.5;
            player->powerUpTimer = POWERUP_DURATION;
            break;
        default:
            break;
    }
    
    powerUp->active = false; // Hilangkan power-up setelah diambil
}

int main() {
    InitWindow(800, 600, "Power-Up System");
    SetTargetFPS(60);
    srand(time(NULL));

    Player player = {MAX_LIVES, 0, 0, 200.0f, 0, 0};
    PowerUp powerUp = {{rand() % 700 + 50, rand() % 500 + 50}, EXTRA_LIFE, true};

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Kurangi timer invincibility dan power-up
        if (player.invincibleTimer > 0) player.invincibleTimer -= deltaTime;
        if (player.powerUpTimer > 0) {
            player.powerUpTimer -= deltaTime;
            if (player.powerUpTimer <= 0) {
                player.doubleScoreActive = 0;
                player.speed = 200.0f;
            }
        }

        // Simulasi terkena hit saat tombol SPACE ditekan
        if (IsKeyPressed(KEY_SPACE)) {
            TakeDamage(&player);
        }

        // Simulasi mengambil power-up
        if (IsKeyPressed(KEY_P) && powerUp.active) {
            ApplyPowerUp(&player, &powerUp);
        }

        // Gambar layar
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawText(TextFormat("Nyawa: %d", player.lives), 350, 250, 20, BLACK);
        DrawText(TextFormat("Skor: %d", player.score), 350, 280, 20, BLACK);
        DrawText(TextFormat("Kecepatan: %.1f", player.speed), 350, 310, 20, BLACK);
        if (player.invincibleTimer > 0) {
            DrawText("Invincible!", 350, 340, 20, RED);
        }
        if (player.doubleScoreActive) {
            DrawText("Double Score Active!", 350, 370, 20, BLUE);
        }
        if (powerUp.active) {
            DrawCircleV(powerUp.position, 10, GREEN);
            DrawText("Power-Up di sini! (Tekan P)", (int)powerUp.position.x - 40, (int)powerUp.position.y - 20, 10, BLACK);
        }
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
