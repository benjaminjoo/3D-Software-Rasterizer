#pragma once

#include <memory>
#include <iostream>

#include "Definitions.h"
#include "Canvas.h"


class Button
{
	std::shared_ptr<Canvas> S = std::make_shared<Canvas>("Editor", EDITOR_WIDTH, EDITOR_HEIGHT, 999.9);
	screenCoord				size = { 32, 32 };
	screenCoord				position = { 0 * 32, 1 * 32 };;
	bool					isActive = false;
	tool					toolName = none;
	Uint32					colour = BLUE;

public:
	Button();
	Button(std::shared_ptr<Canvas> canvas, int posH, int posV, bool on, tool name, Uint32 col);
	~Button();

	void turnOn();
	void turnOff();
	void displayIcon();
};

