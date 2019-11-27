#pragma once

#include <SDL/SDL.h>
#include "Definitions.h"
#include "Camera.h"
#include "Canvas.h"


class EventHandler
{
public:

	double stepP;
	double turnP;
	double sensitivity;

	projectionStyle visualStyle = wireframe;;

	bool gouraudShading		= true;
	bool visualiseDepth		= false;
	bool torchOn			= false;
	bool showCrosshair		= true;
	bool showFPS			= true;
	bool showPolyN			= true;
	bool showPosition		= true;
	bool gravityOn			= false;
	bool isPaused			= true;
	bool mouseLookBlocked	= false;

	SDL_Event event			= { 0 };

	bool quit				= false;

	double turnH			= 0.0f;
	double turnV			= 0.0f;
	double turnR			= 0.0f;

	double moveP			= 0.0f;
	double strafeP			= 0.0f;
	double riseP			= 0.0f;
	double tiltP			= 0.0f;

	double turnVmin			= PI * 0.5f;
	double turnVmax			= 3 * PI * 0.5f;

	double torchIntensity	= 10.0f;
	double maxIllumination	= 1.0f;

	EventHandler(double step, double turn, double sens);
	~EventHandler();

	void HandleUserEvents();
	void ceaseMotion();
};

