#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class SolidPrism: public SolidBody
{

	double			edgeX;
	double			edgeY;
	double			edgeZ;

public:

	SolidPrism();
	SolidPrism(double, double, double, Uint32, double, double, double);
	SolidPrism(double, double, double, double, double, double, double, double, double, Uint32, int, double, double, double);
	~SolidPrism();

	int				getTotalVert();
	int				getTotalPoly();
	void			getTriangleData_(triangle3dV*);
	void			constructShadowVolume(vect3);
};

