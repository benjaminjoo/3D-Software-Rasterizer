#pragma once

#include <iostream>

#include "Definitions.h"



class Canvas
{
	int w			= 1000;
	int h			= 500;
	int size		= 1000 * 500;

	int fontSize	= 8;
	int cursorSize	= 16;
	int iconSize	= 32;

	float zFar		= 100.0f;

public:

	SDL_Window*		sdl_window	= nullptr;
	
	SDL_Renderer*	sdl_screen	= nullptr;
	
	SDL_Texture*	sdl_texture = nullptr;

	Uint32* pixelBuffer;
	float* depthBuffer;

	Canvas(const std::string& windowTitle);
	Canvas(const std::string& windowTitle, int, int, float);
	~Canvas();

	void update();
	void cleanUp();

	void resetPixelBuffer();
	void resetDepthBuffer();

	int getWidth();
	int getHeight();

	void putPixel(const int& x, const int& y, const Uint32& colour);
	void drawLine(const coord2&, const coord2&, const Uint32&);
	void drawLine(const screenCoord&, const screenCoord&, const int&, const Uint32&);
	void drawSpot(const screenCoord&, const Uint32&);
	void drawSpotHighlight(const screenCoord&, const Uint32&);
	void drawCrosshair(int, int, Uint32);
	void drawMouseCursor(editingMode, screenCoord position, Uint32 colour);
	void drawSnapTarget(screenCoord position, Uint32 colour);
	void displayFps(float value, int dec, int shiftH, int shiftV);
	void displayValue(float value, int dec, int shiftH, int shiftV, Uint32 col);
	void displayString(const char* text, int shiftH, int shiftV, Uint32 col);
};

