#pragma once

#include "Definitions.h"
#include "SolidBody.h"		//Base class

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
	vect3*			getVertexData();
	void			getVertexData_(vect3*);
	triangle3dV*	getTriangleData();
	void			getTriangleData_(triangle3dV*);

};

