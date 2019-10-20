#include <iostream>
#include <fstream>
#pragma pack(push, 1)
struct BITMAPFILEHEADER
{
	unsigned short bfType;
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

	if (file.good()) 
	{
		char* info_char = new char[sizeof(BITMAPFILEHEADER)];
		file.read(info_char, sizeof(BITMAPFILEHEADER));
		BITMAPFILEHEADER* bfh = (BITMAPFILEHEADER*)(info_char);

		for (int i = 0; i < sizeof(BITMAPFILEHEADER); i++)
		{
			std::cout << (int) info_char[i] << std::endl;
		}
	}

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
	file >> info_header.biClrImportant;*/

	file.close();

	/*std::cout << file_header.bfType << std::endl;
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
	std::cout << info_header.biClrImportant << std::endl;*/

}