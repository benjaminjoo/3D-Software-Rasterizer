#pragma once

#include "Definitions.h"
#include "Sphere.h"
#include "Prism.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Cube.h"
#include "Model.h"
#include "LightSource.h"

class World
{

public:

	LightSource*	lamps;
	Model*			models;
	Cube*			cubes;
	Prism*			prisms;
	Sphere*			spheres;
	Cylinder*		cylinders;
	Cone*			cones;

	unsigned int	nEntities;
	unsigned int*	polyCountEntities;
	Uint32*			colourEntities;
	bool*			gouraudEntities;

	World();
	~World();

	//unsigned int getNSphere();
};

