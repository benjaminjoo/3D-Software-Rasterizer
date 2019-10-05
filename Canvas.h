#pragma once

#include "Definitions.h"

class Canvas
{
	int w;
	int h;

	int fontSize;
	int cursorSize;
	int iconSize;

	double zFar;

public:

	Uint32* pixelBuffer;
	double* depthBuffer;

	Canvas();
	Canvas(int, int, double);
	~Canvas();

	void resetPixelBuffer();

	void resetDepthBuffer();

	int getWidth();

	int getHeight();

	void drawCrosshair(int, int, Uint32);
	void drawMouseCursor(editingMode, screenCoord position, Uint32 colour);
	void drawSnapTarget(screenCoord position, Uint32 colour);
	bool* GetSingleDigit_8(char letter_No);
	void displayFps(double value, int dec, int shiftH, int shiftV);
	void displayValue(double value, int dec, int shiftH, int shiftV, Uint32 col);
	void displayString(const char* text, int shiftH, int shiftV, Uint32 col);
};

