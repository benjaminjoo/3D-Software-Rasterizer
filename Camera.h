#pragma once

#include "Definitions.h"
#include "LightSource.h"
#include "ViewVolume.h"
#include "Canvas.h"
#include "ShadowVolume.h"
#include <time.h>

class Camera
{
public:
	double			x;
	double			y;
	double			z;

	double			step;
	double			turn;

	double			azm;
	double			alt;
	double			rol;

	vect3			speed;

	double			fovH;

	double			zNear;
	double			zFar;

	int				w;
	int				h;
	int				margin;
	int				scale;
	int				grainSize;

	txt				currentTexture;

	ViewVolume		Frustum;

	clock_t			oldTime;
	clock_t			newTime;
	clock_t			frameTime;
	int				frameCounter;

	Camera();
	Camera(double, double, double, int, int, int);
	Camera(double, double, double, double, double, double, double, double, double, double, double, int, int, int);
	~Camera();

	double getFovH();
	double getFovV();

	double getHRatio();
	double getVRatio();

	transform3d getTransformation();

	bool polyFacingCamera(triangle3dV);
	triangle3dV world2viewT(transform3d, triangle3dV);
	line3d world2viewL(transform3d, line3d);
	point3 world2viewP(transform3d, point3);
	vect3 world2view(transform3d, vect3);

	void clipToFrustumL(line3d*);
	void clipLine(plane, line3d*);
	bool insideFrustum(point3);
	bool assertPointVis(plane, point3);
	int clipToFrustum(triangle3dV, vect3*, textCoord*);
	void clipPoly(int* nVert, vect3* vList, textCoord* uvList, plane clippingPlane);
	void clipEdge(plane p, vect3 startV, vect3 endV, textCoord startUV, textCoord endUV, int* nResult, vect3* temp, textCoord* temp_uv);

	void illuminatePoly(LightSource, triangle3dV*, triangle3dV, bool);
	void illuminatePoly(LightSource, triangle3dV*, triangle3dV, projectionStyle);

	coord2 view2screen(vect3 vertex, double hR, double vR);
	vect3 screen2view(coord2 pixel, double hR, double vR);

	void projectPoly(int, vect3*, textCoord*, Uint32, Uint32*, double*, int, ShadowVolume*, double, double, projectionStyle, double, double, triangle3dV);
	void projectLine(line3d, Uint32*, double*, double hRatio, double vRatio);
	void projectPoint(point3, Uint32*, double hRatio, double vRatio);

	void fillTriangleSolidColour(triangle3dV, triangle2dG, Uint32*, double*, bool);
	void fillTriangleCheckerboard(triangle3dV, triangle2dG, Uint32*, double*, bool);
	void fillTriangleFlatShaded(triangle2dG, Uint32*, double*);
	void fillTriangleGouraudShaded(triangle2dG, Uint32*, double*, bool);
	void fillTriangleDepthVisualised(triangle3dV, triangle2dG, Uint32*, double*, bool);
	void fillTriangleSunlight(triangle3dV, triangle2dG, Uint32*, double*, bool);
	void fillTriangleTorchlight(triangle3dV, triangle2dG, Uint32*, double*, bool, double, double);
	void fillTriangleTorchlightSolidColour(triangle3dV, triangle2dG, Uint32*, double*, bool, double, double);
	void fillTriangleTest(triangle3dV, triangle2dG, Uint32*, double*, bool);
	void fillZBuffer(triangle3dV, triangle2dG, double*);

	void drawLine(coord2, coord2, Uint32, Uint32*);
	void drawLine(screenCoord, screenCoord, int, Uint32, Uint32*);

	void drawSpot(screenCoord, Uint32, Uint32*);

	textCoord getUVCoord(vect3, vect3, textCoord, textCoord, vect3);
	textCoord interpolateUVCoord(textCoord, textCoord, int, int);

	void outputImage(Canvas);

	void updatePosition(double, double, double, double, double, double);
	void centreLook();
};
