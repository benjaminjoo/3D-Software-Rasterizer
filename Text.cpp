#include "Text.h"



Text::Text(const std::string& filename, unsigned w, Uint32 colour) : fileName(filename), wBuffer(w), textColour(colour)
{
	this->readRawText();
	this->initDataBuffer();
}


Text::~Text()
{
	this->destroyDataBuffer();
}


void Text::readRawText()
{
	std::ifstream inputFile(fileName);
	std::string tempLine = "";

	if (inputFile.is_open())
	{
		while (std::getline(inputFile, tempLine))
			textLines.push_back(tempLine);
		inputFile.close();
	}
	else
		std::cout << "Could not open file..." << std::endl;
}


void Text::initDataBuffer()
{
	auto nLines = textLines.size();
	hBuffer = nLines * fontSize;
	dataBuffer = new bool[wBuffer * hBuffer];
	memset(dataBuffer, 0, wBuffer * hBuffer);

	unsigned rowIndex = 0;
	unsigned charIndex = 0;

	for (std::string& line : textLines)
	{
		for (char& ch : line)
		{
			bool* currentChar = getSinglePETSCIICharacter(ch);

			unsigned insertionX = (charIndex + indent) * fontSize;
			unsigned insertionY = rowIndex * fontSize;

			for (unsigned q = 0; q < fontSize; q++)
				for (unsigned p = 0; p < fontSize; p++)
				{
					unsigned x = insertionX + p;
					unsigned y = insertionY + q;
					if (x >= 0 && x < wBuffer && y >= 0 && y < hBuffer)
						dataBuffer[y * wBuffer + x] = currentChar[q * fontSize + p];

				}

			charIndex++;
		}
		charIndex = 0;
		rowIndex++;
	}
}


void Text::destroyDataBuffer()
{
	if (dataBuffer != nullptr)
		delete[] dataBuffer;
}


void Text::setIndentation(unsigned n)
{
	indent = n;
}


bool* Text::getData()
{
	return dataBuffer;
}


void Text::print(std::shared_ptr<Canvas> screen)
{
	unsigned hScreen = static_cast<unsigned>(screen->getHeight());
	unsigned wScreen = static_cast<unsigned>(screen->getWidth());
	if (hScreen >= hBuffer && wScreen >= wBuffer)
	{
		unsigned anchorX = (wScreen - wBuffer) / 2;
		unsigned anchorY = (hScreen - hBuffer) / 2;
		for (unsigned j = 0; j < hBuffer; j++)
		{
			for (unsigned i = 0; i < wBuffer; i++)
			{
				if (dataBuffer[j * wBuffer + i])
					screen->putPixel(anchorX + i, anchorY + j, textColour);
			}
		}
	}
}