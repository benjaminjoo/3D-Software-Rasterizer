#define _CRT_SECURE_NO_DEPRECATE

#include "Definitions.h"
#include "Camera.h"
#include "LightSource.h"
#include "ViewVolume.h"
#include "Canvas.h"
#include "ShadowVolume.h"
#include <math.h>
#include <stdio.h>
#include <time.h>

#define PI					3.141592654

Camera::Camera()
{
	x					= 0.0;
	y					= 0.0;
	z					= 0.0;

	step				= 1.0;
	turn				= 0.1;

	azm					= 0.0;
	alt					= 0.0;
	rol					= 0.0;

	speed				= { 0.01, 0.01, 0.01, 0.0 };

	fovH				= PI * 0.5 / 90 * 85;
	zNear				= 1.0;
	zFar				= 99.9;

	w					= 320;
	h					= 200;
	margin				= 0;
	scale				= 0;
	grainSize			= 1;

	currentTexture		= { 0, 0, 0, nullptr };

	Frustum.initFrustum(this->getFovH(), this->getFovV(), zNear, zFar);

	oldTime				= 0;
	newTime				= 0;
	frameTime			= 0;
	frameCounter		= 0;
}


Camera::Camera(double cx, double cy, double cz, int width, int height, int s)
{
	x					= cx;
	y					= cy;
	z					= cz;

	step				= 1.0;
	turn				= 0.1;

	azm					= 0.0;
	alt					= 0.0;
	rol					= 0.0;

	speed				= { 0.01, 0.01, 0.01, 0.0 };

	fovH				= 90.0;
	zNear				= 1.0;
	zFar				= 999.0;

	w					= width;
	h					= height;
	margin				= 0;
	scale				= s;
	grainSize			= 1;

	currentTexture		= { 0, 0, 0, nullptr };

	Frustum.initFrustum(this->getFovH(), this->getFovV(), zNear, zFar);	//

	oldTime				= 0;
	newTime				= 0;
	frameTime			= 0;
	frameCounter		= 0;
}


Camera::Camera(double cx, double cy, double cz, double az, double al, double rl, double stp, double trn,
	double fov, double nr, double fr, int width, int height, int s)
{
	x					= cx;
	y					= cy;
	z					= cz;

	step				= stp;
	turn				= trn;

	azm					= az;
	alt					= al;
	rol					= rl;

	speed				= { 0.01, 0.01, 0.01, 0.01 };

	fovH				= fov;

	zNear				= nr;
	zFar				= fr;

	w					= width;
	h					= height;
	margin				= 0;
	scale				= s;
	grainSize			= 1;

	currentTexture		= { 0, 0, 0, nullptr };

	Frustum.initFrustum(this->getFovH(), this->getFovV(), zNear, zFar);	//

	oldTime				= 0;
	newTime				= 0;
	frameTime			= 0;
	frameCounter		= 0;
}


Camera::~Camera()
{
}


double Camera::getFovH()
{
	return fovH;
}


double Camera::getFovV()
{
	return (atan(tan(fovH * 0.5) / (w / h))) * 2;
}


double Camera::getHRatio()
{
	return 1 / (tan(fovH / 2));
}


double Camera::getVRatio()
{
	return 1 / (tan(atan(tan(fovH * 0.5) / (w / h) * 2)));
}


transform3d Camera::getTransformation()
{
	transform3d T;

	T.sinAzm = sin(azm + PI * 0.5);
	T.cosAzm = cos(azm + PI * 0.5);
	T.sinAlt = sin(alt + PI * 0.5);
	T.cosAlt = cos(alt + PI * 0.5);
	T.sinRol = sin(rol);
	T.cosRol = cos(rol);

	return T;
}


bool Camera::polyFacingCamera(triangle3dV P)
{
	vect3 eyeVector = subVectors(P.A, { x, y, z, 1.0 });
	return (dotProduct(P.N, eyeVector) < 0) ? true : false;
}


triangle3dV Camera::world2viewT(const transform3d T, const triangle3dV tr)
{
	triangle3dV t;

	t.A = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr.A))));
	t.B = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr.B))));
	t.C = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr.C))));
	t.An = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr.An))));
	t.Bn = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr.Bn))));
	t.Cn = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr.Cn))));

	t.N = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr.N))));
	t.At = tr.At;
	t.Bt = tr.Bt;
	t.Ct = tr.Ct;
	t.texture = tr.texture;
	t.colour = tr.colour;

	return t;
}


line3d Camera::world2viewL(transform3d T, line3d ln)
{
	line3d tempLine;

	tempLine.A = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, ln.A))));
	tempLine.B = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, ln.B))));

	return tempLine;
}


point3 Camera::world2viewP(transform3d T, point3 p)
{
	point3 tempPoint;

	tempPoint.P = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, p.P))));
	tempPoint.P.w = 1.0;
	tempPoint.colour = p.colour;

	return tempPoint;
}


vect3 Camera::world2view(transform3d T, vect3 tr)
{
	vect3 v;

	v = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr))));

	return v;
}


void Camera::clipToFrustumL(line3d* line)
{
	plane currentPlane;

	currentPlane = Frustum.getNearPlane();
	this->clipLine(currentPlane, line);
	currentPlane = Frustum.getTopPlane();
	this->clipLine(currentPlane, line);
	currentPlane = Frustum.getBottomPlane();
	this->clipLine(currentPlane, line);
	currentPlane = Frustum.getLeftPlane();
	this->clipLine(currentPlane, line);
	currentPlane = Frustum.getRightPlane();
	this->clipLine(currentPlane, line);
	currentPlane = Frustum.getFarPlane();
	this->clipLine(currentPlane, line);
}


void Camera::clipLine(plane p, line3d* line)
{
	double t;

	vect3 a		= subVectors(line->A, p.P);
	double sA	= dotProduct(a, p.N);
	vect3 b		= subVectors(line->B, p.P);
	double sB	= dotProduct(b, p.N);

	if (sign(sA) != sign(sB))
	{
		vect3 d = subVectors(line->B, line->A);
		double dist = dotProduct(d, p.N);

		if (sA > 0)
		{
			if (dist)
			{
				t = (dist - sB) / dist;

				line->B.x = line->A.x + t * (line->B.x - line->A.x);
				line->B.y = line->A.y + t * (line->B.y - line->A.y);
				line->B.z = line->A.z + t * (line->B.z - line->A.z);
			}
		}
		if (sB > 0)
		{
			if (dist)
			{
				t = (-dist - sA) / dist;

				line->A.x = line->B.x - t * (line->A.x - line->B.x);
				line->A.y = line->B.y - t * (line->A.y - line->B.y);
				line->A.z = line->B.z - t * (line->A.z - line->B.z);
			}
		}
	}
}


bool Camera::insideFrustum(point3 point)
{
	if (this->assertPointVis(Frustum.getNearPlane(),	point) &&
		this->assertPointVis(Frustum.getTopPlane(),		point) &&
		this->assertPointVis(Frustum.getBottomPlane(),	point) &&
		this->assertPointVis(Frustum.getLeftPlane(),	point) &&
		this->assertPointVis(Frustum.getRightPlane(),	point) &&
		this->assertPointVis(Frustum.getFarPlane(),		point))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool Camera::assertPointVis(plane plane, point3 Point)
{
	return dotProduct(subVectors(Point.P, plane.P), plane.N) >= 0 ? true : false;
}


int Camera::clipToFrustum(const triangle3dV viewT, vect3* vertList, textCoord* uvList)
{
	memset(vertList, 0.0, MAXCLIPVERTS * sizeof(double));

	vertList[0] = viewT.A;	uvList[0] = viewT.At;
	vertList[1] = viewT.B;	uvList[1] = viewT.Bt;
	vertList[2] = viewT.C;	uvList[2] = viewT.Ct;

	int nVert = 3;
	plane currentPlane;

	currentPlane = Frustum.getNearPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);
	currentPlane = Frustum.getTopPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);
	currentPlane = Frustum.getBottomPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);
	currentPlane = Frustum.getLeftPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);
	currentPlane = Frustum.getRightPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);
	currentPlane = Frustum.getFarPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);

	return nVert;
}


inline void Camera::clipPoly(int* nVert, vect3 * vertList, textCoord * uvList, plane clippingPlane)
{
	int nResult = 0;
	vect3 vTemp[MAXCLIPVERTS];
	textCoord uvTemp[MAXCLIPVERTS];

	for (int i = 0; i < *nVert; i++)
	{
		if (i < (*nVert) - 1)
		{ this->clipEdge(clippingPlane,		vertList[i],	vertList[i + 1], uvList[i],		uvList[i + 1],		&nResult, vTemp, uvTemp); }
		else
		{ this->clipEdge(clippingPlane,		vertList[i],		vertList[0], uvList[i],			uvList[0],		&nResult, vTemp, uvTemp); }
	}
	for (int j = 0; j < nResult; j++)
	{
		vertList[j] = vTemp[j];
		uvList[j] = uvTemp[j];
	}

	*nVert = nResult;
}


