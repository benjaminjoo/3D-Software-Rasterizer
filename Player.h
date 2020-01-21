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

	float						x				= 0.0f;
	float						y				= 0.0f;
	float						z				= 0.0f;

	float						azm				= 0.0f;
	float						alt				= 0.0f;
	float						rol				= 0.0f;

	float						runningSpeed	= 0.1f;
	float						turningSpeed	= 0.1f;

	bool						gravitating		= false;

	float						range			= 25.0f;	//50.0f;
	float						closeQuarters	= 10.0f;	//50.0f;
	float						safeDistance	= 25.0f;	//50.0f;

	unsigned int				health			= 100;
	unsigned int				ammo			= 100;

	unsigned int				lastShot		= 0;
	bool						isFiring		= false;
	bool						destroyed		= false;
	bool						underAttack		= false;

	unsigned int				lastHit			= 0;
	vect3						hitFrom			= { 0.0f, 0.0f, 0.0f, 1.0f };


	unsigned int				idlePhase		= 1;
	float						amplitude		= 0.5f;

	std::shared_ptr<SolidBody>	boundingVolume	= nullptr;
	float						bbRadius		= 0.25f;

	std::vector<std::shared_ptr<SolidBody>>		Parts;

	std::shared_ptr<Projection> Renderer		= nullptr;

public:

	Player();
	Player(float, float, float, float, float, float, float, int, int);
	~Player();

	bool isDestroyed();
	void destroy();
	bool isUnderAttack();
	void setUnderAttack(bool);
	void gotHitFrom(vect3);
	void moveOutOfHarmsWay();
	float getBBRadius();
	vect3 getPosition();
	float getRange();
	void takeDamage(unsigned int);
	unsigned int getHealth();

	void addPart(std::shared_ptr<SolidBody>);

private:
	
	void setAmmo(unsigned int);
	void shoot(std::vector<std::shared_ptr<SolidBody>>);

	unsigned int pickTarget(const std::vector<std::shared_ptr<SolidBody>>&);
	unsigned int pickTarget(const std::vector<std::shared_ptr<Player>>&, const unsigned int&);
	bool lockOnTarget(vect3);
	void keepDistanceFrom(vect3);

	void incrementIdlePhase();
	void idle();
};

