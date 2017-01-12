#pragma once

#include <iostream>
#include <Windows.h>
#include <fstream>

struct RGB
{
	INT R;
	INT G;
	INT B;
};

class AsciiArt
{
public:
	AsciiArt(PCHAR, INT);

private:
	PUCHAR LoadBMPImage(PCHAR, BITMAPINFOHEADER*);

	char getASCIIbyRGB(RGB);
private:
	BITMAPINFOHEADER IMAGEINFO;
	PUCHAR IMAGEDATA;

	RGB **rgbs;
};