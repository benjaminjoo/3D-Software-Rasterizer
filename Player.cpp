#include "Player.h"


Player::Player()
{
	boundingVolume = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xffffff00, 1, 0.5f, 24);
}


Player::Player(double px, double py, double pz, double rx, double ry, double rz, double rad, int hlt, int amm, std::shared_ptr<PelvisBone> sk):
	x(px), y(py), z(pz), azm(rx), alt(ry), rol(rz), bbRadius(rad), health(hlt), ammo(amm), skeleton(sk)
{
	boundingVolume = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, px, py, pz, rx, ry, rz, 0xffffff00, 1, 0.5f, 24);
}


Player::~Player()
{
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
	if (lastHit <= 30)
	{
		vect3 currentPos = this->getPosition();
		vect3 safety = unitVector(unitVector(hitFrom ^ vect3{ 0.0f, 0.0f, -1.0f, 1.0f }) + hitFrom);
		//vect3 safety = unitVector(hitFrom ^ currentPos + hitFrom * -1.0f);
		currentPos += safety * (2.0f * runningSpeed);
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


double Player::getBBRadius()
{
	return bbRadius;
}


vect3 Player::getPosition()
{
	return { x, y, z, 1.0f };
}


double Player::getRange()
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
	Parts.push_back(p);
}


void Player::incrementWalkPhase()
{
	currentPhase += walkingSpeed;
	if (currentPhase >= 1.0f)
		currentPhase = 0.0f;
}


void Player::updateSkeleton()
{
	incrementWalkPhase();
	std::cout << currentPhase * 100.0f << "%" << std::endl;
	if (currentPhase < 0.5f)
		this->updateLeftLimb();
	else
		this->updateRightLimb();

}


void Player::updateLeftLimb()
{
	if(skeleton != nullptr)
		skeleton->initiateUpdateLeft();
}


void Player::updateRightLimb()
{
	if(skeleton != nullptr)
		skeleton->initiateUpdateRight();
}


void Player::setAmmo(unsigned int a)
{
	ammo = a;
}


void Player::shoot(std::vector<std::shared_ptr<SolidBody>> Projectiles, unsigned int* polyCount, triangle3dV** mesh)
{
	double muzzleVelocity = 5.0f;
	for (unsigned int i = 0; i < Projectiles.size(); i++)
	{
		if (Projectiles[i]->isVisible() == false)
		{
			vect3 origin	= this->getPosition();
			vect3 rotation	= { cos(-alt) * cos(-azm), cos(-alt) * sin(-azm), sin(-alt), 0.0f };			
			vect3 velocity	= scaleVector(muzzleVelocity, rotation);

			rotateMesh(polyCount[i], mesh[i], -alt, rol, -(azm + PI * 0.5f));

			Projectiles[i]->setFired(true);
			Projectiles[i]->setPosition(origin);
			Projectiles[i]->setVelocity(velocity);			
			Projectiles[i]->setVisibility(true);
			Projectiles[i]->setMotion(true);
			Projectiles[i]->setGravity(true);
			Projectiles[i]->setTicksSinceFired(0);

			lastShot = 0;
			ammo--;

			break;
		}
	}
}


unsigned int Player::pickTarget(const std::vector<std::shared_ptr<SolidBody>>& targets)
{
	double minDist = range * range;
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
			double dist2Target = distanceSquared({ x, y, z, 1.0f }, targets[i]->getPosition());
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
	double minDist = range * range;
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
			double dist2Target = distanceSquared({ x, y, z, 1.0f }, targets[i]->getPosition());
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
	//vect3 currentRot = unitVector({ cos(-alt) * cos(-azm), cos(-alt) * sin(-azm), sin(-alt), 0.0f });
	vect3 target = unitVector(targetPos - currentPos);

	double dz = targetPos.z - z;
	double dist2 = sqrt((targetPos.x - x) * (targetPos.x - x) + (targetPos.y - y) * (targetPos.y - y));
	
	double targetAlt = -atan2(dz, dist2);
	double targetAzm = -atan2((targetPos.y - y), (targetPos.x - x));

	double deltaAzm = targetAzm - azm;
	double deltaAlt = targetAlt - alt;
	
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

	double dSquared = distanceSquared(currentPos, targetPos);

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

	double dSquared = distanceSquared(currentPos, targetPos);

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
	double zCurr = amplitude * sin(((2 * PI) / 120) * idlePhase);
	double zPrev = amplitude * sin(((2 * PI) / 120) * (idlePhase - 1));
	double deltaZ = zCurr - zPrev;
	z += deltaZ;
	incrementIdlePhase();
}



