#include "nazwa.h"
#include "hasbi.h"
#include "supriadi.h"
#include "fawwaz.h"
#include "stdio.h"

Texture2D heartTexture;
Texture2D enemyBulletlv3;
Texture2D menuTexture;
Texture2D soundOnTexture;
Texture2D soundOffTexture;
Texture2D restartTexture;
Texture2D pauseTexture;
Texture2D quitTexture;
Texture2D gamePauseTexture;
Texture2D ufoTexture;

bool soundOn = true;
bool soundAssets = true;
bool startGame = false;

void varMenu(bool *soundAssets)
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

    if (isMouseOver && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        showPopup = !showPopup;
    }

    DrawTextureEx(menuTexture, texturePos, 0.0f, scale, WHITE);

    // Jika pop-up aktif, tampilkan di tengah layar
    if (showPopup)
    {
        int popupWidth = 370;
        int popupHeight = 350;
        int popupX = (SCREEN_WIDTH - popupWidth) / 2;
        int popupY = (SCREEN_HEIGHT - popupHeight) / 2;

        DrawRectangle(popupX - 5, popupY - 5, popupWidth + 10, popupHeight + 10, Fade(BLACK, 0.5f));

        DrawRectangle(popupX, popupY, popupWidth, popupHeight, BLACK);

        DrawRectangleLinesEx((Rectangle){popupX - 2, popupY - 2, popupWidth + 4, popupHeight + 4}, 3, DARKBLUE);
        DrawRectangleLinesEx((Rectangle){popupX - 4, popupY - 4, popupWidth + 8, popupHeight + 8}, 2, BLACK);

        DrawText("Popup Menu", popupX + 20, popupY + 20, 25, WHITE);

        // Posisi elemen dalam pop-up
        int iconStartX = popupX + 20;
        int iconStartY = popupY + 60;
        int iconSpacing = 70;
        float iconScale = 0.5f;
        int textOffsetX = 80;

        // *Cek apakah tombol F ditekan untuk mengubah soundAssets*
        if (IsKeyPressed(KEY_F))
        {
            *soundAssets = !(*soundAssets); // Toggle sound on/off
        }

        // Tombol sound
        Texture2D currentSoundTexture = *soundAssets ? soundOnTexture : soundOffTexture;
        DrawTextureEx(currentSoundTexture, (Vector2){iconStartX, iconStartY}, 0.0f, 0.4f, WHITE);
        DrawText("Press F to Off/On", iconStartX + textOffsetX, iconStartY + 15, 23, WHITE);

        // Tombol Restart
        DrawTextureEx(restartTexture, (Vector2){iconStartX, iconStartY + iconSpacing}, 0.0f, iconScale, WHITE);
        DrawText("Press R to Restart", iconStartX + textOffsetX, iconStartY + iconSpacing + 20, 23, WHITE);

        // Tombol Pause
        DrawTextureEx(pauseTexture, (Vector2){iconStartX, iconStartY + iconSpacing * 2}, 0.0f, iconScale, WHITE);
        DrawText("Press P to Pause", iconStartX + textOffsetX, iconStartY + iconSpacing * 2 + 22, 23, WHITE);

        // Tombol Quit
        DrawTextureEx(quitTexture, (Vector2){iconStartX, iconStartY + iconSpacing * 3}, 0.0f, iconScale, WHITE);
        DrawText("Press Q to Quit", iconStartX + textOffsetX, iconStartY + iconSpacing * 3 + 25, 23, WHITE);
    }
}

void varQuit()
{
    if (IsKeyPressed(KEY_Q))
    {                  // Tekan Q untuk keluar
        CloseWindow(); // Tutup jendela Raylib
    }
}

bool isPaused = false;

void togglePause()
{
    if (IsKeyPressed(KEY_P))
    {
        isPaused = !isPaused;
    }
}

bool getPauseState()
{
    return isPaused;
}

void gamePaused()
{
    float iconScale = 0.5f;
    int textOffsetX = 80; // Jarak teks dari ikon

    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);

    DrawTextureEx(gamePauseTexture, (Vector2){210, 300}, 0.0f, iconScale, WHITE);
    DrawText("Press P to start", 260, 530, 23, WHITE);
}

