#pragma once

#include "Shapes.h"
#include "LightSource.h"
#include "ShadowVolume.h"
#include "EventHandler.h"
#include "Camera.h"
#include <time.h>


class Renderer
{
	Shapes*			Solids;

	Shapes*			Actors;

	Shapes*			Entities;

	LightSource*	Sun;

	Camera*			Player;

	EventHandler*	Controls;

	unsigned int	solidN;
	unsigned int	actorN;

	int*			solidPolyCount;
	int*			actorPolyCount;

	triangle3dV**	solidMesh;
	triangle3dV**	actorMesh;
	triangle3dV**	triangleMesh;
	triangle3dV**	shadowMesh;

	vect3			sunVector;

	int				nShadows;
	ShadowVolume*	vShadows;

	double			hRatio;
	double			vRatio;

	clock_t			oldTime;
	clock_t			newTime;
	clock_t			frameTime;
	int				frameCounter;
	int				polyCounter;

	double			collisionDist;

public:

	Renderer(Shapes*, Shapes*, LightSource*, Camera*, EventHandler*);
	~Renderer();

	void updateCameraPosition(double, double, double, double, double, double);
	void updateCameraPosition(double, double, double, double, double, double, bool);
	void checkCameraCollision();
	bool checkCameraCollision(vect3, int*, int*);
	bool checkCameraCollision(vect3*, int*, int*);

	bool polyFacingLightsource(vect3, triangle3dV);
	void updateShadowVolumes(model);
	void updateBoundingBoxes(model, double);
	void updateEntities(model);
	void renderPoints(int, point3*, Uint32*);
	void renderLines(int, line3d*, Uint32*, double*);
	void renderEntities(model, Uint32*, double*);
	void displayStats(bool, bool, bool, bool, Canvas);

	void updateFrameCounter();
	void resetPolyCounter();
	void incrementPolyCounter();
	void calculateFrametime();
};

