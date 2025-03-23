#include "nazwa.h"
#include "hasbi.h"
#include "supriadi.h"
#include "fawwaz.h"

Texture2D heartTexture;
Texture2D enemyBulletlv3;
Texture2D menuTexture;
Texture2D soundOnTexture;
Texture2D soundOffTexture;
Texture2D restartTexture;
Texture2D pauseTexture;
Texture2D quitTexture;
Texture2D gameOverTexture;
Texture2D ufoTexture;
bool soundOn = true;
bool startGame = false;



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

        // Efek Shadow (kotak lebih besar dengan transparansi)
        DrawRectangle(popupX - 5, popupY - 5, popupWidth + 10, popupHeight + 10, Fade(BLACK, 0.5f));

        // Kotak utama pop-up
        DrawRectangle(popupX, popupY, popupWidth, popupHeight, DARKGRAY);

        // Border tebal (lapisan ganda untuk efek lebih bold)
        DrawRectangleLinesEx((Rectangle){popupX - 2, popupY - 2, popupWidth + 4, popupHeight + 4}, 4, WHITE);
        DrawRectangleLinesEx((Rectangle){popupX - 4, popupY - 4, popupWidth + 8, popupHeight + 8}, 2, BLACK);

        DrawText("Popup Menu", popupX + 20, popupY + 20, 25, WHITE);

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

        // Tombol Restart
        DrawTextureEx(restartTexture, (Vector2){iconStartX, iconStartY + iconSpacing}, 0.0f, iconScale, WHITE);
        DrawText("Press R to Restart", iconStartX + textOffsetX, iconStartY + iconSpacing + 20, 23, WHITE);

        // Tombol Pause
        DrawTextureEx(pauseTexture, (Vector2){iconStartX, iconStartY + iconSpacing * 2}, 0.0f, iconScale, WHITE);
        DrawText("Press Enter to Pause", iconStartX + textOffsetX, iconStartY + iconSpacing * 2 + 22, 23, WHITE);
        
        // Tombol Quit
        DrawTextureEx(quitTexture, (Vector2){iconStartX, iconStartY + iconSpacing * 3}, 0.0f, iconScale, WHITE);
        DrawText("Press Q to Quit", iconStartX + textOffsetX, iconStartY + iconSpacing * 3 + 25, 23, WHITE);
    }
}


void varQuit(){
    if (IsKeyPressed(KEY_Q)) {  // Tekan Q untuk keluar
        CloseWindow();  // Tutup jendela Raylib
    }
}

bool isPaused = false; 

void togglePause() {
    if (IsKeyPressed(KEY_ENTER)) {
        isPaused = !isPaused;
    }
}

bool getPauseState() {
    return isPaused;
}

void gamePaused()
{
        float iconScale = 0.5f;
        int textOffsetX = 80; // Jarak teks dari ikon
    
        DrawTextureEx(gameOverTexture, (Vector2){210, 300}, 0.0f, iconScale, WHITE);
        DrawText("Press Enter to start", 235, 530, 23, WHITE);
        
}

void varRestart(){
    if (IsKeyPressed(KEY_R)){
        InfoPlayer.nyawa = NYAWA_AWAL;
        InfoPlayer.score = 0;
        InitPlayer();
        InitBullets();
    }
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
        // DrawRectangle(GAMEPLAY_WIDTH, 0, MENU_WIDTH, SCREEN_HEIGHT, DARKBLUE);
        DrawRectangle(GAMEPLAY_WIDTH, 0, MENU_WIDTH, SCREEN_HEIGHT, BLACK);
        // DrawRectangle(GAMEPLAY_WIDTH, 0, MENU_WIDTH, SCREEN_HEIGHT, (Color){3, 24, 37, 255});
        DrawText(TextFormat("Level: %d", level), menuX - 85, startY + 80, 30, RAYWHITE);
        varMenu(&isSoundOn);
        // varHeart(heartTexture);
        tampilNyawa();
        Tampil_Score();
        
        if (IsKeyPressed(KEY_ENTER)) {
            *gameStart = true;  // Set gameStart = true untuk keluar dari menu
        }
    }
}
Texture2D eneBul, ufoBroken;
void loadAssetMenu(){
    menuTexture = LoadTexture("asset-menu/6.png");
    soundOnTexture = LoadTexture("asset-menu/7.png");
    soundOffTexture = LoadTexture("asset-menu/5.png");
    restartTexture = LoadTexture("asset-menu/8.png");
    pauseTexture = LoadTexture("asset-menu/9.png");
    quitTexture = LoadTexture("asset-menu/10.png");
    gameOverTexture = LoadTexture("asset-menu/11.png");
    ufoTexture = LoadTexture("assets/ufo.png");
    enemyBulletlv3= LoadTexture("assets/laserUfo.png");
    eneBul= LoadTexture("assets/eneBull.png");
    ufoBroken= LoadTexture("assets/ufoBroken.png");
}
    

void unloadAssetMenu(){
    UnloadTexture(menuTexture);
    UnloadTexture(soundOnTexture);
    UnloadTexture(soundOffTexture);
    UnloadTexture(restartTexture);
    UnloadTexture(pauseTexture);
    UnloadTexture(quitTexture);
    UnloadTexture(gameOverTexture);
}


void DrawLvl2()
{
    DrawLayout();
    DrawPlayer();
    DrawBullets();
    DrawExplosions(explosionsTexture);
    DrawEnemies(ufoTexture, ufoBroken,1.5f,80,110, &gamestate);
    DrawEnemyBullets(eneBul,1.2f, &gamestate);
    tampilspark();
    mainMenu(&gameStart);
}

void level2(float deltaTime){
    setEnemyTypeShoot(&gamestate, 2);
    setMaxEnemyBullet(&gamestate, 9);
    setHealthBroke(&gamestate, 1);
    UpdatePlayer();
    UpdateShooting(deltaTime);
    UpdateBullets();
    UpdateExplosions(deltaTime);
    UpdateEnemies(ufoTexture, -30, 100, 0, 70, &gamestate);
    UpdateEnemyBullets(eneBul,&gamestate);
    CheckEnemyCollisions(65,70,55,10,&gamestate);//x,y,radP,radBE
    EnemiesLoop(ufoTexture, -30, 100, 0, 70,&gamestate);
    DrawLvl2();
    inipowerup();
    UpdateSpark();
}



