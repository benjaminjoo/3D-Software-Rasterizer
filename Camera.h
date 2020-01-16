#pragma once

#include "Definitions.h"
#include "LightSource.h"
#include "ViewVolume.h"
#include "Canvas.h"
#include "ShadowVolume.h"

#include "Projection.h"

#include <time.h>
#include <memory>


class Camera
{
	friend class Renderer;
	friend class Editor;
	friend class Pong;

private:

	double					x;
	double					y;
	double					z;

	double					azm;
	double					alt;
	double					rol;

	double					step;
	double					turn;

	double					fovH;

	double					zNear;
	double					zFar;

	int						w;
	int						h;

	double					hRatio;
	double					vRatio;

	Uint32*					pixelBuffer = nullptr;
	double*					depthBuffer = nullptr;

	std::shared_ptr<Canvas> Screen			= nullptr;
	std::shared_ptr<Projection> Renderer	= nullptr;

	vect3					vertexList[MAXCLIPVERTS];
	textCoord				uvList[MAXCLIPVERTS];

	txt*					currentTexture = nullptr;
	std::vector<txt>		textureData;

	ViewVolume				Frustum;

	clock_t					oldTime			= 0;
	clock_t					newTime			= 0;
	clock_t					frameTime		= 0;
	int						frameCounter	= 0;

public:

	Camera(std::shared_ptr<Projection> R);
	Camera(std::shared_ptr<Projection> R, double, double, double, int, int, int);
	Camera(std::shared_ptr<Projection> R, double, double, double, double, double, double, double, double, double, double, double, int, int, int);
	~Camera();

	mat4x4 getTranslation();
	mat4x4 getRotation();
	mat4x4 getRotation(axis t, double a);
	void world2view(triangle3dV& T, mat4x4& rot, mat4x4& mov);

	void linkToCanvas(std::shared_ptr<Canvas> screen);
	void addTexture(SDL_Surface*);
	void addTexture(txt);
	void renderPoint(point3, Uint32*, double*);
	void renderPolygon(mat4x4& rot, mat4x4& mov, triangle3dV& viewT, LightSource Sun,
		const projectionStyle& visualStyle, double torchIntensity, double maxIllumination);
	void renderMesh(const int& nPoly, triangle3dV* mesh, mat4x4& rot, mat4x4& mov,
		LightSource Sun, const projectionStyle& visualStyle, double torchIntensity, double maxIllumination);
	void renderMesh(const int& nPoly, triangle3dV* mesh, mat4x4& rot, mat4x4& mov,
		vect3 sc, vect3 mv, vect3 rt, LightSource Sun, const projectionStyle& visualStyle, double torchIntensity, double maxIllumination);

private:

	double getFovH();
	double getFovV();

	double getHRatio();
	double getVRatio();

	void clearVertexList();

	bool polyFacingCamera(const triangle3dV&);
	void clipToFrustumL(line3d*);
	void clipLine(plane, line3d*);
	bool insideFrustum(point3);
	bool assertPointVis(plane, point3);
	int clipToFrustum(const triangle3dV&, vect3*, textCoord*);
	void clipPoly(int* nVert, vect3* vList, textCoord* uvList, plane clippingPlane);
	void clipEdge(const plane& p, const vect3& startV, const vect3& endV, const textCoord& startUV, const textCoord& endUV,
		int* nResult, vect3* temp, textCoord* temp_uv);

	coord2 view2screen(const vect3& vertex, const double& hR, const double& vR);

	void projectPoly(int, Uint32, projectionStyle, double, double, triangle3dV);
	void projectLine(line3d, Uint32*, double*, double hRatio, double vRatio);
	void projectPoint(point3, Uint32*, double*);

	void outputImage(Canvas);

	void updatePosition(double, double, double, double, double, double);
	vect3 getPosition();
	void centreLook();
};
