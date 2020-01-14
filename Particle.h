#pragma once

#include "Definitions.h"


class Particle
{
private:
	vect3 position		= { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3 velocity		= { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3 acceleration	= { 0.0f, 0.0f, 0.0f, 1.0f };

	unsigned age		= 0;
	unsigned lifetime	= 0;

	bool active			= false;

	void updatePosition();
	void reset();

public:
	Particle(const unsigned& life);
	~Particle();

	bool isActive();
	void activate(const vect3& pos, const vect3& vel, const vect3& acc);
	void update();
	void updateVelocity();

	vect3 getPosition();
	double getAge();
};

