#include "Menu.h"



Menu::Menu()
{
	anchorX = 0;
	anchorY = 0;
}


Menu::~Menu()
{
}


void Menu::setAnchor(int x, int y)
{
	anchorX = x;
	anchorY = y;
}


void Menu::addTitle(std::string T)
{
	titles.push_back(T);
}
