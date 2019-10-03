#pragma once

#include "Definitions.h"
#include "SolidBody.h"		//Base class

class SolidCube: public SolidBody
{

	double			edge;

public:

	SolidCube();
	SolidCube(double, double, double, Uint32);
	SolidCube(double, double, double, double, double, double, double, double, double, Uint32, int, double);
	~SolidCube();

	int				getTotalVert();
	int				getTotalPoly();
	triangle3dV*	getTriangleData();
	void			getTriangleData_(triangle3dV*);

};

