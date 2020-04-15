#pragma once

#define DLLIMPORT __declspec(dllimport)
#define DLLEXPORT __declspec(dllexport)

#include "..\projekt_JA\RBG.h"

void DLLEXPORT MaximalFilterASM(int height_from, int height_to, int width, unsigned char*** bitmap_arrays);
