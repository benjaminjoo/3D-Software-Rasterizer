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

	double unit				= 1.0f;

	bool sidesOn			= false;
	double depth			= -50.0f;

	double tick				= 0.0f;
	double phase			= 0.0f;
	double frequency		= 1.0f;
	double speed			= 1.0f;

	double amplitude		= 10.0f;

	Uint32 colour			= 0xffffffff;

	void updateMesh();

public:
	DynamicMesh(int x, int y, double f, double s, double u, double a, Uint32, bool side);
	~DynamicMesh();

	void update();
	void renderGrid(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen, mat4x4& RM);
	void renderMesh(std::shared_ptr<Camera> eye, mat4x4& rot, mat4x4& mov, LightSource Sun,
		const projectionStyle& visualStyle, double torchIntensity, double maxIllumination);
};

