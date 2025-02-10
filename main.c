#include "raylib.h"

int main() {
    const int screenWidth = 720;
    const int screenHeight = 960;

    InitWindow(screenWidth, screenHeight, "Hello Raylib");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello World!", 300, 200, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;

