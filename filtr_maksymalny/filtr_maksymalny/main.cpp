#include <iostream>
#include <fstream>
#include <cmath>

#pragma pack(push, 1)
struct BITMAPFILEHEADER
{
	unsigned char bfType[2];
	unsigned int bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int bfOffBits;
};

struct BITMAPINFOHEADER
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
#pragma pack(pop)


int main() {

	BITMAPFILEHEADER file_header;
	BITMAPINFOHEADER info_header;

	std::fstream file("konie.bmp", std::fstream::in | std::fstream::out | std::fstream::binary);
	char* file_char = new char[sizeof(BITMAPFILEHEADER)];
	char* info_char = new char[sizeof(BITMAPINFOHEADER)];

	if (file.good()) 
	{
		
		file.read(file_char, sizeof(BITMAPFILEHEADER));
		BITMAPFILEHEADER* bfh = (BITMAPFILEHEADER*)(info_char);

		file.read(info_char, sizeof(BITMAPINFOHEADER));

		/*for (int i = 0; i < sizeof(BITMAPINFOHEADER); i++)
		{
			std::cout << (int) info_char[i] << std::endl;
		}*/
	}

	file_header.bfType[0] = file_char[0];
	file_header.bfType[1] = file_char[1];
	file_header.bfSize = static_cast<unsigned int>(file_char[5] * pow(2, 24) + file_char[4] * pow(2, 16) + file_char[3] * pow(2, 8) + file_char[2]);
	file_header.bfReserved1 = static_cast<unsigned int>(file_char[7] * pow(2, 8) + file_char[6]);
	file_header.bfReserved2 = static_cast<unsigned int>(file_char[9] * pow(2, 8) + file_char[8]);
	file_header.bfOffBits = static_cast<unsigned int>(file_char[13] * pow(2, 24) + file_char[12] * pow(2, 16) + file_char[11] * pow(2, 8) + file_char[10]);

	info_header.biSize = static_cast<unsigned int>(info_char[3] * pow(2, 24) + info_char[2] * pow(2, 16) + info_char[1] * pow(2, 8) + info_char[0]);
	info_header.biWidth = static_cast<int>(info_char[7] * pow(2, 24) + info_char[6] * pow(2, 16) + info_char[5] * pow(2, 8) + info_char[4]);
	info_header.biHeight = static_cast<int>(info_char[11] * pow(2, 24) + info_char[10] * pow(2, 16) + info_char[9] * pow(2, 8) + info_char[8]);
	info_header.biPlanes = static_cast<unsigned int>(info_char[15] * pow(2, 24) + info_char[14] * pow(2, 16) + info_char[13] * pow(2, 8) + info_char[12]);
	info_header.biBitCount = static_cast<unsigned int>(info_char[19] * pow(2, 24) + info_char[18] * pow(2, 16) + info_char[17] * pow(2, 8) + info_char[16]);
	info_header.biCompression = static_cast<unsigned int>(info_char[23] * pow(2, 24) + info_char[22] * pow(2, 16) + info_char[21] * pow(2, 8) + info_char[20]);
	info_header.biSizeImage = static_cast<unsigned int>(info_char[27] * pow(2, 24) + info_char[26] * pow(2, 16) + info_char[25] * pow(2, 8) + info_char[24]);
	info_header.biXPelsPerMeter = static_cast<int>(info_char[31] * pow(2, 24) + info_char[30] * pow(2, 16) + info_char[29] * pow(2, 8) + info_char[28]);
	info_header.biYPelsPerMeter = static_cast<int>(info_char[35] * pow(2, 24) + info_char[34] * pow(2, 16) + info_char[33] * pow(2, 8) + info_char[32]);
	info_header.biClrUsed = static_cast<unsigned int>(info_char[39] * pow(2, 24) + info_char[38] * pow(2, 16) + info_char[37] * pow(2, 8) + info_char[36]);
	info_header.biClrImportant = static_cast<unsigned int>(info_char[43] * pow(2, 24) + info_char[42] * pow(2, 16) + info_char[41] * pow(2, 8) + info_char[40]);

	/*file >> file_header.bfType;
	file >> file_header.bfSize;
	file >> file_header.bfReserved1;
	file >> file_header.bfReserved2;
	file >> file_header.bfOffBits;

	file >> info_header.biSize;
	file >> info_header.biWidth;
	file >> info_header.biHeight;
	file >> info_header.biPlanes;
	file >> info_header.biBitCount;
	file >> info_header.biCompression;
	file >> info_header.biSizeImage;
	file >> info_header.biXPelsPerMeter;
	file >> info_header.biYPelsPerMeter;
	file >> info_header.biClrUsed;
	file >> info_header.biClrImportant;  nie dzia³a :(((*/

	file.close();

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