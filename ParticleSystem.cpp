#include "ParticleSystem.h"


ParticleSystem::ParticleSystem(const double& sp, const unsigned& n, const unsigned& dens, const Uint32& c) :
	speed(sp), num(n), density(dens)
{
	colour.argb = c;

	loadParticles();
	srand(unsigned(time(NULL)));
}


ParticleSystem::ParticleSystem(const vect3& pos, const vect3& dir, const double& sp, const unsigned& n, const unsigned& dens) :
	origin(pos), direction(dir), speed(sp), num(n), density(dens)
{
	colour.argb = 0xffffffff;

	loadParticles();
	srand(unsigned(time(NULL)));
}


ParticleSystem::~ParticleSystem()
{
}


vect3 ParticleSystem::getRandomVelocity(double sp)
{
	double azm = (double)(rand() % 360 / 180.0f * PI);
	double alt = (double)(rand() % 360 / 180.0f * PI);
	
	return { sp * cos(alt) * cos(azm), sp * cos(alt) * sin(azm), sp * sin(alt), 1.0f };
}


void ParticleSystem::loadParticles()
{
	for (unsigned i = 0; i < num; i++)
		particles.push_back(Particle(255));
}


void ParticleSystem::setOrigin(const vect3& pos)
{
	origin = pos;
}


void ParticleSystem::setDirection(const vect3& dir)
{
	direction = dir;
}


void ParticleSystem::setDispersion(const double& disp)
{
	dispersion = disp;
}


void ParticleSystem::setGravity(bool g)
{
	gravity = g;
}


void ParticleSystem::activate()
{
	active = true;
}


void ParticleSystem::deactivate()
{
	active = false;
}


void ParticleSystem::update()
{
	if (active)
	{
		vect3 velocity = direction.norm() * speed;
		unsigned nEmitted = 0;
		for (auto i = particles.begin(); i != particles.end(); ++i)
		{
			if (!i->isActive() && nEmitted <= density)
			{
				velocity += getRandomVelocity(speed * dispersion);
				i->activate(origin, velocity, { 0.0f, 0.0f, -0.001f, 1.0f });
				nEmitted++;
			}
			else
			{
				i->update();
				if (gravity)
					i->updateVelocity();
			}
		}
	}
}


void ParticleSystem::render(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen)
{
	for (auto i = particles.begin(); i != particles.end(); ++i)
		if (i->isActive())
		{
			double age = i->getAge();
			unsigned char r = static_cast<unsigned char>(static_cast<double>(colour.c[2]) * (1.0f - age));
			unsigned char g = static_cast<unsigned char>(static_cast<double>(colour.c[1]) * (1.0f - age));
			unsigned char b = static_cast<unsigned char>(static_cast<double>(colour.c[0]) * (1.0f - age));
			point3 p;
			p.P = i->getPosition();
			p.colour = getColour(0, r, g, b);
			eye->renderPoint(p, screen->pixelBuffer, screen->depthBuffer);
		}
}
