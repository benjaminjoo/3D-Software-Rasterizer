#include "ParticleSystem.h"


ParticleSystem::ParticleSystem(const float& sp, const float& mass, const unsigned& n, const unsigned& dens, const Uint32& c) :
	speed(sp), num(n), density(dens)
{
	colour.argb = c;

	loadParticles();
	srand(unsigned(time(NULL)));
}


ParticleSystem::ParticleSystem(const vect3& pos, const vect3& dir, const float& sp, const unsigned& n, const unsigned& dens) :
	origin(pos), direction(dir), speed(sp), num(n), density(dens)
{
	colour.argb = 0xffffffff;

	loadParticles();
	srand(unsigned(time(NULL)));
}


ParticleSystem::~ParticleSystem()
{
}


vect3 ParticleSystem::getRandomVelocity(float sp)
{
	float azm = (float)(rand() % 360 / 180.0f * PI);
	float alt = (float)(rand() % 360 / 180.0f * PI);
	
	return { sp * cos(alt) * cos(azm), sp * cos(alt) * sin(azm), sp * sin(alt), 1.0f };
}


void ParticleSystem::loadParticles()
{
	for (unsigned i = 0; i < num; i++)
		particles.push_back(Particle(1.0f, 500));
}


void ParticleSystem::importTerrain(unsigned nPoly, triangle3dV* mesh)
{
	terrainPolyCount = nPoly;
	std::cout << terrainPolyCount << std::endl;
	terrainMesh = new triangle3dV[nPoly];
	terrainMesh = mesh;
}


void ParticleSystem::setOrigin(const vect3& pos)
{
	origin = pos;
}


void ParticleSystem::setDirection(const vect3& dir)
{
	direction = dir;
}


void ParticleSystem::setDispersion(const float& disp)
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
		for (auto& i : particles)
		{
			if (!i.isActive() && nEmitted <= density)
			{
				velocity += getRandomVelocity(speed * dispersion);
				i.activate(origin, velocity, { 0.0f, 0.0f, -0.001f, 1.0f });
				nEmitted++;
			}
			else
			{
				i.update();
				if (i.getPosition().z < 0.0f)
				{
					vect3 currentVelocity = i.getVelocity();
					currentVelocity.z *= -1.5f;			
					currentVelocity += this->getRandomVelocity(currentVelocity.len() * 0.1f);
					i.updateVelocity(currentVelocity);
				}
				if (gravity)
					i.updateVelocity();
			}
		}
	}
}


void ParticleSystem::handleCollisions()
{
	for (auto& p : particles)
	{
		vect3 displacement = p.getVelocity();
		vect3 oldPos = p.getPosition();
		vect3 newPos = oldPos + displacement;
		
		if (terrainMesh != nullptr)
		{
			for (unsigned i = 0; i < terrainPolyCount; ++i)
			{
				triangle3dV tempWall = terrainMesh[i];
		
				if (particleApproachingWall(oldPos, displacement, tempWall))
				{
					std::cout << ".";
				}
			}
		}
	}
}


bool ParticleSystem::particleApproachingWall(vect3& p, vect3& v, triangle3dV& T)
{
	float v2n = v * T.N;								//Magnitude of velocity projected to plane normal
	float dist = distPoint2Plane(p, T);				//Distance from point to plane
	if (dist >= 0.0f && v2n < 0.0f)
	{
		float t = dist / -v2n;
		vect3 intersection{ p.x + v.x * t, p.y + v.y * t, p.z + v.z * t, 1.0f };

		float sA = ((T.A - T.C) ^ T.N) * (T.A - intersection);
		float sB = ((T.B - T.A) ^ T.N) * (T.B - intersection);
		float sC = ((T.C - T.B) ^ T.N) * (T.C - intersection);

		if ((sA <= 0.0f && sB <= 0.0f && sC <= 0.0f) ||
			(sA >= 0.0f && sB >= 0.0f && sC >= 0.0f))	//If point of intersection lies inside triangle T
		{
			return true;
		}
	}
	return false;
}


void ParticleSystem::render(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen)
{
	for (auto& i : particles)
		if (i.isActive())
		{
			float age = i.getAge();
			float energy = i.getKineticEnergy();

			unsigned char r, g, b;

			if (colourMethod == colourBy::age)
			{
				r = static_cast<unsigned char>(static_cast<float>(colour.c[2]) * (1.0f - age));
				g = static_cast<unsigned char>(static_cast<float>(colour.c[1]) * (1.0f - age));
				b = static_cast<unsigned char>(static_cast<float>(colour.c[0]) * (1.0f - age));
			}
			else if (colourMethod == colourBy::energy)
			{
				r = static_cast<unsigned char>(static_cast<float>(colour.c[2]) * energy * 100.0f);
				g = static_cast<unsigned char>(static_cast<float>(colour.c[1]) * energy * 100.0f);
				b = static_cast<unsigned char>(static_cast<float>(colour.c[0]) * energy * 100.0f);
			}
			else
			{
				r = colour.c[2];
				g = colour.c[1];
				b = colour.c[0];
			}

			point3 p;
			p.P = i.getPosition();
			p.colour = getColour(0, r, g, b);
			eye->renderPoint(1, p, screen->pixelBuffer, screen->depthBuffer);
		}
}
