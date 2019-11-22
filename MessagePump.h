#pragma once

#include <SDL/SDL.h>
#include <memory>

#include "Definitions.h"
#include "Canvas.h"
#include "Editor.h"
#include "ModelElementBuffer.h"

class MessagePump
{
public:

	std::shared_ptr<Editor> E;

	SDL_Event	event		= { 0 };
	bool		quit		= false;

	bool		mouseAiming = false;

	MessagePump(std::shared_ptr<Editor> ed);
	~MessagePump();

	void HandleUserEvents();
};

