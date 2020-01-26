#include "Explosion.h"



Explosion::Explosion(int r, float rad, float lt) : resol(r), radius(rad), lifeTime(lt)
{
	loadPoints();
}


Explosion::~Explosion()
{
}


void Explosion::reportPosition()
{
	std::cout << "Explosion x: " << position.x << std::endl;
	std::cout << "Explosion y: " << position.y << std::endl;
	std::cout << "Explosion z: " << position.z << std::endl;
}


void Explosion::loadPoints()
{
	srand(unsigned int(time(NULL)));

	for (int i = 0; i < resol; i++)
	{
		float angV = -90.0f + float(rand() % 180);
		float angH = float(rand() % 360);
		float rad = float(rand() % 100) / 100.0f * radius;
		point3 temp;
		temp.P.x = (rad * cosf(angV * PI / 180.0f)) * cos(angH * PI / 180.0f);
		temp.P.y = (rad * cosf(angV * PI / 180.0f)) * sin(angH * PI / 180.0f);
		temp.P.z = rad * sinf(angV * PI / 180.0f);
		temp.P.w = 1.0f;

		float bx = -0.5f + float(rand() % 10) / 10.0f;
		float by = -0.5f + float(rand() % 10) / 10.0f;
		float bz = -0.5f + float(rand() % 10) / 10.0f;
		vect3 bias = { bx, by, bz, 1.0f };

		float bs = (rand() % 10 > 7) ? float(rand() % 100) / 100.0f * 2.0 : float(rand() % 100) / 100.0f;

		temp.N = temp.P.norm().scale(bs) + bias.norm().scale(0.1 * bs);

		int c = rand() % 4;
		switch (c)
		{
		case 0:
			temp.colour = 0x000000ff;
			break;
		case 1:
			temp.colour = 0x007f7fff;
			break;
		case 2:
			temp.colour = 0x003f3fff;
			break;
		case 3:
			temp.colour = 0x00ffffff;
			break;
		}

		container.push_back(temp);
	}
}


void Explosion::activate(const vect3& e)
{
	active = true;
	updateCloudPosition(e);
}


void Explosion::deactivate()
{
	active = false;
	used = true;
	phase = 0.0f;
}


bool Explosion::isActive()
{
	return active;
}


bool Explosion::isUsed()
{
	return used;
}


void Explosion::updateCloudPosition(const vect3& p)
{
	position = p;
	for (auto& grain : container)
		grain.P += p;
}


void Explosion::update()
{
	phase += 1.0f;
	if (phase >= 1.0f && phase <= lifeTime)
	{
		for (auto& grain : container)
		{
			vect3 dir = (grain.P - position).norm();
			grain.P += grain.N.scale(1.0f / phase);
		}
	}
	if (phase > lifeTime)
		deactivate();
}


void Explosion::render(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen, mat4x4& RM)
{
	for (auto& grain : container)
		eye->renderPoint(grain, RM, screen->pixelBuffer, screen->depthBuffer);
}