inline void Camera::clipEdge(plane p, vect3 startV, vect3 endV, textCoord startUV, textCoord endUV, int* nResult, vect3 * vTemp, textCoord * uvTemp)
{
	double t;
	//vect3 a = subVectors(startV, p.P);
	vect3 a = { startV.x - p.P.x, startV.y - p.P.y, startV.z - p.P.z, 1.0 };
	//double sStart = dotProduct(a, p.N);
	double sStart = a.x * p.N.x + a.y * p.N.y + a.z * p.N.z;
	//vect3 b = subVectors(endV, p.P);
	vect3 b = { endV.x - p.P.x, endV.y - p.P.y, endV.z - p.P.z, 1.0 };
	//double sEnd = dotProduct(b, p.N);
	double sEnd = b.x * p.N.x + b.y * p.N.y + b.z * p.N.z;
	if (sign(sStart) != sign(sEnd))
	{
		vect3 d = subVectors(endV, startV);
		//double dist = dotProduct(d, p.N);
		double dist = d.x * p.N.x + d.y * p.N.y + d.z * p.N.z;
		
		if(sStart < 0)
		{
			if (dist)
			{
				t = (dist - sEnd) / dist;

				vTemp[*nResult].x = startV.x + t * (endV.x - startV.x);
				vTemp[*nResult].y = startV.y + t * (endV.y - startV.y);
				vTemp[*nResult].z = startV.z + t * (endV.z - startV.z);
				vTemp[*nResult].w = startV.w + t * (endV.w - startV.w);

				uvTemp[*nResult].u = startUV.u + t * (endUV.u - startUV.u);
				uvTemp[*nResult].v = startUV.v + t * (endUV.v - startUV.v);

				(*nResult)++;
			}
		}
		if(sEnd < 0)
		{
			if (dist)
			{
				t = (-dist - sStart) / dist;

				vTemp[*nResult].x = endV.x - t * (startV.x - endV.x);
				vTemp[*nResult].y = endV.y - t * (startV.y - endV.y);
				vTemp[*nResult].z = endV.z - t * (startV.z - endV.z);
				vTemp[*nResult].w = endV.w - t * (startV.w - endV.w);

				uvTemp[*nResult].u = endUV.u - t * (startUV.u - endUV.u);
				uvTemp[*nResult].v = endUV.v - t * (startUV.v - endUV.v);

				(*nResult)++;
			}
			else
			{

				vTemp[*nResult].x = endV.x;
				vTemp[*nResult].y = endV.y;
				vTemp[*nResult].z = endV.z;
				vTemp[*nResult].w = endV.w;

				uvTemp[*nResult].u = endUV.u;
				uvTemp[*nResult].v = endUV.v;
				
				(*nResult)++;
			}
		}
		else
		{

			vTemp[*nResult].x = endV.x;
			vTemp[*nResult].y = endV.y;
			vTemp[*nResult].z = endV.z;
			vTemp[*nResult].w = endV.w;

			uvTemp[*nResult].u = endUV.u;
			uvTemp[*nResult].v = endUV.v;

			(*nResult)++;
		}
	}
	else
	{
		if (sign(sStart) > 0 && sign(sEnd) > 0)
		{

			vTemp[*nResult].x = endV.x;
			vTemp[*nResult].y = endV.y;
			vTemp[*nResult].z = endV.z;
			vTemp[*nResult].w = endV.w;

			uvTemp[*nResult].u = endUV.u;
			uvTemp[*nResult].v = endUV.v;

			(*nResult)++;
		}
	}
}


void Camera::illuminatePoly(LightSource L, triangle3dV* T, const triangle3dV P, bool G)
{
	double illumAll, illumA, illumB, illumC;
	illumAll = illumA = illumB = illumC = 0.0;
	vect3 n;

	if (G)
	{
		n = P.An;
		illumA = L.getIllumination(n);
		clampValue(&illumA, MIN_ILLUMINATION, MAX_ILLUMINATION);
		T->A.w = illumA;

		n = P.Bn;
		illumB = L.getIllumination(n);
		clampValue(&illumB, MIN_ILLUMINATION, MAX_ILLUMINATION);
		T->B.w = illumB;

		n = P.Cn;
		illumC = L.getIllumination(n);
		clampValue(&illumC, MIN_ILLUMINATION, MAX_ILLUMINATION);
		T->C.w = illumC;
	}
	else
	{
		n = P.N;
		illumAll = L.getIllumination(n);
		clampValue(&illumAll, MIN_ILLUMINATION, MAX_ILLUMINATION);
		T->A.w = T->B.w = T->C.w = illumAll;
	}
}


void Camera::illuminatePoly(LightSource L, triangle3dV* T, const triangle3dV P, projectionStyle style)
{
	double illumAll, illumA, illumB, illumC;
	illumAll = illumA = illumB = illumC = 0.0;
	vect3 n;

	if (style == gouraud_shaded ||
		style == sunlight)
	{
		n = P.An;
		illumA = L.getIllumination(n);
		clampValue(&illumA, MIN_ILLUMINATION, MAX_ILLUMINATION);
		T->A.w = illumA;

		n = P.Bn;
		illumB = L.getIllumination(n);
		clampValue(&illumB, MIN_ILLUMINATION, MAX_ILLUMINATION);
		T->B.w = illumB;

		n = P.Cn;
		illumC = L.getIllumination(n);
		clampValue(&illumC, MIN_ILLUMINATION, MAX_ILLUMINATION);
		T->C.w = illumC;
	}
	else
	{
		n = P.N;
		illumAll = L.getIllumination(n);
		clampValue(&illumAll, MIN_ILLUMINATION, MAX_ILLUMINATION);
		T->A.w = T->B.w = T->C.w = illumAll;
	}
}


inline coord2 Camera::view2screen(vect3 vertex, double hR, double vR)
{
	coord2 pixel;

	pixel.x = (int)(w * 0.5 + (vertex.x / vertex.z) * w * 0.5 * hR * 0.95);
	pixel.y = (int)(h * 0.5 - (vertex.y / vertex.z) * w * 0.5 * vR * 0.95);
	pixel.z = 1 / vertex.z;

	return pixel;
}


inline vect3 Camera::screen2view(coord2 pixel, double hR, double vR)
{
	vect3 vertex;

	vertex.x = (pixel.x - w * 0.5) * (1 / pixel.z) / (w * 0.475 * hR);
	vertex.y = (h * 0.5 - pixel.y) * (1 / pixel.z) / (w * 0.475 * vR);
	vertex.z = 1 / pixel.z;
	vertex.w = 1.0;

	return vertex;
}


void Camera::projectLine(line3d line, Uint32* pixelBuffer, double* depthBuffer, double hRatio, double vRatio)
{
	coord2 startP	= this->view2screen(line.A, hRatio, vRatio);
	coord2 endP		= this->view2screen(line.B, hRatio, vRatio);

	if ((startP.x	>= 0	&& startP.x		< w	)	&&
		(startP.y	>= 0	&& startP.y		< h	)	&&
		(endP.x		>= 0	&& endP.x		< w	)	&&
		(endP.y		>= 0	&& endP.y		< h	))
	{
		drawLine(startP, endP, line.colour, pixelBuffer);
	}
}


void Camera::projectPoint(point3 P, Uint32* pixelBuffer, double hRatio, double vRatio)
{
	coord2 cp = this->view2screen(P.P, hRatio, vRatio);
	if ((cp.x >= 0 && cp.x < w) && (cp.y >= 0 && cp.y < h))
	{
		pixelBuffer[cp.y * w + cp.x] = P.colour;
	}
}


void Camera::projectPoly(int n, vect3* vertexList, textCoord* uvList, Uint32 colour, Uint32* pixelBuffer, double* depthBuffer,
	int nShadow, ShadowVolume* SH, double hRatio, double vRatio, projectionStyle style, double torchI, double maxI, triangle3dV originalPoly)
{
	triangle2dG screenT;
	triangle3dV originalT;	//To be passed on to polygon filling routine

	if (style == wireframe)
	{
		coord2 startP, endP;
		for (int i = 0; i < n; i++)
		{
			if (i < n - 1)
			{
				startP = this->view2screen(vertexList[i], hRatio, vRatio);
				endP = this->view2screen(vertexList[i + 1], hRatio, vRatio);
			}
			else
			{
				startP = this->view2screen(vertexList[i], hRatio, vRatio);
				endP = this->view2screen(vertexList[0], hRatio, vRatio);
			}
			drawLine(startP, endP, colour, pixelBuffer);
		}
	}
	else
	{
		for (int i = 0; i < n - 2; i++)
		{
			screenT.a = this->view2screen(vertexList[0], hRatio, vRatio);
			screenT.b = this->view2screen(vertexList[1 + i], hRatio, vRatio);
			screenT.c = this->view2screen(vertexList[2 + i], hRatio, vRatio);

			originalT.A = vertexList[0];
			originalT.B = vertexList[1 + i];
			originalT.C = vertexList[2 + i];

			originalT.N = originalPoly.N;

			screenT.illumA = vertexList[0].w;
			screenT.illumB = vertexList[1 + i].w;
			screenT.illumC = vertexList[2 + i].w;

			screenT.At = uvList[0];
			screenT.Bt = uvList[1 + i];
			screenT.Ct = uvList[2 + i];

			screenT.h = colour;
			bool d = false;

			switch (style)
			{
			case solid_colour:
			{
				this->fillTriangleSolidColour(originalT, screenT, pixelBuffer, depthBuffer, d);
			}
			break;
			case checkerboard:
			{
				this->fillTriangleCheckerboard(originalT, screenT, pixelBuffer, depthBuffer, d);
			}
			break;
			case flat_shaded:
			{
				this->fillTriangleFlatShaded(screenT, pixelBuffer, depthBuffer);
			}
			break;
			case gouraud_shaded:
			{
				this->fillTriangleGouraudShaded(screenT, pixelBuffer, depthBuffer, d);
			}
			break;
			case depth_visualised:
			{
				this->fillTriangleDepthVisualised(originalT, screenT, pixelBuffer, depthBuffer, true);
			}
			break;
			case sunlight:
			{
				this->fillTriangleSunlight(originalT, screenT, pixelBuffer, depthBuffer, false);
			}
			break;
			case torchlight:
			{
				this->fillTriangleTorchlight(originalT, screenT, pixelBuffer, depthBuffer, d, torchI, maxI);
			}
			break;
			case torchlight_solid:
			{
				this->fillTriangleTorchlightSolidColour(originalT, screenT, pixelBuffer, depthBuffer, d, torchI, maxI);
			}
			break;
			case test:
			{
				this->fillZBuffer(originalT, screenT, depthBuffer);
				//this->fillTriangleTest(originalT, screenT, pixelBuffer, depthBuffer, false);
			}
			break;
			default:
			{
				this->fillTriangleFlatShaded(screenT, pixelBuffer, depthBuffer);
			}
			break;
			}
		}
	}
}


inline textCoord Camera::getUVCoord(vect3 startV, vect3 endV, textCoord startC, textCoord endC, vect3 testV)
{
	textCoord testC;

	double a = dotProduct(subVectors(testV, startV), subVectors(endV, startV));
	double b = dotProduct(subVectors(endV, startV), subVectors(endV, startV));

	double d = (b != 0.0) ? (a / b) : (0.0);

	testC.u = startC.u + d * (endC.u - startC.u);
	testC.v = startC.v + d * (endC.v - startC.v);

	return testC;
}


inline textCoord Camera::interpolateUVCoord(textCoord startC, textCoord endC, int step, int currentP)
{
	textCoord testC;

	testC.u = startC.u + (endC.u - startC.u) / step * currentP;
	testC.v = startC.v + (endC.v - startC.v) / step * currentP;

	return testC;
}


