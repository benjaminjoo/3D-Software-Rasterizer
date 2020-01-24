/*Basic Raytracer loosely based on: github.com/ssloy/tinyraytracer/wiki		*/

#pragma once

#include "Camera.h"
#include "Canvas.h"
#include "Scene.h"
#include "EventHandler.h"
#include "Projection.h"
#include "SolidSphere.h"

class RayTracer: public Camera
{
public:
	float x						= 0.0f;
	float y						= 0.0f;
	float z						= 0.0f;
		  
	float azm					= 0.0f;
	float alt					= 0.0f;
	float rol					= 0.0f;

	clock_t	oldTime				= 0;
	clock_t	newTime				= 0;
	clock_t	frameTime			= 0;
	int	frameCounter			= 0;

private:		  
	Uint32 backgroundColour		= getColour(0.2f, 0.7f, 0.8f);

	float step					= 0.0f;
	float turn					= 0.0f;
		  
	float fovH					= PI * 0.5f;
		  
	float zNear					= 0.1f;
	float zFar					= 999.9f;
		  
	int	w						= 600;
	int	h						= 300;
		  
	vect3 viewDirection			= { 0.0f, 0.0f, 0.0f, 0.0f };

	vect3* rays					= nullptr;

	std::shared_ptr<Projection> Renderer = std::make_shared<Projection>();

public:
	RayTracer(	float cx, float cy, float cz,
				float az, float al, float rl,
				float stp, float trn,
				float fov, float near, float far,
				int width, int height);

	~RayTracer();

	void update(std::shared_ptr<EventHandler> controls);
	void render(std::shared_ptr<Canvas> screen, std::shared_ptr<Scene> scene);
	Uint32 castRay(std::shared_ptr<Scene> Scene, vect3& ray_origin, vect3& ray_direction, unsigned n);
	bool intersectScene(std::shared_ptr<Scene> scene, vect3& origin, vect3& direction, vect3& intersectionPoint, vect3& surfaceNormal, matRT& mat);

};

