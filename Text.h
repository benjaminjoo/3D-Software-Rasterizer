#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>

#include "SDL/SDL.h"
#include "Definitions.h"
#include "Canvas.h"

class Text
{
public:
	Text(const std::string& filename, unsigned w, Uint32 colour);
	~Text();

private:
	std::string fileName = "";
	std::vector<std::string> textLines;
	unsigned wBuffer = 0;
	unsigned hBuffer = 0;
	Uint32 textColour;
	bool* dataBuffer = nullptr;
	unsigned indent = 0;
	unsigned fontSize = 8;

	void readRawText();
	void initDataBuffer();
	void destroyDataBuffer();

public:
	void setIndentation(unsigned n);
	bool* getData();
	void print(std::shared_ptr<Canvas> screen);
};

