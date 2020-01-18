#pragma once


#include <vector>
#include <unordered_map>
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
#include "Text.h"
#include "ParticleSystem.h"
#include "DynamicMesh.h"


class Game
{
private:

	std::shared_ptr<Canvas>						Screen				= nullptr;
	std::shared_ptr<Camera>						Eye					= nullptr;
	std::shared_ptr<Projection>					Renderer			= nullptr;
	std::shared_ptr<EventHandler>				Controls			= nullptr;
	std::shared_ptr<LightSource>				Sun					= nullptr;
	std::shared_ptr<Player>						Hero				= nullptr;
	std::shared_ptr<Player>						Enemy				= nullptr;

	std::vector<std::shared_ptr<SolidBody>>		Entities;
	std::vector<std::shared_ptr<SolidBody>>		Balls;
	std::vector<std::shared_ptr<Player>>		Enemies;
	std::vector<std::shared_ptr<SolidBody>>		Projectiles;
	std::vector<std::shared_ptr<DynamicMesh>>	DynamicSurfaces;

	std::unordered_map<std::string, std::shared_ptr<Text>>			TextScreens;
	std::vector<std::shared_ptr<ParticleSystem>>					Emitters;

	std::shared_ptr<SolidBody>					Bar;

	vect3										vertexList[MAXCLIPVERTS];
	textCoord									uvList[MAXCLIPVERTS];

	std::vector<triangle3dV>					collisionPlanes;

	unsigned									ammo				= 0;
	clock_t										lastShot			= 0;
	clock_t										oldTime				= 0;
	clock_t										newTime				= 0;
	clock_t										frameTime			= 0;
	int											frameCounter		= 0;
	int											polyCounter			= 0;

	vect3										gravity				= { 0.0f, 0.0f, -0.01f, 0.0f };

	void updateCameraPosition(const std::shared_ptr<Player>&);
	void updateHeroPosition();
	void updateEnemyPosition();
	void updateEnemiesPositionsAI(aiGoal);
	void updatePlayerModel(std::shared_ptr<Player>);
	void updateEntities();
	void updateBalls();
	void updateProjectiles();

	bool hitTest(const std::shared_ptr<SolidBody>&, std::shared_ptr<Player>);
	bool hitTest(const std::shared_ptr<SolidBody>&, std::vector<std::shared_ptr<SolidBody>>);
	bool updateMovingObject(std::shared_ptr<SolidBody>);
	bool objectApproachingWall(vect3&, vect3&, triangle3dV&);

	void renderAll();

	void updateFrameCounter();
	void resetPolyCounter();
	void incrementPolyCounter();
	void calculateFrametime();
	void displayStats(bool, bool, bool, bool, bool, std::shared_ptr<Canvas>);

public:

	Game(std::shared_ptr<Canvas>, std::shared_ptr<Camera>, std::shared_ptr<EventHandler>,
		std::shared_ptr<LightSource>, std::shared_ptr<Player>, std::shared_ptr<Player>);
	~Game();

	void addEntity(std::shared_ptr<SolidBody>);
	void addBall(std::shared_ptr<SolidBody>);
	void addEnemy(std::shared_ptr<Player>);
	void addTextScreen(std::string, std::shared_ptr<Text>);
	void addEmitter(std::shared_ptr<ParticleSystem>);
	void addDynamicSurface(std::shared_ptr<DynamicMesh>);
	void loadProjectile(unsigned);
	void updateAll();

};
