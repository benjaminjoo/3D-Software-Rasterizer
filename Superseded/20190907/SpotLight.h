#pragma once

#include "Definitions.h"

class SpotLight
{

	vect3 position;

	double intensity;

public:
	SpotLight();
	SpotLight(vect3, double);
	~SpotLight();

	void setPosition(double, double, double);
	vect3 getPosition();
	double getIntensity();

};

