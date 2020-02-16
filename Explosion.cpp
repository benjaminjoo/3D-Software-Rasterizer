#include "Explosion.h"



Explosion::Explosion(int r, int s, float rad, float lt) : resol(r), particleSize(s), radius(rad), lifeTime(lt)
{
	srand(unsigned int(time(NULL)));
	loadPoints();
}


Explosion::Explosion(int r, int s, float rad, float lt, Uint32 c0, Uint32 c1, Uint32 c2, Uint32 c3)
	: resol(r), particleSize(s), radius(rad), lifeTime(lt), colour0(c0), colour1(c1), colour2(c2), colour3(c3)
{
	srand(unsigned int(time(NULL)));
	loadPoints();
}


Explosion::~Explosion()
{
}


void Explosion::initPoint(point3& p)
{
	float angV = -90.0f + float(rand() % 180);
	float angH = float(rand() % 360);
	float rad = float(rand() % 100) / 100.0f * radius;

	p.P.x = (rad * cosf(angV * PI / 180.0f)) * cos(angH * PI / 180.0f);
	p.P.y = (rad * cosf(angV * PI / 180.0f)) * sin(angH * PI / 180.0f);
	p.P.z = rad * sinf(angV * PI / 180.0f);
	p.P.w = 1.0f;

	float bx = -0.5f + float(rand() % 10) / 10.0f;
	float by = -0.5f + float(rand() % 10) / 10.0f;
	float bz = -0.5f + float(rand() % 10) / 10.0f;
	vect3 bias = { bx, by, bz, 1.0f };

	float bs = (rand() % 10 > 7) ? float(rand() % 100) / 100.0f * 2.0f : float(rand() % 100) / 100.0f;

	p.N = p.P.norm().scale(bs) + bias.norm().scale(0.1f * bs);

	int c = rand() % 4;
	switch (c)
	{
	case 0:
		p.colour = colour0;
		break;
	case 1:
		p.colour = colour1;
		break;
	case 2:
		p.colour = colour2;
		break;
	case 3:
		p.colour = colour3;
		break;
	}
}


void Explosion::loadPoints()
{
	for (int i = 0; i < resol; i++)
	{
		point3 temp;
		initPoint(temp);
		container.push_back(temp);
	}
}


void Explosion::resetPoints()
{
	for (auto& temp : container)
		initPoint(temp);
}


void Explosion::deactivate()
{
	active = false;
	used = true;
	phase = 0.0f;
}


void Explosion::reset()
{
	active = false;
	used = false;
	phase = 0.0f;
	position = { 0.0f, 0.0f, 0.0f, 1.0f };
	resetPoints();
}


void Explosion::updateCloudPosition(const vect3& p)
{
	position = p;
	for (auto& grain : container)
		grain.P += p;
}


bool Explosion::isActive()
{
	return active;
}


bool Explosion::isUsed()
{
	return used;
}


void Explosion::activate(const vect3& e)
{
	active = true;
	updateCloudPosition(e);
}


void Explosion::update(float v = 1.0f)
{
	phase += 1.0f;

	if (phase >= 1.0f && phase <= lifeTime)
		for (auto& grain : container)
			grain.P += grain.N.scale(v / phase);

	if (phase > lifeTime)
		reset();	
}


void Explosion::render(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen)
{
	for (auto& grain : container)
		eye->renderPoint(particleSize, grain, screen->pixelBuffer, screen->depthBuffer);
}

