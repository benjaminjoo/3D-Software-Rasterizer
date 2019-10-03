#pragma once
#include "Definitions.h"
#include "SolidBody.h"

class BezierPatch: public SolidBody
{
	unsigned int	lod;
	vect3			ctrlP[9];
	vect3*			vertices;
	bool			isInsideOut;

	Uint32			colour;
	int				texture;

public:
	BezierPatch(int, int, Uint32);
	BezierPatch(int, vect3*);
	BezierPatch(int, vect3, vect3, vect3, vect3, vect3, vect3, vect3, vect3, vect3);
	~BezierPatch();

	void	setControlPoint(unsigned int, vect3);
	void	turnInsideOut();

	int		getTotalVert();
	int		getTotalPoly();

	void	getSpineCoord(vect3, vect3, vect3, vect3*);
	void	getTangent(vect3, vect3, vect3, vect3*);
	vect3	getTangent(vect3, vect3, vect3, int);

	void	getVertexData(point3*);
	void	getVertexData_(vect3*, vect3*);
	void	getTriangleData_(triangle3dV*);
	vect3	getPosition();
	void	constructShadowVolume(vect3);
};

