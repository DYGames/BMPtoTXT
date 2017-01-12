#include "Header.h"

int main()
{
	PCHAR path = new CHAR[256];
	//INT padding;

	std::cin >> path;
	//std::cin >> padding;

	AsciiArt asciiart(path, 1);

	delete path;
	return 0;
}

PUCHAR AsciiArt::LoadBMPImage(PCHAR path, BITMAPINFOHEADER* bitmapInfoHeader)
{
	FILE *filePtr;
	BITMAPFILEHEADER bitmapFileHeader;
	unsigned char *bitmapImage;
	int imageIdx = 0;
	unsigned char tempRGB;

	filePtr = fopen(path, "rb");
	if (filePtr == NULL)
		return NULL;

	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	if (bitmapFileHeader.bfType != 0x4D42)
	{
		fclose(filePtr);
		return NULL;
	}

	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	fread(bitmapImage, bitmapInfoHeader->biSizeImage, 1, filePtr);

	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	imageIdx = 0;
	for (int y = 0; y < bitmapInfoHeader->biHeight; y++)
	{
		for (int x = 0; x < bitmapInfoHeader->biWidth; x++)
		{
			tempRGB = bitmapImage[imageIdx];
			bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
			bitmapImage[imageIdx + 2] = tempRGB;
			imageIdx += 3;
		}
	}

	imageIdx = 0;
	UCHAR temp[3];
	UINT i = 0;
	for (int y = 0; y < bitmapInfoHeader->biHeight / 2; y++)
	{
		int Y = bitmapInfoHeader->biHeight - y - 1;
		for (int x = 0; x < bitmapInfoHeader->biWidth; x++)
		{
			i = 3 * (x + bitmapInfoHeader->biWidth * Y) + (bitmapInfoHeader->biWidth % 4) * Y;

			temp[0] = bitmapImage[imageIdx];
			temp[1] = bitmapImage[imageIdx + 1];
			temp[2] = bitmapImage[imageIdx + 2];

			bitmapImage[imageIdx] = bitmapImage[i];
			bitmapImage[imageIdx + 1] = bitmapImage[i + 1];
			bitmapImage[imageIdx + 2] = bitmapImage[i + 2];

			bitmapImage[i] = temp[0];
			bitmapImage[i + 1] = temp[1];
			bitmapImage[i + 2] = temp[2];

			imageIdx += 3;
		}
		imageIdx += bitmapInfoHeader->biWidth % 4;
	}

	fclose(filePtr);
	return bitmapImage;
}

AsciiArt::AsciiArt(PCHAR path, INT padding)
{
	std::ofstream output;
	output.open("output.txt");

	IMAGEDATA = LoadBMPImage(path, &IMAGEINFO);

	int sizex = IMAGEINFO.biWidth;
	int sizey = IMAGEINFO.biHeight;

	rgbs = new RGB*[sizey / padding];
	for (INT x = 0; x < sizey / padding; x++)
		rgbs[x] = new RGB[sizex / padding];

	UINT i = 0;

	for (int y = 0; y < sizey; y++)
	{
		for (int x = 0; x < sizex; x++)
		{
			rgbs[y][x] = {
				(INT)IMAGEDATA[i],
				(INT)IMAGEDATA[i + 1L],
				(INT)IMAGEDATA[i + 2L],

			};
			output << getASCIIbyRGB(rgbs[y][x]);
			i += 3L;
		}

		output << std::endl;
		i += (sizex) % 4L;
	}

	for (INT x = 0; x < sizey / padding; x++)
		delete[] rgbs[x];
	delete[] rgbs;
	delete IMAGEDATA;
	output.close();
}

char AsciiArt::getASCIIbyRGB(RGB rgb)
{
	static const char ASCIIARRAY[] = { '@', '#', '%', '&', '+', '*', ';', ':', ',', '.', '\`', ' ' };

	char rtn;
	float rgbrt = (rgb.R + rgb.G + rgb.B);
	rtn = ASCIIARRAY[(int)(rgbrt / 67)];


	return rtn;
}