inline void Camera::fillTriangleSolidColour(const triangle3dV T, const triangle2dG t, Uint32* pixelBuffer, double* depthBuffer, bool d)
{
	Uint32 pixelColour = t.h;
	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;
	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int halfW = (int)(w * 0.5);
	int halfH = (int)(h * 0.5);
	int dx, dy;
	double xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	double zLimit[2] = { 0.0, 0.0 };

	vect3 nullVect = { 0.0, 0.0, 0.0, 0.0 };
	vect3 sides[4] = { nullVect, nullVect, nullVect, nullVect };
	vect3 sideL[2] = { nullVect, nullVect };
	vect3 sideR[2] = { nullVect, nullVect };
	textCoord nullCoord = { 0.0, 0.0 };
	textCoord uvSides[4] = { nullCoord, nullCoord, nullCoord, nullCoord };
	textCoord uvSideL[2] = { nullCoord, nullCoord };
	textCoord uvSideR[2] = { nullCoord, nullCoord };

	int endIndex;
	int startX, endX;
	double startZ, endZ, zCurrent;
	double h_ratio, v_ratio;
	h_ratio = this->getHRatio();
	v_ratio = this->getVRatio();
	double hCorr = w * 0.475 * h_ratio;
	double vCorr = w * 0.475 * v_ratio;
	double deltaZ;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z;
			yy = (double)hg - (double)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (double)dy);
			wd = t.a.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;

				endIndex++;
			}
		}
		//Side B-C:
		if ((t.b.y <= hg && t.c.y > hg) || (t.c.y <= hg && t.b.y > hg))
		{
			dx = t.c.x - t.b.x; dy = t.c.y - t.b.y; dz = t.c.z - t.b.z;
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (double)dy);
			wd = t.b.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;

				endIndex++;
			}
		}
		//Side C-A:
		if ((t.c.y <= hg && t.a.y > hg) || (t.a.y <= hg && t.c.y > hg))
		{
			dx = t.a.x - t.c.x; dy = t.a.y - t.c.y; dz = t.a.z - t.c.z;
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (double)dy);
			wd = t.c.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;

				endIndex++;
			}
		}
		if (endIndex == 2)
		{
			if (lineEnd[0] <= lineEnd[1])
			{
				startX = lineEnd[0];
				endX = lineEnd[1];
				startZ = zLimit[0];
				endZ = zLimit[1];
			}
			else
			{
				startX = lineEnd[1];
				endX = lineEnd[0];
				startZ = zLimit[1];
				endZ = zLimit[0];
			}
			int span = abs(endX - startX + 1);
			deltaZ = (endZ - startZ) / (double)span;
			zCurrent = startZ;

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					if (1 / zCurrent < depthBuffer[hg * w + i])
					{
						pixelBuffer[hg * w + i] = pixelColour;
						depthBuffer[hg * w + i] = 1 / zCurrent;
					}
					zCurrent += deltaZ;
				}
			}
		}
	}
}


inline void Camera::fillTriangleCheckerboard(const triangle3dV T, const triangle2dG t, Uint32* pixelBuffer, double* depthBuffer, bool d)
{
	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int halfW = (int)(w * 0.5);
	int halfH = (int)(h * 0.5);
	int dx, dy;
	double dIllum, illum;
	double xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	double zLimit[2] = { 0.0, 0.0 };
	double illLimit[2] = { 0.0, 0.0 };

	vect3 nullVect = { 0.0, 0.0, 0.0, 0.0 };
	vect3 sides[4] = { nullVect, nullVect, nullVect, nullVect };
	vect3 sideL[2] = { nullVect, nullVect };
	vect3 sideR[2] = { nullVect, nullVect };
	textCoord nullCoord = { 0.0, 0.0 };
	textCoord uvSides[4] = { nullCoord, nullCoord, nullCoord, nullCoord };
	textCoord uvSideL[2] = { nullCoord, nullCoord };
	textCoord uvSideR[2] = { nullCoord, nullCoord };

	int endIndex;
	int startX, endX;
	double startZ, endZ, zCurrent, startIll, endIll, illCurrent;
	double h_ratio, v_ratio;
	h_ratio = this->getHRatio();
	v_ratio = this->getVRatio();
	double invertStartPz, invertEndPz, invertCurrentPz;
	double hCorr = w * 0.475 * h_ratio;
	double vCorr = w * 0.475 * v_ratio;
	double deltaZ, deltaIll;

	coord2 currentP, startP, endP;
	vect3 currentVert, startVert, endVert;
	textCoord startUV, endUV, sampleUV;
	int sampleXold = 0, sampleYold = 0, sampleXnew = 0, sampleYnew = 0;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z; dIllum = t.illumB - t.illumA;
			yy = (double)hg - (double)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (double)dy);
			illum = t.illumA + dIllum * (yy / (double)dy);
			wd = t.a.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;

				sides[endIndex * 2] = T.A;
				sides[endIndex * 2 + 1] = T.B;
				uvSides[endIndex * 2] = t.At;
				uvSides[endIndex * 2 + 1] = t.Bt;

				endIndex++;
			}
		}
		//Side B-C:
		if ((t.b.y <= hg && t.c.y > hg) || (t.c.y <= hg && t.b.y > hg))
		{
			dx = t.c.x - t.b.x; dy = t.c.y - t.b.y; dz = t.c.z - t.b.z; dIllum = t.illumC - t.illumB;
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (double)dy);
			illum = t.illumB + dIllum * (yy / (double)dy);
			wd = t.b.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;

				sides[endIndex * 2] = T.B;
				sides[endIndex * 2 + 1] = T.C;
				uvSides[endIndex * 2] = t.Bt;
				uvSides[endIndex * 2 + 1] = t.Ct;

				endIndex++;
			}
		}
		//Side C-A:
		if ((t.c.y <= hg && t.a.y > hg) || (t.a.y <= hg && t.c.y > hg))
		{
			dx = t.a.x - t.c.x; dy = t.a.y - t.c.y; dz = t.a.z - t.c.z; dIllum = t.illumA - t.illumC;
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (double)dy);
			illum = t.illumC + dIllum * (yy / (double)dy);
			wd = t.c.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;

				sides[endIndex * 2] = T.C;
				sides[endIndex * 2 + 1] = T.A;
				uvSides[endIndex * 2] = t.Ct;
				uvSides[endIndex * 2 + 1] = t.At;

				endIndex++;
			}
		}
		if (endIndex == 2)
		{
			if (lineEnd[0] <= lineEnd[1])
			{
				startX = lineEnd[0];
				endX = lineEnd[1];
				startZ = zLimit[0];
				endZ = zLimit[1];
				startIll = illLimit[0];
				endIll = illLimit[1];

				sideL[0] = sides[0];
				sideL[1] = sides[1];
				sideR[0] = sides[2];
				sideR[1] = sides[3];

				uvSideL[0] = uvSides[0];
				uvSideL[1] = uvSides[1];
				uvSideR[0] = uvSides[2];
				uvSideR[1] = uvSides[3];
			}
			else
			{
				startX = lineEnd[1];
				endX = lineEnd[0];
				startZ = zLimit[1];
				endZ = zLimit[0];
				startIll = illLimit[1];
				endIll = illLimit[0];

				sideL[0] = sides[2];
				sideL[1] = sides[3];
				sideR[0] = sides[0];
				sideR[1] = sides[1];

				uvSideL[0] = uvSides[2];
				uvSideL[1] = uvSides[3];
				uvSideR[0] = uvSides[0];
				uvSideR[1] = uvSides[1];
			}
			int span = abs(endX - startX + 1);
			deltaZ = (endZ - startZ) / (double)span;
			deltaIll = ((endIll - startIll) / (double)span) / 100.0;
			zCurrent = startZ;
			illCurrent = startIll / 100.0;

			startP.x = startX;	startP.y = hg;	startP.z = startZ;
			endP.x = endX;		endP.y = hg;	endP.z = endZ;

			invertStartPz = 1 / startP.z;
			startVert.x = (startP.x - double(halfW)) * invertStartPz / hCorr;
			startVert.y = (double(halfH) - startP.y) * invertStartPz / vCorr;
			startVert.z = invertStartPz;

			invertEndPz = 1 / endP.z;
			endVert.x = (endP.x - double(halfW)) * invertEndPz / hCorr;
			endVert.y = (double(halfH) - endP.y) * invertEndPz / vCorr;
			endVert.z = invertEndPz;


			startUV = getUVCoord(sideL[0], sideL[1], uvSideL[0], uvSideL[1], startVert	);
			endUV	= getUVCoord(sideR[0], sideR[1], uvSideR[0], uvSideR[1], endVert	);


			currentP.y = hg;

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					if (1 / zCurrent < depthBuffer[hg * w + i])
					{
						currentP.x = i;
						currentP.z = zCurrent;

						invertCurrentPz = 1 / currentP.z;
						currentVert.x = (currentP.x - double(halfW)) * invertCurrentPz / hCorr;
						currentVert.y = (double(halfH) - currentP.y) * invertCurrentPz / vCorr;
						currentVert.z = invertCurrentPz;

						sampleUV = this->getUVCoord(startVert, endVert, startUV, endUV, currentVert);

						sampleXnew = ((int)(double(currentTexture.w) * sampleUV.u)) % currentTexture.w;
						sampleYnew = ((int)(double(currentTexture.h) * sampleUV.v)) % currentTexture.h;

						Uint32 blue		= getColour(0, 0, 0, (byte)(255 * illCurrent));
						Uint32 white	= getColour(0, (byte)(255 * illCurrent), (byte)(255 * illCurrent), (byte)(255 * illCurrent));

						if (int(sampleXnew / 16) % 2)
						{
							if (int(sampleYnew / 16) % 2)
							{
								pixelBuffer[hg * w + i] = white;
							}
							else
							{
								pixelBuffer[hg * w + i] = blue;
							}
						}
						else
						{
							if (int(sampleYnew / 16) % 2)
							{
								pixelBuffer[hg * w + i] = blue;
							}
							else
							{
								pixelBuffer[hg * w + i] = white;
							}
						}
						depthBuffer[hg * w + i] = 1 / zCurrent;
					}
					zCurrent += deltaZ;
					illCurrent += deltaIll;
				}
			}
		}
	}
}


