#pragma once

#include "Definitions.h"
#include "LightSource.h"
#include "SpotLight.h"
#include "ViewVolume.h"
#include "Canvas.h"

class Camera
{
public:
	double x;
	double y;
	double z;

	double step;
	double turn;

	double azm;
	double alt;
	double rol;

	vect3 speed;

	double fovH;

	double zNear;
	double zFar;

	double torchIntensity;

	int w;
	int h;
	int margin;
	int scale;
	int grainSize;

	txt currentTexture;

	Camera();
	Camera(double, double, double, int, int, int);
	Camera(double, double, double, double, double, double, double, double, double, double, double, int, int, int);
	~Camera();

	double getFovH();
	double getFovV();

	double getHRatio();
	double getVRatio();

	void scaleUp();
	void scaleDown();
	void updateSize();

	bool polyFacingCamera(triangle3dV);
	triangle3dV world2viewT(transform3d, triangle3dV);
	vect3 world2view(transform3d, vect3);
	int clipToFrustum(triangle3dV, vect3*, textCoord*, ViewVolume);
	void clipPoly(int* nVert, vect3* vList, textCoord* uvList, plane clippingPlane);
	void clipEdge(plane p, vect3 startV, vect3 endV, textCoord startUV, textCoord endUV, int* nResult, vect3* temp, textCoord* temp_uv);
	void illuminatePoly(LightSource*, triangle3dV*, triangle3dV, bool);
	coord2 view2screen(vect3 vertex, double hR, double vR);
	vect3 screen2view(coord2 pixel, double hR, double vR);
	void view2text();
	void projectPoly(int, vect3*, textCoord*, Uint32, Canvas, double, double, bool, bool, int, vect3*, double*, triangle3dV);
	void fillTriangleTSL(triangle3dV, triangle2dG, Canvas, bool, int, vect3*, double*);
	void fillTriangleT(triangle3dV, triangle2dG, Canvas, bool);
	void fillTriangleTTorch(triangle3dV, triangle2dG, Canvas, bool);
	void fillTriangleTSolidColour(triangle3dV, triangle2dG, Canvas, bool);
	void fillTriangleTSimple(triangle3dV, triangle2dG, Canvas, bool);
	void fillTriangleS(triangle2dG, Canvas, bool);
	void fillTriangleD(triangle3dV, triangle2dG, Canvas, bool);
	void fillTriangleZ(triangle2dG, Canvas, bool);
	textCoord getUVCoord(vect3, vect3, textCoord, textCoord, vect3);
	textCoord interpolateUVCoord(textCoord, textCoord, int, int);

	void outputImage(Canvas);

	void updatePosition(double, double, double, double, double, double);
	void centreLook();

	void jump(int, triangle3dV*);
	void sufferFromGravity(int, triangle3dV*);
	double getSupport(int, triangle3dV*);

};
