#pragma once

#include "Definitions.h"
#include "SolidBody.h"		//Base class

class SolidCube: public SolidBody
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
	double			txU;

	double			edge;

public:
	SolidCube();
	SolidCube(double, double, double, Uint32);
	SolidCube(double, double, double, double, double, double, double, double, double, Uint32, int, double);
	~SolidCube();

	int				getTotalVert();
	int				getTextureID();
	int				getTotalPoly();
	triangle3dV*	getTriangleData();
	Uint32			getColour();
};

