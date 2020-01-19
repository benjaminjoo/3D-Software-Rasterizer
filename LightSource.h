#pragma once

#include "Definitions.h"

class LightSource
{

	double azm			= 0.0f;
	double alt			= 0.0f;

	double deltaAzm		= 1.0f;
	double deltaAlt		= 0.0f;

	vect3 direction		= { 0.0f, 0.0f, 0.0f, 0.0f };

	double intensity	= 0.0f;

public:

	LightSource();
	LightSource(double, double);
	LightSource(double, double, double);
	~LightSource();

	void setPosition(double, double);
	void update();

	double getIllumination(vect3& N);
	double getBlinnSpecular(vect3 N, vect3 V, double shine);

	vect3 getVector();
};

