#include "Definitions.h"
#include "Camera.h"
#include "LightSource.h"
#include "ViewVolume.h"
#include "Canvas.h"
#include <math.h>
#include <stdio.h>

#define PI					3.141592654

Camera::Camera()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;

	step = 1.0;

	azm = 0.0;
	alt = 0.0;
	rol = 0.0;

	speed = { 0.0, 0.0, 0.0, 0.0 };
	timeToSpendMidAir = 0;

	fovH = PI * 0.5 / 90 * 85;
	zNear = 1.0;
	zFar = 99.9;

	w = 320;
	h = 200;
}


Camera::Camera(double cx, double cy, double cz, int width, int height)
{
	x = cx;
	y = cy;
	z = cz;

	step = 1.0;

	azm = 0.0;
	alt = 0.0;
	rol = 0.0;

	speed = { 0.0, 0.0, 0.0, 0.0 };
	timeToSpendMidAir = 0;

	fovH = 90.0;
	zNear = 1.0;
	zFar = 999.0;

	w = width;
	h = height;
}


Camera::Camera(double cx, double cy, double cz, double stp, double az, double al, double rl,
	double fov, double nr, double fr, int width, int height)
{
	x = cx;
	y = cy;
	z = cz;

	step = stp;

	azm = az;
	alt = al;
	rol = rl;

	speed = { 0.0, 0.0, 0.0, 0.0 };
	timeToSpendMidAir = 0;

	fovH = fov;

	zNear = nr;
	zFar = fr;

	w = width;
	h = height;
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


bool Camera::polyFacingCamera(triangle3dV testV)
{
	bool polygonVisible;

	vect3 eyeVector = subVectors(testV.A, { x, y, z, 1.0 });

	if (dotProduct(testV.N, eyeVector) < 0) { polygonVisible = true; }
	else { polygonVisible = false; }

	return polygonVisible;
}


triangle3dV Camera::world2viewT(transform3d T, triangle3dV tr)
{
	triangle3dV t;

	t.A = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr.A))));
	t.B = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr.B))));
	t.C = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr.C))));
	//t.An = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr.An))));
	//t.Bn = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr.Bn))));
	//t.Cn = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr.Cn))));
	//t.N = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr.N))));

	return t;
}


int Camera::clipToFrustum(triangle3dV viewT, vect3 * vertList, ViewVolume f)
{
	for (int v = 0; v < MAXCLIPVERTS; v++)
	{
		vertList[v] = { 0.0, 0.0, 0.0, 0.0 };
	}

	vertList[0] = viewT.A;
	vertList[1] = viewT.B;
	vertList[2] = viewT.C;

	int nVert = 3;
	plane currentPlane;

	currentPlane = f.getNearPlane();
	this->clipPoly(&nVert, vertList, currentPlane);
	currentPlane = f.getTopPlane();
	this->clipPoly(&nVert, vertList, currentPlane);
	currentPlane = f.getBottomPlane();
	this->clipPoly(&nVert, vertList, currentPlane);
	currentPlane = f.getLeftPlane();
	this->clipPoly(&nVert, vertList, currentPlane);
	currentPlane = f.getRightPlane();
	this->clipPoly(&nVert, vertList, currentPlane);
	currentPlane = f.getFarPlane();
	this->clipPoly(&nVert, vertList, currentPlane);

	return nVert;
}


void Camera::clipPoly(int* nVert, vect3 * vertList, plane clippingPlane)
{
	int nResult = 0;
	vect3 vTemp[MAXCLIPVERTS];

	for (int i = 0; i < *nVert; i++)
	{
		if (i < (*nVert) - 1) { this->clipEdge(clippingPlane, vertList[i], vertList[i + 1], &nResult, vTemp); }
		else { this->clipEdge(clippingPlane, vertList[i], vertList[0], &nResult, vTemp); }
	}
	for (int j = 0; j < nResult; j++)
	{
		vertList[j] = vTemp[j];
	}

	*nVert = nResult;
}


