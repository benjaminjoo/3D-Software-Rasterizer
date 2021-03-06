#include "EventHandler.h"
#include "Camera.h"
#include "Canvas.h"


EventHandler::EventHandler(float step, float turn, float sens) :
	stepP(step), turnP(turn), sensitivity(sens)
{
	std::cout << "EventHandler constructor called" << std::endl;
	SDL_SetRelativeMouseMode(SDL_TRUE);
}


EventHandler::~EventHandler()
{
	std::cout << "EventHandler destructor called" << std::endl;
}


void EventHandler::HandleUserEvents()
{
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_RETURN])
		isFiring = true;
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
		turnV += turnP;
	}
	if (state[SDL_SCANCODE_PAGEDOWN])
	{
		turnV -= turnP;
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

	if (state[SDL_SCANCODE_Y] && !state[SDL_SCANCODE_I])
	{

		riseE = stepP;

	}
	if (state[SDL_SCANCODE_I] && !state[SDL_SCANCODE_Y])
	{

		riseE = -stepP;

	}
	if (state[SDL_SCANCODE_U] && !state[SDL_SCANCODE_J])
	{

		moveE = stepP;

	}
	if (state[SDL_SCANCODE_J] && !state[SDL_SCANCODE_U])
	{

		moveE = -stepP;

	}
	if (state[SDL_SCANCODE_H] && !state[SDL_SCANCODE_K])
	{

		strafeE = stepP;

	}
	if (state[SDL_SCANCODE_K] && !state[SDL_SCANCODE_H])
	{

		strafeE = -stepP;

	}

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_h:
				showHelp = showHelp ? false : true;
				break;
			case SDLK_t:
				visualStyle = projectionStyle::shadow_test;
				break;
			case SDLK_z:
				visualStyle = projectionStyle::wireframe;
				break;
			case SDLK_x:
				visualStyle = projectionStyle::solid_colour;
				break;
			case SDLK_c:
				visualStyle = projectionStyle::checkerboard;
				break;
			case SDLK_v:
				visualStyle = projectionStyle::flat_shaded;
				break;
			case SDLK_b:
				visualStyle = projectionStyle::gouraud_shaded;
				break;
			case SDLK_n:
				visualStyle = projectionStyle::blinn_phong;
				break;
			case SDLK_m:
				visualStyle = projectionStyle::depth_visualised;
				break;
			case SDLK_COMMA:
				visualStyle = projectionStyle::sunlight;
				break;
			case SDLK_PERIOD:
				visualStyle = projectionStyle::torchlight;
				break;
			case SDLK_SLASH:
				visualStyle = projectionStyle::torchlight_solid;
				break;
			case SDLK_f:
				clippingOn = clippingOn ? false : true;
				break;
			case SDLK_g:
				gravityOn = gravityOn ? false : true;
				break;
			case SDLK_TAB:
				showFPS = showFPS ? false : true;
				showPosition = showPosition ? false : true;
				showPolyN = showPolyN ? false : true;
				showAmmo = showAmmo ? false : true;
				break;
			case SDLK_END:
				playerControlled = playerControlled ? false : true;
				enemyControlled = enemyControlled ? false : true;
				break;
			case SDLK_KP_0:
			case SDLK_0:
				toggleAIGoal();
				break;
			case SDLK_KP_5:
				mouseLookBlocked = mouseLookBlocked ? false : true;
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
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				isFiring = true;
			}
		}

		if (event.type == SDL_MOUSEMOTION && !mouseLookBlocked)
		{
			turnH = (float)event.motion.x * sensitivity;
			turnV = (float)event.motion.y * sensitivity;
			if ((float)event.motion.y * sensitivity > turnVmin &&
				(float)event.motion.y* sensitivity < turnVmax)
			{
				turnV = (float)event.motion.y * sensitivity;
			}
			else
			{
				if ((float)event.motion.y * sensitivity < turnVmin)
				{
					turnV = turnVmin;
				}
				else if ((float)event.motion.y * sensitivity > turnVmax)
				{
					turnV = turnVmax;
				}
			}
		}

		if (event.type == SDL_QUIT) { quit = true; }
	}
}


void EventHandler::toggleAIGoal()
{
	switch (purposeOfAI)
	{
	case aiGoal::be_idle:
		purposeOfAI = aiGoal::follow_player;
		break;
	case aiGoal::follow_player:
		purposeOfAI = aiGoal::kill_player;
		break;
	case aiGoal::kill_player:
		purposeOfAI = aiGoal::follow_others;
		break;
	case aiGoal::follow_others:
		purposeOfAI = aiGoal::kill_others;
		break;
	case aiGoal::kill_others:
		purposeOfAI = aiGoal::follow_each_other;
		break;
	case aiGoal::follow_each_other:
		purposeOfAI = aiGoal::kill_each_other;
		break;
	case aiGoal::kill_each_other:
		purposeOfAI = aiGoal::be_idle;
		break;
	}
}


void EventHandler::ceaseMotion()
{
	moveP = 0.0f;
	strafeP = 0.0f;
	riseP = 0.0f;

	moveE = 0.0f;
	strafeE = 0.0f;
	riseE = 0.0f;

	isFiring = false;
}
