#pragma once

#include "Definitions.h"
#include "LightSource.h"
#include "ViewVolume.h"
#include "Canvas.h"
#include "ShadowVolume.h"
#include <time.h>


class Camera
{
	friend class Renderer;
	friend class Editor;
	friend class Pong;

private:

	double			x;
	double			y;
	double			z;

	double			azm;
	double			alt;
	double			rol;

	double			step;
	double			turn;

	double			fovH;

	double			zNear;
	double			zFar;

	int				w;
	int				h;

	txt				currentTexture	= { 0, 0, 0, nullptr };;

	ViewVolume		Frustum;

	clock_t			oldTime			= 0;
	clock_t			newTime			= 0;
	clock_t			frameTime		= 0;
	int				frameCounter	= 0;

public:

	Camera();
	Camera(double, double, double, int, int, int);
	Camera(double, double, double, double, double, double, double, double, double, double, double, int, int, int);
	~Camera();

private:

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


	inline textCoord interpolateUVCoord(const textCoord& startC, const textCoord& endC, const int& step, const int& currentP)
	{
		textCoord testC;

		testC.u = startC.u + (endC.u - startC.u) / step * currentP;
		testC.v = startC.v + (endC.v - startC.v) / step * currentP;

		return testC;
	}


	inline Uint32 modifyColour(const Uint32& inputColour, const double& illumination)
	{
		bool red = true, green = true, blue = true;
		byte a = 0, r, g, b, r2fill = 0, g2fill = 0, b2fill = 0;
		double illSurplus = 0.0;

		if (illumination <= 1.0)
		{
			r = red ? (byte)(double(inputColour >> 16 & 255) * illumination) : 0;
			g = green ? (byte)(double(inputColour >> 8 & 255) * illumination) : 0;
			b = blue ? (byte)(double(inputColour & 255) * illumination) : 0;
		}
		else if (illumination > 1.0)
		{
			illSurplus = illumination - 1.0;

			r = red ? (byte)(double(inputColour >> 16 & 255) * illumination) : 0;
			g = green ? (byte)(double(inputColour >> 8 & 255) * illumination) : 0;
			b = blue ? (byte)(double(inputColour & 255) * illumination) : 0;

			r2fill = 255 - r;	g2fill = 255 - g;	b2fill = 255 - b;
			r += (byte)((double(r2fill) / (MAX_ILLUMINATION - 1.0)) * illSurplus);
			g += (byte)((double(g2fill) / (MAX_ILLUMINATION - 1.0)) * illSurplus);
			b += (byte)((double(b2fill) / (MAX_ILLUMINATION - 1.0)) * illSurplus);
		}

		return (a << 24) | (r << 16) | (g << 8) | (b << 0);
	}

	void outputImage(Canvas);

	void updatePosition(double, double, double, double, double, double);
	vect3 getPosition();
	void centreLook();
};
