#pragma once

#include "Definitions.h"
//DO NOT INCLUDE DERIVED CLASSES!!!

class SolidBody
{
	//double scaleX;
	//double scaleY;
	//double scaleZ;

	//double posX;
	//double posY;
	//double posZ;

	//double rotX;
	//double rotY;
	//double rotZ;

	//Uint32 colour;

public:
	SolidBody();
	~SolidBody();

	virtual int				getTotalVert()		= 0;
	virtual int				getTextureID()		= 0;
	virtual int				getTotalPoly()		= 0;
	virtual triangle3dV*	getTriangleData()	= 0;
	virtual Uint32			getColour()			= 0;
};

