#ifndef SUPRIADI_H
#define SUPRIADI_H

#include "raylib.h"

#define NYAWA_AWAL 16

extern Texture2D nyawaIMG;
extern int score;
extern int NyawaMaks;

void BuatNyawa();
void rubahNyawa();
void Tampil_Nyawa();
void unloadNyawa();
bool gameover();
void restart();

#endif