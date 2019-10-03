#include "Canvas.h"
#include "Fonts.h"


Canvas::Canvas()
{
	w = 640;
	h = 480;
	fontSize = 8;

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
	fontSize = 8;

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
		default:
			currentLetter = letter__;
			break;
		}

	return currentLetter;
}


void Canvas::displayFps(double value, int dec)
{
	int* fract = getFractionals(value, dec);
	int nInt = 0;
	int* dInt = getIntegers(value, &nInt);

	int posH = w - 1 - (dec + 1) * fontSize;
	int posV = fontSize * 2;

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

	posH = w - 1 - (dec + nInt + 2) * fontSize;

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

	posH = w - 1 - (dec + 2) * fontSize;

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