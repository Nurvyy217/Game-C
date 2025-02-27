<<<<<<< HEAD
#include "suci.h"

void initTextures() {
    if (!texturesLoaded) {  // Load hanya sekali
        logoDeveloper = LoadTexture("LogoG6.png");
        gameNamePhoto = LoadTexture("SpaceInviders.png");
        texturesLoaded = true;
    }
}

#include "raylib.h"

// Dimensi layar
const int screenWidth = 800;
const int screenHeight = 600;

// Mendeklarasikan variabel untuk gambar latar belakang
Texture2D background;

// Fungsi untuk menginisialisasi game
void InitGame()
{
    // Inisialisasi jendela
    InitWindow(screenWidth, screenHeight, "Space Invaders");
    
    // Memuat gambar latar belakang
    background = LoadTexture("space_invaders.png");

    SetTargetFPS(60); // Menetapkan batas frame rate ke 60 FPS
}

// Fungsi untuk menggambar layar judul
void DrawTitleScreen()
{
    // Menggambar gambar latar belakang
    DrawTexture(background, 0, 0, WHITE);
    
    // Menampilkan teks judul
    DrawText("SPACE INVADERS", screenWidth / 2 - MeasureText("SPACE INVADERS", 40) / 2, screenHeight / 4, 40, BLUE);

    // Menampilkan teks "Tekan ENTER untuk mulai"
    DrawText("Tekan ENTER untuk mulai", screenWidth / 2 - MeasureText("Tekan ENTER untuk mulai", 20) / 2, screenHeight / 2, 20, LIGHTGRAY);
}

// Fungsi untuk menjalankan logika game
void RunGame()
{
    // Logika game (untuk saat ini hanya sebagai placeholder)
    ClearBackground(DARKBLUE);
    DrawText("Game sedang berjalan...", screenWidth / 2 - MeasureText("Game sedang berjalan...", 20) / 2, screenHeight / 2, 20, WHITE);
}

int main(void)
{
    // Inisialisasi game
    InitGame();

    bool gameStarted = false;

    // Loop utama game
    while (!WindowShouldClose()) // Terus berjalan selama jendela terbuka
    {
        // Mengecek apakah tombol "Enter" ditekan untuk memulai game
        if (!gameStarted && IsKeyPressed(KEY_ENTER))
        {
            gameStarted = true; // Menandakan bahwa game dimulai
        }

        // Menggambarkan tampilan berdasarkan status game
        if (!gameStarted)
        {
            // Menampilkan layar judul
            DrawTitleScreen();
        }
        else
        {
            // Menjalankan game
            RunGame();
        }

        // Memperbarui layar
        EndDrawing();
    }

    // Menutup jendela dan konteks OpenGL
    UnloadTexture(background);  // Menghapus gambar latar belakang
    CloseWindow();

    return 0;
}

=======
haloooo
>>>>>>> abe55eca8498b8bcc1bc036c27747a3a194f17cc
