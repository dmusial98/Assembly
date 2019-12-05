﻿#include "pch.h"
#include <iostream>
#include "BitmapDLL.h"
#include "CppDLL.h"
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include <thread>

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

unsigned char* file_char = new unsigned char[sizeof(_BITMAPFILEHEADER)];
unsigned char* info_char = new unsigned char[sizeof(_BITMAPINFOHEADER)];
unsigned int threadsNumber = 1;

#pragma pack(pop)

bool read_file(std::string fileName, _BITMAPFILEHEADER &fileHeader, _BITMAPINFOHEADER &infoHeader, RGB ** &bitmap, int &rowOffset)
{
	std::fstream file(fileName, std::fstream::in | std::fstream::binary);

	if (file.is_open())
	{
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

		if ((infoHeader.biWidth * 3) % 4 == 0)
			rowOffset = 0;
		else
			rowOffset =  4 - (infoHeader.biWidth * 3) % 4;

		bitmap = new RGB*[infoHeader.biHeight];

		char bin_buffer[1] = { 'a' };

		for (int i = 0; i < infoHeader.biHeight; i++)
		{
			bitmap[i] = new RGB[infoHeader.biWidth];

			for (int j = 0; j < infoHeader.biWidth; j++)
			{
				file.read(reinterpret_cast<char *>(&bitmap[i][j].red), sizeof(char));
				file.read(reinterpret_cast<char *>(&bitmap[i][j].green), sizeof(char));
				file.read(reinterpret_cast<char *>(&bitmap[i][j].blue), sizeof(char));
			
				if (j == infoHeader.biWidth - 1 && rowOffset != 0)
				{
					for (int a = 0; a < rowOffset; a++)
					{
						file.read(bin_buffer, sizeof(char));
					}
				}
			}
		}
	}
	else 
		return false;

	file.close();
	return true;
}

bool write_file(std::string fileName, _BITMAPFILEHEADER &fileHeader, _BITMAPINFOHEADER &infoHeader,int rowOffset, RGB ** bitmap)
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

		char bin_buffer[1] = { 205 };

		for (int i = 0; i < infoHeader.biHeight; i++)
		{
			for (int j = 0; j < infoHeader.biWidth; j++)
			{
				file.write(reinterpret_cast<char *>(&bitmap[i][j].red), sizeof(char));
				file.write(reinterpret_cast<char *>(&bitmap[i][j].green), sizeof(char));
				file.write(reinterpret_cast<char *>(&bitmap[i][j].blue), sizeof(char));

				if (j == infoHeader.biWidth - 1 && rowOffset != 0)
				{
					for (int a = 0; a < rowOffset; a++)
					{
						file.write(bin_buffer, sizeof(char));
					}
				}
			}
		}
	}
	else
		return false;

	file.close();
	return true;
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

