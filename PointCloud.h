#pragma once

#include "Definitions.h"
#include "Canvas.h"
#include "Camera.h"
#include "SolidBody.h"
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

class PointCloud : public SolidBody
{
private:
	std::string fileName = "";

	unsigned nPoints = 0;
	std::vector<point3> pointArray;

	unsigned nPoly = 0;
	std::vector<triangle3dV> triangleArray;

	bool smooth = false;

	Uint32 colour = 0x000000ff;

public:
	PointCloud(const std::string& fn, Uint32 col, bool sm);
	~PointCloud();

	virtual int	getTotalVert();
	virtual int	getTotalPoly();
	virtual void getTriangleData(triangle3dV*);
	void getPoints(std::vector<vertex3>&);

	void invertFaces();
	void smoothSurfaces();
	void renderCloud(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen, mat4x4& RM);
	void renderMesh(std::shared_ptr<Camera> eye, mat4x4& rot, mat4x4& mov, LightSource Sun,
		const projectionStyle& visualStyle, float torchIntensity, float maxIllumination);
};

