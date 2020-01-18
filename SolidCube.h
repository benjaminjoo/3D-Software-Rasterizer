#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class SolidCube: public SolidBody
{

	double			edge;

public:

	SolidCube();
	SolidCube(double, double, double, Uint32);
	SolidCube(double, double, double, double, double, double, double, double, double, Uint32, int, double);
	~SolidCube();

	int	getTotalVert();
	int	getTotalPoly();
	void getTriangleData(triangle3dV*);
};

