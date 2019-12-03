#pragma once


#include <vector>
#include <memory>


#include "Canvas.h"
#include "Camera.h"
#include "EventHandler.h"
#include "LightSource.h"
#include "Player.h"
#include "SolidBody.h"
#include "SolidSphere.h"
#include "SolidCube.h"
#include "Bullet.h"


class Pong
{
private:

	std::shared_ptr<Canvas>						Screen			= nullptr;
	std::shared_ptr<Camera>						Eye				= nullptr;
	std::shared_ptr<EventHandler>				Controls		= nullptr;
	std::shared_ptr<LightSource>				Sun				= nullptr;
	std::shared_ptr<Player>						Hero			= nullptr;

	std::vector<txt>							textureData;

	std::vector<std::shared_ptr<SolidBody>>		Entities;
	std::vector<std::shared_ptr<SolidBody>>		Balls;
	std::vector<std::shared_ptr<SolidBody>>		Projectiles;

	std::shared_ptr<SolidBody>					Bar;

	vect3										vertexList[MAXCLIPVERTS];
	textCoord									uvList[MAXCLIPVERTS];

	double										hRatio;
	double										vRatio;

	triangle3dV**								triangleMesh		= nullptr;
	unsigned int*								polyCount			= nullptr;

	triangle3dV*								playerMesh			= nullptr;
	unsigned int								playerPolyCount		= 0;

	triangle3dV**								ballMesh			= nullptr;
	unsigned int*								ballPolyCount		= 0;

	triangle3dV**								projectileMesh		= nullptr;
	unsigned int*								projectilePolyCount = nullptr;

	unsigned int								ammo				= 0;

	clock_t			lastShot		= 0;
	clock_t			oldTime			= 0;
	clock_t			newTime			= 0;
	clock_t			frameTime		= 0;
	int				frameCounter	= 0;
	int				polyCounter		= 0;

	vect3			gravity			= { 0.0f, 0.0f, -0.01f, 0.0f };

	void updateCameraPosition(const std::shared_ptr<Player>&);
	void updatePlayerPosition();
	void updateEntities();
	void updateBalls();
	void updateProjectiles();

	void hitTest(const std::shared_ptr<SolidBody>&, std::vector<std::shared_ptr<SolidBody>>);

	void updateMovingObject(std::shared_ptr<SolidBody>, int, triangle3dV*);
	bool objectApproachingWall(vect3, vect3, const unsigned int&, const unsigned int&);

	void explodeMesh(vect3, int, triangle3dV*);
	void renderMesh(transform3d, int, triangle3dV*);
	void renderAll();

	void updateFrameCounter();
	void resetPolyCounter();
	void incrementPolyCounter();
	void calculateFrametime();

	void displayStats(bool, bool, bool, bool, bool, std::shared_ptr<Canvas>);

public:

	Pong(std::shared_ptr<Canvas>, std::shared_ptr<Camera>, std::shared_ptr<EventHandler>, std::shared_ptr<LightSource>, std::shared_ptr<Player>);
	~Pong();

	void addTexture(SDL_Surface*);
	void addEntity(std::shared_ptr<SolidBody>);
	void addBall(std::shared_ptr<SolidBody>);
	void loadProjectile(unsigned int);
	void buildMesh();
	void destroyMesh();
	void updateAll();

};

