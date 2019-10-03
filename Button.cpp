#include "Button.h"
#include "Icons.h"

Button::Button()
{
	S			= &Canvas(EDITOR_WIDTH, EDITOR_HEIGHT, 999.9);
	size		= { 32, 32 };
	position	= { 0 * 32, 1 * 32 };
	isActive	= false;
	toolName	= none;
	colour		= 255;
}


Button::Button(Canvas* can, int posH, int posV, bool on, tool name, Uint32 col)
{
	S			= can;
	size		= { 32, 32 };
	position	= { posH * 32, posV * 32 };
	isActive	= on;
	toolName	= name;
	colour		= col;
}


Button::~Button()
{
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
	default:
		currentIcon = blank;
		break;
	}

	for (int j = 0; j < size.y; j++)
	{
		for (int i = 0; i < size.x; i++)
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
