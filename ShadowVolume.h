#pragma once

#include "Definitions.h"

class ShadowVolume
{
	bool isActive;

	plane A;
	plane B;
	plane C;

	plane T;

public:
	ShadowVolume();
	ShadowVolume(bool, plane, plane, plane, plane);
	~ShadowVolume();

	void setPlaneA(plane);
	void setPlaneB(plane);
	void setPlaneC(plane);
	void setPlaneT(plane);

	bool checkVert(vect3);
};

