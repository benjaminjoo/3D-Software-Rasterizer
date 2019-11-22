#include "Button.h"
#include "Icons.h"


Button::Button()
{
	std::cout << "Button constructor called - Button::Button()" << std::endl;
}


Button::Button(std::shared_ptr<Canvas> canvas, int posH, int posV, bool on, tool name, Uint32 col):
	S(canvas), isActive(on), toolName(name), colour(col)
{
	std::cout << "Button constructor called - Button::Button(std::shared_ptr<Canvas> canvas, int posH, int posV, bool on, tool name, Uint32 col)" << std::endl;

	position = { posH * 32, posV * 32 };
}


Button::~Button()
{
	std::cout << "Button destructor is called" << std::endl;
}


void Button::displayIcon()
{
	const bool* currentIcon;
	switch (toolName)
	{
	case arrow:
		currentIcon = arrow_inactive;
		break;
	case cross:
		currentIcon = cross_inactive;
		break;
	case line:
		currentIcon = line_inactive;
		break;
	case move:
		currentIcon = move_inactive;
		break;
	case rotate:
		currentIcon = rotate_inactive;
		break;
	case view_top:
		currentIcon = top_inactive;
		break;
	case view_front:
		currentIcon = front_inactive;
		break;
	case view_side:
		currentIcon = side_inactive;
		break;
	case obj_snap:
		currentIcon = osnap_inactive;
		break;
	case grid_snap:
		currentIcon = grdsnap_inactive;
		break;
	default:
		currentIcon = blank;
		break;
	}

	for (auto j = 0; j < size.y; j++)
	{
		for (auto i = 0; i < size.x; i++)
		{
			if (isActive)
			{
				if (!currentIcon[j * size.x + i])
				{
					S->pixelBuffer[(position.y + j) * S->getWidth() + position.x + i] = colour;
				}
			}
			else
			{
				if (currentIcon[j * size.x + i])
				{
					S->pixelBuffer[(position.y + j) * S->getWidth() + position.x + i] = colour;
				}
			}
		}
	}
}


void Button::turnOn()
{
	isActive = true;
}


void Button::turnOff()
{
	isActive = false;
}
