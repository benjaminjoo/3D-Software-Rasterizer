#include "Player.h"


Player::Player()
{
	Renderer = std::make_shared<Projection>();

	updateBB();
}


Player::Player(float px, float py, float pz, float rx, float ry, float rz,
	float rad, int hlt, int amm) :
	x(px), y(py), z(pz), azm(rx), alt(ry), rol(rz), bbRadius(rad), health(hlt), ammo(amm) 
{
	Renderer = std::make_shared<Projection>();

	updateBB();
}


Player::~Player()
{
}


void Player::setTrailColour(const Uint32& col)
{
	trailColour = col;
}


bool Player::isDestroyed()
{
	return destroyed;
}


void Player::destroy()
{
	destroyed = true;
}


bool Player::isUnderAttack()
{
	return underAttack;
}


void Player::setUnderAttack(bool a)
{
	underAttack = a;
}


void Player::gotHitFrom(vect3 hit)
{
	hitFrom += hit;
}


void Player::moveOutOfHarmsWay()
{
	lastHit++;
	if (lastHit <= 20)
	{
		vect3 currentPos = this->getPosition();
		vect3 toSafety = (hitFrom ^ vect3{ 0.0f, 0.0f, -1.0f, 1.0f }).norm() + hitFrom.norm();
		currentPos += toSafety * (3.0f * runningSpeed);
		x = currentPos.x;
		y = currentPos.y;
		z = currentPos.z;
	}
	else
	{
		lastHit = 0;
		underAttack = false;
		hitFrom = { 0.0f, 0.0f, 0.0f, 1.0f };
	}
}


float Player::getBBRadius()
{
	return bbRadius;
}


vect3 Player::getVelocity()
{
	return velocity;
}


vect3 Player::getPosition()
{
	return { x, y, z, 1.0f };
}


vect3 Player::getBBPosition(unsigned p)
{
	if (p < 8)
		return AABB[p];
	else
		return { x, y, z, 1.0f };
}


float Player::getRange()
{
	return range;
}


void Player::takeDamage(unsigned int damage)
{
	health -= damage;
	if (health < 1)
	{
		destroyed = true;
		isFiring = false;
	}		
}


unsigned int Player::getHealth()
{
	return health;
}


void Player::addPart(std::shared_ptr<SolidBody> p)
{
	p->updateMesh();
	Parts.push_back(p);
}


void Player::setAmmo(unsigned int a)
{
	ammo = a;
}


void Player::updateDirection(const float& turnH, const float& turnV, const float& tiltP)
{
	azm = -turnH;
	alt = -turnV;
	rol = tiltP;
}


void Player::updateVelocity(const float& move, const float& strafe, const float& rise)
{
	velocity = {	-move * cosf(azm) + strafe * cosf(azm + PI * 0.5),
					move * sinf(azm) - strafe * sinf(azm + PI * 0.5),
					rise,
					1.0f };
}


void Player::applyGravity(float g)
{
	velocity.z += g;
}


void Player::setVelocity(vect3 v)
{
	velocity = v;
}


void Player::updatePosition()
{
	x += velocity.x;
	y += velocity.y;
	z += velocity.z;
}


void Player::updatePosition(vect3 p)
{
	x = p.x;
	y = p.y;
	z = p.z;
}


void Player::updateBB()
{
	AABB[0] = { x - bbRadius , y - bbRadius, z - bbRadius, 1.0f };
	AABB[0] = { x - bbRadius , y + bbRadius, z - bbRadius, 1.0f };
	AABB[0] = { x + bbRadius , y - bbRadius, z - bbRadius, 1.0f };
	AABB[0] = { x + bbRadius , y - bbRadius, z - bbRadius, 1.0f };
	AABB[0] = { x - bbRadius , y - bbRadius, z + bbRadius, 1.0f };
	AABB[0] = { x - bbRadius , y + bbRadius, z + bbRadius, 1.0f };
	AABB[0] = { x + bbRadius , y + bbRadius, z + bbRadius, 1.0f };
	AABB[0] = { x + bbRadius , y - bbRadius, z + bbRadius, 1.0f };
}


void Player::shoot(std::vector<std::shared_ptr<SolidBody>> Projectiles)
{
	float muzzleVelocity = 5.0f;
	for (auto& p : Projectiles)
	{
		if (p->isVisible() == false)
		{
			vect3 origin = this->getPosition();
			vect3 rotation = { cos(-alt) * cos(-azm), cos(-alt) * sin(-azm), sin(-alt), 0.0f };
			vect3 velocity = rotation * muzzleVelocity;

			p->setRotation({	static_cast<float>(alt),
								static_cast<float>(rol),
								static_cast<float>(-(azm + PI * 0.5f)), 1.0f });
			p->setFired(true);
			p->setPosition(origin);
			p->setVelocity(velocity);
			p->setVisibility(true);
			p->setMotion(true);
			p->setGravity(true);
			p->setTicksSinceFired(0);			
			p->activateTrail(trailColour);

			lastShot = 0;
			ammo--;

			break;
		}
	}
}


