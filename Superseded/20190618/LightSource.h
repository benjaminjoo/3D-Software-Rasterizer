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

	double getIllumination(vect3);
};