inline void Camera::fillTriangleFlatShaded(const triangle2dG t, Uint32* pixelBuffer, double* depthBuffer)
{
	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int m = pow2(scale);
	int dx, dy;
	double xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	double zLimit[2] = { 0, 0 };
	int endIndex;
	int startX, endX;
	double startZ, endZ, zCurrent, illCurrent;

	illCurrent = t.illumA;
	double deltaZ;

	byte r, g, b;
	r = ((byte)(t.h >> 16 & 0xFF));
	g = ((byte)(t.h >> 8 & 0xFF));
	b = ((byte)(t.h & 0xFF));

	Uint32 pixelColour = getColour(0, (byte)(r * illCurrent), (byte)(g * illCurrent), (byte)(b * illCurrent));

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z;
			yy = (double)hg - (double)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (double)dy);
			wd = t.a.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				endIndex++;
			}
		}
		//Side B-C:
		if ((t.b.y <= hg && t.c.y > hg) || (t.c.y <= hg && t.b.y > hg))
		{
			dx = t.c.x - t.b.x; dy = t.c.y - t.b.y; dz = t.c.z - t.b.z;
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (double)dy);
			wd = t.b.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				endIndex++;
			}
		}
		//Side C-A:
		if ((t.c.y <= hg && t.a.y > hg) || (t.a.y <= hg && t.c.y > hg))
		{
			dx = t.a.x - t.c.x; dy = t.a.y - t.c.y; dz = t.a.z - t.c.z;
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (double)dy);
			wd = t.c.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				endIndex++;
			}
		}
		if (endIndex == 2)
		{
			if (lineEnd[0] <= lineEnd[1])
			{
				startX = lineEnd[0]; startZ = zLimit[0];
				endX = lineEnd[1]; endZ = zLimit[1];
			}
			else
			{
				startX = lineEnd[1]; endX = lineEnd[0];
				startZ = zLimit[1]; endZ = zLimit[0];
			}
			int span = abs(endX - startX + 1);
			deltaZ = (endZ - startZ) / (double)span;
			zCurrent = startZ;
			int hgw = hg * w;
			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					if (1 / zCurrent < depthBuffer[hg * w + i])
					{
						pixelBuffer[hgw + i] = pixelColour;
						depthBuffer[hgw + i] = 1 / zCurrent;
					}
					zCurrent += deltaZ;
				}
			}
		}
	}
}


inline void Camera::fillTriangleGouraudShaded(const triangle2dG t, Uint32* pixelBuffer, double* depthBuffer, bool d)
{
	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int m = pow2(scale);
	int dx, dy;
	double dIllum, illum;
	double xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	double zLimit[2] = { 0.0, 0.0 };
	double illLimit[2] = { 0.0, 0.0 };
	int endIndex;
	int startX, endX;
	double startZ, endZ, zCurrent, startIll, endIll, illCurrent;

	double deltaZ, deltaIll;

	byte r, g, b;
	r = ((byte)(t.h >> 16 & 0xFF));
	g = ((byte)(t.h >> 8 & 0xFF));
	b = ((byte)(t.h & 0xFF));

	double corr = 0.0;

	coord2 currentP, startP, endP;
	vect3 currentV, startV, endV;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z; dIllum = t.illumB - t.illumA;
			yy = (double)hg - (double)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * ((yy + corr) / (double)dy);
			illum = t.illumA + dIllum * (yy / (double)dy);
			wd = t.a.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;
				endIndex++;
			}
		}
		//Side B-C:
		if ((t.b.y <= hg && t.c.y > hg) || (t.c.y <= hg && t.b.y > hg))
		{
			dx = t.c.x - t.b.x; dy = t.c.y - t.b.y; dz = t.c.z - t.b.z; dIllum = t.illumC - t.illumB;
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * ((yy + corr) / (double)dy);
			illum = t.illumB + dIllum * (yy / (double)dy);
			wd = t.b.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;
				endIndex++;
			}
		}
		//Side C-A:
		if ((t.c.y <= hg && t.a.y > hg) || (t.a.y <= hg && t.c.y > hg))
		{
			dx = t.a.x - t.c.x; dy = t.a.y - t.c.y; dz = t.a.z - t.c.z; dIllum = t.illumA - t.illumC;
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * ((yy + corr) / (double)dy);
			illum = t.illumC + dIllum * (yy / (double)dy);
			wd = t.c.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;
				endIndex++;
			}
		}
		if (endIndex == 2)
		{
			if (lineEnd[0] <= lineEnd[1])
			{
				startX = lineEnd[0];
				endX = lineEnd[1];
				startZ = zLimit[0];
				endZ = zLimit[1];
				startIll = illLimit[0];
				endIll = illLimit[1];
			}
			else
			{
				startX = lineEnd[1];
				endX = lineEnd[0];
				startZ = zLimit[1];
				endZ = zLimit[0];
				startIll = illLimit[1];
				endIll = illLimit[0];
			}
			int span = abs(endX - startX + 1);
			deltaZ = (endZ - startZ) / (double)span;
			deltaIll = ((endIll - startIll) / (double)span) / 100.0;
			zCurrent = startZ;
			illCurrent = startIll / 100.0;

			startP.x = startX;	startP.y = hg;	startP.z = startZ;
			endP.x = endX;		endP.y = hg;	endP.z = endZ;
			startV = this->screen2view(startP, this->getHRatio(), this->getVRatio());
			endV = this->screen2view(endP, this->getHRatio(), this->getVRatio());

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					if (1 / zCurrent < depthBuffer[hg * w + i])
					{
						for (int p = 0; p < m; p++)
						{
							for (int q = 0; q < m; q++)
							{
								pixelBuffer[(m * hg + p) * w + m * i + q] = getColour(0, (byte)(r * illCurrent), (byte)(g * illCurrent), (byte)(b * illCurrent));
								currentP.x = m * i + q;
								currentP.y = m * hg + p;
								currentP.z = zCurrent;
								currentV = this->screen2view(currentP, this->getHRatio(), this->getVRatio());

							}
						}
						depthBuffer[hg * w + i] = 1 / zCurrent;
					}
					zCurrent += deltaZ;
					illCurrent += deltaIll;
				}
			}
		}
	}
}


inline void Camera::fillTriangleDepthVisualised(const triangle3dV T, const triangle2dG t, Uint32* pixelBuffer, double* depthBuffer, bool d)
{
	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int halfW = (int)(w * 0.5);
	int halfH = (int)(h * 0.5);
	int dx, dy;
	double xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	double zLimit[2] = { 0.0, 0.0 };

	int endIndex;
	int startX, endX;
	double startZ, endZ, zCurrent;
	double h_ratio, v_ratio;
	h_ratio = this->getHRatio();
	v_ratio = this->getVRatio();
	double invertCurrentZ;
	double hCorr = w * 0.475 * h_ratio;
	double vCorr = w * 0.475 * v_ratio;
	double deltaZ;
	coord2 currentP, startP, endP;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z;
			yy = (double)hg - (double)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (double)dy);
			wd = t.a.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				endIndex++;
			}
		}
		//Side B-C:
		if ((t.b.y <= hg && t.c.y > hg) || (t.c.y <= hg && t.b.y > hg))
		{
			dx = t.c.x - t.b.x; dy = t.c.y - t.b.y; dz = t.c.z - t.b.z;
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (double)dy);
			wd = t.b.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				endIndex++;
			}
		}
		//Side C-A:
		if ((t.c.y <= hg && t.a.y > hg) || (t.a.y <= hg && t.c.y > hg))
		{
			dx = t.a.x - t.c.x; dy = t.a.y - t.c.y; dz = t.a.z - t.c.z;
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (double)dy);
			wd = t.c.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				endIndex++;
			}
		}
		if (endIndex == 2)
		{
			if (lineEnd[0] <= lineEnd[1])
			{
				startX = lineEnd[0];
				endX = lineEnd[1];
				startZ = zLimit[0];
				endZ = zLimit[1];
			}
			else
			{
				startX = lineEnd[1];
				endX = lineEnd[0];
				startZ = zLimit[1];
				endZ = zLimit[0];
			}
			int span = abs(endX - startX + 1);
			deltaZ = (endZ - startZ) / (double)span;
			zCurrent = startZ;

			currentP.y = hg;

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					invertCurrentZ = 1 / zCurrent;
					if (invertCurrentZ < depthBuffer[hg * w + i])
					{
						pixelBuffer[hg * w + i] = getColour(0, (byte)(255 * zCurrent), (byte)(255 * zCurrent), (byte)(255 * zCurrent));
						depthBuffer[hg * w + i] = invertCurrentZ;
					}
					zCurrent += deltaZ;
				}
			}
		}
	}
}


