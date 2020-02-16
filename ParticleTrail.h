#pragma once

#include "Definitions.h"
#include "Camera.h"
#include "Canvas.h"
#include "Particle.h"

#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>


class ParticleTrail
{
private:

	vect3 origin		= { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3 position		= { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3 velocity		= { 0.0f, 0.0f, 0.0f, 1.0f };
	int density			= 20;
	int particleSize	= 1;
	float radius		= 1.0f;
	float lifeTime		= 60.0f;
	float phase			= 0.0f;
	bool used			= false;
	bool active			= false;		//Visible, actively emitting particles
	bool live			= false;		//Visible, may not be emitting particles
	Uint32 colour		= 0x0000ff00;

	std::vector<Particle> particles;

public:

	ParticleTrail(int d, int s, float rad, float lt, Uint32 col);
	~ParticleTrail();

private:

	void loadParticles();
	void resetParticles();
	void initParticle(Particle& t);

public:

	bool isActive();
	bool isLive();
	bool isUsed();
	void activate(const vect3& p, const vect3& v, const Uint32& trlCol);
	void deactivate();
	void reset();
	void update();
	void render(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen);
};

