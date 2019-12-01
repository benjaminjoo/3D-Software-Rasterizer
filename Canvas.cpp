#include "Canvas.h"
#include "Fonts.h"

#include <SDL/SDL.h>
#include <SDLImage/SDL_image.h>


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


bool* Canvas::GetSingleDigit_8(char letter_No)
{
	bool* currentLetter;

		switch (letter_No)
		{
		case 0:
		case '0':
			currentLetter = number_0;
			break;
		case 1:
		case '1':
			currentLetter = number_1;
			break;
		case 2:
		case '2':
			currentLetter = number_2;
			break;
		case 3:
		case '3':
			currentLetter = number_3;
			break;
		case 4:
		case '4':
			currentLetter = number_4;
			break;
		case 5:
		case '5':
			currentLetter = number_5;
			break;
		case 6:
		case '6':
			currentLetter = number_6;
			break;
		case 7:
		case '7':
			currentLetter = number_7;
			break;
		case 8:
		case '8':
			currentLetter = number_8;
			break;
		case 9:
		case '9':
			currentLetter = number_9;
			break;
		case '.':
			currentLetter = decimal_p;
			break;
		case '%':
			currentLetter = percent_;
			break;
		case 'a':
			currentLetter = letter_a;
			break;
		case 'A':
			currentLetter = letter_A;
			break;
		case 'b':
			currentLetter = letter_b;
			break;
		case 'B':
			currentLetter = letter_B;
			break;
		case 'c':
			currentLetter = letter_c;
			break;
		case 'C':
			currentLetter = letter_C;
			break;
		case 'd':
			currentLetter = letter_d;
			break;
		case 'D':
			currentLetter = letter_D;
			break;
		case 'e':
			currentLetter = letter_e;
			break;
		case 'E':
			currentLetter = letter_E;
			break;
		case 'f':
			currentLetter = letter_f;
			break;
		case 'F':
			currentLetter = letter_F;
			break;
		case 'g':
			currentLetter = letter_g;
			break;
		case 'G':
			currentLetter = letter_G;
			break;
		case 'h':
			currentLetter = letter_h;
			break;
		case 'H':
			currentLetter = letter_H;
			break;
		case 'i':
			currentLetter = letter_i;
			break;
		case 'I':
			currentLetter = letter_I;
			break;
		case 'j':
			currentLetter = letter_j;
			break;
		case 'J':
			currentLetter = letter_J;
			break;
		case 'k':
			currentLetter = letter_k;
			break;
		case 'K':
			currentLetter = letter_K;
			break;
		case 'l':
			currentLetter = letter_l;
			break;
		case 'L':
			currentLetter = letter_L;
			break;
		case 'm':
			currentLetter = letter_m;
			break;
		case 'M':
			currentLetter = letter_M;
			break;
		case 'n':
			currentLetter = letter_n;
			break;
		case 'N':
			currentLetter = letter_N;
			break;
		case 'o':
			currentLetter = letter_o;
			break;
		case 'O':
			currentLetter = letter_O;
			break;
		case 'p':
			currentLetter = letter_p;
			break;
		case 'P':
			currentLetter = letter_P;
			break;
		case 'q':
			currentLetter = letter_q;
			break;
		case 'Q':
			currentLetter = letter_Q;
			break;
		case 'r':
			currentLetter = letter_r;
			break;
		case 'R':
			currentLetter = letter_R;
			break;
		case 's':
			currentLetter = letter_s;
			break;
		case 'S':
			currentLetter = letter_S;
			break;
		case 't':
			currentLetter = letter_t;
			break;
		case 'T':
			currentLetter = letter_T;
			break;
		case 'u':
			currentLetter = letter_u;
			break;
		case 'U':
			currentLetter = letter_U;
			break;
		case 'v':
			currentLetter = letter_v;
			break;
		case 'V':
			currentLetter = letter_V;
			break;
		case 'w':
			currentLetter = letter_w;
			break;
		case 'W':
			currentLetter = letter_W;
			break;
		case 'x':
			currentLetter = letter_x;
			break;
		case 'X':
			currentLetter = letter_X;
			break;
		case 'y':
			currentLetter = letter_y;
			break;
		case 'Y':
			currentLetter = letter_Y;
			break;
		case 'z':
			currentLetter = letter_z;
			break;
		case 'Z':
			currentLetter = letter_Z;
			break;
		default:
			currentLetter = letter__;
			break;
		}

	return currentLetter;
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
		bool* currentMap = this->GetSingleDigit_8((char)(fract[p]));
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
		bool* currentMap = this->GetSingleDigit_8((char)(dInt[p]));
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

	bool* currentMap = this->GetSingleDigit_8('.');
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
		bool* currentMap = this->GetSingleDigit_8((char)(fract[p]));
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
		bool* currentMap = this->GetSingleDigit_8((char)(dInt[p]));
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
		bool* currentMap = this->GetSingleDigit_8('.');
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
	//else
	//{
	//	bool* currentMap = plus_sign;
	//	for (int j = 0; j < fontSize; j++)
	//	{
	//		for (int i = 0; i < fontSize; i++)
	//		{
	//			if (currentMap[j * fontSize + i])
	//			{
	//				pixelBuffer[(posV + j) * w + posH + i] = colour;
	//			}
	//		}
	//	}
	//}

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
		bool* currentMap = this->GetSingleDigit_8(text[c]);
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


