#pragma once

#include "Definitions.h"

class Torus
{
	double x;
	double y;
	double z;

	double radiusH;
	double radiusV;

	int resolH;
	int resolV;

	Uint32 c;

public:

	Torus();
	Torus(double, double, double, Uint32);
	Torus(double, double, double, double, double, int, int, Uint32);


	~Torus();

	int				getTotalVert();
	int				getTotalPoly();
	vect3*			getVertexData();
	triangle3d*		getTriangleData();
	Uint32			getColour();
	unsigned char	getRedValue();
	unsigned char	getGreenValue();
	unsigned char	getBlueValue();
};
