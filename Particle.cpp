#include "Particle.h"


Particle::Particle(const float& m, const unsigned& life) : mass(m), age(0), lifetime(life)
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


void Particle::deactivate()
{
	active = false;
	position = { 0.0f, 0.0f, 0.0f, 1.0f };
	velocity = { 0.0f, 0.0f, 0.0f, 1.0f };
	acceleration = { 0.0f, 0.0f, 0.0f, 1.0f };
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
		//if (kEnergy <= 0.0f)
		//	reset();
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


void Particle::setPosition(const vect3& p)
{
	position = p;
}


void Particle::setVelocity(const vect3& v)
{
	velocity = v;
}


void Particle::setAge(const unsigned& a)
{
	age = a;
}


vect3 Particle::getPosition()
{
	return position;
}


vect3 Particle::getVelocity()
{
	return velocity;
}


float Particle::getKineticEnergy()
{
	return kEnergy;
}


float Particle::getAge()
{
	return static_cast<float>(age) / lifetime;
}
