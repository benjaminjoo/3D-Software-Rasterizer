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

	vect3 reflect(vect3 I, vect3 N)
	{
		return I - N.scale(2.0f * (I * N));
	}

	vect3 refract(vect3 I, vect3 N, const float eta_t, const float eta_i = 1.0f)
	{
		float cosI = -std::max(-1.0f, std::min(1.0f, I.norm() * N.norm()));
		if (cosI < 0.0f)
			return refract(I, N.scale(-1.0f), eta_i, eta_t);
		float eta = eta_i / eta_t;
		float k = 1.0f - eta * eta * (1.0f - cosI * cosI);
		if (k < 0.0f)
			return { 1.0f, 0.0f, 0.0f, 0.0f };
		else
		{
			return I.scale(eta) + N.scale(eta * cosI - sqrtf(k));
		}
	}

	void update(std::shared_ptr<EventHandler> controls);
	void render(std::shared_ptr<Canvas> screen, std::shared_ptr<Scene> scene);
	Uint32 castRay(std::shared_ptr<Scene> Scene, vect3& ray_origin, vect3& ray_direction, unsigned n);
	bool intersectScene(std::shared_ptr<Scene> scene, vect3& origin, vect3& direction, vect3& intersectionPoint, vect3& surfaceNormal, matRT& mat);
};

