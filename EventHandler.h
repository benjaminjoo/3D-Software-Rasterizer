#pragma once

#include <SDL/SDL.h>
#include <memory>
#include "Definitions.h"

class EventHandler
{
public:

	float stepP;
	float turnP;
	float sensitivity;

	projectionStyle visualStyle = projectionStyle::wireframe;
	aiGoal			purposeOfAI = aiGoal::be_idle;

	bool gouraudShading = true;
	bool visualiseDepth = false;
	bool torchOn = false;
	bool showCrosshair = true;
	bool showFPS = true;
	bool showPosition = true;
	bool showPolyN = true;
	bool showAmmo = true;
	bool showHelp = false;
	bool gravityOn = false;
	bool clippingOn = true;
	bool isPaused = false;
	bool isFiring = false;
	bool mouseLookBlocked = false;
	bool playerControlled = true;
	bool enemyControlled = false;

	SDL_Event event = { 0 };

	bool quit = false;

	float turnH = 0.0f;
	float turnV = 0.0f;
	float turnR = 0.0f;

	float moveP = 0.0f;
	float strafeP = 0.0f;
	float riseP = 0.0f;
	float tiltP = 0.0f;

	float turnHe = 0.0f;
	float turnVe = 0.0f;
	float turnRe = 0.0f;

	float moveE = 0.0f;
	float strafeE = 0.0f;
	float riseE = 0.0f;
	float tiltE = 0.0f;

	float turnVmin = PI * 0.5f;
	float turnVmax = 3 * PI * 0.5f;

	float torchIntensity = 10.0f;
	float maxIllumination = 1.0f;

	EventHandler(float step, float turn, float sens);
	~EventHandler();

	void toggleAIGoal();
	void HandleUserEvents();
	void ceaseMotion();
};

