#include "Particle.h"


Particle::Particle(const double& m, const unsigned& life) : mass(m), age(0), lifetime(life)
{
	updateKineticEnergy();
}


Particle::~Particle()
{
}


void Particle::updatePosition()
{
	position += velocity;
}


void Particle::reset()
{
	age = 0;
	active = false;
	position = { 0.0f, 0.0f, 0.0f, 1.0f };
	velocity = { 0.0f, 0.0f, 0.0f, 1.0f };
	acceleration = { 0.0f, 0.0f, 0.0f, 1.0f };
	kEnergy = 0.0f;
}


bool Particle::isActive()
{
	return active;
}


void Particle::activate(const vect3& pos, const vect3& vel, const vect3& acc)
{
	active = true;
	position = pos;
	velocity = vel;
	acceleration = acc;
	updateKineticEnergy();
}


void Particle::update()
{
	if (active)
	{
		updatePosition();
		age++;
		if (age >= lifetime)
			reset();
		if (kEnergy <= 0.0f)
			reset();
	}
}


void Particle::updateVelocity()
{
	velocity += acceleration;
	updateKineticEnergy();
}


void Particle::updateVelocity(vect3& v)
{
	velocity += v;
	updateKineticEnergy();
}


void Particle::updateKineticEnergy()
{
	kEnergy = 0.5f * mass * velocity.len2();
}


vect3 Particle::getPosition()
{
	return position;
}


vect3 Particle::getVelocity()
{
	return velocity;
}


double Particle::getKineticEnergy()
{
	return kEnergy;
}


double Particle::getAge()
{
	return static_cast<double>(age) / lifetime;
}
