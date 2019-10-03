#pragma once

#include "Definitions.h"

class Canvas
{
	int w;
	int h;

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
};

