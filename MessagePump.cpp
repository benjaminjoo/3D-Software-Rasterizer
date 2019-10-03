#include "MessagePump.h"
#include "stdio.h"


MessagePump::MessagePump(Editor* ed)
{
	E			= ed;

	event		= { 0 };

	quit		= false;

	mouseAiming = false;
}


MessagePump::~MessagePump()
{
}


void MessagePump::HandleUserEvents()
{
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_DELETE:
				E->deleteSelected();
				break;
			case SDLK_ESCAPE:
				E->deselectAll();
				E->activateSelection();
				break;
			case SDLK_p:
				E->activatePlacement();
				break;
			case SDLK_l:
				E->activateLineDrawing();
				break;
			case SDLK_m:
				E->activateRelocation();
				break;
			case SDLK_r:
				E->activateRotation();
				break;
			case SDLK_g:
				E->toggleGridSnap();
				break;
			case SDLK_LSHIFT:
			case SDLK_RSHIFT:
				E->switchOrthoOn();
				break;
			case SDLK_s:
				E->quickSave();
				break;
			}
		}
		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_LSHIFT:
			case SDLK_RSHIFT:
				E->switchOrthoOff();
				break;
			}
		}

		if (event.type == SDL_MOUSEWHEEL)
		{
			if (event.wheel.y > 0)
			{
				E->scale *= 1.2f;
				E->updateZoom();
			}
			else if (event.wheel.y < 0)
			{
				E->scale *= (1.0f / 1.2f);
				E->updateZoom();
			}
		}

		if (SDL_MOUSEBUTTONDOWN)
		{
			mouseAiming = true;
			if (event.button.button == SDL_BUTTON_MIDDLE)
			{
				E->updatePan();
			}
		}
	}

	if (event.type == SDL_MOUSEMOTION)
	{
		mouseAiming = false;
		E->mouseMotion((int)event.motion.x, (int)event.motion.y);
	}

	if (event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_RELEASED && mouseAiming)
	{
		screenCoord mouseClickLocation;
		SDL_GetMouseState(&mouseClickLocation.x, &mouseClickLocation.y);
		E->leftMouseClick(mouseClickLocation);

		mouseAiming = false;
	}

	if (event.type == SDL_QUIT) { quit = true; }
}
