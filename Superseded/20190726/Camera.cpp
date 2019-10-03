#define _CRT_SECURE_NO_DEPRECATE

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

	fovH = PI * 0.5 / 90 * 85;
	zNear = 1.0;
	zFar = 99.9;

	w = 320;
	h = 200;
	scale = 0;
}


Camera::Camera(double cx, double cy, double cz, int width, int height, int s)
{
	x = cx;
	y = cy;
	z = cz;

	step = 1.0;

	azm = 0.0;
	alt = 0.0;
	rol = 0.0;

	speed = { 0.0, 0.0, 0.0, 0.0 };

	fovH = 90.0;
	zNear = 1.0;
	zFar = 999.0;

	w = width;
	h = height;
	scale = s;
}


Camera::Camera(double cx, double cy, double cz, double stp, double az, double al, double rl,
	double fov, double nr, double fr, int width, int height, int s)
{
	x = cx;
	y = cy;
	z = cz;

	step = stp;

	azm = az;
	alt = al;
	rol = rl;

	speed = { 0.0, 0.0, 0.0, 0.0 };

	fovH = fov;

	zNear = nr;
	zFar = fr;

	w = width;
	h = height;
	scale = s;
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


void Camera::scaleUp()
{
	if (scale < 4) 
	{
		scale++;
		this->updateSize();
	}
}


void Camera::scaleDown()
{
	if (scale > 0)
	{
		scale--;
		this->updateSize();
	}
}


void Camera::updateSize()
{
	w = SCREEN_WIDTH / pow2(scale);
	h = SCREEN_HEIGHT / pow2(scale);
}


bool Camera::polyFacingCamera(triangle3dV testV)
{
	vect3 eyeVector = subVectors(testV.A, { x, y, z, 1.0 });
	return (dotProduct(testV.N, eyeVector) < 0) ? true : false;
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
	t.At = tr.At;
	t.Bt = tr.Bt;
	t.Ct = tr.Ct;
	t.texture = tr.texture;

	return t;
}


int Camera::clipToFrustum(triangle3dV viewT, vect3 * vertList, textCoord * uvList, ViewVolume f)
{
	for (int v = 0; v < MAXCLIPVERTS; v++) { vertList[v] = { 0.0, 0.0, 0.0, 0.0 }; uvList[v] = { 0.0, 0.0 }; }

	vertList[0] = viewT.A;	uvList[0] = viewT.At;
	vertList[1] = viewT.B;	uvList[1] = viewT.Bt;
	vertList[2] = viewT.C;	uvList[2] = viewT.Ct;

	int nVert = 3;
	plane currentPlane;

	currentPlane = f.getNearPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);
	currentPlane = f.getTopPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);
	currentPlane = f.getBottomPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);
	currentPlane = f.getLeftPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);
	currentPlane = f.getRightPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);
	currentPlane = f.getFarPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);

	return nVert;
}


void Camera::clipPoly(int* nVert, vect3 * vertList, textCoord * uvList, plane clippingPlane)
{
	int nResult = 0;
	vect3 vTemp[MAXCLIPVERTS];
	textCoord uvTemp[MAXCLIPVERTS];

	for (int i = 0; i < *nVert; i++)
	{
		if (i < (*nVert) - 1) { this->clipEdge(clippingPlane, vertList[i], vertList[i + 1], uvList[i], uvList[i + 1],
								&nResult, vTemp, uvTemp); }
		else { this->clipEdge(clippingPlane, vertList[i], vertList[0], uvList[i], uvList[0], &nResult, vTemp, uvTemp); }
	}
	for (int j = 0; j < nResult; j++)
	{
		vertList[j] = vTemp[j];
		uvList[j] = uvTemp[j];
	}

	*nVert = nResult;
}


void Camera::clipEdge(plane p, vect3 startV, vect3 endV, textCoord startUV, textCoord endUV, int* nResult, vect3 * vTemp, textCoord * uvTemp)
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


vect3 Camera::screen2view(coord2 pixel, double hR, double vR)
{
	vect3 vertex;

	vertex.x = (pixel.x - w * 0.5) * (1 / pixel.z) / (w * 0.5 * hR * 0.95);
	vertex.y = (h * 0.5 - pixel.y) * (1 / pixel.z) / (w * 0.5 * vR * 0.95);
	vertex.z = 1 / pixel.z;
	vertex.w = 1.0;

	return vertex;
}


void Camera::view2text()
{

}


