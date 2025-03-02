#include "nazwa.h"
#include "hasbi.h"

Texture2D heartTexture;
Texture2D menuTexture;
Texture2D soundOnTexture;
Texture2D soundOffTexture;
Texture2D restartTexture;
Texture2D pauseTexture;
Texture2D quitTexture;
bool soundOn = true;
bool startGame = false;
void varHeart() {
    // Koordinat menu
    int menuX = GAMEPLAY_WIDTH + MENU_WIDTH / 2;
    int startY = 40;
    
    // Skala gambar
    float scale = 0.025f;

    // Gambar teks "Health"
    DrawText("Health", menuX - 85, startY + 140, 25, RAYWHITE);

    // Cetak gambar hati dalam 3 baris x 5 kolom
    int heartsDrawn = 0; // Jumlah hati yang sudah digambar

    for (int i = 0; i < 3; i++) {  // Loop untuk baris (max 3)
        for (int j = 0; j < 5; j++) {  // Loop untuk kolom (max 5)
            if (heartsDrawn < playerHealth) { // Hanya gambar sesuai `playerHealth`
                DrawTextureEx(
                    heartTexture,
                    (Vector2){menuX - 85 + (j * (heartTexture.width * scale + 5)), startY + 170 + (i * (heartTexture.height * scale + 5))},
                    0.0f, scale, WHITE
                );
                heartsDrawn++; // Tambah jumlah hati yang digambar
            }
        }
    }
}


void varMenu(bool *isSoundOn)
{
    static bool showPopup = false; // Status pop-up

    int menuX = GAMEPLAY_WIDTH;
    int startY = 20;
    
    float scale = 0.58f;
    int texWidth = menuTexture.width * scale;
    int texHeight = menuTexture.height * scale;

    Vector2 texturePos = {menuX + MENU_WIDTH - texWidth - 20, startY};
    Vector2 mousePos = GetMousePosition();

    bool isMouseOver = (mousePos.x >= texturePos.x && mousePos.x <= texturePos.x + texWidth) &&
                       (mousePos.y >= texturePos.y && mousePos.y <= texturePos.y + texHeight);

    if (isMouseOver && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        showPopup = !showPopup;
    }

    DrawTextureEx(menuTexture, texturePos, 0.0f, scale, WHITE);

    // Jika pop-up aktif, tampilkan di tengah layar
    if (showPopup) {
        int popupWidth = 370;
        int popupHeight = 350;
        int popupX = (SCREEN_WIDTH - popupWidth) / 2;
        int popupY = (SCREEN_HEIGHT - popupHeight) / 2;

        DrawRectangle(popupX, popupY, popupWidth, popupHeight, DARKGRAY);
        DrawText("Popup Menu", popupX + 20, popupY + 20, 25, WHITE);

        // Tombol Close
        int closeBtnSize = 30;
        int closeX = popupX + popupWidth - closeBtnSize - 10;
        int closeY = popupY + 10;
        
        DrawRectangle(closeX, closeY, closeBtnSize, closeBtnSize, RED);
        DrawText("X", closeX + 8, closeY + 5, 20, WHITE);

        if (mousePos.x >= closeX && mousePos.x <= closeX + closeBtnSize &&
            mousePos.y >= closeY && mousePos.y <= closeY + closeBtnSize &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            showPopup = false;
        }

        // Posisi elemen dalam pop-up
        int iconStartX = popupX + 20; // Posisi kiri dalam pop-up
        int iconStartY = popupY + 60; // Posisi awal untuk elemen
        int iconSpacing = 70; // Jarak antar elemen
        float iconScale = 0.5f;
        int textOffsetX = 80; // Jarak teks dari ikon

        // Tombol suara dalam pop-up
        Texture2D currentSoundTexture = *isSoundOn ? soundOnTexture : soundOffTexture;
        DrawTextureEx(currentSoundTexture, (Vector2){iconStartX, iconStartY}, 0.0f, 0.4f, WHITE);
        DrawText("Press S to Off/On", iconStartX + textOffsetX, iconStartY + 15, 23, WHITE);
        
        if (mousePos.x >= iconStartX && mousePos.x <= iconStartX + (soundOnTexture.width * iconScale) &&
            mousePos.y >= iconStartY && mousePos.y <= iconStartY + (soundOnTexture.height * iconScale) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *isSoundOn = !(*isSoundOn);
        }

        DrawTextureEx(restartTexture, (Vector2){iconStartX, iconStartY + iconSpacing}, 0.0f, iconScale, WHITE);
        DrawText("press R to Restart", iconStartX + textOffsetX, iconStartY + iconSpacing + 20, 23, WHITE);

        DrawTextureEx(pauseTexture, (Vector2){iconStartX, iconStartY + iconSpacing * 2}, 0.0f, iconScale, WHITE);
        DrawText("press Enter to Pause", iconStartX + textOffsetX, iconStartY + iconSpacing * 2 + 22, 23, WHITE);
        
        DrawTextureEx(quitTexture, (Vector2){iconStartX, iconStartY + iconSpacing * 3}, 0.0f, iconScale, WHITE);
        DrawText("press Q to Quit", iconStartX + textOffsetX, iconStartY + iconSpacing * 3 + 25, 23, WHITE);
    }
}


