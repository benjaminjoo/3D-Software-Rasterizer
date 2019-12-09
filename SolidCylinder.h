#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class SolidCylinder: public SolidBody
{

	double			radius;
	double			height;
	int				resol;

public:

	SolidCylinder();
	SolidCylinder(double, double, double, Uint32);
	SolidCylinder(double, double, double, double, double, double, double, double, double, Uint32, int, double, double, int);
	~SolidCylinder();

	int				getTotalVert();
	int				getTotalPoly();
	void			getVertexData_(vect3*);
	void			getTriangleData_(triangle3dV*);
	void			constructShadowVolume(vect3);
};