void varRestart()
{
    if (IsKeyPressed(KEY_R))
    {
        InfoPlayer.nyawa = NYAWA_AWAL;
        InfoPlayer.score = 0;
        ResetEnemies();
        ResetEnemyBullets();
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
    if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
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
    if (!*gameStart)
    {
        // DrawRectangle(GAMEPLAY_WIDTH, 0, MENU_WIDTH, SCREEN_HEIGHT, DARKBLUE);
        DrawRectangle(GAMEPLAY_WIDTH, 0, MENU_WIDTH, SCREEN_HEIGHT, BLACK);
        // DrawRectangle(GAMEPLAY_WIDTH, 0, MENU_WIDTH, SCREEN_HEIGHT, (Color){3, 24, 37, 255});
        DrawText(TextFormat("Level: %d", level), menuX - 85, startY + 80, 30, RAYWHITE);
        varMenu(&isSoundOn);
        // varHeart(heartTexture);
        tampilNyawa();
        Tampil_Score();
        TampilInfoPowerup();    
    }
}

BulletNode *BulletHead = NULL;
Texture2D bulletTexture;

void InitBullets()
{
    if (BulletHead == NULL)
    {
        BulletNode *current = NULL;

        for (int i = 0; i < MAX_BULLETS; i++)
        {
            BulletNode *newNode = (BulletNode *)malloc(sizeof(BulletNode));
            if (newNode == NULL)
            {
                // Gagal alokasi memori, bisa beri pesan atau hentikan
                printf("Gagal alokasi memori untuk BulletNode\n");
                return;
            }
            newNode->data.active = false;
            newNode->next = NULL;

            if (BulletHead == NULL)
            {
                BulletHead = newNode;
                current = BulletHead;
            }
            else
            {
                current->next = newNode;
                current = newNode;
            }
        }
    }
    else
    {
        BulletNode *current = BulletHead;
        while (current != NULL)
        {
            current->data.active = false;
            current = current->next;
        }
    }
}

void ShootBullet()
{
    int maxBullets = InfoPlayer.DoubleAttack ? 10 : 5;
    int currentActiveBullets = 0;
    BulletNode *current = BulletHead;

    while (current != NULL)
    {
        if (current->data.active)
            currentActiveBullets++;
        current = current->next;
    }

    if (currentActiveBullets >= maxBullets)
        return;

    current = BulletHead;

    while (current != NULL)
    {
        if (!current->data.active)
        {
            current->data.position = (Vector2){(player.position.x - 25) + player.texture.width * 0.6f / 2, (player.position.y + player.texture.width * 0.6f / 2) - 110};
            PlaySound(shootSound);
            current->data.active = true;
            break;
        }
        current = current->next;
    }
}



void UpdateBullets()
{
    BulletNode *current = BulletHead;

    while (current != NULL)
    {
        if (current->data.active)
        {
            current->data.position.y -= BULLET_SPEED;
            if (current->data.position.y < 0)
            {
                current->data.active = false;
            }
        }
        current = current->next;
    }
}

void DrawBullets()
{
    BulletNode *current = BulletHead;
    while (current != NULL)
    {
        if (current->data.active)
        {
            DrawTexture(bulletTexture, current->data.position.x, current->data.position.y, WHITE);
        }
        current = current->next;
    }
}

void freeBullets()
{
    BulletNode *current = BulletHead;
    while (current != NULL)
    {
        BulletNode *temp = current;
        current = current->next;
        free(temp);
    }
    BulletHead = NULL;
}

void ResetPlayerBulet()
{
    BulletNode *current = BulletHead;
    while (current != NULL)
    {
        current->data.active = false;
        current = current->next;
    }
}

ExplosionNode *ExplosionHead = NULL;
Texture2D explosionsTexture;

void CreateExplosion(Vector2 position)
{
    ExplosionNode *current = ExplosionHead;
    ExplosionNode *last = NULL;

    // Cari node yang tidak aktif
    while (current != NULL)
    {
        if (!current->data.active)
        {
            current->data.position = position;
            current->data.active = true;
            current->data.frame = 0;
            current->data.timer = 0;
            return;
        }
        last = current;
        current = current->next;
    }

    // Jika tidak ada node yang tidak aktif, buat node baru
    ExplosionNode *newNode = (ExplosionNode *)malloc(sizeof(ExplosionNode));
    newNode->data.position = position;
    newNode->data.active = true;
    newNode->data.frame = 0;
    newNode->data.timer = 0;
    newNode->next = NULL;

    if (ExplosionHead == NULL)
    {
        ExplosionHead = newNode;
    }
    else
    {
        last->next = newNode;
    }
}

void UpdateExplosions(float deltaTime)
{
    ExplosionNode *current = ExplosionHead;

    while (current != NULL)
    {
        if (current->data.active)
        {
            current->data.timer += deltaTime;
            if (current->data.timer > 0.1f)
            {
                current->data.frame++;
                current->data.timer = 0;
            }

            if (current->data.frame >= 5)
            {
                current->data.active = false;
            }
        }
        current = current->next;
    }
}

void DrawExplosions(Texture2D explosionTexture)
{
    ExplosionNode *current = ExplosionHead;
    while (current != NULL)
    {
        if (current->data.active)
        {
            Rectangle source = {current->data.frame * 64, 0, 64, 64};
            Rectangle dest = {current->data.position.x, current->data.position.y, 128, 128};
            DrawTexturePro(explosionsTexture, source, dest, (Vector2){18, 0}, 0, WHITE);
        }
        current = current->next;
    }
}

void freeExplosions()
{
    ExplosionNode *current = ExplosionHead;
    while (current != NULL)
    {
        ExplosionNode *temp = current;
        current = current->next;
        free(temp);
    }
    ExplosionHead = NULL;
}

void ResetExplosions()
{
    ExplosionNode *current = ExplosionHead;
    while (current != NULL)
    {
        current->data.active = false;
        current = current->next;
    }
}

Texture2D eneBul, ufoBroken;
void loadAssetMenu()
{

    menuTexture = LoadTexture("assets/menuTexture.png");
    soundOnTexture = LoadTexture("assets/soundOn.png");
    soundOffTexture = LoadTexture("assets/soundOff.png");
    restartTexture = LoadTexture("assets/restart.png");
    pauseTexture = LoadTexture("assets/pause.png");
    quitTexture = LoadTexture("assets/quit.png");
    gamePauseTexture = LoadTexture("assets/gamePause.png");
    bulletTexture = LoadTexture("assets/bullet.png");
    explosionsTexture = LoadTexture("assets/Explosions.png");
    ufoTexture = LoadTexture("assets/ufo.png");
    enemyBulletlv3 = LoadTexture("assets/laserUfo.png");
    eneBul = LoadTexture("assets/eneBull.png");
    ufoBroken = LoadTexture("assets/ufoBroken.png");
}

void unloadAssetMenu()
{
    UnloadTexture(menuTexture);
    UnloadTexture(soundOnTexture);
    UnloadTexture(soundOffTexture);
    UnloadTexture(restartTexture);
    UnloadTexture(pauseTexture);
    UnloadTexture(quitTexture);
    UnloadTexture(gamePauseTexture);
}

void DrawLvl2()
{
    DrawLayout();
    DrawPlayer();
    DrawBullets();
    DrawExplosions(explosionsTexture);
    DrawEnemies(ufoTexture, ufoBroken, 1.5f, 80, 110, &gamestate);
    DrawEnemyBullets(eneBul, 1.2f, &gamestate);
    tampilspark();
}

void level2(float deltaTime)
{
    setEnemyTypeShoot(&gamestate, 2);
    setMaxEnemyBullet(&gamestate, 9);
    setHealthBroke(&gamestate, 1);
    UpdatePlayer();
    UpdateShooting(deltaTime);
    UpdateBullets();
    UpdateExplosions(deltaTime);
    UpdateEnemies(ufoTexture, -30, 100, 0, 70, &gamestate);
    UpdateEnemyBullets(eneBul, &gamestate);
    CheckEnemyCollisions(65, 70, 55, 10, &gamestate); // x,y,radP,radBE
    EnemiesLoop(ufoTexture, -30, 100, 0, 70, &gamestate);
    DrawLvl2();
    inipowerup();
    UpdateSpark();
}
