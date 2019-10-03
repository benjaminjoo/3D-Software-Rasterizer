#pragma once

#include "Definitions.h"

class Cube
{
	double x;
	double y;
	double z;

	double s;

	Uint32 c;

public:
	Cube();
	Cube(double, double, double, Uint32);
	Cube(double, double, double, double, Uint32);
	~Cube();

	triangle3d* getTriangleData();
	Uint32		getColour();
};

