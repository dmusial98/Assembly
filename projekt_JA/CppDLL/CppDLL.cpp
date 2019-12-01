#include "stdafx.h"
#include "CppDLL.h"
#include<iostream>
#include"cstdio"


void MaximalFilterCPP(RGB ** bitmap, RGB ** bitmap_copy, int from_height, int to_height, int width)
    {
	unsigned char temp_array[9];

	for (int i = from_height; i < to_height; i++)
	{
		for (int j = 1; j < width - 1; j++)
		{
			temp_array[0] = bitmap[i - 1][j - 1].red;
			temp_array[1] = bitmap[i - 1][j].red;
			temp_array[2] = bitmap[i - 1][j + 1].red;
			temp_array[3] = bitmap[i][j - 1].red;
			temp_array[4] = bitmap[i][j].red;
			temp_array[5] = bitmap[i][j + 1].red;
			temp_array[6] = bitmap[i + 1][j - 1].red;
			temp_array[7] = bitmap[i + 1][j].red;
			temp_array[8] = bitmap[i + 1][j + 1].red;

			bitmap_copy[i][j].red = *std::max_element(&temp_array[0], temp_array + 9);

			temp_array[0] = bitmap[i - 1][j - 1].green;
			temp_array[1] = bitmap[i - 1][j].green;
			temp_array[2] = bitmap[i - 1][j + 1].green;
			temp_array[3] = bitmap[i][j - 1].green;
			temp_array[4] = bitmap[i][j].green;
			temp_array[5] = bitmap[i][j + 1].green;
			temp_array[6] = bitmap[i + 1][j - 1].green;
			temp_array[7] = bitmap[i + 1][j].green;
			temp_array[8] = bitmap[i + 1][j + 1].green;

			bitmap_copy[i][j].green = *std::max_element(temp_array, temp_array + 9);

			temp_array[0] = bitmap[i - 1][j - 1].blue;
			temp_array[1] = bitmap[i - 1][j].blue;
			temp_array[2] = bitmap[i - 1][j + 1].blue;
			temp_array[3] = bitmap[i][j - 1].blue;
			temp_array[4] = bitmap[i][j].blue;
			temp_array[5] = bitmap[i][j + 1].blue;
			temp_array[6] = bitmap[i + 1][j - 1].blue;
			temp_array[7] = bitmap[i + 1][j].blue;
			temp_array[8] = bitmap[i + 1][j + 1].blue;

			bitmap_copy[i][j].blue = *std::max_element(temp_array, temp_array + 9);
		}
	}
	
}

