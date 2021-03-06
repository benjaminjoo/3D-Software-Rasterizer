#pragma once

#include "Definitions.h"
#include "Camera.h"
#include "Canvas.h"

#include <memory>
#include <vector>


class DynamicMesh
{
private:

	point3* pGrid			= nullptr;
	triangle3dV* mesh		= nullptr;
	triangle3dV* sideMesh	= nullptr;

	int size				= 0;
	int width				= 0;
	int height				= 0;
	int polyCount			= 0;
	int polyCountS			= 0;

	float unit				= 1.0f;

	bool sidesOn			= false;
	float depth				= -50.0f;

	float tick					= 0.0f;
	float phase				= 0.0f;
	float frequency			= 1.0f;
	float speed				= 1.0f;

	float amplitude			= 10.0f;

	Uint32 colour			= 0xffffffff;

	void updateMesh();

public:
	DynamicMesh(int x, int y, float f, float s, float u, float a, Uint32, bool side);
	~DynamicMesh();

	void update();
	void renderGrid(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen);
	void renderMesh(std::shared_ptr<Camera> eye, LightSource Sun,
		const projectionStyle& visualStyle, float torchIntensity, float maxIllumination);
};

