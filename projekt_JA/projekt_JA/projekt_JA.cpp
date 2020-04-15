#include "pch.h"
#include <iostream>
#include "BitmapDLL.h"
#include "CppDLL.h"
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include <thread>
#include <chrono>
#include <ctime>

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
std::chrono::duration<double> CPPTime;
std::chrono::duration<double> ASMTime;

#pragma pack(pop)

bool read_file_(std::string fileName, _BITMAPFILEHEADER &fileHeader, _BITMAPINFOHEADER &infoHeader, 
	unsigned char *** bitmap_arrays, int &rowOffset)
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
			rowOffset = 4 - (infoHeader.biWidth * 3) % 4;

		bitmap_arrays[0] = new unsigned char* [infoHeader.biHeight];
		bitmap_arrays[1] = new unsigned char* [infoHeader.biHeight];
		bitmap_arrays[2] = new unsigned char* [infoHeader.biHeight];

		char bin_buffer[1] = { 'a' };

		for (int i = 0; i < infoHeader.biHeight; i++)
		{
			bitmap_arrays[0][i] = new unsigned char [infoHeader.biWidth];
			bitmap_arrays[1][i] = new unsigned char[infoHeader.biWidth];
			bitmap_arrays[2][i] = new unsigned char[infoHeader.biWidth];

			for (int j = 0; j < infoHeader.biWidth; j++)
			{
				file.read(reinterpret_cast<char *>(&bitmap_arrays[0][i][j]), sizeof(char));
				file.read(reinterpret_cast<char *>(&bitmap_arrays[1][i][j]), sizeof(char));
				file.read(reinterpret_cast<char *>(&bitmap_arrays[2][i][j]), sizeof(char));

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

bool write_file_(std::string fileName, _BITMAPFILEHEADER &fileHeader, _BITMAPINFOHEADER &infoHeader, int rowOffset, unsigned char *** bitmap)
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
				file.write(reinterpret_cast<char *>(&bitmap[3][i][j]), sizeof(char));
				file.write(reinterpret_cast<char *>(&bitmap[4][i][j]), sizeof(char));
				file.write(reinterpret_cast<char *>(&bitmap[5][i][j]), sizeof(char));

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

int main(int argc, char* argv[])
 {
	if (argc == 5 || argc == 4)
	{

		std::string readFileName = argv[1], writeFileNameCPP = argv[2], writeFileNameASM = argv[3];
		if (argc == 5) //podano liczbę wątków
		{
			threadsNumber = atoi(argv[4]);

			if (threadsNumber < 1 || threadsNumber > 64)
				threadsNumber = std::thread::hardware_concurrency();
		}
		else
		{
			threadsNumber = std::thread::hardware_concurrency();
		}

		_BITMAPFILEHEADER file_header;
		_BITMAPINFOHEADER info_header;
	
		int rowOffset = 0;

		unsigned char *** bitmap_arrays = new unsigned char **[6]; 
		//pierwsze trzy tablice: red, green, blue
		//nastepne trzy to red_copy, green_copy i blue_copy

		if (read_file_(readFileName, file_header, info_header, bitmap_arrays, rowOffset))
		{


			bitmap_arrays[3] = new unsigned char *[info_header.biHeight];
			bitmap_arrays[4] = new unsigned char *[info_header.biHeight];
			bitmap_arrays[5] = new unsigned char *[info_header.biHeight];

			for (int i = 0; i < info_header.biHeight; i++)
			{
				bitmap_arrays[3][i] = new unsigned char[info_header.biWidth];
				bitmap_arrays[4][i] = new unsigned char[info_header.biWidth];
				bitmap_arrays[5][i] = new unsigned char[info_header.biWidth];
			}

			std::vector<std::thread> threads;

			int from;
			int to;

			from = 1;
			to = info_header.biHeight / threadsNumber;

			auto startTime = std::chrono::steady_clock::now();
			
			for (int i = 0; i < threadsNumber; i++)
			{ //pętla odpowiedzialna za rozdysponowanie zadań wątkom funkcji ASM
				if (threadsNumber == 1)
					to = info_header.biHeight - 1;

				threads.push_back(std::move(std::thread(MaximalFilterASM, from, to, info_header.biWidth, bitmap_arrays)));

				from = to;
				to += info_header.biHeight / threadsNumber;

				if (to >= info_header.biHeight - 1)
					to = info_header.biHeight - 1;
			}

			for (int i = 0; i < threads.size(); i++)
			{
				threads[i].join();		
			}

			auto endTime = std::chrono::steady_clock::now();
			ASMTime = endTime - startTime; //zliczenie czasu dla ASM

			if (!write_file_(writeFileNameASM, file_header, info_header, rowOffset, bitmap_arrays))
				std::cout << "Wrong name of file for write - ASM\n";

			std::cout << "ASM time: " << ASMTime.count() << std::endl;
	

			threads.clear();

			from = 1;
			to = info_header.biHeight / threadsNumber;

			startTime = std::chrono::steady_clock::now();

			for (int i = 0; i < threadsNumber; i++)
			{ //pętla odpowiedzialna za rozdysponowanie zadań wątkom funkcji C++
				if (threadsNumber == 1)
					to--;

				threads.push_back(std::move(std::thread(MaximalFilterCPP, bitmap_arrays, from, to, info_header.biWidth)));

				from = to;
				to += info_header.biHeight / threadsNumber;

				if (to >= info_header.biHeight - 1)
					to = info_header.biHeight - 1;
			}

			for (int i = 0; i < threads.size(); i++)
			{
				threads[i].join();
			}

			endTime = std::chrono::steady_clock::now();

			CPPTime = endTime - startTime; //zliczenie czasu dla C++

			if (!write_file_(writeFileNameCPP, file_header, info_header, rowOffset, bitmap_arrays))
				std::cout << "Wrong name of file for write - CPP\n";

			std::cout << "CPP time: " << CPPTime.count() << std::endl;
		}
		else
		{
			std::cout << "Cannot open file " << argv[1] << std::endl;
		}
	}
	else
	{
		std::cout << "Incorrect arguments.";
	}
	return 0;
}