void Camera::projectPoly(int n, vect3 * vertexList, textCoord * uvList, Uint32 colour, txt textureData, Canvas screen,
						double hRatio, double vRatio, bool g, bool d)
{
	triangle2dG screenT;
	triangle3dV originalT;	//To be pass on to polygon filling routine

	for (int i = 0; i < n - 2; i++)
	{
		screenT.a = this->view2screen(vertexList[0], hRatio, vRatio);
		screenT.b = this->view2screen(vertexList[1 + i], hRatio, vRatio);
		screenT.c = this->view2screen(vertexList[2 + i], hRatio, vRatio);

		originalT.A = vertexList[0];
		originalT.B = vertexList[1 + i];
		originalT.C = vertexList[2 + i];

		screenT.illumA = vertexList[0].w;
		screenT.illumB = vertexList[1 + i].w;
		screenT.illumC = vertexList[2 + i].w;

		screenT.At = uvList[0];
		screenT.Bt = uvList[1 + i];
		screenT.Ct = uvList[2 + i];

		screenT.h = colour;

		if (g)
		{
			this->fillTriangleT(originalT, screenT, textureData, screen, d);
		}
		else
		{
			this->fillTriangleZ(screenT, screen, d);
		}


	}
}


textCoord Camera::getUVCoord(vect3 startV, vect3 endV, textCoord startC, textCoord endC, vect3 testV)
{
	textCoord testC;
	double deltaX, deltaY, deltaZ, length, deltaXt, deltaYt, deltaZt, t, deltaU, deltaV, tx, ty, tz, d;
	deltaX = endV.x - startV.x;
	deltaY = endV.y - startV.y;
	deltaZ = endV.z - startV.z;
	length = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
	deltaXt = testV.x - startV.x;
	deltaYt = testV.y - startV.y;
	deltaZt = testV.z - startV.z;
	t = deltaXt * deltaXt + deltaYt * deltaYt + deltaZt * deltaZt;
	//if (length != 0.0) { d = t / length; }
	if (length != 0.0) { d = sqrt(t / length); }
	else { d = 0.0; }


	/*textCoord testC;
	double deltaX, deltaY, deltaZ, deltaXt, deltaYt, deltaZt, deltaU, deltaV, d;
	deltaX = endV.x - startV.x;
	//deltaY = endV.y - startV.y;
	//deltaZ = endV.z - startV.z;
	deltaXt = testV.x - startV.x;
	//deltaYt = testV.y - startV.y;
	//deltaZt = testV.z - startV.z;

	//if (deltaX != 0.0) { d = deltaXt / deltaX; }
	//else if (deltaY != 0.0) { d = deltaYt / deltaY; }
	//else if (deltaZ != 0.0) { d = deltaZt / deltaZ; }
	//else { d = 0.0; }

	if (deltaX != 0.0) { d = abs(deltaXt / deltaX); }
	else { d = 1.0; }*/

	deltaU = (endC.u - startC.u);
	deltaV = (endC.v - startC.v);

	testC.u = startC.u + d * deltaU;
	testC.v = startC.v + d * deltaV;

	return testC;
}


void Camera::fillTriangleT(triangle3dV T, triangle2dG t, txt textureData, Canvas screen, bool d)
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

	double deltaZ, deltaIll;

	byte r, g, b;
	r = ((byte)(t.h >> 16 & 0xFF));
	g = ((byte)(t.h >> 8 & 0xFF));
	b = ((byte)(t.h & 0xFF));

	coord2 currentP, startP, endP;
	vect3 currentVert, startVert, endVert;
	textCoord startUV, endUV, sampleUV;
	int sampleX, sampleY;
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
			startVert = this->screen2view(startP, this->getHRatio(), this->getVRatio());
			endVert = this->screen2view(endP, this->getHRatio(), this->getVRatio());
			startUV = getUVCoord(sideL[0], sideL[1], uvSideL[0], uvSideL[1], startVert);
			endUV = getUVCoord(sideR[0], sideR[1], uvSideR[0], uvSideR[1], endVert);

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < SCREEN_WIDTH) && (hg >= 0 && hg < SCREEN_HEIGHT))
				{
					if (1 / zCurrent < screen.depthBuffer[hg * w + i])
					{
						if (d)
						{
							for (int p = 0; p < m; p++)
							{
								for (int q = 0; q < m; q++)
								{
									screen.pixelBuffer[(m * hg + p) * SCREEN_WIDTH + m * i + q] = getColour(0, (byte)(255 * zCurrent), (byte)(255 * zCurrent), (byte)(255 * zCurrent));
								}
							}
						}
						else
						{		
							if (textureData.ID > 0)
							{
								currentP.x = i;
								currentP.y = hg;
								currentP.z = zCurrent;
								currentVert = this->screen2view(currentP, this->getHRatio(), this->getVRatio());

								sampleUV = this->getUVCoord(startVert, endVert, startUV, endUV, currentVert);

								sampleX = ((int)(double(textureData.w) * sampleUV.u));
								sampleY = ((int)(double(textureData.h) * sampleUV.v));

								if (sampleX < 0) { sampleX = 0; }
								if (sampleY < 0) { sampleY = 0; }
								if (sampleX >= textureData.w) { sampleX = textureData.w - 1; }
								if (sampleY >= textureData.h) { sampleY = textureData.h - 1; }

								if ((sampleX >= 0 && sampleX < textureData.w) && (sampleY >= 0 && sampleY < textureData.h))
								{
									finalPixel = textureData.pixels[sampleY * textureData.w + sampleX];
								}
								else
								{
									finalPixel = 255;
								}
										
								screen.pixelBuffer[hg * SCREEN_WIDTH + i] = finalPixel;
							}
							else
							{									
								screen.pixelBuffer[hg * SCREEN_WIDTH + i] = getColour(0, (byte)(r * illCurrent), (byte)(g * illCurrent), (byte)(b * illCurrent));
							}
						}
						screen.depthBuffer[hg * w + i] = 1 / zCurrent;
					}
					zCurrent += deltaZ;
					illCurrent += deltaIll;
				}
			}
		}
	}
}