inline void Camera::fillTriangleSunlight(const triangle3dV T, const triangle2dG t, Uint32* pixelBuffer, double* depthBuffer, bool d)
{
	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int halfW = (int)(w * 0.5);
	int halfH = (int)(h * 0.5);
	int dx, dy;
	double dIllum, illum;
	double xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	double zLimit[2] = { 0.0, 0.0 };
	double illLimit[2] = { 0.0, 0.0 };

	vect3 nullVect = { 0.0, 0.0, 0.0, 0.0 };
	vect3 sides[4] = { nullVect, nullVect, nullVect, nullVect };
	vect3 sideL[2] = { nullVect, nullVect };
	vect3 sideR[2] = { nullVect, nullVect };
	textCoord nullCoord = { 0.0, 0.0 };
	textCoord uvSides[4] = { nullCoord, nullCoord, nullCoord, nullCoord };
	textCoord uvSideL[2] = { nullCoord, nullCoord };
	textCoord uvSideR[2] = { nullCoord, nullCoord };

	int endIndex;
	int startX, endX;
	double startZ, endZ, zCurrent, startIll, endIll, illCurrent;
	double h_ratio, v_ratio;
	h_ratio = this->getHRatio();
	v_ratio = this->getVRatio();
	double invertStartPz, invertEndPz, invertCurrentZ;
	double hCorr = w * 0.475 * h_ratio;
	double vCorr = w * 0.475 * v_ratio;
	double deltaZ, deltaIll;

	coord2 currentP, startP, endP;
	vect3 currentVert, startVert, endVert;
	textCoord startUV, endUV, sampleUV;
	int sampleXold = 0, sampleYold = 0, sampleXnew = 0, sampleYnew = 0;
	Uint32 finalPixel;

	/**/
	byte a = 0, r, g, b, r2fill = 0, g2fill = 0, b2fill = 0;
	double illSurplus = 0.0;
	/**/

	for (int hg = yMin; hg < yMax; hg++)
	{
		int hgw = hg * w;
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z; dIllum = t.illumB - t.illumA;
			yy = (double)hg - (double)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (double)dy);
			illum = t.illumA + dIllum * (yy / (double)dy);
			wd = t.a.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;

				sides[endIndex * 2] = T.A;
				sides[endIndex * 2 + 1] = T.B;
				uvSides[endIndex * 2] = t.At;
				uvSides[endIndex * 2 + 1] = t.Bt;

				endIndex++;
			}
		}
		//Side B-C:
		if ((t.b.y <= hg && t.c.y > hg) || (t.c.y <= hg && t.b.y > hg))
		{
			dx = t.c.x - t.b.x; dy = t.c.y - t.b.y; dz = t.c.z - t.b.z; dIllum = t.illumC - t.illumB;
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (double)dy);
			illum = t.illumB + dIllum * (yy / (double)dy);
			wd = t.b.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;

				sides[endIndex * 2] = T.B;
				sides[endIndex * 2 + 1] = T.C;
				uvSides[endIndex * 2] = t.Bt;
				uvSides[endIndex * 2 + 1] = t.Ct;

				endIndex++;
			}
		}
		//Side C-A:
		if ((t.c.y <= hg && t.a.y > hg) || (t.a.y <= hg && t.c.y > hg))
		{
			dx = t.a.x - t.c.x; dy = t.a.y - t.c.y; dz = t.a.z - t.c.z; dIllum = t.illumA - t.illumC;
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (double)dy);
			illum = t.illumC + dIllum * (yy / (double)dy);
			wd = t.c.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;

				sides[endIndex * 2] = T.C;
				sides[endIndex * 2 + 1] = T.A;
				uvSides[endIndex * 2] = t.Ct;
				uvSides[endIndex * 2 + 1] = t.At;

				endIndex++;
			}
		}
		if (endIndex == 2)
		{
			if (lineEnd[0] <= lineEnd[1])
			{
				startX = lineEnd[0];
				endX = lineEnd[1];
				startZ = zLimit[0];
				endZ = zLimit[1];
				startIll = illLimit[0];
				endIll = illLimit[1];

				sideL[0] = sides[0];
				sideL[1] = sides[1];
				sideR[0] = sides[2];
				sideR[1] = sides[3];

				uvSideL[0] = uvSides[0];
				uvSideL[1] = uvSides[1];
				uvSideR[0] = uvSides[2];
				uvSideR[1] = uvSides[3];
			}
			else
			{
				startX = lineEnd[1];
				endX = lineEnd[0];
				startZ = zLimit[1];
				endZ = zLimit[0];
				startIll = illLimit[1];
				endIll = illLimit[0];

				sideL[0] = sides[2];
				sideL[1] = sides[3];
				sideR[0] = sides[0];
				sideR[1] = sides[1];

				uvSideL[0] = uvSides[2];
				uvSideL[1] = uvSides[3];
				uvSideR[0] = uvSides[0];
				uvSideR[1] = uvSides[1];
			}
			int span = abs(endX - startX + 1);
			deltaZ = (endZ - startZ) / (double)span;
			deltaIll = ((endIll - startIll) / (double)span) / 100.0;
			zCurrent = startZ;
			illCurrent = startIll / 100.0;

			startP.x = startX;	startP.y = hg;	startP.z = startZ;
			endP.x = endX;		endP.y = hg;	endP.z = endZ;

			invertStartPz = 1 / startP.z;
			startVert.x = (startP.x - double(halfW)) * invertStartPz / hCorr;
			startVert.y = (double(halfH) - startP.y) * invertStartPz / vCorr;
			startVert.z = invertStartPz;

			invertEndPz = 1 / endP.z;
			endVert.x = (endP.x - double(halfW)) * invertEndPz / hCorr;
			endVert.y = (double(halfH) - endP.y) * invertEndPz / vCorr;
			endVert.z = invertEndPz;

			startUV = getUVCoord(sideL[0], sideL[1], uvSideL[0], uvSideL[1], startVert	);
			endUV	= getUVCoord(sideR[0], sideR[1], uvSideR[0], uvSideR[1], endVert	);

			currentP.y = hg;

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					invertCurrentZ = 1 / zCurrent;
					if (invertCurrentZ < depthBuffer[hgw + i])
					{
						currentP.x = i;
						currentP.z = zCurrent;

						currentVert.x = (currentP.x - double(halfW)) * invertCurrentZ / hCorr;
						currentVert.y = (double(halfH) - currentP.y) * invertCurrentZ / vCorr;
						currentVert.z = invertCurrentZ;

						sampleUV = this->getUVCoord(startVert, endVert, startUV, endUV, currentVert);

						sampleXnew = ((int)(currentTexture.w * sampleUV.u)) % currentTexture.w;
						sampleYnew = ((int)(currentTexture.h * sampleUV.v)) % currentTexture.h;

						if (sampleXnew < 0) { sampleXnew = currentTexture.w + sampleXnew % currentTexture.w; }
						if (sampleYnew < 0) { sampleYnew = currentTexture.h + sampleYnew % currentTexture.h; }

						if ((sampleXnew != sampleXold) || (sampleYnew != sampleYold))
						{
							finalPixel = currentTexture.pixels[sampleYnew * currentTexture.w + sampleXnew];
						}

						sampleXold = sampleXnew;
						sampleYold = sampleYnew;

						pixelBuffer[hgw + i] = modifyColour(finalPixel, illCurrent);

						/*
						//TO SAVE COST OF FUNCTION CALL--------------------------------------------------V
						if (illCurrent <= 1.0)
						{
							r = (byte)(double(finalPixel >> 16 & 255) * illCurrent);
							g = (byte)(double(finalPixel >> 8 & 255) * illCurrent);
							b = (byte)(double(finalPixel & 255) * illCurrent);
						}
						else
						{
							illSurplus = illCurrent - 1.0;

							r = (byte)(double(finalPixel >> 16 & 255) * illCurrent);
							g = (byte)(double(finalPixel >> 8 & 255) * illCurrent);
							b = (byte)(double(finalPixel & 255) * illCurrent);

							r2fill = 255 - r;	g2fill = 255 - g;	b2fill = 255 - b;
							r += (byte)((double(r2fill) / (MAX_ILLUMINATION - 1.0)) * illSurplus);
							g += (byte)((double(g2fill) / (MAX_ILLUMINATION - 1.0)) * illSurplus);
							b += (byte)((double(b2fill) / (MAX_ILLUMINATION - 1.0)) * illSurplus);
						}

						pixelBuffer[hgw + i] = (a << 24) | (r << 16) | (g << 8) | (b << 0);
						//TO SAVE COST OF FUNCTION CALL--------------------------------------------------^
						*/

						depthBuffer[hgw + i] = invertCurrentZ;
					}
					zCurrent += deltaZ;
					illCurrent += deltaIll;
				}
			}
		}
	}
}


