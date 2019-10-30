#include "pch.h"
#include <iostream>
#include "BitmapDLL.h"
#include "CppDLL.h"
#include <windows.h>
#include<fstream>
#include<string>
#include<cstdio>
#include<algorithm>


#pragma pack(push, 1)
struct _BITMAPFILEHEADER
{
	unsigned char bfType[2];
	unsigned int bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int bfOffBits;
};

struct _BITMAPINFOHEADER
{
	unsigned int biSize;
	int biWidth;
	int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
};

struct RGB {
	 unsigned char red, green, blue;
};

unsigned char* file_char = new unsigned char[sizeof(_BITMAPFILEHEADER)];
unsigned char* info_char = new unsigned char[sizeof(_BITMAPINFOHEADER)];

#pragma pack(pop)

void read_file(std::string fileName, _BITMAPFILEHEADER &fileHeader, _BITMAPINFOHEADER &infoHeader, RGB ** &bitmap, int &rowOffset)
{
	std::fstream file(fileName, std::fstream::in | std::fstream::binary);


	file.read(reinterpret_cast<char *>(file_char), sizeof(_BITMAPFILEHEADER));
	file.read(reinterpret_cast<char *>(info_char), sizeof(_BITMAPINFOHEADER));

	fileHeader.bfType[0] = file_char[0];
	fileHeader.bfType[1] = file_char[1];
	fileHeader.bfSize = static_cast<unsigned int>(file_char[5] * pow(2, 24) + file_char[4] * pow(2, 16) + file_char[3] * pow(2, 8) + file_char[2]);
	fileHeader.bfReserved1 = static_cast<unsigned short>(file_char[7] * pow(2, 8) + file_char[6]);
	fileHeader.bfReserved2 = static_cast<unsigned short>(file_char[9] * pow(2, 8) + file_char[8]);
	fileHeader.bfOffBits = static_cast<unsigned int>(file_char[13] * pow(2, 24) + file_char[12] * pow(2, 16) + file_char[11] * pow(2, 8) + file_char[10]);

	infoHeader.biSize = static_cast<unsigned int>(info_char[3] * pow(2, 24) + info_char[2] * pow(2, 16) + info_char[1] * pow(2, 8) + info_char[0]);
	infoHeader.biWidth = static_cast<int>(info_char[7] * pow(2, 24) + info_char[6] * pow(2, 16) + info_char[5] * pow(2, 8) + info_char[4]);
	infoHeader.biHeight = static_cast<int>(info_char[11] * pow(2, 24) + info_char[10] * pow(2, 16) + info_char[9] * pow(2, 8) + info_char[8]);
	infoHeader.biPlanes = static_cast<unsigned short>(info_char[13] * pow(2, 8) + info_char[12]);
	infoHeader.biBitCount = static_cast<unsigned short>(info_char[15] * pow(2, 8) + info_char[14]);
	infoHeader.biCompression = static_cast<unsigned int>(info_char[19] * pow(2, 24) + info_char[18] * pow(2, 16) + info_char[17] * pow(2, 8) + info_char[16]);
	infoHeader.biSizeImage = static_cast<unsigned int>(info_char[23] * pow(2, 24) + info_char[22] * pow(2, 16) + info_char[21] * pow(2, 8) + info_char[20]);
	infoHeader.biXPelsPerMeter = static_cast<int>(info_char[27] * pow(2, 24) + info_char[26] * pow(2, 16) + info_char[25] * pow(2, 8) + info_char[24]);
	infoHeader.biYPelsPerMeter = static_cast<int>(info_char[31] * pow(2, 24) + info_char[30] * pow(2, 16) + info_char[29] * pow(2, 8) + info_char[28]);
	infoHeader.biClrUsed = static_cast<unsigned int>(info_char[35] * pow(2, 24) + info_char[34] * pow(2, 16) + info_char[33] * pow(2, 8) + info_char[32]);
	infoHeader.biClrImportant = static_cast<unsigned int>(info_char[39] * pow(2, 24) + info_char[38] * pow(2, 16) + info_char[37] * pow(2, 8) + info_char[36]);


	rowOffset = infoHeader.biWidth % 4;

	bitmap = new RGB*[infoHeader.biHeight];
	

	for (int i = 0; i < infoHeader.biHeight; i++)
	{
		bitmap[i] = new RGB[infoHeader.biWidth + rowOffset];

		for (int j = 0; j < infoHeader.biWidth + rowOffset; j++)
		{
			file.read(reinterpret_cast<char *>(&bitmap[i][j].red), sizeof(char));
			file.read(reinterpret_cast<char *>(&bitmap[i][j].green), sizeof(char));
			file.read(reinterpret_cast<char *>(&bitmap[i][j].blue), sizeof(char));
		}
	}

	file.close();
}

