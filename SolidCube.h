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
	void			getTriangleData_(triangle3dV*);
	vect3			getPosition();
	void			constructShadowVolume(vect3);
};

