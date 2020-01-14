#pragma once

#include "Definitions.h"
#include "Particle.h"
#include "Camera.h"
#include "Canvas.h"

#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>


class ParticleSystem
{
private:
	vect3 origin		= { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3 direction		= { 0.0f, 0.0f, 0.0f, 1.0f };

	double speed		= 0.0f;
	double dispersion	= 0.0f;
	unsigned num		= 0;
	unsigned density	= 0;

	bool active			= false;
	bool gravity		= false;

	colour32 colour;

	std::vector<Particle> particles;

	vect3 getRandomVelocity(double sp);

public:
	ParticleSystem(const double& sp, const unsigned& n, const unsigned& dens, const Uint32& c);
	ParticleSystem(const vect3& p, const vect3& dir, const double& sp, const unsigned& n, const unsigned& dens);
	~ParticleSystem();

	void loadParticles();
	void setOrigin(const vect3& pos);
	void setDirection(const vect3& dir);
	void setDispersion(const double& disp);
	void setGravity(bool g);
	void activate();
	void deactivate();
	void update();
	void render(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen);
};

