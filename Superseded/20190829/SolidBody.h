#pragma once

#include "Definitions.h"
//DO NOT INCLUDE DERIVED CLASSES!!!

class SolidBody
{
protected:

	double			scaleX;
	double			scaleY;
	double			scaleZ;

	double			posX;
	double			posY;
	double			posZ;

	double			rotX;
	double			rotY;
	double			rotZ;

	vect3			position;
	vect3			velocity;

	Uint32			colour;
	int				texture;
	double			txU;

public:

	SolidBody();
	~SolidBody();

	virtual int				getTotalVert()					= 0;
	virtual int				getTotalPoly()					= 0;
	virtual triangle3dV*	getTriangleData()				= 0;
	virtual void			getTriangleData_(triangle3dV*)	= 0;
};

