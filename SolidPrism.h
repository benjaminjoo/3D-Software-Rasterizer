#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class SolidPrism: public SolidBody
{

	float			edgeX	= 0.0f;
	float			edgeY	= 0.0f;
	float			edgeZ	= 0.0f;

public:

	SolidPrism();
	SolidPrism(float, float, float, Uint32, float, float, float);
	SolidPrism(float, float, float, float, float, float, float, float, float, Uint32, int, float, float, float);
	~SolidPrism();

	int	getTotalVert();
	int	getTotalPoly();
	void getTriangleData(triangle3dV*);
};