/*void Camera::clipPoly(int* nVert, vect3* vertList, plane clippingPlane)
{
	int nResult = 0;
	vect3 vTemp[MAXCLIPVERTS];
	for (int v = 0; v < MAXCLIPVERTS; v++)
	{
		vTemp[v] = { 0.0, 0.0, 0.0, 0.0 };
	}
	for (int i = 0; i < *nVert; i++)
	{
		if (i < (*nVert) - 1)
		{
			this->clipEdge(clippingPlane, vertList[i], vertList[i + 1], &nResult, vTemp);
		}
		else
		{
			this->clipEdge(clippingPlane, vertList[i], vertList[0], &nResult, vTemp);
		}
	}
	for (int j = 0; j < nResult; j++)
	{
		vertList[j] = vTemp[j];
	}
	if (nResult < MAXCLIPVERTS)
	{
		for (int k = nResult; k < MAXCLIPVERTS; k++)
		{
			vertList[k] = { 0.0, 0.0, 0.0, 0.0 };
		}
	}
	for (int m = 0; m < MAXCLIPVERTS; m++)
	{
		vTemp[m] = { 0.0, 0.0, 0.0, 0.0 };
	}
	* nVert = nResult;
}*/


void Camera::clipEdge(plane p, vect3 startV, vect3 endV, int* nResult, vect3 * vTemp)
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
		//if (sign(sStart) == -1)
		if(sStart < 0)
		{
			if (dist)
			{
				t = (dist - sEnd) / dist;

				vTemp[*nResult].x = startV.x + t * (endV.x - startV.x);
				vTemp[*nResult].y = startV.y + t * (endV.y - startV.y);
				vTemp[*nResult].z = startV.z + t * (endV.z - startV.z);
				vTemp[*nResult].w = startV.w + t * (endV.w - startV.w);
				(*nResult)++;
			}
		}
		//if (sign(sEnd) == -1)
		if(sEnd < 0)
		{
			if (dist)
			{
				t = (-dist - sStart) / dist;

				vTemp[*nResult].x = endV.x - t * (startV.x - endV.x);
				vTemp[*nResult].y = endV.y - t * (startV.y - endV.y);
				vTemp[*nResult].z = endV.z - t * (startV.z - endV.z);
				vTemp[*nResult].w = endV.w - t * (startV.w - endV.w);
				(*nResult)++;
			}
			else
			{

				vTemp[*nResult].x = endV.x;
				vTemp[*nResult].y = endV.y;
				vTemp[*nResult].z = endV.z;
				vTemp[*nResult].w = endV.w;
				(*nResult)++;
			}
		}
		else
		{

			vTemp[*nResult].x = endV.x;
			vTemp[*nResult].y = endV.y;
			vTemp[*nResult].z = endV.z;
			vTemp[*nResult].w = endV.w;
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
			(*nResult)++;
		}
	}
}


void Camera::illuminatePoly(LightSource * L, triangle3dV * T, triangle3dV P, bool G)
{
	int nLight = sizeof(*L) / sizeof(LightSource);
	double illumAll, illumA, illumB, illumC;
	illumAll = illumA = illumB = illumC = 0.0;
	vect3 n;

	if (G)
	{
		n = P.An;
		for (int p = 0; p < nLight; p++)
		{
			illumA = L[p].getIllumination(n);
		}
		clampValue(&illumA, 0.05, 1.0);
		T->A.w = illumA;

		n = P.Bn;
		for (int p = 0; p < nLight; p++)
		{
			illumB = L[p].getIllumination(n);
		}
		clampValue(&illumB, 0.05, 1.0);
		T->B.w = illumB;

		n = P.Cn;
		for (int p = 0; p < nLight; p++)
		{
			illumC = L[p].getIllumination(n);
		}
		clampValue(&illumC, 0.05, 1.0);
		T->C.w = illumC;
	}
	else
	{
		n = P.N;
		for (int p = 0; p < nLight; p++)
		{
			illumAll = L[p].getIllumination(n);
		}
		clampValue(&illumAll, 0.05, 1.0);
		T->A.w = T->B.w = T->C.w = illumAll;
	}

}


coord2 Camera::view2screen(vect3 vertex, double hR, double vR)
{
	coord2 pixel;

	pixel.x = (int)(w * 0.5 + (vertex.x / vertex.z) * w * 0.5 * hR * 0.95);
	pixel.y = (int)(h * 0.5 - (vertex.y / vertex.z) * w * 0.5 * vR * 0.95);
	pixel.z = 1 / vertex.z;

	return pixel;
}


