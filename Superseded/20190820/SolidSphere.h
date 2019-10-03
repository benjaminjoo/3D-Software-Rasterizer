#pragma once

#include "Definitions.h"
#include "SolidBody.h"		//Base class

class SolidSphere: public SolidBody
{
	double			scaleX;
	double			scaleY;
	double			scaleZ;

	double			posX;
	double			posY;
	double			posZ;

	double			rotX;
	double			rotY;
	double			rotZ;

	Uint32			colour;
	int				texture;

	double			radius;
	int				resol;

public:
	SolidSphere();
	SolidSphere(double, double, double, Uint32);
	SolidSphere(double, double, double, double, double, double, double, double, double, Uint32, int, double, int);
	~SolidSphere();

	int				getTotalVert();
	int				getTotalPoly();
	vect3*			getVertexData();
	triangle3dV*	getTriangleData();
};
