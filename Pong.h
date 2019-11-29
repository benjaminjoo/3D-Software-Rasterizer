#pragma once


#include <vector>
#include <memory>


#include "Canvas.h"
#include "Camera.h"
#include "EventHandler.h"
#include "LightSource.h"
#include "SolidBody.h"
#include "SolidSphere.h"


class Pong
{
private:

	std::shared_ptr<Canvas>						Screen			= nullptr;
	std::shared_ptr<Camera>						Eye				= nullptr;
	std::shared_ptr<EventHandler>				Controls		= nullptr;
	std::shared_ptr<LightSource>				Sun				= nullptr;
	std::shared_ptr<SolidSphere>				Ball			= nullptr;

	std::vector<txt>							textureData;

	std::vector<std::shared_ptr<SolidBody>>		Entities;

	std::shared_ptr<SolidBody>					Bar;

	vect3										vertexList[MAXCLIPVERTS];
	textCoord									uvList[MAXCLIPVERTS];

	double										hRatio;
	double										vRatio;

	triangle3dV**								triangleMesh	= nullptr;
	unsigned int*								polyCount		= nullptr;

	triangle3dV*								ballMesh		= nullptr;
	unsigned int								ballPolyCount	= 0;
	double										ballRadius		= 0.0f;

	clock_t			oldTime			= 0;
	clock_t			newTime			= 0;
	clock_t			frameTime		= 0;
	int				frameCounter	= 0;
	int				polyCounter		= 0;

	int				bounceCount		= 0;

	vect3			gravity			= { 0.0f, 0.0f, -0.01f, 0.0f };

	void updateCameraPosition();
	void updateEntities();
	void updateBall();
	bool ballApproachingWall(vect3 p, vect3 v, const unsigned int& i, const unsigned int& j);

	void renderMesh(transform3d eyePosition, int nPoly, triangle3dV* mesh);
	void renderAll();

	void updateFrameCounter();
	void resetPolyCounter();
	void incrementPolyCounter();
	void calculateFrametime();

	void displayStats(bool, bool, bool, bool, std::shared_ptr<Canvas>);

public:

	Pong(std::shared_ptr<Canvas> screen, std::shared_ptr<Camera> eye, std::shared_ptr<EventHandler> controls,
			std::shared_ptr<LightSource> sun, std::shared_ptr<SolidSphere> ball);
	~Pong();

	void addTexture(SDL_Surface*);
	void addEntity(std::shared_ptr<SolidBody>);
	void buildMesh();
	void destroyMesh();
	void updateAll();

};

