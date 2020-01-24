#pragma once

#include "Definitions.h"
#include "SolidSphere.h"
#include <vector>


class Scene
{
	friend class RayTracer;

private:
	std::vector<std::shared_ptr<SolidSphere>>	Spheres;
	std::vector<triangle3dV>					Mesh;
	std::vector<vect3>							LightSources;

public:
	Scene();
	~Scene();

	void addSphere(std::shared_ptr<SolidSphere> sphere);
	void addMesh(int n, triangle3dV* mesh);
	void addLightSource(vect3 light_source);
};

