#pragma once

#include "Definitions.h"
#include "LightSource.h"
#include "ViewVolume.h"
#include "Canvas.h"

class Camera
{
public:
	double x;
	double y;
	double z;

	double step;

	double azm;
	double alt;
	double rol;

	vect3 speed;
	int	timeToSpendMidAir;

	double fovH;

	double zNear;
	double zFar;

	int w;
	int h;

	Camera();
	Camera(double, double, double, int, int);
	Camera(double, double, double, double, double, double, double, double, double, double, int, int);
	~Camera();

	double getFovH();
	double getFovV();

	double getHRatio();
	double getVRatio();

	bool polyFacingCamera(triangle3dV);
	triangle3dV world2viewT(transform3d, triangle3dV);
	int clipToFrustum(triangle3dV, vect3*, ViewVolume);
	void clipPoly(int* nVert, vect3* vList, plane clippingPlane);
	void clipEdge(plane p, vect3 startV, vect3 endV, int* nResult, vect3* temp);
	void illuminatePoly(LightSource*, triangle3dV*, triangle3dV, bool);
	coord2 view2screen(vect3 vertex, double hR, double vR);
	void projectPoly(int, vect3*, Uint32, Canvas, double, double);
	void fillTriangle(triangle2dG, Canvas);
	void fillTriangleZ(triangle2dG, Canvas);

	void updatePosition(double, double, double, double, double, double);

	void jump(int, triangle3dV*);
	void sufferFromGravity(int, triangle3dV*);
	double getSupport(int, triangle3dV*);

};
