#pragma once

#include "Definitions.h"

class Menu
{
private:

	int anchorX;
	int anchorY;

	std::vector<std::string> titles;

public:

	Menu();
	~Menu();

	void setAnchor(int, int);
	void addTitle(std::string);
};