int main(int argc, char* argv[])
{
	if (argc == 5)
	{ 
		std::string readFileName = argv[1], writeFileNameCPP = argv[2], writeFileNameASM = argv[3];
		threadsNumber = atoi(argv[4]);

		if (threadsNumber < 1 || threadsNumber > 64)
			threadsNumber = std::thread::hardware_concurrency();

		_BITMAPFILEHEADER file_header;
		_BITMAPINFOHEADER info_header;
		RGB ** bitmap = nullptr;
		RGB ** bitmap_copy = nullptr;
		int rowOffset = 0;


		if (!read_file(readFileName, file_header, info_header, bitmap, rowOffset))
			std::cout << "Can not open file.\n";

		bitmap_copy = new RGB *[info_header.biHeight];

		for (int i = 0; i < info_header.biHeight; i++)
			bitmap_copy[i] = new RGB[info_header.biWidth];
		std::cout << "RED:\n\n";
		printf("0 0: %#x 1: %#X 2: %#x 3: %#x 4: %#x\n", bitmap[0][0].red, bitmap[0][1].red, bitmap[0][2].red, bitmap[0][3].red, bitmap[0][4].red);
		printf("1 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[1][0].red, bitmap[1][1].red, bitmap[1][2].red, bitmap[1][3].red, bitmap[1][4].red);
		printf("2 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[2][0].red, bitmap[2][1].red, bitmap[2][2].red, bitmap[2][3].red, bitmap[2][4].red);
		std::cout << "GREEN\n\n";
		printf("0 0: %#x 1: %#X 2: %#x 3: %#x 4: %#x\n", bitmap[0][0].green, bitmap[0][1].green, bitmap[0][2].green, bitmap[0][3].green, bitmap[0][4].green);
		printf("1 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[1][0].green, bitmap[1][1].green, bitmap[1][2].green, bitmap[1][3].green, bitmap[1][4].green);
		printf("2 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[2][0].green, bitmap[2][1].green, bitmap[2][2].green, bitmap[2][3].green, bitmap[2][4].green);
		std::cout << "BLUE\n\n";
		printf("0 0: %#x 1: %#X 2: %#x 3: %#x 4: %#x\n", bitmap[0][0].blue, bitmap[0][1].blue, bitmap[0][2].blue, bitmap[0][3].blue, bitmap[0][4].blue);
		printf("1 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[1][0].blue, bitmap[1][1].blue, bitmap[1][2].blue, bitmap[1][3].blue, bitmap[1][4].blue);
		printf("2 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[2][0].blue, bitmap[2][1].blue, bitmap[2][2].blue, bitmap[2][3].blue, bitmap[2][4].blue);

		std::cout << "RED:\n\n";
		printf("1 0: %#x 1: %#X 2: %#x 3: %#x 4: %#x\n", bitmap[1][0].red, bitmap[1][1].red, bitmap[1][2].red, bitmap[1][3].red, bitmap[1][4].red);
		printf("2 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[2][0].red, bitmap[2][1].red, bitmap[2][2].red, bitmap[2][3].red, bitmap[2][4].red);
		printf("3 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[3][0].red, bitmap[3][1].red, bitmap[3][2].red, bitmap[3][3].red, bitmap[3][4].red);
		std::cout << "GREEN\n\n";
		printf("1 0: %#x 1: %#X 2: %#x 3: %#x 4: %#x\n", bitmap[1][0].green, bitmap[1][1].green, bitmap[1][2].green, bitmap[1][3].green, bitmap[1][4].green);
		printf("2 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[2][0].green, bitmap[2][1].green, bitmap[2][2].green, bitmap[2][3].green, bitmap[2][4].green);
		printf("3 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[3][0].green, bitmap[3][1].green, bitmap[3][2].green, bitmap[3][3].green, bitmap[3][4].green);
		std::cout << "BLUE\n\n";
		printf("1 0: %#x 1: %#X 2: %#x 3: %#x 4: %#x\n", bitmap[1][0].blue, bitmap[1][1].blue, bitmap[1][2].blue, bitmap[1][3].blue, bitmap[1][4].blue);
		printf("2 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[2][0].blue, bitmap[2][1].blue, bitmap[2][2].blue, bitmap[2][3].blue, bitmap[2][4].blue);
		printf("3 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[3][0].blue, bitmap[3][1].blue, bitmap[3][2].blue, bitmap[3][3].blue, bitmap[3][4].blue);

		std::cout << "RED:\n\n";
		printf("2 0: %#x 1: %#X 2: %#x 3: %#x 4: %#x\n", bitmap[2][0].red, bitmap[2][1].red, bitmap[2][2].red, bitmap[2][3].red, bitmap[2][4].red);
		printf("3 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[3][0].red, bitmap[3][1].red, bitmap[3][2].red, bitmap[3][3].red, bitmap[3][4].red);
		printf("4 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[4][0].red, bitmap[4][1].red, bitmap[4][2].red, bitmap[4][3].red, bitmap[4][4].red);
		std::cout << "GREEN\n\n";
		printf("2 0: %#x 1: %#X 2: %#x 3: %#x 4: %#x\n", bitmap[2][0].green, bitmap[2][1].green, bitmap[2][2].green, bitmap[2][3].green, bitmap[2][4].green);
		printf("3 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[3][0].green, bitmap[3][1].green, bitmap[3][2].green, bitmap[3][3].green, bitmap[3][4].green);
		printf("4 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[4][0].green, bitmap[4][1].green, bitmap[4][2].green, bitmap[4][3].green, bitmap[4][4].green);
		std::cout << "BLUE\n\n";
		printf("2 0: %#x 1: %#X 2: %#x 3: %#x 4: %#x\n", bitmap[2][0].blue, bitmap[2][1].blue, bitmap[2][2].blue, bitmap[2][3].blue, bitmap[2][4].blue);
		printf("3 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[3][0].blue, bitmap[3][1].blue, bitmap[3][2].blue, bitmap[3][3].blue, bitmap[3][4].blue);
		printf("4 0: %#x 1: %#x 2: %#x 3: %#x 4: %#x\n", bitmap[4][0].blue, bitmap[4][1].blue, bitmap[4][2].blue, bitmap[4][3].blue, bitmap[4][4].blue);

		
		std::cout << "&bitmap[0] " << &bitmap[0] << std::endl;
		std::cout << "&bitmap[1] " << &bitmap[1] << std::endl;
		std::cout << "&bitmap[2] " << &bitmap[2] << std::endl;
		std::cout << "&bitmap[0][0] " << &bitmap[0][0] << std::endl;
		std::cout << "&bitmap[1][0] " << &bitmap[1][0] << std::endl;
		std::cout << "&bitmap[2][0] " << &bitmap[2][0] << std::endl;

		std::vector<std::thread> threads;

		int from = 1;
		int to = info_header.biHeight / threadsNumber;
		for (int i = 0; i < threadsNumber; i++)
		{
			if (threadsNumber == 1)
				to--;

			threads.push_back(std::move(std::thread(MaximalFilterCPP, bitmap, bitmap_copy, from, to, info_header.biWidth)));

			from = to;
			to += info_header.biHeight / threadsNumber;

			if (to >= info_header.biHeight - 1)
				to = info_header.biHeight - 1;
		}

		for (int i = 0; i < threads.size(); i++)
		{
			threads[i].join();
		}

		if (!write_file(writeFileNameCPP, file_header, info_header, rowOffset, bitmap_copy))
			std::cout << "Wrong name of file for write";

		threads.clear();

		from = 1;
		to = info_header.biHeight / threadsNumber;
		for (int i = 0; i < threadsNumber; i++)
		{
			if (threadsNumber == 1)
				to--;

			threads.push_back(std::move(std::thread(MaximalFilterASM, bitmap, bitmap_copy, from, to, info_header.biWidth)));

			from = to;
			to += info_header.biHeight / threadsNumber;

			if (to >= info_header.biHeight - 1)
				to = info_header.biHeight - 1;
		}

		for (int i = 0; i < threads.size(); i++)
		{
			threads[i].join();
		}

		if (!write_file(writeFileNameASM, file_header, info_header, rowOffset, bitmap_copy))
			std::cout << "Wrong name of file for write";
	}
	else
	{
		std::cout << "Incorrect arguments.";
	}
	return 0;
}

