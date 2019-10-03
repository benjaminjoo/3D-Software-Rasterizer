#pragma once

#include "Camera.h"
#include "Canvas.h"
#include "Shapes.h"

class RayTracer
{
	int		width;
	int		height;

	Camera*	Player;
	Canvas*	Screen;

	vect3*	field;

	Shapes* Solids;
	int solidN;
	int* solidPolyCount;

	triangle3dV** triangleMesh;

public:
	RayTracer(int, int, Camera*, Canvas*, Shapes*);
	~RayTracer();

	bool checkIntersection(int, Uint32*);
	void calculatePixel();
};

