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
	vect3 world2view(const transform3d, const vect3);

	void clipToFrustumL(line3d*);
	void clipLine(plane, line3d*);
	bool insideFrustum(point3);
	bool assertPointVis(plane, point3);
	int clipToFrustum(const triangle3dV&, vect3*, textCoord*);
	void clipPoly(int* nVert, vect3* vList, textCoord* uvList, plane clippingPlane);
	void clipEdge(const plane& p, const vect3& startV, const vect3& endV, const textCoord& startUV, const textCoord& endUV,
		int* nResult, vect3* temp, textCoord* temp_uv);

	void illuminatePoly(LightSource, triangle3dV*, const triangle3dV, bool);
	void illuminatePoly(LightSource, triangle3dV*, const triangle3dV&, const projectionStyle&);

	coord2 view2screen(const vect3& vertex, const double& hR, const double& vR);
	vect3 screen2view(coord2 pixel, double hR, double vR);

	void projectPoly(int, vect3*, textCoord*, Uint32, Uint32*, double*, int, ShadowVolume*, double, double, projectionStyle, double, double, triangle3dV);
	void projectLine(line3d, Uint32*, double*, double hRatio, double vRatio);
	void projectPoint(point3, Uint32*, double hRatio, double vRatio);

	void fillTriangleSolidColour(const triangle3dV&, const triangle2dG&, Uint32*&, double*&, const bool&);
	void fillTriangleCheckerboard(const triangle3dV&, const triangle2dG&, Uint32*&, double*&, const bool&);
	void fillTriangleFlatShaded(const triangle2dG&, Uint32*&, double*&);
	void fillTriangleGouraudShaded(const triangle2dG&, Uint32*&, double*&, const bool&);
	void fillTriangleDepthVisualised(const triangle3dV&, const triangle2dG&, Uint32*&, double*&, const bool&);
	void fillTriangleSunlight(const triangle3dV&, const triangle2dG&, Uint32*&, double*&, const bool&);
	void fillTriangleTorchlight(const triangle3dV&, const triangle2dG&, Uint32*&, double*&, const bool&, const double&, const double&);
	void fillTriangleTorchlightSolidColour(const triangle3dV&, const triangle2dG&, Uint32*&, double*&, const bool&, const double&, const double&);
	void fillTriangleTest(const triangle3dV&, const triangle2dG&, Uint32*&, double*&, const bool&);
	void fillZBuffer(const triangle3dV&, const triangle2dG&, double*&);

	void drawLine(const coord2&, const coord2&, const Uint32&, Uint32*);
	void drawLine(const screenCoord&, const screenCoord&, const int&, const Uint32&, Uint32*);

	void drawSpot(screenCoord, Uint32, Uint32*);

	//textCoord getUVCoord(const vect3&, const vect3&, const textCoord&, const textCoord&, const vect3&);
	//textCoord getUVCoord(vect3, vect3, textCoord, textCoord, vect3);
	inline textCoord getUVCoord(const vect3& startV, const vect3& endV, const textCoord& startC, const textCoord& endC, const vect3& testV)
	{
		textCoord testC;

		double a = dotProduct(subVectors(testV, startV), subVectors(endV, startV));
		double b = dotProduct(subVectors(endV, startV), subVectors(endV, startV));
		double d = (b != 0.0) ? (a / b) : (0.0);

		testC.u = startC.u + d * (endC.u - startC.u);
		testC.v = startC.v + d * (endC.v - startC.v);

		return testC;
	}
	//textCoord interpolateUVCoord(textCoord, textCoord, int, int);
	inline textCoord interpolateUVCoord(const textCoord& startC, const textCoord& endC, const int& step, const int& currentP)
	{
		textCoord testC;

		testC.u = startC.u + (endC.u - startC.u) / step * currentP;
		testC.v = startC.v + (endC.v - startC.v) / step * currentP;

		return testC;
	}

	void outputImage(Canvas);

	void updatePosition(double, double, double, double, double, double);
	void centreLook();
};
