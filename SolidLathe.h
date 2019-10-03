#pragma once

#include "Definitions.h"
#include "SolidBody.h"		//Base class

class SolidLathe: public SolidBody
{
	int				nProfile;
	polyNode*		polyLine;

	double			offset;
	double			aStart;
	double			aEnd;

	int				resolH;
	int				resolV;

public:

	SolidLathe();
	SolidLathe(double, double, double, double, double, double, double, double, double,
		Uint32, int, int, polyNode*, double, double, double, int, int);
	~SolidLathe();

	int				numberOfStepsH();
	//int				getTotalVert();
	//int				getTotalPoly();
	//int				numberOfNodes();
	//int				numberOfArcs();
	//int				numberOfFlats();
	//int*			arcDirections();
	//polyNode*		arcCentres();
	//polyNode*		tessellatePolyline();
	//polyNode*		get2dNormals();
	//vect3*			getVertexData();
	//triangle3dV*	getTriangleDataFlat();
	//void			getTriangleData_(triangle3dV*);
	//vect3			getPosition();
};	

