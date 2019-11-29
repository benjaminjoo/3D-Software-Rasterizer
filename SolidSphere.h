#pragma once

#include "Definitions.h"
#include "SolidBody.h"		//Base class

class SolidSphere: public SolidBody
{

	double			radius;
	int				resol;

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
	vect3			getPosition();
	void			constructShadowVolume(vect3);
};