void Camera::projectPoly(int n, vect3 * vertexList, Uint32 colour, Canvas screen, double hRatio, double vRatio, bool g, bool d)
{
	triangle2dG screenT;

	for (int i = 0; i < n - 2; i++)
	{
		screenT.a = this->view2screen(vertexList[0], hRatio, vRatio);
		screenT.b = this->view2screen(vertexList[1 + i], hRatio, vRatio);
		screenT.c = this->view2screen(vertexList[2 + i], hRatio, vRatio);

		screenT.illumA = vertexList[0].w;
		screenT.illumB = vertexList[1 + i].w;
		screenT.illumC = vertexList[2 + i].w;

		screenT.h = colour;

		if (g)
		{
			this->fillTriangle(screenT, screen, d);
		}
		else
		{
			this->fillTriangleZ(screenT, screen, d);
		}


	}
}


void Camera::fillTriangle(triangle2dG t, Canvas screen, bool d)
{
	int yMin, yMax;

	yMin = t.a.y;
	if (t.b.y < yMin) { yMin = t.b.y; }
	if (t.c.y < yMin) { yMin = t.c.y; }

	yMax = t.a.y;
	if (t.b.y > yMax) { yMax = t.b.y; }
	if (t.c.y > yMax) { yMax = t.c.y; }

	int wd = 0;
	int dx, dy, startX, endX, span, endIndex, bufferIndex;
	int lineEnd[2] = { 0, 0 };
	double xx, yy, zz, dz, dIllum, illum, startZ, endZ, zCurrent, startIll, endIll, illCurrent, diffZ, diffIll;
	double zLimit[2] = { 0.0, 0.0 };
	double illLimit[2] = { 0.0, 0.0 };

	byte r, g, b;
	r = ((byte)(t.h >> 16 & 0xFF));
	g = ((byte)(t.h >> 8 & 0xFF));
	b = ((byte)(t.h & 0xFF));

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side 0-1:
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
				endIndex++;
			}
		}
		//Side 1-2:
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
				endIndex++;
			}
		}
		//Side 2-0:
		if ((t.c.y <= hg && t.a.y > hg) || (t.a.y <= hg && t.c.y > hg))
		{
			dx = t.a.x - t.c.x; dy = t.a.y - t.c.y; dz = t.a.z - t.c.z; dIllum = t.illumA - t.illumC;
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy/ (double)dy);
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
			span = abs(endX - startX + 1);

			diffZ = (endZ - startZ) / (double)span;
			diffIll = (endIll - startIll) / (double)span;

			for (int i = startX; i < endX + 1; i++)
			{	
				zCurrent = startZ + diffZ * ((double)i - (double)startX);		//Z-buffering happens in this block
				illCurrent = (startIll + diffIll * ((double)i - (double)startX)) / 100.0;
				if (1 / zCurrent < screen.depthBuffer[hg * w + i])				
				{
					if (d)
					{
						//screen.pixelBuffer[hg * w + i] = getColour(0, (byte)(255 * zCurrent), (byte)(255 * zCurrent), (byte)(255 * zCurrent));						
						screen.pixelBuffer[hg * w + i] = (0 << 24) | ((byte)(255 * zCurrent) << 16) |
																	  ((byte)(255 * zCurrent) << 8) | (byte)(255 * zCurrent);

					}
					else
					{
						//screen.pixelBuffer[hg * w + i] = getColour(0, (byte)(r * illCurrent), (byte)(g * illCurrent), (byte)(b * illCurrent));						
						screen.pixelBuffer[hg * w + i] = (0 << 24) | ((byte)(r * illCurrent) << 16) |
																	   ((byte)(g* illCurrent) << 8) | (byte)(b * illCurrent);
					}
					screen.depthBuffer[hg * w + i] = 1 / zCurrent;
				}				
			}
		}
	}
}


