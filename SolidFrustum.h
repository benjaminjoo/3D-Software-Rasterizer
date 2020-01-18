#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class SolidFrustum: public SolidBody
{
private:

	double zNear	= 1.0f;
	double zFar		= 99.9f;
	double fovH		= PI * 0.5f;
	double aspRatio	= 0.5625f;

	Uint32 colour	= 0x000000ff;

public:
	SolidFrustum();
	SolidFrustum(double, double, double, double, double, double, double, Uint32);
	~SolidFrustum();

	int	getTotalVert();
	int	getTotalPoly();
	void getTriangleData(triangle3dV*);
};

