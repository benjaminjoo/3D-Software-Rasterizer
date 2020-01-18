#pragma once

#include <vector>
#include <memory>
#include <vector>
#include <iostream>

#include "Definitions.h"
#include "Projection.h"
#include "SolidBody.h"
#include "SolidSphere.h"


class Player
{

	friend class Game;

private:

	double						x				= 0.0f;
	double						y				= 0.0f;
	double						z				= 0.0f;

	double						azm				= 0.0f;
	double						alt				= 0.0f;
	double						rol				= 0.0f;

	double						runningSpeed	= 0.1f;
	double						turningSpeed	= 0.1f;

	bool						gravitating		= false;

	double						range			= 50.0f;	//25.0f;
	double						closeQuarters	= 50.0f;	//10.0f;
	double						safeDistance	= 50.0f;	//25.0f;

	unsigned int				health			= 100;
	unsigned int				ammo			= 100;

	unsigned int				lastShot		= 0;
	bool						isFiring		= false;
	bool						destroyed		= false;
	bool						underAttack		= false;

	unsigned int				lastHit			= 0;
	vect3						hitFrom			= { 0.0f, 0.0f, 0.0f, 1.0f };


	unsigned int				idlePhase		= 1;
	double						amplitude		= 0.5f;

	std::shared_ptr<SolidBody>	boundingVolume	= nullptr;
	double						bbRadius		= 0.25f;

	std::vector<std::shared_ptr<SolidBody>>		Parts;

	std::shared_ptr<Projection> Renderer		= nullptr;

public:

	Player(std::shared_ptr<Projection>);
	Player(std::shared_ptr<Projection>, double, double, double, double, double, double, double, int, int);
	~Player();

	bool isDestroyed();
	void destroy();
	bool isUnderAttack();
	void setUnderAttack(bool);
	void gotHitFrom(vect3);
	void moveOutOfHarmsWay();
	double getBBRadius();
	vect3 getPosition();
	double getRange();
	void takeDamage(unsigned int);
	unsigned int getHealth();

	void addPart(std::shared_ptr<SolidBody>);

private:
	
	void setAmmo(unsigned int);
	void shoot(std::vector<std::shared_ptr<SolidBody>>, unsigned int*, triangle3dV**);

	unsigned int pickTarget(const std::vector<std::shared_ptr<SolidBody>>&);
	unsigned int pickTarget(const std::vector<std::shared_ptr<Player>>&, const unsigned int&);
	bool lockOnTarget(vect3);
	void keepDistanceFrom(vect3);

	void incrementIdlePhase();
	void idle();
};

