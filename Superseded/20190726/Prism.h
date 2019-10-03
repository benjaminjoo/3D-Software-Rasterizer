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

	Uint32 col;

public:

	Prism();
	Prism(double, double, double, double, Uint32);
	Prism(double, double, double, double, double, double, Uint32);
	~Prism();

	int				getTotalVert();
	int				getTotalPoly();
	triangle3dV*	getTriangleData();
	Uint32			getColour();
	//unsigned char	getRedValue();
	//unsigned char	getGreenValue();
	//unsigned char	getBlueValue();
};

