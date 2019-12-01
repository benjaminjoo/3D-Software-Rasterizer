#include "SolidBody.h"

#include "Definitions.h"

SolidBody::SolidBody()
{
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


void SolidBody::updateVelocity(vect3 v)
{
	velocity.x += v.x;
	velocity.y += v.y;
	velocity.z += v.z;
	velocity.w += v.w;
}


void SolidBody::updateAngularVelocity(vect3 av)
{
	angularVelocity.x += av.x;
	angularVelocity.y += av.y;
	angularVelocity.z += av.z;
	angularVelocity.w += av.w;
}


void SolidBody::setTexture(int t)
{
	texture = t;
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


void SolidBody::updatePosition(vect3 m)
{
	vect3 temp = addVectors(position, m);
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


double SolidBody::getBBRadius()
{
	return bbRadius;
}


bool SolidBody::isInMotion()
{
	return inMotion;
}


void SolidBody::setInMotion()
{
	inMotion = true;
}


void SolidBody::stop()
{
	inMotion = false;
	//position = { 0.0f, 0.0f, 0.0f, 1.0f };
}


bool SolidBody::isVisible()
{
	return visible;
}


void SolidBody::setVisibility(bool vis)
{
	visible = vis;
}