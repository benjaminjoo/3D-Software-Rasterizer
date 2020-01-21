#pragma once

#include "Definitions.h"
#include "Particle.h"
#include "Camera.h"
#include "Canvas.h"

#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>


enum class colourBy
{
	age,
	energy,
	uniform
};


class ParticleSystem
{
private:
	vect3 origin						= { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3 direction						= { 0.0f, 0.0f, 0.0f, 1.0f };

	float particleMass					= 1.0f;

	float speed						= 0.0f;
	float dispersion					= 0.0f;
	unsigned num						= 0;
	unsigned density					= 0;

	bool active							= false;
	bool gravity						= false;

	colour32 colour;
	colourBy colourMethod				= colourBy::uniform;

	std::vector<Particle> particles;

	triangle3dV* terrainMesh			= nullptr;
	unsigned terrainPolyCount			= 0;

	vect3 getRandomVelocity(float sp);

public:
	ParticleSystem(const float& sp, const float& mass, const unsigned& n, const unsigned& dens, const Uint32& c);
	ParticleSystem(const vect3& p, const vect3& dir, const float& sp, const unsigned& n, const unsigned& dens);
	~ParticleSystem();

	void loadParticles();
	void importTerrain(unsigned nPoly, triangle3dV* mesh);
	void setOrigin(const vect3& pos);
	void setDirection(const vect3& dir);
	void setDispersion(const float& disp);
	void setGravity(bool g);
	void activate();
	void deactivate();
	void update();
	void handleCollisions();
	bool particleApproachingWall(vect3& p, vect3& v, triangle3dV& T);

	void render(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen, mat4x4& RM);
};

