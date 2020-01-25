#include "RayTracer.h"



RayTracer::RayTracer(float cx, float cy, float cz, float az, float al, float rl, float stp, float trn,
	float fov, float nr, float fr, int width, int height) :
	x(cx), y(cy), z(cz), step(stp), turn(trn), azm(az), alt(al), rol(rl),
	fovH(fov), zNear(nr), zFar(fr), w(width), h(height)
{
	rays = new vect3[w * h];
	float aspectRatio = static_cast<float>(h) / static_cast<float>(w);
	float z_ = 1.0f;
	float w_ = 1.0f;
	float unit = (2.0f * tan(fovH * 0.5f)) / static_cast<float>(w);
	float xStart = -tan(fovH * 0.5f) + 0.5f * unit;
	float yStart = tan(fovH * 0.5f) * aspectRatio - 0.5f * unit;
	for(int j = 0; j < h; j++)
		for (int i = 0; i < w; i++)
		{
			float x_ = xStart + i * unit;
			float y_ = yStart - j * unit;
			vect3 d = { x_, y_, z_, w_ };
			rays[j * w + i] = d.norm();
		}
}


RayTracer::~RayTracer()
{
	delete[] rays;
}


void RayTracer::update(std::shared_ptr<EventHandler> controls)
{
	azm = -controls->turnH;
	alt = -controls->turnV;
	rol = controls->tiltP;

	x -= controls->moveP * (float)cos(azm) - controls->strafeP * (float)cos(azm + PI * 0.5);
	y += controls->moveP * (float)sin(azm) - controls->strafeP * (float)sin(azm + PI * 0.5);
	z += controls->riseP;

	frameCounter++;
	if (frameCounter == 30)
	{
		oldTime = newTime;
		newTime = clock();
		frameTime = newTime - oldTime;
		frameCounter = 0;
	}
}


void RayTracer::render(std::shared_ptr<Canvas> screen, std::shared_ptr<Scene> scene)
{
	vect3 origin = { x, y, z, 1.0f };	

	for (int j = 0; j < h; j++)
		for (int i = 0; i < w; i++)
		{
			Uint32 pixelColour = castRay(scene, origin, rays[j * w + i], 0);
			screen->putPixel(i, j, pixelColour);
		}
}


Uint32 RayTracer::castRay(std::shared_ptr<Scene> scene, vect3& ray_origin, vect3& ray_direction, unsigned n = 0)
{
	vect3 point, normal;
	matRT mat;

	if (n > 4 || !intersectScene(scene, ray_origin, ray_direction, point, normal, mat))
		return backgroundColour;

	vect3 reflectionDirection = ray_direction.reflect(normal).norm();
	vect3 reflectionOrigin = reflectionDirection * normal < 0.0f ?	point - normal.scale(0.0001f) :
																	point + normal.scale(0.0001f);
	colour32 reflectionColour;
	reflectionColour.argb = castRay(scene, reflectionOrigin, reflectionDirection, n + 1);

	vect3 refractionDirection = ray_direction.refract(normal, mat.refractIndex).norm();
	vect3 refractionOrigin = refractionDirection * normal < 0.0f ?	point - normal.scale(0.0001f) :
																	point + normal.scale(0.0001f);
	colour32 refractionColour;
	refractionColour.argb = castRay(scene, refractionOrigin, refractionDirection, n + 1);

	float diffuseIllumination = 0.0f;
	float specularIllumination = 0.0f;

	for (auto& L : scene->LightSources)
	{
		vect3 lightDirection = (L - point).norm();
		float lightDistance = (L - point).len();

		vect3 shadowOrigin = lightDirection * normal < 0.0f ?	point - normal.scale(0.0001f) :
																point + normal.scale(0.0001f);

		vect3 shadowPoint, shadowNormal;
		matRT tempMat;

		if (intersectScene(scene, shadowOrigin, lightDirection, shadowPoint, shadowNormal, tempMat) &&
			(shadowPoint - shadowOrigin).len() < lightDistance)
			continue;

		diffuseIllumination += std::max(0.0f, lightDirection * normal) * L.w;
		specularIllumination += pow(std::max(0.0f, lightDirection.reflect(normal).norm() * ray_direction), mat.spec_exp) * L.w;
	}

	colour32 materialColour;
	materialColour.argb = mat.diff_colour;
	
	float r = static_cast<float>(materialColour.c[2]) / 255.0f * diffuseIllumination * mat.albedo[0]
		+ specularIllumination * mat.albedo[1] + static_cast<float>(reflectionColour.c[2]) / 255.0f * mat.albedo[2]
		+ static_cast<float>(refractionColour.c[2]) / 255.0f * mat.albedo[3];

	float g = static_cast<float>(materialColour.c[1]) / 255.0f * diffuseIllumination * mat.albedo[0]
		+ specularIllumination * mat.albedo[1] + static_cast<float>(reflectionColour.c[1]) / 255.0f * mat.albedo[2]
		+ static_cast<float>(refractionColour.c[1]) / 255.0f * mat.albedo[3];

	float b = static_cast<float>(materialColour.c[0]) / 255.0f * diffuseIllumination * mat.albedo[0]
		+ specularIllumination * mat.albedo[1] + static_cast<float>(reflectionColour.c[0]) / 255.0f * mat.albedo[2]
		+ static_cast<float>(refractionColour.c[0]) / 255.0f * mat.albedo[3];

	return getColour(r, g, b);
}


bool RayTracer::intersectScene(std::shared_ptr<Scene> scene, vect3& origin, vect3& direction,
								vect3& intersectionPoint, vect3& surfaceNormal, matRT& mat)
{
	float sDist = zFar;
	for (auto& S : scene->Spheres)
	{
		float dist;
		if (S->intersect(origin, direction, dist) && dist < sDist)
		{
			sDist = dist;
			intersectionPoint = origin + direction.scale(dist);
			surfaceNormal = (intersectionPoint - S->position).norm();
			mat = S->material;
		}
	}
	return sDist < zFar;
}
