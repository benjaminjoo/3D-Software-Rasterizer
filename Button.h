#pragma once

#include "Definitions.h"
#include "Canvas.h"


class Button
{
	Canvas*			S;
	screenCoord		size;
	screenCoord		position;
	bool			isActive;
	tool			toolName;
	Uint32			colour;

public:
	Button();
	Button(Canvas*, int, int, bool, tool, Uint32);
	~Button();

	void turnOn();
	void turnOff();
	void displayIcon();
};

