#pragma once

#include <iostream>

#include "Definitions.h"

class Canvas
{
	int w			= 1000;
	int h			= 500;

	int fontSize	= 8;
	int cursorSize	= 16;
	int iconSize	= 32;

	double zFar		= 100.0f;

public:

	SDL_Window*		sdl_window	= nullptr;
	
	SDL_Renderer*	sdl_screen	= nullptr;
	
	SDL_Texture*	sdl_texture = nullptr;

	Uint32* pixelBuffer;
	double* depthBuffer;

	Canvas();
	Canvas(int, int, double);
	~Canvas();

	void update();

	void cleanUp();

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

