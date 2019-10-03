#pragma once

#include "Definitions.h"

class LightSource
{

	double azm;
	double alt;

	double intensity;

public:

	LightSource();
	LightSource(double, double);
	LightSource(double, double, double);
	~LightSource();

	void setPosition(double, double);

	double getIllumination(vect3);

	vect3 getVector();
};

