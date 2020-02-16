#pragma once

#include "Definitions.h"
#include "Camera.h"
#include "Canvas.h"

#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>


class Explosion
{
private:

	vect3 position		= { 0.0f, 0.0f, 0.0f, 1.0f };
	int resol			= 16;
	int particleSize	= 1;
	float radius		= 1.0f;
	float lifeTime		= 10.0f;
	float phase			= 0.0f;
	bool used			= false;
	bool active			= false;

	Uint32 colour0		= 0x000000ff;
	Uint32 colour1		= 0x007f7fff;
	Uint32 colour2		= 0x003f3fff;
	Uint32 colour3		= 0x00ffffff;

	std::vector<point3> container;

public:

	Explosion(int r, int s, float rad, float lt);
	Explosion(int r, int s, float rad, float lt,
				Uint32 c0,
				Uint32 c2,
				Uint32 c3,
				Uint32 c4);
	~Explosion();

private:

	void initPoint(point3& p);
	void loadPoints();
	void resetPoints();	
	void deactivate();
	void reset();
	void updateCloudPosition(const vect3& p);

public:
	
	bool isActive();
	bool isUsed();	
	void activate(const vect3& e);
	void update(float v);
	void render(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen);
};

