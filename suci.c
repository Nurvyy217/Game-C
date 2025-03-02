#include "suci.h"
#include <stdio.h>
Texture2D logoDeveloper;
Texture2D gameNamePhoto;
Texture2D background;
bool texturesLoaded = false; // Cek apakah gambar sudah di-load

void initLoadScreen()
{
    if (!texturesLoaded)
    { // Load hanya sekali
        logoDeveloper = LoadTexture("assets/logoG6.png");
        gameNamePhoto = LoadTexture("assets/spaceInviders.png");
        texturesLoaded = true;
    }
}

void initBG(){
    background= LoadTexture("assets/background.png");
}

