#pragma once

#include "Definitions.h"

class Sphere
{
	double x;
	double y;
	double z;

	double radius;

	int resol;

public:

	Sphere();
	Sphere(double, double, double, int);
	Sphere(double, double, double, double, int);


	~Sphere();
	
	int getTotal();
	vect3* getVertexData();
};
