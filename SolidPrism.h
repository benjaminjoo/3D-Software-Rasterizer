#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class SolidPrism: public SolidBody
{

	double			edgeX	= 0.0f;
	double			edgeY	= 0.0f;
	double			edgeZ	= 0.0f;

public:

	SolidPrism();
	SolidPrism(double, double, double, Uint32, double, double, double);
	SolidPrism(double, double, double, double, double, double, double, double, double, Uint32, int, double, double, double);
	~SolidPrism();

	int	getTotalVert();
	int	getTotalPoly();
	void getTriangleData(triangle3dV*);
};

