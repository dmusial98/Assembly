#include "stdafx.h"
#include "CppDLL.h"
#include<iostream>
#include"cstdio"


void MaximalFilterCPP(unsigned char *** bitmap_arrays, int from_height, int to_height, int width)
{
	unsigned char temp_array[9];

	for (int k = 0; k < 3; k++)
	{
		for (int i = from_height; i < to_height; i++)
		{
			for (int j = 1; j < width - 1; j++)
			{
				temp_array[0] = bitmap_arrays[k][i - 1][j - 1];
				temp_array[1] = bitmap_arrays[k][i - 1][j];
				temp_array[2] = bitmap_arrays[k][i - 1][j + 1];
				temp_array[3] = bitmap_arrays[k][i][j - 1];
				temp_array[4] = bitmap_arrays[k][i][j];
				temp_array[5] = bitmap_arrays[k][i][j + 1];
				temp_array[6] = bitmap_arrays[k][i + 1][j - 1];
				temp_array[7] = bitmap_arrays[k][i + 1][j];
				temp_array[8] = bitmap_arrays[k][i + 1][j + 1];

				bitmap_arrays[k+3][i][j] = *std::max_element(&temp_array[0], temp_array + 9);
			}
		}
	}
}

