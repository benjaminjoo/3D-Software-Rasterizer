#pragma once

#include "Definitions.h"
#include "SolidSphere.h"

class SolidEgg: public SolidBody
{
	double			radius;
	int				resol;

	double			distortion;

public:
	SolidEgg();
	SolidEgg(double, double, double, Uint32);
	SolidEgg(double, double, double, double, double, double, double, double, double, double, Uint32, int, double, int);
	~SolidEgg();

	int				getTotalVert();
	int				getTotalPoly();
	void			getVertexData_(vect3*);
	void			getTriangleData_(triangle3dV*);
	vect3			getPosition();
	void			constructShadowVolume(vect3);
};

