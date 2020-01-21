#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class SolidCube: public SolidBody
{

	float			edge;

public:

	SolidCube();
	SolidCube(float, float, float, Uint32);
	SolidCube(float, float, float, float, float, float, float, float, float, Uint32, int, float);
	~SolidCube();

	int	getTotalVert();
	int	getTotalPoly();
	void getTriangleData(triangle3dV*);
};

