#include "Canvas.h"


Canvas::Canvas()
{
	w = 640;
	h = 480;

	c = 0;
	zFar = 0.0;

	pixelBuffer = new Uint32[w * h];
	depthBuffer = new double[w * h];

	for (int i = 0; i < w * h; i++)
	{
		pixelBuffer[i] = c;
		depthBuffer[i] = zFar;
	}
}

Canvas::Canvas(int width, int height, Uint32 colour, double z)
{
	w = width;
	h = height;

	c = colour;
	zFar = z;

	pixelBuffer = new Uint32[w * h];
	depthBuffer = new double[w * h];

	for (int i = 0; i < w * h; i++)
	{
		pixelBuffer[i] = c;
		depthBuffer[i] = zFar;
	}
}


Canvas::~Canvas()
{
}


void Canvas::resetPixelBuffer()
{
	for (int i = 0; i < w * h; i++)
	{
		pixelBuffer[i] = c;
	}
}

void Canvas::resetDepthBuffer()
{
	for (int i = 0; i < w * h; i++)
	{
		depthBuffer[i] = zFar;
	}
}


int Canvas::getWidth()
{
	return w;
}


int Canvas::getHeight()
{
	return h;
}


void Canvas::drawCrosshair(int hole, int size, Uint32 colour)
{
	for (int i = 0; i < (size - hole); i++)
	{
		pixelBuffer[(h / 2 - 1) * w + w / 2 + hole + i] = colour;
		pixelBuffer[(h / 2) * w + w / 2 + hole + i] = colour;

		pixelBuffer[(h / 2 + hole + i) * w + w / 2 - 1] = colour;
		pixelBuffer[(h / 2 + hole + i) * w + w / 2] = colour;
	}

	for (int i = size - hole; i > 0; i--)
	{
		pixelBuffer[(h / 2 - 1) * w - w / 2 - hole - i] = colour;
		pixelBuffer[(h / 2) * w - w / 2 - hole - i] = colour;

		pixelBuffer[(h / 2 - hole - i) * w + w / 2 - 1] = colour;
		pixelBuffer[(h / 2 - hole - i) * w + w / 2] = colour;
	}
}