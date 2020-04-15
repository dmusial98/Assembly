#pragma once

#ifdef BITMAPCPP_EXPORTS
#define BITMAPCPP_API __declspec(dllexport)
#else
#define BITMAPCPP_API __declspec(dllimport)
#endif

#include<algorithm>
#include"..\projekt_JA\RBG.h"

void BITMAPCPP_API MaximalFilterCPP(unsigned char *** bitmap_arrays, int from_height, int to_height, int width);
 