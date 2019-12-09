#include "Player.h"


Player::Player()
{
	boundingVolume = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xffffff00, 1, 0.5f, 24);
}


Player::Player(double px, double py, double pz, double rx, double ry, double rz, int hlt, int amm):
	x(px), y(py), z(pz), azm(rx), alt(ry), rol(rz), health(hlt), ammo(amm)
{
	boundingVolume = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, px, py, pz, rx, ry, rz, 0xffffff00, 1, 0.5f, 24);
}


Player::~Player()
{
}


void Player::addPart(std::shared_ptr<SolidBody> p)
{
	Parts.push_back(p);
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
			vect3 origin	= { x, y, z, 1.0f };
			vect3 rotation	= { cos(-alt) * cos(-azm), cos(-alt) * sin(-azm), sin(-alt), 0.0f };			
			vect3 velocity	= scaleVector(muzzleVelocity, rotation);

			rotateMesh(polyCount[i], mesh[i], -alt, rol, -(azm + PI * 0.5f));

			Projectiles[i]->setPosition(origin);
			Projectiles[i]->setVelocity(velocity);
			Projectiles[i]->setVisibility(true);
			Projectiles[i]->setMotion(true);

			lastShot = 0;
			ammo--;

			break;
		}
	}
}