void write_file(std::string fileName, _BITMAPFILEHEADER &fileHeader, _BITMAPINFOHEADER &infoHeader,int rowOffset, RGB ** bitmap)
{
	std::fstream file(fileName, std::ios_base::out | std::ios_base::binary);

	if (file.is_open())
	{
		file.write(reinterpret_cast<char *>(&fileHeader.bfType), sizeof(char) * 2);
		file.write(reinterpret_cast<char *>(&fileHeader.bfSize), sizeof(fileHeader.bfSize));
		file.write(reinterpret_cast<char *>(&fileHeader.bfReserved1), sizeof(fileHeader.bfReserved1));
		file.write(reinterpret_cast<char *>(&fileHeader.bfReserved2), sizeof(fileHeader.bfReserved2));
		file.write(reinterpret_cast<char *>(&fileHeader.bfOffBits), sizeof(fileHeader.bfOffBits));

		file.write(reinterpret_cast<char *>(&infoHeader.biSize), sizeof(infoHeader.biSize));
		file.write(reinterpret_cast<char *>(&infoHeader.biWidth), sizeof(infoHeader.biWidth));
		file.write(reinterpret_cast<char *>(&infoHeader.biHeight), sizeof(infoHeader.biHeight));
		file.write(reinterpret_cast<char *>(&infoHeader.biPlanes), sizeof(infoHeader.biPlanes));
		file.write(reinterpret_cast<char *>(&infoHeader.biBitCount), sizeof(infoHeader.biBitCount));
		file.write(reinterpret_cast<char *>(&infoHeader.biCompression), sizeof(infoHeader.biCompression));
		file.write(reinterpret_cast<char *>(&infoHeader.biSizeImage), sizeof(infoHeader.biSizeImage));
		file.write(reinterpret_cast<char *>(&infoHeader.biXPelsPerMeter), sizeof(infoHeader.biXPelsPerMeter));
		file.write(reinterpret_cast<char *>(&infoHeader.biYPelsPerMeter), sizeof(infoHeader.biYPelsPerMeter));
		file.write(reinterpret_cast<char *>(&infoHeader.biClrUsed), sizeof(infoHeader.biClrUsed));
		file.write(reinterpret_cast<char *>(&infoHeader.biClrImportant), sizeof(infoHeader.biClrImportant));

		for (int i = 0; i < infoHeader.biHeight; i++)
		{
			for (int j = 0; j < infoHeader.biWidth + rowOffset; j++)
			{
				file.write(reinterpret_cast<char *>(&bitmap[i][j].red), sizeof(char));
				file.write(reinterpret_cast<char *>(&bitmap[i][j].green), sizeof(char));
				file.write(reinterpret_cast<char *>(&bitmap[i][j].blue), sizeof(char));
			}
		}
	}

	file.close();
}

void display_headers(_BITMAPFILEHEADER file_header, _BITMAPINFOHEADER info_header)
{
	std::cout << file_header.bfType << std::endl;
	std::cout << file_header.bfSize << std::endl;
	std::cout << file_header.bfReserved1 << std::endl;
	std::cout << file_header.bfReserved2 << std::endl;
	std::cout << file_header.bfOffBits << std::endl;

	std::cout << info_header.biSize << std::endl;
	std::cout << info_header.biWidth << std::endl;
	std::cout << info_header.biHeight << std::endl;
	std::cout << info_header.biPlanes << std::endl;
	std::cout << info_header.biBitCount << std::endl;
	std::cout << info_header.biCompression << std::endl;
	std::cout << info_header.biSizeImage << std::endl;
	std::cout << info_header.biXPelsPerMeter << std::endl;
	std::cout << info_header.biYPelsPerMeter << std::endl;
	std::cout << info_header.biClrUsed << std::endl;
	std::cout << info_header.biClrImportant << std::endl;
}


int main()
{
	
	Function1c();
	Function2c();
	FunctionCpp();

	
	_BITMAPFILEHEADER file_header;
	_BITMAPINFOHEADER info_header;
	RGB ** bitmap = nullptr;
	RGB ** bitmap_copy = nullptr;
	int rowOffset = 0;

	read_file("obraz2.bmp", file_header, info_header, bitmap, rowOffset);

	

	//algorytm
	bitmap_copy = new RGB *[info_header.biHeight];

	for (int i = 0; i < info_header.biHeight; i++)
		bitmap_copy[i] = new RGB[info_header.biWidth + rowOffset];

		unsigned char temp_array[9];

		for (int i = 1; i < info_header.biHeight - 1; i++)
		{
			for (int j = 1; j < info_header.biWidth + rowOffset - 1; j++)
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

				bitmap_copy[i][j].green = *std::max_element(temp_array, temp_array+9);

				temp_array[0] = bitmap[i - 1][j - 1].blue;
				temp_array[1] = bitmap[i - 1][j].blue;
				temp_array[2] = bitmap[i - 1][j + 1].blue;
				temp_array[3] = bitmap[i][j - 1].blue;
				temp_array[4] = bitmap[i][j].blue;
				temp_array[5] = bitmap[i][j + 1].blue;
				temp_array[6] = bitmap[i + 1][j - 1].blue;
				temp_array[7] = bitmap[i + 1][j].blue;
				temp_array[8] = bitmap[i + 1][j + 1].blue;

				bitmap_copy[i][j].blue = *std::max_element(temp_array, temp_array+9);
			}
		}

		write_file("obraz2a.bmp", file_header, info_header, rowOffset, bitmap_copy);

	

	return 0;
}

