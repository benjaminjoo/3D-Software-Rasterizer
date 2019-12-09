#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class SolidSphere: public SolidBody
{

	double			radius	= 0.5f;
	int				resol	= 12;

public:

	SolidSphere();
	SolidSphere(double, double, double, Uint32);
	SolidSphere(double, double, double, double, double, double, double, double, double, Uint32, int, double, int);
	~SolidSphere();

	void			setRadius(double);
	double			getRadius();
	int				getTotalVert();
	int				getTotalPoly();
	void			getVertexData_(vect3*);
	void			getTriangleData_(triangle3dV*);
	void			constructShadowVolume(vect3);
};
