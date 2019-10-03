#include "SolidBody.h"

#include "Definitions.h"

SolidBody::SolidBody()
{
	scale				= { 1.0, 1.0, 1.0, 1.0 };
	position			= { 0.0, 0.0, 0.0, 1.0 };
	rotation			= { 0.0, 0.0, 0.0, 1.0 };

	velocity			= { 0.0, 0.0, 0.0, 1.0 };
	angularVelocity		= { 0.0, 0.0, 0.0, 1.0 };

	colour				= 255;
	texture				= 0;
	txU					= 1.0;

	castsShadows		= false;

	bBoxActive			= false;
	BB					= { (0.0, 0.0, 0.0, 1.0), (0.0, 0.0, 0.0, 1.0) };
}


SolidBody::~SolidBody()
{
}


bool SolidBody::assertShadowCasting()
{
	return castsShadows;
}


void SolidBody::setScale(vect3 sc)
{
	scale.x = sc.x;
	scale.y = sc.y;
	scale.z = sc.z;
	scale.w = sc.w;
}


void SolidBody::setPosition(vect3 p)
{
	position.x = p.x;
	position.y = p.y;
	position.z = p.z;
	position.w = p.w;
}


void SolidBody::setRotation(vect3 r)
{
	rotation.x = r.x;
	rotation.y = r.y;
	rotation.z = r.z;
	rotation.w = r.w;
}


void SolidBody::setVelocity(vect3 v)
{
	velocity.x = v.x;
	velocity.y = v.y;
	velocity.z = v.z;
	velocity.w = v.w;
}


void SolidBody::setAngularVelocity(vect3 av)
{
	angularVelocity.x = av.x;
	angularVelocity.y = av.y;
	angularVelocity.z = av.z;
	angularVelocity.w = av.w;
}


vect3 SolidBody::getPosition()
{
	return position;
}


vect3 SolidBody::getVelocity()
{
	return velocity;
}


vect3 SolidBody::getAngularVelocity()
{
	return angularVelocity;
}


void SolidBody::updatePosition()
{
	vect3 temp = addVectors(position, velocity);
	position = temp;
}


void SolidBody::updateRotation()
{
	vect3 temp = addVectors(position, angularVelocity);
	rotation = temp;
}


bool SolidBody::getBBState()
{
	return bBoxActive;
}


void SolidBody::activateBBox()
{
	bBoxActive = true;
	/*printf("BB minExt\tx: %4.4f\ty: %4.4f\tz: %4.4f\t\tmaxExt\tx: %4.4f\ty: %4.4f\tz: %4.4f\n",
			BB.minExt.x, BB.minExt.y, BB.minExt.z, BB.maxExt.x, BB.maxExt.y, BB.maxExt.z);*/
}


void SolidBody::updatebBBox(vect3 min_ext, vect3 max_ext)
{
	BB.minExt = min_ext;
	BB.maxExt = max_ext;
}


boundingBox SolidBody::getBB()
{
	return BB;
}