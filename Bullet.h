#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class Bullet: public SolidBody
{
private:

	double	dia				= 0.01f;
	double	len				= 1.0f;
	double	muzzleVelocity	= 5.0f;

public:

	Bullet();
	Bullet(double, double, double, double, double, double, Uint32);
	~Bullet();

	int		getTotalVert();
	int		getTotalPoly();
	void	getTriangleData_(triangle3dV*);
	void	constructShadowVolume(vect3);
	double	getMuzzleVelocity();
};

