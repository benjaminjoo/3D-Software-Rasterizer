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


class Pong
{
private:

	std::shared_ptr<Canvas>						Screen			= nullptr;
	std::shared_ptr<Camera>						Eye				= nullptr;
	std::shared_ptr<Projection>					Renderer		= nullptr;
	std::shared_ptr<EventHandler>				Controls		= nullptr;
	std::shared_ptr<Text>						ControlsText	= nullptr;
	std::shared_ptr<LightSource>				Sun				= nullptr;
	std::shared_ptr<Player>						Hero			= nullptr;
	std::shared_ptr<Player>						Enemy			= nullptr;
	std::shared_ptr<ParticleSystem>				Emitter			= nullptr;

	std::vector<std::shared_ptr<SolidBody>>		Entities;
	std::vector<std::shared_ptr<SolidBody>>		Balls;
	std::vector<std::shared_ptr<Player>>		Enemies;
	std::vector<std::shared_ptr<SolidBody>>		Projectiles;

	std::unordered_map<std::string, std::shared_ptr<Text>>			TextScreens;

	std::shared_ptr<SolidBody>					Bar;

	vect3										vertexList[MAXCLIPVERTS];
	textCoord									uvList[MAXCLIPVERTS];

	double										hRatio;
	double										vRatio;

	triangle3dV**								triangleMesh		= nullptr;
	unsigned int*								polyCount			= nullptr;

	triangle3dV**								playerMesh			= nullptr;
	unsigned int*								playerPolyCount		= nullptr;

	triangle3dV**								enemyMesh			= nullptr;
	unsigned int*								enemyPolyCount		= nullptr;

	triangle3dV**								enemiesMesh			= nullptr;
	unsigned int*								enemiesPartCount	= nullptr;
	unsigned int**								enemiesPolyCount	= nullptr;
	unsigned int**								enemiesMeshIndices  = nullptr;

	triangle3dV**								skeletonMesh		= nullptr;
	unsigned int*								skeletonPolyCount	= nullptr;

	triangle3dV**								ballMesh			= nullptr;
	unsigned int*								ballPolyCount		= nullptr;

	triangle3dV**								explosionMesh		= nullptr;
	unsigned int*								explosionPolyCount	= nullptr;

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
	void updateHeroPosition();
	void updateEnemyPosition();
	void updateEnemyPositionAI(aiGoal);
	void updateEnemiesPositionsAI(aiGoal);
	void updatePlayerModel(std::shared_ptr<Player>);
	void updateEntities();
	void updateBalls();
	void updateProjectiles();

	bool hitTest(const std::shared_ptr<SolidBody>&, std::shared_ptr<Player>);
	bool hitTest(const std::shared_ptr<SolidBody>&, std::vector<std::shared_ptr<SolidBody>>);

	bool updateMovingObject(std::shared_ptr<SolidBody>, int, triangle3dV*);
	bool objectApproachingWall(vect3, vect3, triangle3dV);

	void explodeMesh(double, vect3, int, triangle3dV*);
	void explodeDebris(double, vect3, int, triangle3dV*);
	void renderAll();

	void updateFrameCounter();
	void resetPolyCounter();
	void incrementPolyCounter();
	void calculateFrametime();

	void displayStats(bool, bool, bool, bool, bool, std::shared_ptr<Canvas>);

public:

	Pong(std::shared_ptr<Canvas>, std::shared_ptr<Camera>, std::shared_ptr<Projection>, std::shared_ptr<EventHandler>,
		std::shared_ptr<LightSource>, std::shared_ptr<Player>, std::shared_ptr<Player>);
	~Pong();

	void addEntity(std::shared_ptr<SolidBody>);
	void addBall(std::shared_ptr<SolidBody>);
	void addEnemy(std::shared_ptr<Player>);
	void addTextScreen(std::string, std::shared_ptr<Text>);
	void addEmitter(std::shared_ptr<ParticleSystem>);
	void loadProjectile(unsigned int);
	void buildMesh();
	void destroyMesh();
	void updateAll();

};

