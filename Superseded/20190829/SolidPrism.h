#pragma once

#include "Definitions.h"
#include "SolidBody.h"		//Base class

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
	triangle3dV*	getTriangleData();
	void			getTriangleData_(triangle3dV*);

};

