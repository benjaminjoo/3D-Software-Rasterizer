#pragma once

#include <SDL/SDL.h>
#include "Definitions.h"
#include "Camera.h"
#include "Canvas.h"


class EventHandler
{
public:

	projectionStyle visualStyle;

	bool gouraudShading;
	bool visualiseDepth;
	bool torchOn;
	bool showCrosshair;
	bool showFPS;
	bool showPolyN;
	bool showPosition;
	bool gravityOn;
	bool isPaused;
	bool mouseLookBlocked;

	SDL_Event event;

	bool quit;

	double turnH;
	double turnV;
	double turnR;

	double moveP;
	double strafeP;
	double riseP;
	double tiltP;

	double stepP;
	double turnP;
	double sensitivity;

	double turnVmin;
	double turnVmax;

	double			torchIntensity;
	double			maxIllumination;

	EventHandler(double step, double turn, double sens);
	~EventHandler();

	void HandleUserEvents();
	void ceaseMotion();
};

