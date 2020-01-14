#include "Canvas.h"

#include <SDL/SDL.h>
#include <SDLImage/SDL_image.h>

#include "Fonts.h"
//#include "c64Fonts.h"


Canvas::Canvas(const std::string& windowTitle)
{
	std::cout << "Canvas constructor called" << std::endl;

	sdl_window	= SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, 0);

	sdl_screen	= SDL_CreateRenderer(sdl_window, -1, 0);

	sdl_texture = SDL_CreateTexture(sdl_screen, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, w, h);

	SDL_ShowCursor(SDL_DISABLE);

	pixelBuffer = new Uint32[w * h];
	depthBuffer = new double[w * h];

	for (int i = 0; i < w * h; i++)
	{
		pixelBuffer[i] = 0;
		depthBuffer[i] = zFar;
	}
}


Canvas::Canvas(const std::string& windowTitle, int width, int height, double z):
	w(width), h(height), zFar(z)
{
	std::cout << "Canvas constructor called" << std::endl;

	sdl_window	= SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, 0);

	sdl_screen	= SDL_CreateRenderer(sdl_window, -1, 0);

	sdl_texture = SDL_CreateTexture(sdl_screen, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, w, h);

	SDL_ShowCursor(SDL_DISABLE);

	pixelBuffer = new Uint32[w * h];
	depthBuffer = new double[w * h];

	for (int i = 0; i < w * h; i++)
	{
		pixelBuffer[i] = 0;
		depthBuffer[i] = zFar;
	}
}


Canvas::~Canvas()
{
	std::cout << "Canvas destructor called" << std::endl;
}


void Canvas::update()
{
	SDL_UpdateTexture(sdl_texture, nullptr, pixelBuffer, getWidth() * sizeof(Uint32));

	//SDL_RenderClear(sdl_screen);

	SDL_RenderCopy(sdl_screen, sdl_texture, nullptr, nullptr);

	SDL_RenderPresent(sdl_screen);
}


void Canvas::cleanUp()
{
	SDL_DestroyTexture(sdl_texture);

	SDL_DestroyRenderer(sdl_screen);

	SDL_DestroyWindow(sdl_window);
}


void Canvas::resetPixelBuffer()
{
	memset(pixelBuffer, 0, w * h * 4);
}

void Canvas::resetDepthBuffer()
{
	for (double* i = depthBuffer, *end = &depthBuffer[w * h]; i != end; i++)
		* i = zFar;
}


int Canvas::getWidth()
{
	return w;
}


int Canvas::getHeight()
{
	return h;
}


