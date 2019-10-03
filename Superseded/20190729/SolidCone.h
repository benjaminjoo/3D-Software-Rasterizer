#pragma once

#include "Definitions.h"
#include "SolidBody.h"		//Base class

class SolidCone: public SolidBody
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
	double			height;
	int				resol;

public:
	SolidCone();
	SolidCone(double, double, double, Uint32);
	SolidCone(double, double, double, double, double, double, double, double, double, Uint32, int, double, double, int);
	~SolidCone();

	int				getTotalVert();
	int				getTextureID();
	int				getTotalPoly();
	vect3*			getVertexData();
	triangle3dV*	getTriangleData();
	Uint32			getColour();
};

