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

struct surfacePoint
{
	surfacePoint(float v, float h, float rad, vect3 ct, Uint32 c)
		:
		angV(v),
		angH(h),
		colour(c),
		r(rad),
		centre(ct)
	{}

	float angV;
	float angH;

	float velocityV = 0.0f;
	float velocityH = 0.0f;

	float r;
	vect3 centre;

	Uint32 colour;

	void setVelocity(float v, float h)
	{
		velocityV = v;
		velocityH = h;
	}

	void update()
	{
		angV += velocityV;
		angH += velocityH;
	}

	point3 position()
	{
		float aV = angV * PI / 180.0f;
		float aH = -(angH + 180.0f) * PI / 180.0f;

		vect3 position =
		{
			r * cos(aV) * cos(aH),
			r * cos(aV) * sin(aH),
			r * sin(aV),
			1.0f
		};

		point3 temp;
		temp.colour = colour;
		temp.P = position;
		temp.N = (position - centre).norm();

		return temp;
	}
};

class PointCloud : public SolidBody
{
private:
	std::string fileName = "";

	unsigned nPoints = 0;
	std::vector<point3> pointArray;

	unsigned nPoly = 0;
	std::vector<triangle3dV> triangleArray;
	bool smooth = false;

	unsigned nEdge = 0;
	std::vector<line3d> edgeArray;

	unsigned nSurfPoints = 0;
	std::vector<surfacePoint> surfPointArray;

	std::vector<point3> gridArray;
	bool grid			= false;
	float gridVspacing	= 15.0f;
	float gridHspacing	= 15.0f;
	unsigned gridVresol = 720;
	unsigned gridHresol = 360;
	Uint32 gridColour	= 0x007f7fff;
	
	Uint32 colour = 0x000000ff;

	float radius = 10.0f;

	void constructGrid();

public:
	PointCloud(const std::string& fn, Uint32 col, bool sm);
	PointCloud(SDL_Surface* T, float rad, int res);
	PointCloud(const std::string& fn, float sx, float sy, float sz);
	~PointCloud();

	virtual int	getTotalVert();
	virtual int	getTotalPoly();
	virtual void getTriangleData(triangle3dV*);
	void getPoints(std::vector<vertex3>&);

	Uint32 getPixel(const std::shared_ptr<txt>& image, float x, float y);
	void invertFaces();
	void smoothSurfaces();
	void setRadius(float r);
	void setGrid();
	void setGrid(float vs, float hs, unsigned vres, unsigned hres, Uint32 c);
	void addSurfacePoint(float v, float h, float rad, vect3 ct, Uint32 c);
	void addSurfacePoint(float v, float h, float vv, float vh, float rad, vect3 ct, Uint32 c);
	void updateSurfacePoints();
	void renderCloud(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen);
	void renderMesh(std::shared_ptr<Camera> eye, LightSource Sun,
		const projectionStyle& visualStyle, float torchIntensity, float maxIllumination);
};

