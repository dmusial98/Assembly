// BitmapDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "BitmapDLL.h"

extern "C" int Function1();
extern "C" int Function2();


int DLLEXPORT Function1c()
{
	return Function1();
}

int DLLEXPORT Function2c()
{
	return Function2();
}




