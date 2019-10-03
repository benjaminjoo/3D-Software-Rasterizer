#pragma once

#include "Definitions.h"
#include "SolidBody.h"		//Base class

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

	int				getTotalVert();
	int				getTotalPoly();
	vect3*			getVertexData();
	void			getVertexData_(vect3*);
	triangle3dV*	getTriangleData();
	void			getTriangleData_(triangle3dV*);

};

