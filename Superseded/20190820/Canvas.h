#pragma once

#include "Definitions.h"

class Canvas
{
	int w;
	int h;

	int fontSize;

	Uint32 c;
	double zFar;

public:

	Uint32* pixelBuffer;
	double* depthBuffer;

	Canvas();
	Canvas(int, int, Uint32, double);
	~Canvas();

	void resetPixelBuffer();

	void resetDepthBuffer();

	int getWidth();

	int getHeight();

	void drawCrosshair(int, int, Uint32);
	bool* GetSingleDigit_8(char letter_No);
	void displayFps(double value, int dec);
	void displayPosition(double value, int dec, int shiftV, Uint32 col);
};

