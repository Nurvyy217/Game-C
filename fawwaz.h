#include "raylib.h"

#define BOSS_SPEED 5


// Bosses
typedef struct {
    Vector2 position;
    Texture2D texture;
} Bosses;



void InitBosses();
void DrawBosses();

//
void DrawPositions();