#pragma once

#include <SDL.h>
#include "Definitions.h"
//#include "Camera.h"
#include "Canvas.h"
#include "Editor.h"
#include "ModelElementBuffer.h"

class MessagePump
{
public:

	Editor*					E;

	SDL_Event	event;
	bool		quit;

	bool		mouseAiming;

	MessagePump(Editor*);
	~MessagePump();

	void HandleUserEvents();
};