void Canvas::putPixel(int x, int y, Uint32 colour)
{
	if (x >= 0 && x < w && y >= 0 && y < h)
	{
		pixelBuffer[y * w + x] = colour;
	}
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


void Canvas::drawMouseCursor(editingMode mode, screenCoord position, Uint32 colour)
{
	switch (mode)
	{
		case Selection:
		{
			for (int j = 0; j < cursorSize; j++)
			{
				for (int i = 0; i < cursorSize; i++)
				{
					if (arrow_medium[j * cursorSize + i])
					{
						int currentX = position.x + i;
						int currentY = position.y + j;
						if ((currentX > 0 && currentX < w) && (currentY > 0 && currentY < h))
						{
							pixelBuffer[currentY * w + currentX] = colour;
						}				
					}
				}
			}
		}
		break;
		case Placement:
		case LineDrawing:
		{
			for (int i = 0; i < w; i += 2)
			{
				pixelBuffer[position.y * w + i] = colour;
			}
			for (int j = 0; j < h; j += 2)
			{
				pixelBuffer[j * w + position.x] = colour;
			}
		}
		break;
		case Relocation:
		case CopyRelocation:
		{
			for (int j = 0; j < cursorSize; j++)
			{
				for (int i = 0; i < cursorSize; i++)
				{
					if (move_medium[j * cursorSize + i])
					{
						int currentX = position.x + i;
						int currentY = position.y + j;
						if ((currentX > 0 && currentX < w) && (currentY > 0 && currentY < h))
						{
							pixelBuffer[currentY * w + currentX] = colour;
						}
					}
				}
			}
		}
		break;
		case Rotation:
		case CopyRotation:
		{
			for (int j = 0; j < cursorSize; j++)
			{
				for (int i = 0; i < cursorSize; i++)
				{
					if (rot_medium[j * cursorSize + i])
					{
						int currentX = position.x + i;
						int currentY = position.y + j;
						if ((currentX > 0 && currentX < w) && (currentY > 0 && currentY < h))
						{
							pixelBuffer[currentY * w + currentX] = colour;
						}
					}
				}
			}
		}
		break;
	}
}


void Canvas::drawSnapTarget(screenCoord P, Uint32 colour)
{
	const bool spot[64] = { 1, 1, 1, 1, 1, 1, 1, 1,
							1, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 0, 0, 0, 1,
							1, 1, 1, 1, 1, 1, 1, 1 };

	int xStart = P.x - 4;
	int yStart = P.y - 4;
	for (int j = yStart, q = 0; j < yStart + 8; j++, q++)
	{
		for (int i = xStart, p = 0; i < xStart + 8; i++, p++)
		{
			if ((i >= 0 && i < w) && (j >= 0 && j < h) && spot[q * 8 + p])
			{
				pixelBuffer[j * w + i] = colour;
			}
		}
	}
}


void Canvas::displayFps(double value, int dec, int shiftH, int shiftV)
{
	int* fract = getFractionals(value, dec);
	int nInt = 0;
	int* dInt = getIntegers(value, &nInt);

	int posH = w - 1 - (dec + 1 + shiftH) * fontSize;
	int posV = fontSize * (2 + +shiftV);

	for (int p = 0; p < dec; p++)
	{
		bool* currentMap = getSinglePETSCIICharacter((char)(fract[p]) + 48);
		for (int j = 0; j < fontSize; j++)
		{
			for (int i = 0; i < fontSize; i++)
			{
				if (currentMap[j * fontSize + i])
				{
					pixelBuffer[(posV + j) * w + posH + i] = 255;
				}
			}
		}
		posH += fontSize;
	}

	posH = w - 1 - (dec + nInt + 2 + shiftH) * fontSize;

	for (int p = 0; p < nInt; p++)
	{
		bool* currentMap = getSinglePETSCIICharacter((char)(dInt[p]) + 48);
		for (int j = 0; j < fontSize; j++)
		{
			for (int i = 0; i < fontSize; i++)
			{
				if (currentMap[j * fontSize + i])
				{
					pixelBuffer[(posV + j) * w + posH + i] = 255;
				}
			}
		}
		posH += fontSize;
	}

	posH = w - 1 - (dec + 2 + shiftH) * fontSize;

	bool* currentMap = getSinglePETSCIICharacter('.');
	for (int j = 0; j < fontSize; j++)
	{
		for (int i = 0; i < fontSize; i++)
		{
			if (currentMap[j * fontSize + i])
			{
				pixelBuffer[(posV + j) * w + posH + i] = 255;
			}
		}
	}


	delete[] fract;
	delete[] dInt;
}


void Canvas::displayValue(double value, int dec, int shiftH, int shiftV, Uint32 colour)
{
	bool isSigned;
	isSigned = value >= 0.0 ? false : true;
	if (value < 0.0) { value = -value; }
	int* fract = getFractionals(value, dec);
	int nInt = 0;
	int* dInt = getIntegers(value, &nInt);

	int posH = w - 1 - (dec + 1 + shiftH) * fontSize;
	int posV = h - fontSize * shiftV;

	for (int p = 0; p < dec; p++)
	{
		bool* currentMap = getSinglePETSCIICharacter((char)(fract[p]) + 48);
		for (int j = 0; j < fontSize; j++)
		{
			for (int i = 0; i < fontSize; i++)
			{
				if (currentMap[j * fontSize + i])
				{
					pixelBuffer[(posV + j) * w + posH + i] = colour;
				}
			}
		}
		posH += fontSize;
	}

	posH = w - 1 - (dec + nInt + 2 + shiftH) * fontSize;

	for (int p = 0; p < nInt; p++)
	{
		bool* currentMap = getSinglePETSCIICharacter((char)(dInt[p]) + 48);
		for (int j = 0; j < fontSize; j++)
		{
			for (int i = 0; i < fontSize; i++)
			{
				if (currentMap[j * fontSize + i])
				{
					pixelBuffer[(posV + j) * w + posH + i] = colour;
				}
			}
		}
		posH += fontSize;
	}

	posH = w - 1 - (dec + 2 + shiftH) * fontSize;

	if (dec > 0)
	{
		bool* currentMap = getSinglePETSCIICharacter('.');
		for (int j = 0; j < fontSize; j++)
		{
			for (int i = 0; i < fontSize; i++)
			{
				if (currentMap[j * fontSize + i])
				{
					pixelBuffer[(posV + j) * w + posH + i] = colour;
				}
			}
		}
	}

	posH = w - 1 - (dec + nInt + 3 + shiftH) * fontSize;

	if (isSigned)
	{
		bool* currentMap = minus_sign;
		for (int j = 0; j < fontSize; j++)
		{
			for (int i = 0; i < fontSize; i++)
			{
				if (currentMap[j * fontSize + i])
				{
					pixelBuffer[(posV + j) * w + posH + i] = colour;
				}
			}
		}
	}

	delete[] fract;
	delete[] dInt;
}


void Canvas::displayString(const char* text, int shiftH, int shiftV, Uint32 col)
{	
	int nChar = 0;
	while (text[nChar] != '\0') { nChar++; }

	int posH = w - fontSize * shiftH;
	int posV = h - fontSize * shiftV;

	for (int c = 0; c < nChar; c++)
	{
		bool* currentMap = getSinglePETSCIICharacter(text[c]);
		for (int j = 0; j < fontSize; j++)
		{
			for (int i = 0; i < fontSize; i++)
			{
				if (currentMap[j * fontSize + i])
				{
					pixelBuffer[(posV + j) * w + posH + i] = col;
				}
			}
		}
		posH += fontSize;
	}
}


