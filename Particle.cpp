#include "Particle.h"


Particle::Particle(const unsigned& life) : age(0), lifetime(life)
{
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
}


void Particle::update()
{
	if (active)
	{
		updatePosition();
		age++;
		if (age >= lifetime)
			reset();
	}
}


void Particle::updateVelocity()
{
	velocity += acceleration;
}


vect3 Particle::getPosition()
{
	return position;
}


double Particle::getAge()
{
	return static_cast<double>(age) / lifetime;
}
