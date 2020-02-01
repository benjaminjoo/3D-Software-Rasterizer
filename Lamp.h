#pragma once

#include "Definitions.h"
#include "ViewVolume.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

class Lamp
{
	friend class SolidBody;
	friend class Game;

private:
	float x				= 0.0f;
	float y				= 0.0f;
	float z				= 0.0f;

	float azm			= 0.0f;
	float alt			= 0.0f;
	float rol			= 0.0f;

	float fovH			= PI * 0.5f;

	float zNear			= 0.01f;
	float zFar			= 999.9f;

	float intensity		= 1.0f;

	int w				= 300;
	int h				= 150;

	mat4x4 M			= { 0.0f };
	mat4x4 R			= { 0.0f };
	mat4x4 RM			= { 0.0f };
	mat4x4 MR			= { 0.0f };

	bool isDynamic		= true;

	float hRatio;
	float vRatio;

	ViewVolume Frustum;

	vect3 vertexList[MAXCLIPVERTS];

	//float* depthBuffer	= nullptr;
	int* depthBuffer = nullptr;

public:

	Lamp(float cx, float cy, float cz, float az, float al, float rl,
				float fov, float nr, float fr, int width, int height);
	~Lamp();

	void printDepthBuffer();

	void update();
	bool pointLit(const vect3& V);
	bool pointInsideFrustum(const vect3& V);
	float getIllumination(vect3& P, vect3& N);
	float getBlinnSpecular(vect3& P, vect3& N, vect3& V, float& shine);

private:

	void resetDepthBuffer();
	float getFovH();
	float getFovV();
	float getHRatio();
	float getVRatio();
	mat4x4 getTranslation(vect3 mv);
	mat4x4 getTranslation();
	mat4x4 getRotation(axis t, float a);
	mat4x4 getRotation();
	void clearVertexList();
	int GetYMax3(coord2* p);
	int GetYMin3(coord2* p);
	bool polyFacingLamp(const triangle3dV&);
	void object2world(mat4x4& MR, mat4x4& R, triangle3dV& T);
	void world2view(triangle3dV& T);
	coord2 view2screen(const vect3& vertex);	
	bool pointBehindPlane(const plane& p, const vect3& V);
	int clipToFrustum(const triangle3dV&, vect3*);
	void clipPoly(int* nVert, vect3* vList, plane clippingPlane);
	void clipEdge(const plane& p, const vect3& startV, const vect3& endV, int* nResult, vect3* temp);
	void scanMesh(const int& nPoly, triangle3dV* mesh);
	void scanMesh(const int& nPoly, triangle3dV* mesh, vect3 mv, vect3 rt);
	void scanPolygon(triangle3dV& viewT);
	void projectPoly(int);
	void fillTriangleDepth(const triangle2dG& t);
};