inline void Camera::fillTriangleTorchlight(const triangle3dV T, const triangle2dG t, Uint32* pixelBuffer, double* depthBuffer, bool d,
		double lightIntensity, double maxIllumination)
{
	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int halfW = (int)(w * 0.5);
	int halfH = (int)(h * 0.5);
	int dx, dy;
	double xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	double zLimit[2] = { 0.0, 0.0 };

	vect3 nullVect = { 0.0, 0.0, 0.0, 0.0 };
	vect3 sides[4] = { nullVect, nullVect, nullVect, nullVect };
	vect3 sideL[2] = { nullVect, nullVect };
	vect3 sideR[2] = { nullVect, nullVect };
	textCoord nullCoord = { 0.0, 0.0 };
	textCoord uvSides[4] = { nullCoord, nullCoord, nullCoord, nullCoord };
	textCoord uvSideL[2] = { nullCoord, nullCoord };
	textCoord uvSideR[2] = { nullCoord, nullCoord };

	int endIndex;
	int startX, endX;
	double startZ, endZ, zCurrent, illCurrent;
	double h_ratio, v_ratio;
	h_ratio = this->getHRatio();
	v_ratio = this->getVRatio();
	double invertStartPz, invertEndPz, invertCurrentZ;
	double hCorr = w * 0.475 * h_ratio;
	double vCorr = w * 0.475 * v_ratio;
	double deltaZ;

	coord2 currentP, startP, endP;
	vect3 currentVert, startVert, endVert;
	textCoord startUV, endUV, sampleUV;
	int sampleXold = 0, sampleYold = 0, sampleXnew = 0, sampleYnew = 0;
	Uint32 finalPixel;
	double distSquared;
	vect3 lightVector;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z;
			yy = (double)hg - (double)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (double)dy);
			wd = t.a.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				sides[endIndex * 2] = T.A;
				sides[endIndex * 2 + 1] = T.B;
				uvSides[endIndex * 2] = t.At;
				uvSides[endIndex * 2 + 1] = t.Bt;

				endIndex++;
			}
		}
		//Side B-C:
		if ((t.b.y <= hg && t.c.y > hg) || (t.c.y <= hg && t.b.y > hg))
		{
			dx = t.c.x - t.b.x; dy = t.c.y - t.b.y; dz = t.c.z - t.b.z;
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (double)dy);
			wd = t.b.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				sides[endIndex * 2] = T.B;
				sides[endIndex * 2 + 1] = T.C;
				uvSides[endIndex * 2] = t.Bt;
				uvSides[endIndex * 2 + 1] = t.Ct;

				endIndex++;
			}
		}
		//Side C-A:
		if ((t.c.y <= hg && t.a.y > hg) || (t.a.y <= hg && t.c.y > hg))
		{
			dx = t.a.x - t.c.x; dy = t.a.y - t.c.y; dz = t.a.z - t.c.z;
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (double)dy);
			wd = t.c.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				sides[endIndex * 2] = T.C;
				sides[endIndex * 2 + 1] = T.A;
				uvSides[endIndex * 2] = t.Ct;
				uvSides[endIndex * 2 + 1] = t.At;

				endIndex++;
			}
		}
		if (endIndex == 2)
		{
			if (lineEnd[0] <= lineEnd[1])
			{
				startX = lineEnd[0];
				endX = lineEnd[1];
				startZ = zLimit[0];
				endZ = zLimit[1];
				sideL[0] = sides[0];
				sideL[1] = sides[1];
				sideR[0] = sides[2];
				sideR[1] = sides[3];

				uvSideL[0] = uvSides[0];
				uvSideL[1] = uvSides[1];
				uvSideR[0] = uvSides[2];
				uvSideR[1] = uvSides[3];
			}
			else
			{
				startX = lineEnd[1];
				endX = lineEnd[0];
				startZ = zLimit[1];
				endZ = zLimit[0];
				sideL[0] = sides[2];
				sideL[1] = sides[3];
				sideR[0] = sides[0];
				sideR[1] = sides[1];

				uvSideL[0] = uvSides[2];
				uvSideL[1] = uvSides[3];
				uvSideR[0] = uvSides[0];
				uvSideR[1] = uvSides[1];
			}
			int span = abs(endX - startX + 1);
			deltaZ = (endZ - startZ) / (double)span;
			zCurrent = startZ;

			startP.x = startX;	startP.y = hg;	startP.z = startZ;
			endP.x = endX;		endP.y = hg;	endP.z = endZ;

			invertStartPz = 1 / startP.z;
			startVert.x = (startP.x - double(halfW)) * invertStartPz / hCorr;
			startVert.y = (double(halfH) - startP.y) * invertStartPz / vCorr;
			startVert.z = invertStartPz;

			invertEndPz = 1 / endP.z;
			endVert.x = (endP.x - double(halfW)) * invertEndPz / hCorr;
			endVert.y = (double(halfH) - endP.y) * invertEndPz / vCorr;
			endVert.z = invertEndPz;

			startUV = getUVCoord(sideL[0], sideL[1], uvSideL[0], uvSideL[1], startVert);
			endUV = getUVCoord(sideR[0], sideR[1], uvSideR[0], uvSideR[1], endVert);

			currentP.y = hg;

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					invertCurrentZ = 1 / zCurrent;
					if (invertCurrentZ < depthBuffer[hg * w + i])
					{
						currentP.x = i;
						currentP.z = zCurrent;

						currentVert.x = (currentP.x - double(halfW)) * invertCurrentZ / hCorr;
						currentVert.y = (double(halfH) - currentP.y) * invertCurrentZ / vCorr;
						currentVert.z = invertCurrentZ;

						illCurrent = 1.0;
						lightVector = unitVector(currentVert);

						distSquared = distanceSquared(currentVert, { 0.0, 0.0, 0.0, 1.0 });
						if (distSquared >= 0.0)
						{
							illCurrent -= 1.0 / dotProductSquared({ 0.0, 0.0, 1.0, 1.0 }, lightVector);
							illCurrent += lightIntensity / distSquared;
						}

						clampValue(&illCurrent, 0.025, maxIllumination);

						sampleUV = this->getUVCoord(startVert, endVert, startUV, endUV, currentVert);

						sampleXnew = ((int)(currentTexture.w * sampleUV.u)) % currentTexture.w;
						sampleYnew = ((int)(currentTexture.h * sampleUV.v)) % currentTexture.h;

						if (sampleYnew < 0) { sampleYnew = currentTexture.h + sampleYnew % currentTexture.h; }
						if (sampleXnew < 0) { sampleXnew = currentTexture.w + sampleXnew % currentTexture.w; }

						if ((sampleXnew != sampleXold) || (sampleYnew != sampleYold))
						{
							finalPixel = currentTexture.pixels[sampleYnew * currentTexture.w + sampleXnew];
						}

						sampleXold = sampleXnew;
						sampleYold = sampleYnew;

						pixelBuffer[hg * w + i] = modifyColour(finalPixel, illCurrent);
						depthBuffer[hg * w + i] = invertCurrentZ;
					}
					zCurrent += deltaZ;
				}
			}
		}
	}
}


inline void Camera::fillTriangleTorchlightSolidColour(const triangle3dV T, const triangle2dG t, Uint32* pixelBuffer, double* depthBuffer, bool d,
	double lightIntensity, double maxIllumination)
{
	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int halfW = (int)(w * 0.5);
	int halfH = (int)(h * 0.5);
	int dx, dy;
	double xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	double zLimit[2] = { 0.0, 0.0 };

	vect3 nullVect = { 0.0, 0.0, 0.0, 0.0 };
	vect3 sides[4] = { nullVect, nullVect, nullVect, nullVect };
	vect3 sideL[2] = { nullVect, nullVect };
	vect3 sideR[2] = { nullVect, nullVect };

	int endIndex;
	int startX, endX;
	double startZ, endZ, zCurrent, illCurrent;
	double h_ratio, v_ratio;
	h_ratio = this->getHRatio();
	v_ratio = this->getVRatio();
	double invertEndPz, invertCurrentZ, invertCurrentPz;
	double hCorr = w * 0.475 * h_ratio;
	double vCorr = w * 0.475 * v_ratio;
	double deltaZ;

	coord2 currentP, startP, endP;
	vect3 currentVert, startVert, endVert;
	textCoord startUV, endUV;
	int sampleXold = 0, sampleYold = 0, sampleXnew = 0, sampleYnew = 0;
	double distSquared;
	vect3 lightVector;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z;
			yy = (double)hg - (double)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (double)dy);
			wd = t.a.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				sides[endIndex * 2] = T.A;
				sides[endIndex * 2 + 1] = T.B;

				endIndex++;
			}
		}
		//Side B-C:
		if ((t.b.y <= hg && t.c.y > hg) || (t.c.y <= hg && t.b.y > hg))
		{
			dx = t.c.x - t.b.x; dy = t.c.y - t.b.y; dz = t.c.z - t.b.z;
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (double)dy);
			wd = t.b.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				sides[endIndex * 2] = T.B;
				sides[endIndex * 2 + 1] = T.C;

				endIndex++;
			}
		}
		//Side C-A:
		if ((t.c.y <= hg && t.a.y > hg) || (t.a.y <= hg && t.c.y > hg))
		{
			dx = t.a.x - t.c.x; dy = t.a.y - t.c.y; dz = t.a.z - t.c.z;
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (double)dy);
			wd = t.c.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				sides[endIndex * 2] = T.C;
				sides[endIndex * 2 + 1] = T.A;

				endIndex++;
			}
		}
		if (endIndex == 2)
		{
			if (lineEnd[0] <= lineEnd[1])
			{
				startX = lineEnd[0];
				endX = lineEnd[1];
				startZ = zLimit[0];
				endZ = zLimit[1];
				sideL[0] = sides[0];
				sideL[1] = sides[1];
				sideR[0] = sides[2];
				sideR[1] = sides[3];
			}
			else
			{
				startX = lineEnd[1];
				endX = lineEnd[0];
				startZ = zLimit[1];
				endZ = zLimit[0];
				sideL[0] = sides[2];
				sideL[1] = sides[3];
				sideR[0] = sides[0];
				sideR[1] = sides[1];
			}
			int span = abs(endX - startX + 1);
			deltaZ = (endZ - startZ) / (double)span;
			zCurrent = startZ;
			
			currentP.y = hg;

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					invertCurrentZ = 1 / zCurrent;
					if (invertCurrentZ < depthBuffer[hg * w + i])
					{
						currentP.x = i;
						currentP.z = zCurrent;

						currentVert.x = (currentP.x - double(halfW)) * invertCurrentZ / hCorr;
						currentVert.y = (double(halfH) - currentP.y) * invertCurrentZ / vCorr;
						currentVert.z = invertCurrentZ;

						illCurrent = 1.0;
						lightVector = unitVector(currentVert);

						distSquared = distanceSquared(currentVert, { 0.0, 0.0, 0.0, 1.0 });
						if (distSquared >= 0.0)
						{
							illCurrent -= 1.0 / dotProductSquared({ 0.0, 0.0, 1.0, 1.0 }, lightVector);
							illCurrent += lightIntensity / distSquared;
						}

						clampValue(&illCurrent, 0.025, maxIllumination);

						pixelBuffer[hg * w + i] = modifyColour(t.h, illCurrent);
						depthBuffer[hg * w + i] = invertCurrentZ;
					}
					zCurrent += deltaZ;
				}
			}
		}
	}
}


inline void Camera::fillZBuffer(const triangle3dV T, const triangle2dG t, double* depthBuffer)
{
	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int halfW = (int)(w * 0.5);
	int halfH = (int)(h * 0.5);
	int dx, dy;
	double xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	double zLimit[2] = { 0.0, 0.0 };

	int endIndex;
	int startX, endX;
	double startZ, endZ, zCurrent;
	double h_ratio, v_ratio;
	h_ratio = this->getHRatio();
	v_ratio = this->getVRatio();
	double invertCurrentZ;
	double hCorr = w * 0.475 * h_ratio;
	double vCorr = w * 0.475 * v_ratio;
	double deltaZ;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z;
			yy = (double)hg - (double)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (double)dy);
			wd = t.a.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				endIndex++;
			}
		}
		//Side B-C:
		if ((t.b.y <= hg && t.c.y > hg) || (t.c.y <= hg && t.b.y > hg))
		{
			dx = t.c.x - t.b.x; dy = t.c.y - t.b.y; dz = t.c.z - t.b.z;
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (double)dy);
			wd = t.b.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				endIndex++;
			}
		}
		//Side C-A:
		if ((t.c.y <= hg && t.a.y > hg) || (t.a.y <= hg && t.c.y > hg))
		{
			dx = t.a.x - t.c.x; dy = t.a.y - t.c.y; dz = t.a.z - t.c.z;
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (double)dy);
			wd = t.c.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				endIndex++;
			}
		}
		if (endIndex == 2)
		{
			if (lineEnd[0] <= lineEnd[1])
			{
				startX = lineEnd[0];
				endX = lineEnd[1];
				startZ = zLimit[0];
				endZ = zLimit[1];
			}
			else
			{
				startX = lineEnd[1];
				endX = lineEnd[0];
				startZ = zLimit[1];
				endZ = zLimit[0];
			}
			int span = abs(endX - startX + 1);
			deltaZ = (endZ - startZ) / (double)span;
			zCurrent = startZ;

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					invertCurrentZ = 1 / zCurrent;
					if (invertCurrentZ < depthBuffer[hg * w + i])
					{
						depthBuffer[hg * w + i] = invertCurrentZ;
					}
					zCurrent += deltaZ;
				}
			}
		}
	}
}


