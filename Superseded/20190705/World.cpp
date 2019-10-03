#include "World.h"

#include "Definitions.h"
#include "Sphere.h"
#include "Prism.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Cube.h"
#include "Model.h"
#include "Camera.h"
#include "Canvas.h"
#include "LightSource.h"

World::World()
{
	lamps				= new LightSource[MAXLIGHTSOURCES];
	models				= new Model[MAXMODELS];
	cubes				= new Cube[MAXCUBES];
	prisms				= new Prism[MAXPRISMS];
	spheres				= new Sphere[MAXSPHERES];
	cylinders			= new Cylinder[MAXCYLINDERS];
	cones				= new Cone[MAXCONES];

	polyCountEntities	= new unsigned int[MAXENTITIES];
	colourEntities		= new Uint32[MAXENTITIES];
	gouraudEntities		= new bool[MAXENTITIES];
}

World::~World()
{
}


