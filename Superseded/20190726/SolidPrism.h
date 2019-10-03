#pragma once

#include "Definitions.h"
#include "SolidBody.h"		//Base class

class SolidPrism: public SolidBody
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

	double			edgeX;
	double			edgeY;
	double			edgeZ;

public:
	SolidPrism();
	SolidPrism(double, double, double, Uint32, double, double, double);
	SolidPrism(double, double, double, double, double, double, double, double, double, Uint32, int, double, double, double);
	~SolidPrism();

	int				getTotalVert();
	int				getTextureID();
	int				getTotalPoly();
	triangle3dV*	getTriangleData();
	Uint32			getColour();
};