unsigned int Player::pickTarget(const std::vector<std::shared_ptr<SolidBody>>& targets)
{
	float minDist = range * range;
	unsigned int targetIndex = targets.size();

	for (unsigned int i = 0; i < targets.size(); i++)
	{
		if (!targets[i]->isDestroyed())
		{
			minDist = distanceSquared({ x, y, z, 1.0f }, targets[i]->getPosition());
			break;
		}
	}
	for (unsigned int i = 0; i < targets.size(); i++)
	{
		if (!targets[i]->isDestroyed())
		{
			float dist2Target = distanceSquared({ x, y, z, 1.0f }, targets[i]->getPosition());
			if (dist2Target <= minDist)
			{
				minDist = dist2Target;
				targetIndex = i;
			}
		}
	}

	return targetIndex;
}


unsigned int Player::pickTarget(const std::vector<std::shared_ptr<Player>>& targets, const unsigned int& self)
{
	float minDist = range * range;
	unsigned int targetIndex = targets.size();

	for (unsigned int i = 0; i < targets.size(); i++)
	{
		if (!targets[i]->isDestroyed() && i != self)
		{
			minDist = distanceSquared({ x, y, z, 1.0f }, targets[i]->getPosition());
			break;
		}
	}
	for (unsigned int i = 0; i < targets.size(); i++)
	{
		if (!targets[i]->isDestroyed() && i != self)
		{
			float dist2Target = distanceSquared({ x, y, z, 1.0f }, targets[i]->getPosition());
			if (dist2Target <= minDist)
			{
				minDist = dist2Target;
				targetIndex = i;
			}
		}
	}

	return targetIndex;
}


bool Player::lockOnTarget(vect3 targetPos)
{
	bool result = false;

	vect3 currentPos = this->getPosition();
	vect3 target = unitVector(targetPos - currentPos);

	float dz = targetPos.z - z;
	float dist2 = sqrt((targetPos.x - x) * (targetPos.x - x) + (targetPos.y - y) * (targetPos.y - y));
	
	float targetAlt = -atan2(dz, dist2);
	float targetAzm = -atan2((targetPos.y - y), (targetPos.x - x));

	float deltaAzm = targetAzm - azm;
	float deltaAlt = targetAlt - alt;
	
	if (deltaAzm < 0.0f)
	{
		if (abs(deltaAzm) > turningSpeed)
			azm -= turningSpeed;
		else
			azm = targetAzm;
	}			
	if (deltaAzm > 0.0f)
	{
		if (abs(deltaAzm) > turningSpeed)
			azm += turningSpeed;
		else
			azm = targetAzm;
	}
	
	if (deltaAlt < 0.0f)
	{
		if (abs(deltaAlt) > turningSpeed)
			alt -= turningSpeed;
		else
			alt = targetAlt;
	}
	if (deltaAlt > 0.0f)
	{
		if (abs(deltaAlt) > turningSpeed)
			alt += turningSpeed;
		else
			alt = targetAlt;
	}

	float dSquared = distanceSquared(currentPos, targetPos);

	if (dSquared >= closeQuarters * closeQuarters)				//If target is too far away
	{
		currentPos += target * runningSpeed;

		x = currentPos.x;
		y = currentPos.y;
		z = currentPos.z;
	}

	if (dSquared < closeQuarters * closeQuarters * 0.9f)		//If target is too close
	{
		currentPos -= target * runningSpeed;
		x = currentPos.x;
		y = currentPos.y;
		z = currentPos.z;
	}

	if (dSquared <= range * range && azm == targetAzm && alt == targetAlt) //if (dSquared <= dReq * dReq * 10.0f && azm == targetAzm && alt == targetAlt)
		result = true;

	return result;
}


void Player::keepDistanceFrom(vect3 targetPos)
{
	vect3 currentPos = this->getPosition();
	vect3 target = unitVector(targetPos - currentPos);

	float dSquared = distanceSquared(currentPos, targetPos);

	if (dSquared < safeDistance * safeDistance * 0.9f)
	{
		currentPos -= target * runningSpeed;
		x = currentPos.x;
		y = currentPos.y;
		z = currentPos.z;
	}
}


void Player::incrementIdlePhase()
{
	idlePhase = idlePhase < 121 ? idlePhase + 1 : 1;
}


void Player::idle()
{
	float zCurr = amplitude * sin(((2 * PI) / 120) * idlePhase);
	float zPrev = amplitude * sin(((2 * PI) / 120) * (idlePhase - 1));
	float deltaZ = zCurr - zPrev;
	z += deltaZ;
	incrementIdlePhase();
}