inline void Camera::fillTriangleTest(const triangle3dV T, const triangle2dG t, Uint32* pixelBuffer, double* depthBuffer, bool d)
{
	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int halfW = (int)(w * 0.5);
	int halfH = (int)(h * 0.5);
	int dx, dy;
	double dIllum, illum;
	double xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	double zLimit[2] = { 0.0, 0.0 };
	double illLimit[2] = { 0.0, 0.0 };

	vect3 nullVect = { 0.0, 0.0, 0.0, 0.0 };
	vect3 sides[4] = { nullVect, nullVect, nullVect, nullVect };
	vect3 sideL[2] = { nullVect, nullVect };
	vect3 sideR[2] = { nullVect, nullVect };
	textCoord nullCoord = { 0.0, 0.0 };
	textCoord uvSides[4] = { nullCoord, nullCoord, nullCoord, nullCoord };
	textCoord uvSideL[2] = { nullCoord, nullCoord };
	textCoord uvSideR[2] = { nullCoord, nullCoord };

	int endIndex;
	int startX, endX;
	double startZ, endZ, zCurrent, startIll, endIll, illCurrent;
	double h_ratio, v_ratio;
	h_ratio = this->getHRatio();
	v_ratio = this->getVRatio();
	double invertStartPz, invertEndPz, invertCurrentZ;
	double hCorr = w * 0.475 * h_ratio;
	double vCorr = w * 0.475 * v_ratio;
	double deltaZ, deltaIll;

	coord2 currentP, startP, endP;
	vect3 startVert, endVert;
	textCoord startUV, endUV, sampleUV;
	int sampleXold = 0, sampleYold = 0, sampleXnew = 0, sampleYnew = 0;
	Uint32 finalPixel;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z; dIllum = t.illumB - t.illumA;
			yy = (double)hg - (double)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (double)dy);
			illum = t.illumA + dIllum * (yy / (double)dy);
			wd = t.a.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;

				sides[endIndex * 2] = T.A;
				sides[endIndex * 2 + 1] = T.B;
				uvSides[endIndex * 2] = t.At;
				uvSides[endIndex * 2 + 1] = t.Bt;

				endIndex++;
			}
		}
		//Side B-C:
		if ((t.b.y <= hg && t.c.y > hg) || (t.c.y <= hg && t.b.y > hg))
		{
			dx = t.c.x - t.b.x; dy = t.c.y - t.b.y; dz = t.c.z - t.b.z; dIllum = t.illumC - t.illumB;
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (double)dy);
			illum = t.illumB + dIllum * (yy / (double)dy);
			wd = t.b.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;

				sides[endIndex * 2] = T.B;
				sides[endIndex * 2 + 1] = T.C;
				uvSides[endIndex * 2] = t.Bt;
				uvSides[endIndex * 2 + 1] = t.Ct;

				endIndex++;
			}
		}
		//Side C-A:
		if ((t.c.y <= hg && t.a.y > hg) || (t.a.y <= hg && t.c.y > hg))
		{
			dx = t.a.x - t.c.x; dy = t.a.y - t.c.y; dz = t.a.z - t.c.z; dIllum = t.illumA - t.illumC;
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (double)dy);
			illum = t.illumC + dIllum * (yy / (double)dy);
			wd = t.c.x + int(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;

				sides[endIndex * 2] = T.C;
				sides[endIndex * 2 + 1] = T.A;
				uvSides[endIndex * 2] = t.Ct;
				uvSides[endIndex * 2 + 1] = t.At;

				endIndex++;
			}
		}
		if (endIndex == 2)
		{
			if (lineEnd[0] <= lineEnd[1])
			{
				startX = lineEnd[0];
				endX = lineEnd[1];
				startZ = zLimit[0];
				endZ = zLimit[1];
				startIll = illLimit[0];
				endIll = illLimit[1];

				sideL[0] = sides[0];
				sideL[1] = sides[1];
				sideR[0] = sides[2];
				sideR[1] = sides[3];

				uvSideL[0] = uvSides[0];
				uvSideL[1] = uvSides[1];
				uvSideR[0] = uvSides[2];
				uvSideR[1] = uvSides[3];
			}
			else
			{
				startX = lineEnd[1];
				endX = lineEnd[0];
				startZ = zLimit[1];
				endZ = zLimit[0];
				startIll = illLimit[1];
				endIll = illLimit[0];

				sideL[0] = sides[2];
				sideL[1] = sides[3];
				sideR[0] = sides[0];
				sideR[1] = sides[1];

				uvSideL[0] = uvSides[2];
				uvSideL[1] = uvSides[3];
				uvSideR[0] = uvSides[0];
				uvSideR[1] = uvSides[1];
			}
			int span = abs(endX - startX + 1);
			deltaZ = (endZ - startZ) / (double)span;
			deltaIll = ((endIll - startIll) / (double)span) / 100.0;
			zCurrent = startZ;
			illCurrent = startIll / 100.0;

			startP.x = startX;	startP.y = hg;	startP.z = startZ;
			endP.x = endX;		endP.y = hg;	endP.z = endZ;

			invertStartPz = 1 / startP.z;
			startVert.x = (startP.x - double(halfW)) * invertStartPz / hCorr;
			startVert.y = (double(halfH) - startP.y) * invertStartPz / vCorr;
			startVert.z = invertStartPz;

			invertEndPz = 1 / endP.z;
			endVert.x = (endP.x - double(halfW)) * invertEndPz / hCorr;
			endVert.y = (double(halfH) - endP.y) * invertEndPz / vCorr;
			endVert.z = invertEndPz;

			startUV = getUVCoord(sideL[0], sideL[1], uvSideL[0], uvSideL[1], startVert);
			endUV = getUVCoord(sideR[0], sideR[1], uvSideR[0], uvSideR[1], endVert);

			currentP.y = hg;

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					invertCurrentZ = 1 / zCurrent;
					if (invertCurrentZ < depthBuffer[hg * w + i])
					{
						sampleUV.u = startUV.u + ((endUV.u - startUV.u) / ((double)endX - (double)startX)) * i;
						sampleUV.v = startUV.v + ((endUV.v - startUV.v) / ((double)endX - (double)startX)) * i;

						sampleXnew = ((int)(currentTexture.w * sampleUV.u)) % currentTexture.w;
						sampleYnew = ((int)(currentTexture.h * sampleUV.v)) % currentTexture.h;

						if (sampleXnew < 0) { sampleXnew = currentTexture.w + sampleXnew % currentTexture.w; }
						if (sampleYnew < 0) { sampleYnew = currentTexture.h + sampleYnew % currentTexture.h; }

						if ((sampleXnew != sampleXold) || (sampleYnew != sampleYold))
						{
							if ((sampleXnew >= 0 && sampleXnew < currentTexture.w) && (sampleYnew >= 0 && sampleYnew < currentTexture.h))
							{
								finalPixel = currentTexture.pixels[sampleYnew * currentTexture.w + sampleXnew];
							}
							else
							{
								if (sampleXnew < 0)
								{
									finalPixel = getColour(0, 255, 127, 0);
								}
								if (sampleYnew < 0)
								{
									finalPixel = getColour(0, 127, 127, 255);
								}
							}
						}

						sampleXold = sampleXnew;
						sampleYold = sampleYnew;

						pixelBuffer[hg * w + i] = modifyColour(finalPixel, illCurrent);
						depthBuffer[hg * w + i] = invertCurrentZ;
					}
					zCurrent += deltaZ;
					illCurrent += deltaIll;
				}
			}
		}
	}
}


