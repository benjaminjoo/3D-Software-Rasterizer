#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class SolidCylinder: public SolidBody
{

	float			radius;
	float			height;
	int				resol;

public:

	SolidCylinder();
	SolidCylinder(float, float, float, Uint32);
	SolidCylinder(float, float, float, float, float, float, float, float, float, Uint32, int, float, float, int);
	SolidCylinder(float, float, float, float, float, float, float, float, float, Uint32, int, float, float, int, bool);
	~SolidCylinder();

	int	getTotalVert();
	int	getTotalPoly();
	void getVertexData(vect3*);
	void getTriangleData(triangle3dV*);
};

