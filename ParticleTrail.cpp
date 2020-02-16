#include "ParticleTrail.h"


ParticleTrail::ParticleTrail(int d, int s, float rad, float lt, Uint32 col)
	: density(d), particleSize(s), radius(rad), lifeTime(lt), colour(col)
{
	loadParticles();
}


ParticleTrail::~ParticleTrail()
{

}


void ParticleTrail::loadParticles()
{
	for (int i = 0; i < density * 100; i++)
	{
		Particle temp(0.1f, lifeTime);
		initParticle(temp);
		particles.push_back(temp);
	}
}


void ParticleTrail::resetParticles()
{
	for (auto& temp : particles)
		initParticle(temp);
}


void ParticleTrail::initParticle(Particle& t)
{
	t.reset();

	float dx = static_cast<float>(-100 + rand() % 200) / 100.0f * radius;
	float dy = static_cast<float>(-100 + rand() % 200) / 100.0f * radius;
	float dz = static_cast<float>(-100 + rand() % 200) / 100.0f * radius;

	vect3 tempPosition = { dx, dy, dz, 1.0f };

	t.setPosition(tempPosition);
}


void ParticleTrail::reset()
{
	active = false;
	live = false;
	used = false;
	phase = 0.0f;
	origin = { 0.0f, 0.0f, 0.0f, 1.0f };
	resetParticles();
}


bool ParticleTrail::isActive()
{
	return active;
}


bool ParticleTrail::isLive()
{
	return live;
}


bool ParticleTrail::isUsed()
{
	return used;
}


void ParticleTrail::activate(const vect3& p, const vect3& v, const Uint32& trlCol)
{
	active = true;
	live = true;
	origin = p;
	velocity = v;
	position = origin;
	colour = trlCol;
}


void ParticleTrail::deactivate()
{
	active = false;
	used = true;
	phase = 0.0f;
}


void ParticleTrail::update()
{
	if (live)
	{
		phase += 1.0f;

		if (active)
			position += velocity;

		if (phase < lifeTime)
		{
			unsigned nEmitted = 0;
			vect3 unitDisplacement = velocity.scale(1.0f / density);

			for (auto& p : particles)
			{
				if (!p.isActive() && nEmitted <= density)
				{
					vect3 currentPos = position + unitDisplacement.scale(static_cast<float>(nEmitted));
					vect3 bias = p.getPosition();
					currentPos += bias;

					vect3 currentVelocity = { 0.0f, 0.0f, 0.0f, 1.0f };
					vect3 acceleration = { 0.0f, 0.0f, 0.0f, 1.0f };
					
					p.activate(currentPos, currentVelocity, acceleration);
					nEmitted++;
				}
				else
					p.update();
			}
		}
		else
			reset();
	}
}


void ParticleTrail::render(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen)
{
	for (auto& i : particles)
		if (i.isActive())
		{
			point3 p;
			p.P = i.getPosition();

			float age = i.getAge();
			unsigned char r, g, b;
			
			colour32 clr;
			clr.argb = colour;
			
			r = static_cast<unsigned char>(static_cast<float>(clr.c[2])* (1.0f - age));
			g = static_cast<unsigned char>(static_cast<float>(clr.c[1])* (1.0f - age));
			b = static_cast<unsigned char>(static_cast<float>(clr.c[0])* (1.0f - age));
			
			p.colour = getColour(0, r, g, b);

			eye->renderPoint(1, p, screen->pixelBuffer, screen->depthBuffer);
		}
}

