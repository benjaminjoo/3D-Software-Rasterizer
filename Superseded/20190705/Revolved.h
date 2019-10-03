#pragma once

#include "Definitions.h"

class Revolved
{
	double		offset;

	double		aStart;
	double		aEnd;

	int			resolH;
	int			resolV;

	int			n;
	polyNode*	polyLine;

	Uint32		c;

public:
	Revolved();
	Revolved(double cx, double cy, double cz, double off, double aS, double aE, int rH, int rV, int nP, polyNode* pLine, Uint32 col);
	~Revolved();

	Uint32 getColour();
	int numberOfStepsH();
	int getTotalPoly();
	int numberOfNodes();
	int numberOfArcs();
	int numberOfFlats();
	int* arcDirections();
	polyNode* arcCentres();
	polyNode* tessellatePolyline();
	polyNode* get2dNormals();
	vect3* getVertexData();
	triangle3dV* getTriangleData();
	triangle3dV* getTriangleDataN();


};

