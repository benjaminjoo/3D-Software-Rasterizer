#pragma once

#include <vector>
#include <memory>

#include "Definitions.h"
#include "SolidBody.h"
#include "SolidSphere.h"

class Player
{

	friend class Pong;

private:

	double						x				= 0.0f;
	double						y				= 0.0f;
	double						z				= 0.0f;

	double						azm				= 0.0f;
	double						alt				= 0.0f;
	double						rol				= 0.0f;

	unsigned int				health			= 100;
	unsigned int				ammo			= 100;

	unsigned int				lastShot		= 0;

	std::shared_ptr<SolidBody>	boundingVolume	= nullptr;
	double						bbRadius		= 0.25f;

public:

	Player();
	Player(double, double, double, double, double, double, int, int);
	~Player();

private:

	void shoot(std::vector<std::shared_ptr<SolidBody>>, unsigned int*, triangle3dV**);
};

