#include "nazwa.h"
#include "hasbi.h"
#include "supriadi.h"

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


void UpdateEnemy()
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemy[i].isActive)
        {
            enemy[i].position.y += enemy[i].speed.y;
            enemy[i].position.x += enemy[i].speed.x;

            if (enemy[i].position.x <= -85 || enemy[i].position.x >= GAMEPLAY_WIDTH - (ufoTexture.width * 2.0f) + 85)
            {
                enemy[i].speed.x *= -1; // Pantulan jika mencapai batas
            }

            if (enemy[i].position.y > SCREEN_HEIGHT)
            {
                enemy[i].position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 170), GetRandomValue(-200, -150)};
                enemy[i].hasShot = false; // Reset tembakan jika musuh respawn dari atas
            }

            // Jika musuh boleh menembak dan belum menembak sebelumnya, maka tembak
            if (enemy[i].canShoot && !enemy[i].hasShot && GetRandomValue(0, 100) < 2)
            {
                EnemyShooting();
            }
        }
    }
}

void SpawnEnemy()
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemy[i].isActive)
        {
            enemy[i].position = (Vector2){GetRandomValue(50, GAMEPLAY_WIDTH - 170), GetRandomValue(-200, -150)};
            enemy[i].speed = (Vector2){GetRandomValue(-2, 2) * 0.5, GetRandomValue(1, 2) * 0.8};
            enemy[i].canShoot = (GetRandomValue(0, 1) == 1);
            enemy[i].isActive = true;
            enemy[i].hasShot = false;
            break;
        }
    }
}

void EnemiesLooping()
{
    static float enemiesSpawnTimer = 0.0f;
    enemiesSpawnTimer += GetFrameTime();

    if (enemiesSpawnTimer >= 1.5f)
    { // Setiap 2 detik, spawn asteroid baru
        SpawnEnemy();
        enemiesSpawnTimer = 0.0f;
    }
    UpdateEnemy();
}

void EnemyShooting()
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemy[i].isActive)
            continue; // Musuh mati tidak boleh menembak

        for (int j = 0; j < MAX_ENEMY_BULLETS; j++)
        {
            if (!enemyBullet[j].isActive)
            {
                enemyBullet[j].position = (Vector2){(enemy[i].position.x - 30) + ufoTexture.width,
                                                     (enemy[i].position.y + ufoTexture.width + 20)};
                enemyBullet[j].isActive = true;
                enemyBullet[j].shooterIndex = i; // Tandai peluru ini ditembak oleh musuh ke-i
                if (GetRandomValue(0, 1) == 0)
                {
                    enemyBullet[j].speed = (Vector2){0, SPEED_ENEMY_BULLETS}; // Vertikal (ke bawah)
                }
                else
                {
                    int dirX = GetRandomValue(0, 1) == 0 ? -1 : 1; // Bisa kiri atau kanan
                    enemyBullet[j].speed = (Vector2){dirX * 2.0f, SPEED_ENEMY_BULLETS}; // Diagonal
                }
                enemy[i].hasShot = true;
                break;
            }
        }
    }
}

Explosion explosion[MAX_EXPLOSIONS];
Texture2D explosionTexture;

void CreateExplosions(Vector2 position)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (!explosion[i].active)
        {
            explosion[i].position = position; // Menyesuaikan agar pusat
            // explosion[i].position.y = position.y + 70;
            explosion[i].active = true;
            explosion[i].frame = 0;
            explosion[i].timer = 0;
            break; // Keluar setelah menemukan slot kosong
        }
    }
}

void UpdateExplosion(float deltaTime)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (explosion[i].active)
        {
            explosion[i].timer += deltaTime;
            if (explosion[i].timer > 0.1f)
            { // Ubah frame setiap 0.1 detik
                explosion[i].frame++;
                explosion[i].timer = 0;
            }
            if (explosion[i].frame >= 5)
            { // Misal animasi punya 5 frame
                explosion[i].active = false;
            }
        }
    }
}

void DrawExplosion(Texture2D explosionTexture)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (explosion[i].active)
        {
            Rectangle source = {explosion[i].frame * 64, 0, 64, 64}; // Misal sprite 64x64
            Rectangle dest = {explosion[i].position.x, explosion[i].position.y, 128, 128};
            DrawTexturePro(explosionTexture, source, dest, (Vector2){18, 0}, 0, WHITE);
        }
    }
}


