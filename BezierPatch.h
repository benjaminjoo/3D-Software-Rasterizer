#pragma once
#include "Definitions.h"
#include "SolidBody.h"
#include <memory>

class BezierPatch : public SolidBody
{
	unsigned int	lod;
	vect3			ctrlP[9];
	bool			isInsideOut;

	Uint32			colour;
	int				texture;

public:
	BezierPatch(unsigned int, int, Uint32);
	BezierPatch(unsigned int, vect3*);
	BezierPatch(unsigned int, vect3, vect3, vect3, vect3, vect3, vect3, vect3, vect3, vect3);
	~BezierPatch();

	void	setControlPoint(unsigned int, vect3);
	void	turnInsideOut();

	int		getTotalVert();
	int		getTotalPoly();

	void	getSpineCoord(vect3, vect3, vect3, vect3*);
	void	getSpineCoord(vect3, vect3, vect3, std::shared_ptr<vect3[]>);
	void	getTangent(vect3, vect3, vect3, vect3*);
	vect3	getTangent(vect3, vect3, vect3, int);

	void	getVertexData(point3*);
	void	getVertexData_(std::shared_ptr<vect3[]>, std::shared_ptr<vect3[]>);
	void	getTriangleData_(triangle3dV*);
	void	constructShadowVolume(vect3);
};

