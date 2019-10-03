#include "Projectile.h"



Projectile::Projectile()
{
	position	= { 0.0, 0.0, 0.0, 1.0 };
	velocity	= { 0.0, 0.0, 0.0, 1.0 };

	shape		= nullptr;
}


Projectile::Projectile(vect3 pos, vect3 vel, SolidBody* sh)
{
	position	= pos;
	velocity	= vel;

	shape		= sh;
}


Projectile::~Projectile()
{
}


void Projectile::setPosition(vect3)
{

}


void Projectile::setVelocity(vect3)
{

}


vect3 Projectile::getPosition()
{
	return position;
}


vect3 Projectile::getVelocity()
{
	return velocity;
}


SolidBody* Projectile::getShape()
{
	return shape;
}


bool Projectile::checkForCollision(int, triangle3dV*)
{
	return false;
}


void Projectile::actOnCollision()
{

}
