#pragma once

#include "Definitions.h"
#include "LightSource.h"
#include "ViewVolume.h"
#include "Canvas.h"
#include "Projection.h"

#include <time.h>
#include <memory>


class Camera
{
	friend class SolidBody;
	friend class Editor;
	friend class Game;

private:

	double					x;
	double					y;
	double					z;

	double					azm;
	double					alt;
	double					rol;

	vect3					viewDirection = { 0.0f, 0.0f, 0.0f, 1.0f };

	double					step;
	double					turn;

	double					fovH;

	double					zNear;
	double					zFar;

	int						w;
	int						h;

	double					hRatio;
	double					vRatio;

	Uint32*					pixelBuffer		= nullptr;
	double*					depthBuffer		= nullptr;

	std::shared_ptr<Canvas> Screen			= nullptr;
	std::shared_ptr<Projection> Renderer	= nullptr;

	vect3					vertexList[MAXCLIPVERTS];
	textCoord				uvList[MAXCLIPVERTS];
	double					specularList[MAXCLIPVERTS];

	double					illSpec[3]		= { 0.0f };

	txt*					currentTexture = nullptr;
	std::vector<txt>		textureData;

	ViewVolume				Frustum;

	clock_t					oldTime			= 0;
	clock_t					newTime			= 0;
	clock_t					frameTime		= 0;
	int						frameCounter	= 0;

public:

	Camera();
	Camera(double, double, double, int, int, int);
	Camera(double, double, double, double, double, double, double, double, double, double, double, int, int, int);
	~Camera();

	void linkToCanvas(std::shared_ptr<Canvas> screen);

	void addTexture(SDL_Surface*);

	void addTexture(txt);

	void renderPoint(point3, mat4x4&, Uint32*, double*);

	void renderMesh(const int& nPoly, triangle3dV* mesh, mat4x4& rot, mat4x4& mov,
		LightSource Sun, const projectionStyle& visualStyle, double torchIntensity, double maxIllumination);

	void renderMesh(const int& nPoly, triangle3dV* mesh, mat4x4& rot, mat4x4& mov, vect3 mv, vect3 rt,
		LightSource Sun, const projectionStyle& visualStyle, double torchIntensity, double maxIllumination);

private:

	double getFovH();

	double getFovV();

	double getHRatio();

	double getVRatio();

	void updateViewDirection();

	bool polyFacingCamera(const triangle3dV&);

	void clearVertexList();

	int clipToFrustum(const triangle3dV&, vect3*, textCoord*, double*);

	void clipPoly(int* nVert, vect3* vList, textCoord* uvList, double* specList, plane clippingPlane);

	void clipEdge(const plane& p, const vect3& startV, const vect3& endV, const textCoord& startUV, const textCoord& endUV,
		const double& startSpec, const double& endSpec, int* nResult, vect3* temp, textCoord* temp_uv, double* temp_spec);

	bool pointInsideFrustum(vect3& V);

	bool pointBehindPlane(const plane& p, vect3& V);

	void centreLook();

	mat4x4 getTranslation(vect3 mv);

	mat4x4 getTranslation();

	mat4x4 getRotation(axis t, double a);

	mat4x4 getRotation();

	void object2world(mat4x4& MR, mat4x4& R, triangle3dV& T);

	void world2view(mat4x4& RM, mat4x4& R, triangle3dV& T);

	void world2viewPointM(point3& P, mat4x4& RM);

	void projectPoint(point3, Uint32*, double*);

	void renderPolygon(mat4x4& rot, mat4x4& mov, triangle3dV& viewT, LightSource Sun,
		const projectionStyle& visualStyle, double torchIntensity, double maxIllumination);

	void getSpec(double* illSpec, LightSource Light, const triangle3dV& worldT, double shine);

	coord2 view2screen(const vect3& vertex, const double& hR, const double& vR);

	void projectPoly(int, Uint32, projectionStyle, double, double, triangle3dV);

	void outputImage(Canvas);
};
