#include "EventHandler.h"
#include "Camera.h"
#include "Canvas.h"


EventHandler::EventHandler(double step, double turn, double sens)
{
	visualStyle			= wireframe;

	gouraudShading		= true;
	visualiseDepth		= false;
	torchOn				= false;
	showCrosshair		= true;
	showFPS				= true;
	showPolyN			= true;
	showPosition		= true;
	gravityOn			= false;
	isPaused			= true;
	mouseLookBlocked	= false;

	event				= { 0 };

	quit				= false;

	turnH				= 0.0f;
	turnV				= 0.0f;
	turnR				= 0.0f;

	moveP				= 0.0f;
	strafeP				= 0.0f;
	riseP				= 0.0f;
	tiltP				= 0.0f;

	stepP				= step;
	turnP				= turn;
	sensitivity			= sens;

	turnVmin			= PI * 0.5f;
	turnVmax			= 3 * PI * 0.5f;

	torchIntensity		= 10.0f;
	maxIllumination		= 1.0f;
}


EventHandler::~EventHandler()
{
}


void EventHandler::HandleUserEvents()
{
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_Q] && !state[SDL_SCANCODE_E])
	{
		if ((state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) && state[SDL_SCANCODE_SPACE])
		{
			riseP = stepP * 8;
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
			riseP = -stepP * 8;
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
			moveP = stepP * 8;
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
			moveP = stepP * 8;
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
			moveP = -stepP * 8;
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
	if (state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D])
	{
		if ((state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) && state[SDL_SCANCODE_SPACE])
		{
			strafeP = -stepP * 8;
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
			strafeP = stepP * 8;
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
	if (state[SDL_SCANCODE_DOWN] && !state[SDL_SCANCODE_UP])
	{
		if ((state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) && state[SDL_SCANCODE_SPACE])
		{
			moveP = -stepP * 8;
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
				strafeP = -stepP * 8;
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
				strafeP = +stepP * 8;
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
	
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_z:
				visualStyle = wireframe;
				break;
			case SDLK_x:
				visualStyle = solid_colour;
				break;
			case SDLK_c:
				visualStyle = checkerboard;
				break;
			case SDLK_v:
				visualStyle = flat_shaded;
				break;
			case SDLK_b:
				visualStyle = gouraud_shaded;
				break;
			case SDLK_n:
				visualStyle = depth_visualised;
				break;
			case SDLK_m:
				visualStyle = sunlight;
				break;
			case SDLK_COMMA:
				visualStyle = torchlight;
				break;
			case SDLK_PERIOD:
				visualStyle = torchlight_solid;
				break;
			case SDLK_SLASH:
				visualStyle = test;
				break;
			case SDLK_g:
				gouraudShading = gouraudShading ? false : true;
				break;
			case SDLK_TAB:
				showFPS = showFPS ? false : true;
				showPosition = showPosition ? false : true;
				break;
			case SDLK_KP_5:
				mouseLookBlocked = mouseLookBlocked ? false : true;
				break;
			case SDLK_p:
				//player->outputImage(*screen);
				break;
			case SDLK_KP_PLUS:
				if (torchIntensity < 1000.0f)
				{
					torchIntensity += 0.25f;
				}
				break;
			case SDLK_KP_MINUS:
				if (torchIntensity > 0.75f)
				{
					torchIntensity -= 0.25f;
				}
				break;
			case SDLK_KP_MULTIPLY:
				if (maxIllumination < 2.0f)
				{
					maxIllumination += 0.05f;
				}
				break;
			case SDLK_KP_DIVIDE:
				if (maxIllumination > 1.00f)
				{
					maxIllumination -= 0.05f;
				}
				break;
			case SDLK_BACKSPACE:
				isPaused = isPaused ? false : true;
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

			}
		}

		if (event.type == SDL_MOUSEMOTION && !mouseLookBlocked)
		{
			turnH = (double)event.motion.x * sensitivity;
			turnV = (double)event.motion.y * sensitivity;
			if ((double)event.motion.y * sensitivity > turnVmin &&
				(double)event.motion.y * sensitivity < turnVmax)
			{
				turnV = (double)event.motion.y * sensitivity;
			}
			else
			{
				if ((double)event.motion.y * sensitivity < turnVmin)
				{
					turnV = turnVmin;
				}
				else if ((double)event.motion.y * sensitivity > turnVmax)
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
