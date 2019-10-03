#pragma once

#include <SDL.h>
#include "Definitions.h"
#include "Camera.h"
#include "Canvas.h"

class EventHandler
{
public:
	//Context variables:
	bool gouraudShading;
	bool visualiseDepth;
	bool torchOn;
	bool showCrosshair;
	bool showFPS;
	bool showPosition;
	bool gravityOn;
	bool isPaused;
	bool mouseLookBlocked;

	SDL_Event event;
	Camera* player;
	Canvas* screen;

	bool quit;

	double turnH;
	double turnV;
	double turnR;

	double moveP;
	double strafeP;
	double riseP;
	double stepP;
	double turnP;
	double tiltP;

	double turnVmin;
	double turnVmax;

	int nSupport;
	triangle3dV* supports;

	EventHandler(double, double, Camera*, Canvas*, int, triangle3dV*);
	~EventHandler();

	void HandleUserEvents();
	void ceaseMotion();
};

