#pragma once

#include <stdio.h>
#include "Definitions.h"

class Extruded
{
	double		offsetX;
	double		offsetY;

	int			resProfile;
	int			resPath;

	int			nProfile;
	int			nPath;

	polyNode*	pProfile;
	polyNode*	pPath;

	Uint32		c;

public:
	Extruded();
	Extruded(double offX, double offY, int resPrf, int resPth, int nPrf, int nPth, polyNode* pPrf, polyNode* pPth, Uint32 col);
	~Extruded();

	Uint32 getColour();
	int numberOfProfileNodes();
	int numberOfPathNodes();
	int getTotalPoly();
	int numberOfProfileArcs();
	int numberOfProfileFlats();
	int numberOfPathArcs();
	int numberOfPathFlats();
	int* arcDirectionsProfile();
	int* arcDirectionsPath();
	polyNode* arcCentresProfile();
	polyNode* arcCentresPath();
	polyNode* tessellateProfile();
	polyNode* tessellatePath();
	double* getPathBisectorAng();
	double* getSinB();
	double* getCosB();
	polyNode* get2dNormals();
	vect3* getVertexData();
	triangle3dV* getTriangleData();

	triangle3dV* getTriangleDataN();
};

