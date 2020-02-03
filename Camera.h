#pragma once

#include "Definitions.h"
#include "LightSource.h"
#include "Lamp.h"
#include "ViewVolume.h"
#include "Canvas.h"
#include "Projection.h"

#include <time.h>
#include <memory>
#include <string>


class Camera
{
	friend class SolidBody;
	friend class Editor;
	friend class Game;

private:

	float					x;
	float					y;
	float					z;

	float					azm;
	float					alt;
	float					rol;

	vect3					viewDirection = { 0.0f, 0.0f, 0.0f, 1.0f };

	mat4x4					rotation = { 0.0f };
	mat4x4					translation = { 0.0f };
	mat4x4					transformation = { 0.0f };

	float					step;
	float					turn;

	float					fovH;

	float					zNear;
	float					zFar;

	int						w;
	int						h;

	float					hRatio;
	float					vRatio;

	Uint32*					pixelBuffer		= nullptr;
	float*					depthBuffer		= nullptr;

	std::shared_ptr<Canvas> Screen			= nullptr;
	std::shared_ptr<Projection> Renderer	= nullptr;

	vect3					vertexListV[MAXCLIPVERTS];
	vect3					vertexListW[MAXCLIPVERTS];
	textCoord				uvList[MAXCLIPVERTS];
	float					specularList[MAXCLIPVERTS];

	float					illSpec[3]		= { 0.0f };

	txt*					currentTexture = nullptr;
	std::vector<txt>		textureData;

	ViewVolume				Frustum;

	clock_t					oldTime			= 0;
	clock_t					newTime			= 0;
	clock_t					frameTime		= 0;
	int						frameCounter	= 0;

public:

	Camera();
	Camera(float, float, float, int, int, int);
	Camera(float, float, float, float, float, float, float, float, float, float, float, int, int, int);
	~Camera();

	void linkToCanvas(std::shared_ptr<Canvas> screen);

	void addTexture(SDL_Surface*);

	void addTexture(txt);

	void update();

	void renderPoint(point3, Uint32*, float*);

	void renderVisiblePoint(point3 p, Uint32*, float*);

	void renderMesh(const int& nPoly, triangle3dV* mesh, std::shared_ptr<Lamp> spotlight,
		LightSource Sun, const projectionStyle& visualStyle, float torchIntensity, float maxIllumination);

	void renderMesh(const int& nPoly, triangle3dV* mesh, vect3 mv, vect3 rt, std::shared_ptr<Lamp> spotlight,
		LightSource Sun, const projectionStyle& visualStyle, float torchIntensity, float maxIllumination);

private:

	float getFovH();

	float getFovV();

	float getHRatio();

	float getVRatio();

	void updateViewDirection();

	void updateViewVolume();

	bool pointFacingCamera(const point3& p);

	bool polyFacingCamera(const triangle3dV&);

	void clearVertexList();

	int clipToFrustum(const triangle3dV&, vect3*, textCoord*, float*);

	int clipToFrustumT(const triangle3dV&, vect3*, textCoord*, float*);

	void clipPoly(int* nVert, vect3* vList, textCoord* uvList, float* specList, plane clippingPlane);

	void clipEdge(const plane& p, const vect3& startV, const vect3& endV, const textCoord& startUV, const textCoord& endUV,
		const float& startSpec, const float& endSpec, int* nResult, vect3* temp, textCoord* temp_uv, float* temp_spec);

	bool pointInsideFrustum(vect3& V);

	bool pointBehindPlane(const plane& p, vect3& V);

	void centreLook();

	mat4x4 getTranslation(vect3 mv) const;

	mat4x4 getTranslation() const;

	mat4x4 getRotation(axis t, float a) const;

	mat4x4 getRotation(vect3 rt) const;

	mat4x4 getRotation() const;

	void object2world(const mat4x4& MR, const mat4x4& R, triangle3dV& T) const;

	void world2view(triangle3dV& T) const;

	void world2view(int n);

	void world2viewPointM(point3& P) const;

	void projectPoint(point3, Uint32*, float*);

	void renderPolygon(triangle3dV& viewT, std::shared_ptr<Lamp> spotlight, LightSource Sun,
		const projectionStyle& visualStyle, float torchIntensity, float maxIllumination);

	void renderPolygonShadows(triangle3dV& viewT, std::shared_ptr<Lamp> spotlight, LightSource Sun,
		const projectionStyle& visualStyle, float torchIntensity, float maxIllumination);

	void getSpecular (float* illSpec, LightSource Light, const triangle3dV& worldT, float shine);

	coord2 view2screen(const vect3& vertex, const float& hR, const float& vR);

	void projectPoly(int, Uint32, std::shared_ptr<Lamp> spotlight, projectionStyle, float, float, triangle3dV);
};
