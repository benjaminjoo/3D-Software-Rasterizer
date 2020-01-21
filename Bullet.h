#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class Bullet: public SolidBody
{
private:

	float	dia				= 0.01f;
	float	len				= 1.0f;
	float	muzzleVelocity	= 5.0f;

public:

	Bullet();
	Bullet(float, float, float, float, float, float, Uint32);
	~Bullet();

	int	getTotalVert();
	int	getTotalPoly();
	void getTriangleData(triangle3dV*);
	float getMuzzleVelocity();
};

