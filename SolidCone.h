#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class SolidCone: public SolidBody
{

	double			radius;
	double			height;
	int				resol;

public:

	SolidCone();
	SolidCone(double, double, double, Uint32);
	SolidCone(double, double, double, double, double, double, double, double, double, Uint32, int, double, double, int);
	~SolidCone();

	int	getTotalVert();
	int	getTotalPoly();
	void getVertexData(vect3*);
	void getTriangleData(triangle3dV*);
};

