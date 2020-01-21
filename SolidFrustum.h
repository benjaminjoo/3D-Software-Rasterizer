#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class SolidFrustum: public SolidBody
{
private:

	float zNear	= 1.0f;
	float zFar		= 99.9f;
	float fovH		= PI * 0.5f;
	float aspRatio	= 0.5625f;

	Uint32 colour	= 0x000000ff;

public:
	SolidFrustum();
	SolidFrustum(float, float, float, float, float, float, float, Uint32);
	~SolidFrustum();

	int	getTotalVert();
	int	getTotalPoly();
	void getTriangleData(triangle3dV*);
};

