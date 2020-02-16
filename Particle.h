#pragma once

#include "Definitions.h"


class Particle
{
private:

	vect3 position		= { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3 velocity		= { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3 acceleration	= { 0.0f, 0.0f, 0.0f, 1.0f };

	float mass			= 1.0f;
	float kEnergy		= 0.0f;

	unsigned age		= 0;
	unsigned lifetime	= 0;

	bool active			= false;

	void updatePosition();	

public:

	Particle(const float& m, const unsigned& life);
	~Particle();

	bool isActive();
	void activate(const vect3& pos, const vect3& vel, const vect3& acc);
	void deactivate();
	void reset();
	void update();
	void updateVelocity();
	void updateVelocity(vect3& v);
	void updateKineticEnergy();

	void setPosition(const vect3& p);
	void setVelocity(const vect3& v);
	void setAge(const unsigned& a);
	vect3 getPosition();
	vect3 getVelocity();
	float getAge();
	float getKineticEnergy();
	
};

