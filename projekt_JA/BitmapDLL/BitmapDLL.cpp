// BitmapDLL.cpp : Defines the exported functions for the DLL application
#include "stdafx.h"
#include "BitmapDLL.h"

//extern "C" int Function1(int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K);
extern "C" int MaximalFilter(RGB** bitmap, RGB** bitmap_copy, int height_from, int height_to, int width);


//int DLLEXPORT Function1c(int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K)
//{
//	return Function1(A, B, C, D, E, F, G, H, I , J, K);
//}

void DLLEXPORT MaximalFilterASM(RGB** bitmap, RGB** bitmap_copy, int height_from, int height_to, int width)
{
	MaximalFilter(bitmap, bitmap_copy, height_from, height_to, width);
}