void varQuit(){

}

void varPause(){

}

void varRestart(){

}


void varSound(bool *isSoundOn)
{
    int menuX = GAMEPLAY_WIDTH + MENU_WIDTH / 2;
    int startY = 80;
    
    float scale1 = 0.07f;
    
    int posX = menuX - (soundOnTexture.width * scale1) / 2;
    int posY = startY + 120;
    int width = soundOnTexture.width * scale1;
    int height = soundOnTexture.height * scale1;

    // Ambil posisi mouse
    Vector2 mousePos = GetMousePosition();

    // Cek apakah mouse ada di dalam area tombol suara
    bool isHovered = (mousePos.x >= posX && mousePos.x <= posX + width &&
                      mousePos.y >= posY && mousePos.y <= posY + height);

    // Jika tombol suara diklik, ubah gambar
    if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *isSoundOn = !(*isSoundOn);
    }

    // Pilih tekstur berdasarkan status
    Texture2D currentTexture = *isSoundOn ? soundOnTexture : soundOffTexture;

    // Gambar tombol suara sesuai status saat ini
    DrawTextureEx(currentTexture, (Vector2){posX, posY}, 0.0f, scale1, WHITE);
}


void mainMenu(bool *gameStart)
{
    // Koordinat menu
    int menuX = GAMEPLAY_WIDTH + MENU_WIDTH / 2;
    int startY = 20;
    bool isSoundOn = true;
    if(!*gameStart){
        DrawRectangle(GAMEPLAY_WIDTH, 0, MENU_WIDTH, SCREEN_HEIGHT, DARKGRAY);
        DrawText(TextFormat("Level: %d", level), menuX - 85, startY + 80, 30, RAYWHITE);
        DrawText(TextFormat("Score: %d", score), menuX - 85, startY + 120, 25, RAYWHITE);
        varMenu(&isSoundOn);
        varHeart(heartTexture);
        
        if (IsKeyPressed(KEY_ENTER)) {
            *gameStart = true;  // Set gameStart = true untuk keluar dari menu
        }
    }
}




void loadAssetMenu(){
    heartTexture = LoadTexture("asset-menu/1.png");
    menuTexture = LoadTexture("asset-menu/6.png");
    soundOnTexture = LoadTexture("asset-menu/7.png");
    soundOffTexture = LoadTexture("asset-menu/5.png");
    restartTexture = LoadTexture("asset-menu/8.png");
    pauseTexture = LoadTexture("asset-menu/9.png");
    quitTexture = LoadTexture("asset-menu/10.png");
}
    

void unloadAssetMenu(){
    UnloadTexture(heartTexture);
    UnloadTexture(menuTexture);
    UnloadTexture(soundOnTexture);
    UnloadTexture(soundOffTexture);
    UnloadTexture(restartTexture);
    UnloadTexture(pauseTexture);
    UnloadTexture(quitTexture);
}