void Camera::fillTriangleS(triangle2dG t, Canvas screen, bool d)
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
				if ((i >= 0 && i < SCREEN_WIDTH) && (hg >= 0 && hg < SCREEN_HEIGHT))
				{
					if (1 / zCurrent < screen.depthBuffer[hg * w + i])
					{
						if (d)
						{					
							for (int p = 0; p < m; p++)
							{
								for (int q = 0; q < m; q++)
								{
									screen.pixelBuffer[(m * hg + p) * SCREEN_WIDTH + m * i + q] = getColour(0, (byte)(255 * zCurrent), (byte)(255 * zCurrent), (byte)(255 * zCurrent));
								}
							}
						}
						else
						{
							for (int p = 0; p < m; p++)
							{
								for(int q = 0; q < m; q++)
								{
									screen.pixelBuffer[(m * hg + p) * SCREEN_WIDTH + m * i + q] = getColour(0, (byte)(r * illCurrent), (byte)(g * illCurrent), (byte)(b * illCurrent));
									currentP.x = m * i + q;
									currentP.y = m * hg + p;
									currentP.z = zCurrent;
									currentV = this->screen2view(currentP, this->getHRatio(), this->getVRatio());

								}								
							}				
						}
						screen.depthBuffer[hg * w + i] = 1 / zCurrent;
					}
					zCurrent += deltaZ;
					illCurrent += deltaIll;
				}
			}
		}
	}
}


void Camera::fillTriangleZ(triangle2dG t, Canvas screen, bool d)
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

	byte r, g, b;
	r = ((byte)(t.h >> 16 & 0xFF));
	g = ((byte)(t.h >> 8 & 0xFF));
	b = ((byte)(t.h & 0xFF));

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
			int hgw = hg * w;
			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					zCurrent = startZ + ((endZ - startZ) / (double)span) * ((double)i - (double)startX);			
					if (1 / zCurrent < screen.depthBuffer[hg * w + i])
					{
						//illCurrent = (t.illumA + t.illumB + t.illumC) / 3;
						illCurrent = t.illumA;

						if (d)
						{							
							for (int p = 0; p < m; p++)
							{
								for (int q = 0; q < m; q++)
								{
									screen.pixelBuffer[(m * hg + p) * SCREEN_WIDTH + m * i + q] = getColour(0, (byte)(255 * zCurrent), (byte)(255 * zCurrent), (byte)(255 * zCurrent));
								}
							}
						}
						else
						{							
							for (int p = 0; p < m; p++)
							{
								for (int q = 0; q < m; q++)
								{
									screen.pixelBuffer[(m * hg + p) * SCREEN_WIDTH + m * i + q] = getColour(0, (byte)(r * illCurrent), (byte)(g * illCurrent), (byte)(b * illCurrent));
								}
							}
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


void Camera::outputImage(Canvas screen)
{
	FILE* f = fopen("screenshot.ppm", "w");
	if (f == NULL)
	{
		printf("Could not write to file...\n");
	}
	else
	{
		fprintf(f, "P3\n");
		fprintf(f, "%d %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
		fprintf(f, "255\n");

		byte r, g, b;
		Uint32 colour;
		for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
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