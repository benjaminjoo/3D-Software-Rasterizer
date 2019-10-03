#pragma once

#include "Definitions.h"

class Cylinder
{
	double x;
	double y;
	double z;

	double radius;
	double height;

	int resol;

	Uint32 c;
public:
	Cylinder();
	Cylinder(double, double, double, double, int, Uint32);
	Cylinder(double, double, double, double, double, int, Uint32);

	~Cylinder();

	int				getTotalVert();
	int				getTotalPoly();
	vect3*			getVertexData();
	triangle3d*		getTriangleData();
	Uint32			getColour();
	unsigned char	getRedValue();
	unsigned char	getGreenValue();
	unsigned char	getBlueValue();
};

