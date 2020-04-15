// BitmapDLL.cpp : Defines the exported functions for the DLL application
#include "stdafx.h"
#include "BitmapDLL.h"

extern "C" void MaximalFilter(int height_from, int height_to, int width, unsigned char*** bitmap_arrays);

void DLLEXPORT MaximalFilterASM(int height_from, int height_to, int width, unsigned char*** bitmap_arrays)
{
	MaximalFilter(height_from, height_to, width, bitmap_arrays);
}
