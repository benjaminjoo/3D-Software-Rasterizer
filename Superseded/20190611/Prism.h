#pragma once

#include "Definitions.h"

class Prism
{
	double x;
	double y;
	double z;

	double a;
	double b;
	double c;

	int resol;

public:

	Prism();
	Prism(double, double, double, int);
	Prism(double, double, double, double, double, double, int);

	~Prism();

	int getTotal();
	vect3* getVertexData();
};

