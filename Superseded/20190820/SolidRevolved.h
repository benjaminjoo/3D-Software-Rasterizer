#pragma once

#include "Definitions.h"
#include "SolidBody.h"		//Base class

class SolidRevolved: public SolidBody
{
	double			scaleX;
	double			scaleY;
	double			scaleZ;

	double			posX;
	double			posY;
	double			posZ;

	double			rotX;
	double			rotY;
	double			rotZ;

	Uint32			colour;
	int				texture;

	int				n;
	polyNode*		polyLine;

	double			offset;

	double			aStart;
	double			aEnd;

	int				resolH;
	int				resolV;

public:
	SolidRevolved();
	SolidRevolved(double, double, double, double, double, double, double, double, double,
		Uint32, int, int, polyNode*, double, double, double, int, int);
	~SolidRevolved();

	int				numberOfStepsH();
	int				getTotalVert();
	int				getTotalPoly();
	int				numberOfNodes();
	int				numberOfArcs();
	int				numberOfFlats();
	int*			arcDirections();
	polyNode*		arcCentres();
	polyNode*		tessellatePolyline();
	polyNode*		get2dNormals();
	vect3*			getVertexData();
	triangle3dV*	getTriangleDataFlat();
	triangle3dV*	getTriangleData();
};

