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


void Player::shoot(std::vector<std::shared_ptr<SolidBody>> Projectiles, unsigned int* polyCount, triangle3dV** mesh)
{
	double muzzleVelocity = 1.0f;
	for (unsigned int i = 0; i < Projectiles.size(); i++)
	{
		if (Projectiles[i]->isVisible() == false)
		{
			vect3 origin = { x, y, z, 1.0f };
			vect3 velocity = {	cos(-alt) * cos(-azm)	* muzzleVelocity,
								cos(-alt) * sin(-azm)	* muzzleVelocity,
								sin(-alt)				* muzzleVelocity, 0.0f };
			Projectiles[i]->setPosition(origin);
			Projectiles[i]->setVelocity(velocity);
			Projectiles[i]->setInMotion();
			Projectiles[i]->setVisibility(true);
			lastShot = 0;
			ammo--;
			transformMesh(polyCount[i], mesh[i], origin);
			break;
		}
	}
}
