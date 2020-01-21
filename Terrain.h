#pragma once

#include <SDLImage/SDL_image.h>
#include "Definitions.h"
#include "Camera.h"
#include "Canvas.h"

#include <string>
#include <memory>
#include <vector>
#include <iostream>


class Terrain
{
private:
	Uint32* pixelGrid		= nullptr;
	float* heightMap		= nullptr;
	point3* pGrid			= nullptr;
	triangle3dV* mesh		= nullptr;
	triangle3dV* sideMesh	= nullptr;

	std::string fileName	= "";
	txt texture;

	int sizeX				= 10;
	int sizeY				= 10;
	float centreX			= 0.0f;
	float centreY			= 0.0f;
	float unit				= 1.0f;
	float scale				= 1.0f;
	float depth				= -100.0f;

	Uint32 colour			= 0xffffffff;

	bool sidesOn			= false;

	int polyCount			= 0;
	int polyCountS			= 0;

	template <typename T>
	void wrap(T& value, const T& limit)
	{
		if (limit > 0.0f)
		{
			while (value < 0.0f)
				value += limit;
			while (value > limit)
				value -= limit;
		}
	}

public:
	Terrain(std::string fn, int sx, int sy, float cx, float cy, float u, float sc, float dp, Uint32, bool side);
	~Terrain();

	void addTexture(SDL_Surface* T);
	void getSample();
	void displaySample(std::shared_ptr<Canvas> screen, int s);
	void createPointCloud(float hg_scale);
	void createMesh();
	void renderGrid(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen, mat4x4& RM);
	void renderMesh(std::shared_ptr<Camera> eye, mat4x4& rot, mat4x4& mov, LightSource Sun,
		const projectionStyle& visualStyle, float torchIntensity, float maxIllumination);
};

