#include "EventHandler.h"
#include "Camera.h"


EventHandler::EventHandler(double step, double turn, Camera* P, Canvas* C, int n, triangle3dV* s)
{
	gouraudShading		= true;
	visualiseDepth		= false;
	torchOn				= false;
	showCrosshair		= true;
	showFPS				= true;
	showPosition		= true;
	gravityOn			= false;
	isPaused			= true;
	mouseLookBlocked	= false;

	//grainSize			= 1;

	event				= { 0 };
	player				= P;
	screen				= C;

	quit				= false;

	turnH				= 0.0;
	turnV				= 0.0;
	turnR				= 0.0;

	moveP				= 0.0;
	strafeP				= 0.0;
	riseP				= 0.0;
	tiltP				= 0.0;
	stepP				= step;
	turnP				= turn;

	turnVmin			= PI * 0.5;
	turnVmax			= 3 * PI * 0.5;

	nSupport			= n;
	supports			= s;
}


EventHandler::~EventHandler()
{
}


void EventHandler::HandleUserEvents()
{
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_Q] && !state[SDL_SCANCODE_E])
	{
		if ((state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) && state[SDL_SCANCODE_SPACE])
		{
			riseP = stepP * 20;
		}
		else if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
		{
			riseP = stepP * 4;
		}
		else
		{
			riseP = stepP;
		}
	}
	if (state[SDL_SCANCODE_E] && !state[SDL_SCANCODE_Q])
	{
		if ((state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) && state[SDL_SCANCODE_SPACE])
		{
			riseP = -stepP * 20;
		}
		else if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
		{
			riseP = -stepP * 4;
		}
		else
		{
			riseP = -stepP;
		}
	}
	if (state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S])
	{
		if ((state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) && state[SDL_SCANCODE_SPACE])
		{
			moveP = stepP * 20;
		}
		else if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
		{
			moveP = stepP * 4;
		}
		else
		{
			moveP = stepP;
		}
	}
	if (state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN])
	{
		if ((state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) && state[SDL_SCANCODE_SPACE])
		{
			moveP = stepP * 20;
		}
		else if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
		{
			moveP = stepP * 4;
		}
		else
		{
			moveP = stepP;
		}
	}
	if (state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_W])
	{
		if ((state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) && state[SDL_SCANCODE_SPACE])
		{
			moveP = -stepP * 20;
		}
		else if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
		{
			moveP = -stepP * 4;
		}
		else
		{
			moveP = -stepP;
		}
	}
	if (state[SDL_SCANCODE_DOWN] && !state[SDL_SCANCODE_UP])
	{
		if ((state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) && state[SDL_SCANCODE_SPACE])
		{
			moveP = -stepP * 20;
		}
		else if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
		{
			moveP = -stepP * 4;
		}
		else
		{
			moveP = -stepP;
		}
	}
	if (state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT])
	{
		if (state[SDL_SCANCODE_LALT])
		{
			if ((state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) && state[SDL_SCANCODE_SPACE])
			{
				strafeP = -stepP * 20;
			}
			else if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
			{
				strafeP = -stepP * 4;
			}
			else
			{
				strafeP = -stepP;
			}
		}
		else
		{
			turnH -= turnP;
		}
	}
	if (state[SDL_SCANCODE_RIGHT] && !state[SDL_SCANCODE_LEFT])
	{
		if (state[SDL_SCANCODE_LALT])
		{
			if ((state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) && state[SDL_SCANCODE_SPACE])
			{
				strafeP = +stepP * 20;
			}
			else if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
			{
				strafeP = +stepP * 4;
			}
			else
			{
				strafeP = +stepP;
			}
		}
		else
		{
			turnH += turnP;
		}
	}
	if (state[SDL_SCANCODE_PAGEUP])
	{
		turnV -= turnP;
	}
	if (state[SDL_SCANCODE_PAGEDOWN])
	{
		turnV += turnP;
	}
	if (state[SDL_SCANCODE_1])
	{
		tiltP -= turnP;
	}
	if (state[SDL_SCANCODE_4])
	{
		tiltP += turnP;
	}
	if (state[SDL_SCANCODE_HOME])
	{
		turnV = -PI;
		tiltP = 0.0;
	}
	if (state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D])
	{
		if ((state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) && state[SDL_SCANCODE_SPACE])
		{
			strafeP = -stepP * 20;
		}
		else if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
		{
			strafeP = -stepP * 4;
		}
		else
		{
			strafeP = -stepP;
		}
	}
	if (state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_A])
	{
		if ((state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) && state[SDL_SCANCODE_SPACE])
		{
			strafeP = stepP * 20;
		}
		else if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
		{
			strafeP = stepP * 4;
		}
		else
		{
			strafeP = stepP;
		}
	}

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_g:
				gouraudShading = gouraudShading ? false : true;
				break;
			case SDLK_z:
				visualiseDepth = visualiseDepth ? false : true;
				break;
			case SDLK_t:
				torchOn = torchOn ? false : true;
				break;
			case SDLK_c:
				gravityOn = gravityOn ? false : true;
				break;
			case SDLK_x:
				showCrosshair = showCrosshair ? false : true;
				break;
			/*case SDLK_b:
				showBuoys = showBuoys ? false : true;
				break;*/
			case SDLK_TAB:
				showFPS = showFPS ? false : true;
				showPosition = showPosition ? false : true;
				break;
			case SDLK_m:
				isPaused = isPaused ? false : true;
				break;
			case SDLK_KP_5:
				mouseLookBlocked = mouseLookBlocked ? false : true;
				break;
			case SDLK_p:
				player->outputImage(*screen);
				break;
			case SDLK_KP_PLUS:
				if (player->margin < MAX_MARGIN)
				{
					player->margin += 1;
				}
				break;
			case SDLK_KP_MINUS:
				if (player->margin > 1)
				{
					player->margin -= 1;
				}
				break;
			case SDLK_ESCAPE:
				quit = true;
				break;
			}
		}

		if (SDL_MOUSEBUTTONDOWN)
		{
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				player->jump(nSupport, supports);
			}
		}

		if (event.type == SDL_MOUSEMOTION && !mouseLookBlocked)
		{
			turnH = (double)event.motion.x * SENSITIVITY;
			turnV = (double)event.motion.y * SENSITIVITY;
			if ((double)event.motion.y * SENSITIVITY > turnVmin &&
				(double)event.motion.y * SENSITIVITY < turnVmax)
			{
				turnV = (double)event.motion.y * SENSITIVITY;
			}
			else
			{
				if ((double)event.motion.y * SENSITIVITY < turnVmin)
				{
					turnV = turnVmin;
				}
				else if ((double)event.motion.y * SENSITIVITY > turnVmax)
				{
					turnV = turnVmax;
				}
			}
		}

		if (event.type == SDL_QUIT) { quit = true; }
	}
}


void EventHandler::ceaseMotion()
{
	moveP = 0.0;
	strafeP = 0.0;
	riseP = 0.0;
}
