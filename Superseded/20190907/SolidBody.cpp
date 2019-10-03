#include "SolidBody.h"

#include "Definitions.h"

SolidBody::SolidBody()
{
	scaleX		= 1.0;
	scaleY		= 1.0;
	scaleZ		= 1.0;

	//posX		= 0.0;
	//posY		= 0.0;
	//posZ		= 0.0;

	rotX		= 0.0;
	rotY		= 0.0;
	rotZ		= 0.0;

	position	= { 0.0, 0.0, 0.0, 1.0 };
	velocity	= { 0.0, 0.0, 0.0, 1.0 };

	colour		= 255;
	texture		= 0;
	txU			= 1.0;


}


SolidBody::~SolidBody()
{
}


void SolidBody::setPosition(vect3 p)
{
	//position = p;
	position.x = p.x;
	position.y = p.y;
	position.z = p.z;
	position.w = p.w;
}


void SolidBody::setVelocity(vect3 v)
{
	//velocity = v;
	velocity.x = v.x;
	velocity.y = v.y;
	velocity.z = v.z;
	velocity.w = v.w;
}

vect3 SolidBody::getPosition()
{
	return position;
}


vect3 SolidBody::getVelocity()
{
	return velocity;
}


void SolidBody::updatePosition()
{
	vect3 temp = addVectors(position, velocity);
	position = temp;
}
