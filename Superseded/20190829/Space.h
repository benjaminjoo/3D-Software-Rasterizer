#pragma once

#include "Definitions.h"

class Space
{

public:
	Space();
	~Space();

	virtual int				getTotalVert() = 0;
	virtual int				getTotalPoly() = 0;
	virtual triangle3dV*	getTriangleData()	= 0;
};

