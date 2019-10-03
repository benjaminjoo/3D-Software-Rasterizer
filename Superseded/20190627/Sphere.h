#pragma once

#include "Definitions.h"

class Sphere
{
	double x;
	double y;
	double z;

	double radius;

	int resol;

	bool gouraud;

	Uint32 c;

public:

	Sphere();
	Sphere(double, double, double, int, Uint32);
	Sphere(double, double, double, double, int, bool, Uint32);


	~Sphere();
	
	int				getTotalVert();
	int				getTotalPoly();
	vect3*			getVertexData();
	triangle3dV*	getTriangleData();
	Uint32			getColour();
	unsigned char	getRedValue();
	unsigned char	getGreenValue();
	unsigned char	getBlueValue();
	bool			isGouraudOn();
};
