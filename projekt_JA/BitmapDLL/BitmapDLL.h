#pragma once

#define DLLIMPORT __declspec(dllimport)
#define DLLEXPORT __declspec(dllexport)

#include "..\projekt_JA\RBG.h"

int DLLEXPORT Function1c(int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K);
void DLLEXPORT MaximalFilterASM(RGB** bitmap, RGB** bitmap_copy, int height_from, int height_to, int width);