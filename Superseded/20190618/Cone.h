#pragma once

#include "Definitions.h"

class Cone
{
	double x;
	double y;
	double z;

	double radius;
	double height;

	int resol;

	Uint32 c;
public:
	Cone();
	Cone(double, double, double, double, int, Uint32);
	Cone(double, double, double, double, double, int, Uint32);

	~Cone();

	int				getTotalVert();
	int				getTotalPoly();
	vect3*			getVertexData();
	triangle3d*		getTriangleData();
	Uint32			getColour();
	unsigned char	getRedValue();
	unsigned char	getGreenValue();
	unsigned char	getBlueValue();
};


