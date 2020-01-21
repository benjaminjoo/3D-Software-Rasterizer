#pragma once

#include "Definitions.h"

class LightSource
{

	float azm			= 0.0f;
	float alt			= 0.0f;

	float deltaAzm		= 1.0f;
	float deltaAlt		= 0.0f;

	vect3 direction		= { 0.0f, 0.0f, 0.0f, 0.0f };

	float intensity	= 0.0f;

public:

	LightSource();
	LightSource(float, float);
	LightSource(float, float, float);
	~LightSource();

	void setPosition(float, float);
	void update();

	float getIllumination(vect3& N);
	float getBlinnSpecular(vect3 N, vect3 V, float shine);

	vect3 getVector();
};