inline void Camera::drawLine(coord2 startP, coord2 endP, Uint32 colour, Uint32* pixelBuffer)
{
	/*No clipping takes place in this method, if either startP or endP
	are off screen, line is discarded entirely. It is assumed that all
	lines form sides of triangles already clipped to the view frustum*/

	/*Check if line endpoints differ, and if so, see how much they do*/
	bool diffX = (endP.x - startP.x == 0) ? false : true;
	bool diffY = (endP.y - startP.y == 0) ? false : true;
	int deltaX = endP.x - startP.x;
	int deltaY = endP.y - startP.y;

	int x1, y1, x2, y2;

	if (!diffX && !diffY)
	{
		/*No extent - draw single pixel*/
		int xCurrent = startP.x;
		int yCurrent = startP.y;
		if ((xCurrent >= 0 && xCurrent < w) && (yCurrent >= 0 && yCurrent < h))
		{
			pixelBuffer[yCurrent * w + xCurrent] = colour;
		}
	}
	if (!diffX && diffY)
	{
		/*No 'X' extent - draw vertical columns of pixels*/
		if (startP.y < endP.y)
		{
			x1 = startP.x;	y1 = startP.y;
			x2 = endP.x;	y2 = endP.y;
		}
		else
		{
			x1 = endP.x;	y1 = endP.y;
			x2 = startP.x;	y2 = startP.y;
		}
		for (int xCurrent = x1, yCurrent = y1; yCurrent < y2; yCurrent++)
		{
			if ((xCurrent >= 0 && xCurrent < w) && (yCurrent >= 0 && yCurrent < h))
			{
				pixelBuffer[yCurrent * w + xCurrent] = colour;
			}
		}
	}
	if (!diffY && diffX)
	{
		/*No 'Y' extent - draw horizontal row of pixels*/
		if (startP.x < endP.x)
		{
			x1 = startP.x;	y1 = startP.y;
			x2 = endP.x;	y2 = endP.y;
		}
		else
		{
			x1 = endP.x;	y1 = endP.y;
			x2 = startP.x;	y2 = startP.y;
		}
		for (int xCurrent = x1, yCurrent = y1; xCurrent < x2; xCurrent++)
		{
			if ((xCurrent >= 0 && xCurrent < w) && (yCurrent >= 0 && yCurrent < h))
			{
				pixelBuffer[yCurrent * w + xCurrent] = colour;
			}
		}
	}
	if (diffX && diffY)
	{
		/*Line extends both in the 'X' and 'Y' directions*/
		if (abs(deltaX) == abs(deltaY))
		{
			/*Line extends equally in both directions*/
			int stepY = 0;
			if (startP.x < endP.x)
			{
				x1 = startP.x;	y1 = startP.y;
				x2 = endP.x;	y2 = endP.y;
				stepY = (startP.y < endP.y) ? 1 : -1;
			}
			else
			{
				x1 = endP.x;	y1 = endP.y;
				x2 = startP.x;	y2 = startP.y;
				stepY = (startP.y < endP.y) ? -1 : 1;
			}
			for (int xCurrent = x1, yCurrent = y1; xCurrent < x2; xCurrent++, yCurrent += stepY)
			{
				if ((xCurrent >= 0 && xCurrent < w) && (yCurrent >= 0 && yCurrent < h))
				{
					pixelBuffer[yCurrent * w + xCurrent] = colour;
				}
			}
		}
		else
		{
			/*Generic line*/
			if (abs(deltaX) >= abs(deltaY))
			{
				/*Line is 'X'-major*/
				int stepY = 0, xCount = 0, yCount = 0;
				if (startP.x < endP.x)
				{
					x1 = startP.x;	y1 = startP.y;
					x2 = endP.x;	y2 = endP.y;
					stepY = (startP.y < endP.y) ? 1 : -1;
				}
				else
				{
					x1 = endP.x;	y1 = endP.y;
					x2 = startP.x;	y2 = startP.y;
					stepY = (startP.y < endP.y) ? -1 : 1;
				}
				for (int xCurrent = x1; xCurrent < x2; xCurrent++, xCount++)
				{
					int yCurrent = y1 + yCount;
					if ((xCurrent >= 0 && xCurrent < w) && (yCurrent >= 0 && yCurrent < h))
					{
						pixelBuffer[yCurrent * w + xCurrent] = colour;
					}
					float yIdeal = ((float)deltaY / (float)deltaX) * xCount;
					if (abs(yIdeal - yCount) > 0.5f) { yCount += stepY; }
				}
			}
			else
			{
				/*Line is 'Y'-major*/
				int stepX = 0, xCount = 0, yCount = 0;
				if (startP.y < endP.y)
				{
					x1 = startP.x;	y1 = startP.y;
					x2 = endP.x;	y2 = endP.y;
					stepX = (startP.x < endP.x) ? 1 : -1;
				}
				else
				{
					x1 = endP.x;	y1 = endP.y;
					x2 = startP.x;	y2 = startP.y;
					stepX = (startP.x < endP.x) ? -1 : 1;
				}
				for (int yCurrent = y1; yCurrent < y2; yCurrent++, yCount++)
				{
					int xCurrent = x1 + xCount;
					if ((xCurrent >= 0 && xCurrent < w) && (yCurrent >= 0 && yCurrent < h))
					{
						pixelBuffer[yCurrent * w + xCurrent] = colour;
					}
					float xIdeal = ((float)deltaX / (float)deltaY) * yCount;
					if (abs(xIdeal - xCount) > 0.5f) { xCount += stepX; }
				}
			}
		}
	}
}


void Camera::drawLine(screenCoord startP, screenCoord endP, int step, Uint32 colour, Uint32* pixelBuffer)
{
	/*No clipping takes place in this method, if either startP or endP
	are off screen, line is discarded entirely. It is assumed that all
	lines form sides of triangles already clipped to the view frustum*/

	/*Check if line endpoints differ, and if so, see how much they do*/
	bool diffX = (endP.x - startP.x == 0) ? false : true;
	bool diffY = (endP.y - startP.y == 0) ? false : true;
	int deltaX = endP.x - startP.x;
	int deltaY = endP.y - startP.y;

	int x1, y1, x2, y2;

	if (!diffX && !diffY)
	{
		/*No extent - draw single pixel*/
		int xCurrent = startP.x;
		int yCurrent = startP.y;
		if ((xCurrent >= 0 && xCurrent < w) && (yCurrent >= 0 && yCurrent < h))
		{
			pixelBuffer[yCurrent * w + xCurrent] = colour;
		}
	}
	if (!diffX && diffY)
	{
		/*No 'X' extent - draw vertical columns of pixels*/
		if (startP.y < endP.y)
		{
			x1 = startP.x;	y1 = startP.y;
			x2 = endP.x;	y2 = endP.y;
		}
		else
		{
			x1 = endP.x;	y1 = endP.y;
			x2 = startP.x;	y2 = startP.y;
		}
		for (int xCurrent = x1, yCurrent = y1; yCurrent < y2; yCurrent += step)
		{
			if ((xCurrent >= 0 && xCurrent < w) && (yCurrent >= 0 && yCurrent < h))
			{
				pixelBuffer[yCurrent * w + xCurrent] = colour;
			}
		}
	}
	if (!diffY && diffX)
	{
		/*No 'Y' extent - draw horizontal row of pixels*/
		if (startP.x < endP.x)
		{
			x1 = startP.x;	y1 = startP.y;
			x2 = endP.x;	y2 = endP.y;
		}
		else
		{
			x1 = endP.x;	y1 = endP.y;
			x2 = startP.x;	y2 = startP.y;
		}
		for (int xCurrent = x1, yCurrent = y1; xCurrent < x2; xCurrent += step)
		{
			if ((xCurrent >= 0 && xCurrent < w) && (yCurrent >= 0 && yCurrent < h))
			{
				pixelBuffer[yCurrent * w + xCurrent] = colour;
			}
		}
	}
	if (diffX && diffY)
	{
		/*Line extends both in the 'X' and 'Y' directions*/
		if (abs(deltaX) == abs(deltaY))
		{
			/*Line extends equally in both directions*/
			int stepY = 0;
			if (startP.x < endP.x)
			{
				x1 = startP.x;	y1 = startP.y;
				x2 = endP.x;	y2 = endP.y;
				stepY = (startP.y < endP.y) ? 1 : -1;
			}
			else
			{
				x1 = endP.x;	y1 = endP.y;
				x2 = startP.x;	y2 = startP.y;
				stepY = (startP.y < endP.y) ? -1 : 1;
			}
			for (int xCurrent = x1, yCurrent = y1; xCurrent < x2; xCurrent++, yCurrent += stepY)
			{
				if ((xCurrent >= 0 && xCurrent < w) && (yCurrent >= 0 && yCurrent < h))
				{
					pixelBuffer[yCurrent * w + xCurrent] = colour;
				}
			}
		}
		else
		{
			/*Generic line*/
			if (abs(deltaX) >= abs(deltaY))
			{
				/*Line is 'X'-major*/
				int stepY = 0, xCount = 0, yCount = 0;
				if (startP.x < endP.x)
				{
					x1 = startP.x;	y1 = startP.y;
					x2 = endP.x;	y2 = endP.y;
					stepY = (startP.y < endP.y) ? 1 : -1;
				}
				else
				{
					x1 = endP.x;	y1 = endP.y;
					x2 = startP.x;	y2 = startP.y;
					stepY = (startP.y < endP.y) ? -1 : 1;
				}
				for (int xCurrent = x1; xCurrent < x2; xCurrent++, xCount++)
				{
					int yCurrent = y1 + yCount;
					if ((xCurrent >= 0 && xCurrent < w) && (yCurrent >= 0 && yCurrent < h))
					{
						pixelBuffer[yCurrent * w + xCurrent] = colour;
					}
					float yIdeal = ((float)deltaY / (float)deltaX) * xCount;
					if (abs(yIdeal - yCount) > 0.5f) { yCount += stepY; }
				}
			}
			else
			{
				/*Line is 'Y'-major*/
				int stepX = 0, xCount = 0, yCount = 0;
				if (startP.y < endP.y)
				{
					x1 = startP.x;	y1 = startP.y;
					x2 = endP.x;	y2 = endP.y;
					stepX = (startP.x < endP.x) ? 1 : -1;
				}
				else
				{
					x1 = endP.x;	y1 = endP.y;
					x2 = startP.x;	y2 = startP.y;
					stepX = (startP.x < endP.x) ? -1 : 1;
				}
				for (int yCurrent = y1; yCurrent < y2; yCurrent++, yCount++)
				{
					int xCurrent = x1 + xCount;
					if ((xCurrent >= 0 && xCurrent < w) && (yCurrent >= 0 && yCurrent < h))
					{
						pixelBuffer[yCurrent * w + xCurrent] = colour;
					}
					float xIdeal = ((float)deltaX / (float)deltaY) * yCount;
					if (abs(xIdeal - xCount) > 0.5f) { xCount += stepX; }
				}
			}
		}
	}
}


void Camera::drawSpot(screenCoord P, Uint32 colour, Uint32* pixelBuffer)
{
	const bool spot[16] = { 0, 1, 1, 0,
							1, 1, 1, 1,
							1, 1, 1, 1,
							0, 1, 1, 0 };

	int xStart = P.x - 2;
	int yStart = P.y - 2;
	for (int j = yStart, q = 0; j < yStart + 4; j++, q++)
	{
		for (int i = xStart, p = 0; i < xStart + 4; i++, p++)
		{
			if ((i >= 0 && i < w) && (j >= 0 && j < h) && spot[q * 4 + p])			
			{
				pixelBuffer[j * w + i] = colour;
			}
		}
	}
}


void Camera::updatePosition(double turnH, double turnV, double tiltP, double moveP, double strafeP, double riseP)
{
	azm = -turnH;
	alt = -turnV;
	rol = tiltP;

	x -= moveP * cos(azm) - strafeP * cos(azm + PI * 0.5);
	y += moveP * sin(azm) - strafeP * sin(azm + PI * 0.5);
	z += riseP;
}


void Camera::centreLook()
{
	alt = 0.0;
}


void Camera::outputImage(Canvas screen)
{
	FILE* f = fopen("screenshot_scanline.ppm", "w");
	if (f == NULL)
	{
		printf("Could not write to file...\n");
	}
	else
	{
		fprintf(f, "P3\n");
		fprintf(f, "%d %d\n", w, h);
		fprintf(f, "255\n");

		byte r, g, b;
		Uint32 colour;
		for (int i = 0; i < w * h; i++)
		{
			colour = screen.pixelBuffer[i];
			r = colour >> 16	& 0xFF;
			g = colour >> 8		& 0xFF;
			b = colour			& 0xFF;
			fprintf(f, "%d %d %d\n", r, g, b);
		}

		fclose(f);
	}
}