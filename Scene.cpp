#include "Scene.h"


Scene::Scene()
{
}


Scene::~Scene()
{
}


void Scene::addSphere(std::shared_ptr<SolidSphere> sphere)
{
	Spheres.push_back(sphere);
}


void Scene::addMesh(int n, triangle3dV* mesh)
{
	for (int i = 0; i < n; i++)
		Mesh.push_back(mesh[i]);
}


void Scene::addLightSource(vect3 light_source)
{
	LightSources.push_back(light_source);
}
