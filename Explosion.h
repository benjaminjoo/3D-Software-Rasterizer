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
	float radius		= 1.0f;
	float lifeTime		= 10.0f;
	float phase			= 0.0f;
	bool used			= false;
	bool active			= false;
	std::vector<point3> container;

public:
	Explosion(int r, float rad, float lt);
	~Explosion();

	void reportPosition();

	void loadPoints();
	void activate(const vect3& e);
	void deactivate();
	bool isActive();
	bool isUsed();
	void updateCloudPosition(const vect3& p);
	void update();
	void render(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen);
};

