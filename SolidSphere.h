#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class SolidSphere: public SolidBody
{

	float			radius	= 0.5f;
	int				resol	= 12;

public:

	SolidSphere();
	SolidSphere(float, float, float, Uint32);
	SolidSphere(float, float, float, float, float, float, float, float, float, Uint32, int, float, int);
	SolidSphere(float, float, float, float, float, float, float, float, float, Uint32, int, float, int, bool);
	~SolidSphere();

	void setRadius(float);
	float getRadius();
	int	getTotalVert();
	int	getTotalPoly();
	void getVertexData(vect3*);
	void getTriangleData(triangle3dV*);
};