void CheckEnemyCollision()
{
    Vector2 playerPosition = (Vector2){player.position.x + 185, player.position.y + 150};

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemy[i].isActive)
            continue; // Lewati musuh yang sudah mati

        Vector2 enemiesPosition = (Vector2){enemy[i].position.x + 95, enemy[i].position.y + 70};

        // **Cek tabrakan dengan peluru pemain (Musuh tertembak)**
        for (int j = 0; j < MAX_BULLETS; j++)
        {
            if (CheckCollisionCircles(enemiesPosition, 40, bullet[j].position, 8))
            {
                // printf("Peluru [%d] mengenai musuh [%d]!\n", j, i);
                CreateExplosions(enemiesPosition);
                bullet[j].active = false; 

                if (enemy[i].isActive) {
                    enemy[i].isActive = false; 
                    updateScore();
                    CreateExplosions(enemiesPosition);
                    // printf("Musuh [%d] hancur!\n", i);
                }
                updateScore();
                CreateExplosions(enemiesPosition);

                // Matikan semua peluru yang ditembak oleh musuh ini
                // for (int k = 0; k < MAX_ENEMY_BULLETS; k++)
                // {
                //     if (enemyBullet[k].shooterIndex == i)
                //     {
                //         enemyBullet[k].isActive = false;
                //     }
                // }
                break; 
            }
        }

        // **Cek tabrakan antara musuh dan pemain (Player menabrak musuh)**
        if (CheckCollisionCircles(playerPosition, 45, enemiesPosition, 40))
        {
            updateNyawa();
            CreateExplosions(playerPosition);
            enemy[i].isActive = false; // Hancurkan musuh
            break;
        }

        // **Cek tabrakan antara peluru musuh dan pemain (Player tertembak)**
        for (int k = 0; k < MAX_ENEMY_BULLETS; k++)
        {
            if (enemyBullet[k].isActive && CheckCollisionCircles(enemyBullet[k].position, 5, playerPosition, 45))
            {
                enemyBullet[k].isActive = false;
                updateNyawa();
                CreateExplosions(playerPosition);
                break;
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
        
        if (IsKeyPressed(KEY_ENTER)) {
            *gameStart = true;  // Set gameStart = true untuk keluar dari menu
        }
    }
}


// void inputName(user *U) {
    
//     InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Form Input Nama - Raylib");

//     int letterCount = 0; 
//     bool mouseOnText = false;
//     bool submitted = false;

//     Rectangle textBox = { SCREEN_WIDTH/2 - 100, 150, 200, 40 };
    
//     SetTargetFPS(60);

//     while (!WindowShouldClose()) {
//         Vector2 mousePoint = GetMousePosition();
//         mouseOnText = CheckCollisionPointRec(mousePoint, textBox);

//         // Jika Textbox aktif dan pengguna mengetik
//         if (mouseOnText) {
//             int key = GetCharPressed();
//             while (key > 0) {
//                 if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS)) {
//                     U->name[letterCount] = (char)key;
//                     U->name[letterCount + 1] = '\0'; // Tambahkan null terminator
//                     letterCount++;
//                 }
//                 key = GetCharPressed();
//             }

//             // Hapus karakter dengan Backspace
//             if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
//                 letterCount--;
//                 U->name[letterCount] = '\0';
//             }
//         }

//         // Cek jika tombol "Submit" ditekan
//         Rectangle submitButton = { SCREEN_WIDTH/2 - 50, 210, 100, 40 };
//         if (CheckCollisionPointRec(mousePoint, submitButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
//             submitted = true;
//         }

//         // Gambar UI
//         BeginDrawing();
//             ClearBackground(RAYWHITE);

//             DrawText("Masukkan Nama Anda:", SCREEN_WIDTH/2 - 100, 120, 20, DARKGRAY);

//             // Gambar kotak input
//             DrawRectangleRec(textBox, LIGHTGRAY);
//             DrawRectangleLinesEx(textBox, 2, mouseOnText ? RED : DARKGRAY);
//             DrawText(U->name, textBox.x + 5, textBox.y + 10, 20, BLACK);

//             // Gambar tombol submit
//             DrawRectangleRec(submitButton, BLUE);
//             DrawText("Submit", submitButton.x + 20, submitButton.y + 10, 20, WHITE);

//             // Jika sudah submit, tampilkan hasilnya
//             if (submitted) {
//                 DrawText(TextFormat("Nama Anda: %s", U->name), SCREEN_WIDTH/2 - 100, 270, 20, DARKGREEN);
//             }

//         EndDrawing();
//     }

//     CloseWindow();
// }

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
    UnloadTexture(ufoTexture);
    UnloadTexture(asteroidTextures);
    UnloadTexture(explosionTexture);
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



