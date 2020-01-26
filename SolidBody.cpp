#include "SolidBody.h"

#include "Definitions.h"

SolidBody::SolidBody()
{
}


SolidBody::~SolidBody()
{
	if (mesh != nullptr)
		delete[] mesh;
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


vect3 SolidBody::getScale()
{
	return scale;
}


vect3 SolidBody::getPosition()
{
	return position;
}


vect3 SolidBody::getRotation()
{
	return rotation;
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


void SolidBody::updateVelocity(float drag)
{
	velocity = velocity * drag;
}


void SolidBody::updateAngularVelocity(vect3 av)
{
	angularVelocity.x += av.x;
	angularVelocity.y += av.y;
	angularVelocity.z += av.z;
	angularVelocity.w += av.w;
}


bool SolidBody::stoppedMoving()
{
	bool result = false;

	if (velocity.x < 0.01f && velocity.y < 0.01f && velocity.z < 0.01f)
		result = true;

	return result;
}


void SolidBody::setTexture(int t)
{
	texture = t;
}


void SolidBody::setMaterial(matRT m)
{
	material = m;
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
	vect3 temp = addVectors(rotation, angularVelocity);
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


float SolidBody::getBBRadius()
{
	return bbRadius;
}


void SolidBody::setBBRadius(float r)
{
	bbRadius = r;
}


bool SolidBody::isGravitating()
{
	return gravitating;
}


void SolidBody::setGravity(bool g)
{
	gravitating = g;
}


bool SolidBody::isFired()
{
	return fired;
}


void SolidBody::setFired(bool f)
{
	fired = f;
}


bool SolidBody::isInMotion()
{
	return inMotion;
}


void SolidBody::setMotion(bool m)
{
	inMotion = m;
}


bool SolidBody::isVisible()
{
	return visible;
}


void SolidBody::setVisibility(bool vis)
{
	visible = vis;
}


bool SolidBody::isBreakable()
{
	return breakable;
}


void SolidBody::setBreakability(bool b)
{
	breakable = b;
}


bool SolidBody::isDestroyed()
{
	return destroyed;
}


void SolidBody::destroy()
{
	destroyed = true;
}


bool SolidBody::isVanished()
{
	return vanished;
}


void SolidBody::vanish()
{
	vanished = true;
}


hit_response SolidBody::getBehaviour()
{
	return behaviour;
}


void SolidBody::setBehaviour(hit_response b)
{
	behaviour = b;
}


unsigned int SolidBody::getTicksSinceHit()
{
	return ticksSinceHit;
}


void SolidBody::incrementTicksSinceHit()
{
	ticksSinceHit++;
}


unsigned int SolidBody::getTicksSinceFired()
{
	return ticksSinceFired;
}


void SolidBody::setTicksSinceFired(unsigned int t)
{
	ticksSinceFired = t;
}


void SolidBody::incrementTicksSinceFired()
{
	ticksSinceFired++;
}


void SolidBody::updateColour(Uint32 c)
{
	colour = c;
}


void SolidBody::incrementBounceCount()
{
	nBounces++;
}


int SolidBody::getBounceCount()
{
	return nBounces;
}


void SolidBody::explode()
{

}


void SolidBody::updateMesh()
{
	Projector->transformMesh(nPoly, mesh, scale.x, scale.y, scale.z,
										position.x, position.y, position.z,
										rotation.x, rotation.y, rotation.z);
}


void SolidBody::render(std::shared_ptr<Camera> eye, bool trans, mat4x4& rot, mat4x4& mov,
				LightSource sun, const projectionStyle& style, float torch, float ill)
{
	if (mesh != nullptr)
	{
		if (trans)
			eye->renderMesh(getTotalPoly(), mesh, rot, mov, position, rotation, sun, style, torch, ill);
		else
			eye->renderMesh(getTotalPoly(), mesh, rot, mov, sun, style, torch, ill);
	}
}