#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class Projectile
{

	vect3 position;
	vect3 velocity;

	SolidBody* shape;

public:

	Projectile();
	Projectile(vect3, vect3, SolidBody*);
	~Projectile();

	void setPosition(vect3);
	void setVelocity(vect3);

	vect3 getPosition();
	vect3 getVelocity();
	SolidBody* getShape();

	bool checkForCollision(int, triangle3dV*);
	void actOnCollision();

};

