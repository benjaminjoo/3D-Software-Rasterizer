#pragma once

#include "Definitions.h"
//DO NOT INCLUDE DERIVED CLASSES!!!

class SolidBody
{
public:
	SolidBody();
	~SolidBody();

	virtual int				getTotalVert()		= 0;
	virtual int				getTotalPoly()		= 0;
	virtual triangle3dV*	getTriangleData()	= 0;

	//virtual void			setPosition(vect3)	= 0;
};

