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

	int				getTotalVert();
	int				getTotalPoly();
	triangle3dV*	getTriangleData();
	Uint32			getColour();
	unsigned char	getRedValue();
	unsigned char	getGreenValue();
	unsigned char	getBlueValue();

	void updateX(double);
	void updateY(double);
	void updateZ(double);
	void updateC(Uint32);
	void updateS(double);
};