/*void Camera::fillTriangle(triangle2dG t, Canvas screen, bool d)
{
	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int dx, dy;
	double dIllum, illum;
	double xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	double zLimit[2] = { 0.0, 0.0 };
	double illLimit[2] = { 0.0, 0.0 };
	int endIndex;
	int startX, endX;
	double startZ, endZ, zCurrent, startIll, endIll, illCurrent;

	byte r, g, b;
	r = ((byte)(t.h >> 16 & 0xFF));
	g = ((byte)(t.h >> 8 & 0xFF));
	b = ((byte)(t.h & 0xFF));

	double corr = 0.0;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side 0-1:
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
		//Side 1-2:
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
		//Side 2-0:
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
			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					zCurrent = startZ + ((endZ - startZ) / (double)span) * ((double)i - (double)startX);		//Z-buffering happens in this block
					illCurrent = (startIll + ((endIll - startIll) / (double)span) * ((double)i - (double)startX)) / 100.0;
					//clampValue(&illCurrent, 0.125, 1.0);
					if (1 / zCurrent < screen.depthBuffer[hg * w + i])
					{
						if (d)
						{
							screen.pixelBuffer[hg * w + i] = getColour(0, (byte)(255 * zCurrent), (byte)(255 * zCurrent), (byte)(255 * zCurrent));
						}
						else
						{
							screen.pixelBuffer[hg * w + i] = getColour(0, (byte)(r * illCurrent), (byte)(g * illCurrent), (byte)(b * illCurrent));
						}
						screen.depthBuffer[hg * w + i] = 1 / zCurrent;
					}
				}
			}
		}
	}
}*/


void Camera::fillTriangleZ(triangle2dG t, Canvas screen, bool d)
{
	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int dx, dy;
	double xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	double zLimit[2] = { 0, 0 };
	int endIndex;
	int startX, endX;
	double startZ, endZ, zCurrent, illCurrent;

	byte r, g, b;
	r = ((byte)(t.h >> 16 & 0xFF));
	g = ((byte)(t.h >> 8 & 0xFF));
	b = ((byte)(t.h & 0xFF));

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side 0-1:
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
		//Side 1-2:
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
		//Side 2-0:
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
			int hgw = hg * w;
			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					zCurrent = startZ + ((endZ - startZ) / (double)span) * ((double)i - (double)startX);
					//zCurrent = startZ + ((endZ - startZ) / (double)wd) * (double)i;		//Z-buffering happens in this block
					if (1 / zCurrent < screen.depthBuffer[hg * w + i])
					{
						illCurrent = (t.illumA + t.illumB + t.illumC) / 3;

						if (d)
						{
							screen.pixelBuffer[hgw + i] = getColour(0, (byte)(255 * zCurrent), (byte)(255 * zCurrent), (byte)(255 * zCurrent));
						}
						else
						{
							screen.pixelBuffer[hgw + i] = getColour(0, (byte)(r * illCurrent), (byte)(g * illCurrent), (byte)(b * illCurrent));
						}

						screen.depthBuffer[hgw + i] = 1 / zCurrent;
					}
				}
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


void Camera::jump(int n, triangle3dV * S)
{
	double supportLevel = this->getSupport(n, S);
	if (z <= supportLevel)
	{
		speed.z = 3.0;
	}
}


void Camera::sufferFromGravity(int n, triangle3dV * S)
{
	double supportLevel = this->getSupport(n, S);
	if (z > supportLevel)
	{
		speed.z -= GRAVITY;
	}

	if (speed.z > 0)
	{
		z += speed.z;
	}
	else if (speed.z < 0)
	{
		if (z > supportLevel)
		{
			z += speed.z;
		}
	}
}


double Camera::getSupport(int n, triangle3dV * S)
{
	double hSupport = 5.0;

	vect3 p = { x, y, z, 0.0 };
	for (int i = 0; i < n; i++)
	{
		vect3 a = { S[i].B.x - S[i].A.x, S[i].B.y - S[i].A.y, 0.0, 0.0 };
		vect3 b = { S[i].C.x - S[i].B.x, S[i].C.y - S[i].B.y, 0.0, 0.0 };
		vect3 c = { S[i].A.x - S[i].C.x, S[i].A.y - S[i].C.y, 0.0, 0.0 };
		vect3 aN = { -a.y, a.x, 0.0, 0.0 };
		vect3 bN = { -b.y, b.x, 0.0, 0.0 };
		vect3 cN = { -c.y, c.x, 0.0, 0.0 };
		vect3 pA = subVectors(S[i].A, p);
		vect3 pB = subVectors(S[i].B, p);
		vect3 pC = subVectors(S[i].C, p);
		double dotA, dotB, dotC;
		dotA = dotProduct(pA, aN);
		dotB = dotProduct(pB, bN);
		dotC = dotProduct(pC, cN);
		if (dotA < 0.0 && dotB < 0.0 && dotC < 0.0)
		{
			if ((S[i].A.z + S[i].B.z + S[i].C.z) / 3.0 <= z)
			{
				hSupport = (S[i].A.z + S[i].B.z + S[i].C.z) / 3.0 + 10.0;
			}
		}
	}

	return hSupport;
}

