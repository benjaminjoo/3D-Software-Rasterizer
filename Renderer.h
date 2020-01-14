#pragma once

#include "Shapes.h"
#include "LightSource.h"
#include "ShadowVolume.h"
#include "EventHandler.h"
#include "Camera.h"
#include <time.h>
#include <memory>
#include <fstream>


class Renderer
{
	std::shared_ptr<Shapes>			Solids;

	std::shared_ptr<Shapes>			Actors;

	std::shared_ptr<Shapes>			Entities;

	std::shared_ptr<LightSource>	Sun;

	std::shared_ptr<Camera>			Eye;

	std::shared_ptr<EventHandler>	Controls;

	unsigned int	solidN;
	unsigned int	actorN;

	triangle3dV**	solidMesh		= nullptr;
	triangle3dV**	actorMesh		= nullptr;
	triangle3dV**	triangleMesh	= nullptr;
	triangle3dV**	shadowMesh		= nullptr;

	vect3			sunVector;

	int				nShadows;
	ShadowVolume*	vShadows;

	double			hRatio;
	double			vRatio;

	clock_t			oldTime			= 0;
	clock_t			newTime			= 0;
	clock_t			frameTime		= 0;
	int				frameCounter	= 0;
	int				polyCounter		= 0;

	double			collisionDist	= 1.0f;

public:

	Renderer(std::shared_ptr<Shapes> solids, std::shared_ptr<Shapes> actors, std::shared_ptr<LightSource> sun,
				std::shared_ptr<Camera> eye, std::shared_ptr<EventHandler> controls);
	~Renderer();

	void updateCameraPosition();
	void updateCameraPosition(double, double, double, double, double, double, bool);
	void checkCameraCollision();
	bool checkCameraCollision(vect3, int*, int*);
	bool checkCameraCollision(vect3*, int*, int*);

	bool polyFacingLightsource(vect3, triangle3dV);
	void updateShadowVolumes(model);
	void updateBoundingBoxes(model, double);
	void updateEntities(model);
	void renderPoints(int, point3*, Uint32*, double*);
	void renderEntities(model, Uint32*, double*);
	void displayStats(bool, bool, bool, bool, std::shared_ptr<Canvas>);

	void updateFrameCounter();
	void resetPolyCounter();
	void incrementPolyCounter();
	void calculateFrametime();

	void exportMesh(const std::string& fileName);
};

