#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class SolidCone: public SolidBody
{

	float			radius;
	float			height;
	int				resol;

public:

	SolidCone();
	SolidCone(float, float, float, Uint32);
	SolidCone(float, float, float, float, float, float, float, float, float, Uint32, int, float, float, int);
	~SolidCone();

	int	getTotalVert();
	int	getTotalPoly();
	void getVertexData(vect3*);
	void getTriangleData(triangle3dV*);
};

