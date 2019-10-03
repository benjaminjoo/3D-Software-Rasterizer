#include "Definitions.h"


void clampValue(double* value, double lower, double upper)
{
	if (*value < lower)
	{
		*value = lower;
	}
	else if (*value > upper)
	{
		*value = upper;
	}
}


Uint32 getColour(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
{
	return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}


vect3 addVectors(vect3 a, vect3 b)
{
	vect3 temp;

	temp.x = a.x + b.x;
	temp.y = a.y + b.y;
	temp.z = a.z + b.z;
	temp.w = 1.0;

	return temp;
}


vect3 subVectors(vect3 a, vect3 b)
{
	vect3 temp;

	temp.x = a.x - b.x;
	temp.y = a.y - b.y;
	temp.z = a.z - b.z;
	temp.w = 1.0;

	return temp;
}


vect3 unitVector(vect3 v)
{
	vect3 temp;

	double length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	temp.x = v.x / length;
	temp.y = v.y / length;
	temp.z = v.z / length;

	return temp;
}


vect3 dirVector(double azm, double alt)
{
	vect3 temp;

	temp.x = cos(alt * PI / 180.0) * cos(azm * PI / 180.0);
	temp.y = cos(alt * PI / 180.0) * sin(azm * PI / 180.0);
	temp.z = sin(alt * PI / 180.0);
	temp.w = 0.0;

	return temp;
}


double dotProduct(vect3 a, vect3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}


vect3 crossProduct(vect3 a, vect3 b)
{
	vect3 t;

	t.x = a.y * b.z - a.z * b.y;
	t.y = a.z * b.x - a.x * b.z;
	t.z = a.x * b.y - a.y * b.x;
	t.w = 0.0;

	return t;
}


vect3 multiplyMxV(mat4x4 m, vect3 v)
{
	vect3 t;

	t.x = v.x * m._00 + v.y * m._01 + v.z * m._02 + v.w * m._03;
	t.y = v.x * m._10 + v.y * m._11 + v.z * m._12 + v.w * m._13;
	t.z = v.x * m._20 + v.y * m._21 + v.z * m._22 + v.w * m._23;
	t.w = v.x * m._30 + v.y * m._31 + v.z * m._32 + v.w * m._33;

	return t;
}


vect3 multiplyVxM(mat4x4 m, vect3 v)
{
	vect3 t;

	t.x = v.x * m._00 + v.y * m._10 + v.z * m._20 + v.w * m._30;
	t.y = v.x * m._01 + v.y * m._11 + v.z * m._21 + v.w * m._31;
	t.z = v.x * m._02 + v.y * m._12 + v.z * m._22 + v.w * m._32;
	t.w = v.x * m._03 + v.y * m._13 + v.z * m._23 + v.w * m._33;

	return t;
}


vect3 rotX(double angle, vect3 v)
{
	vect3 t;

	double a = angle;
	mat4x4 m = {		1,          0,         0,          0,
						0,     cos(a),    sin(a),          0,
						0,    -sin(a),    cos(a),          0,
						0,          0,         0,          1 };
	t = multiplyVxM(m, v);

	return t;
}


vect3 rotY(double angle, vect3 v)
{
	vect3 t;

	double a = angle;
	mat4x4 m = {   cos(a),          0,   -sin(a),          0,
					    0,          1,         0,          0,
				   sin(a),          0,    cos(a),          0,
					    0,          0,         0,          1 };
	t = multiplyVxM(m, v);

	return t;
}


vect3 rotZ(double angle, vect3 v)
{
	vect3 t;

	double a = angle;
	mat4x4 m = {   cos(a),     sin(a),         0,          0,
				  -sin(a),     cos(a),         0,          0,
					    0,          0,         1,          0,
					    0,          0,         0,          1 };
	t = multiplyVxM(m, v);

	return t;
}


vect3 rotXrad(double sinA, double cosA, vect3 v)
{
	vect3 t;

	mat4x4 m = {        1,          0,         0,          0,
						0,       cosA,      sinA,          0,
						0,      -sinA,      cosA,          0,
						0,          0,         0,          1 };
	t = multiplyVxM(m, v);

	return t;
}


vect3 rotYrad(double sinA, double cosA, vect3 v)
{
	vect3 t;

	mat4x4 m = {     cosA,          0,     -sinA,          0,
						0,          1,         0,          0,
				     sinA,          0,      cosA,          0,
						0,          0,         0,          1 };
	t = multiplyVxM(m, v);

	return t;
}


vect3 rotZrad(double sinA, double cosA, vect3 v)
{
	vect3 t;

	mat4x4 m = {     cosA,       sinA,         0,          0,
				    -sinA,       cosA,         0,          0,
						0,          0,         1,          0,
						0,          0,         0,          1 };
	t = multiplyVxM(m, v);

	return t;
}


vect3 scale(double x, double y, double z, vect3 v)
{
	vect3 t;

	double scaleX = x;
	double scaleY = y;
	double scaleZ = z;
	mat4x4 m = {   scaleX,          0,         0,          0,
						0,     scaleY,         0,          0,
						0,          0,    scaleZ,          0,
						0,          0,         0,          1 };
	t = multiplyMxV(m, v);

	return t;
}


vect3 translate(double x, double y, double z, vect3 v)
{
	vect3 t;

	double deltaX = x;
	double deltaY = y;
	double deltaZ = z;
	mat4x4 m = {		1,          0,         0,     deltaX,
						0,          1,         0,     deltaY,
						0,          0,         1,     deltaZ,
						0,          0,         0,          1 };
	t = multiplyMxV(m, v);

	return t;
}


vect3 world2view(Camera p, vect3 v)
{
	vect3 t;

	double azm = p.azm;
	double alt = p.alt;
	double rol = p.rol;

	t = rotZ(rol, rotX(alt + PI * 0.5, rotZ(azm + PI / 2, translate(-p.x, -p.y, -p.z, v))));

	return t;
}


vect3 world2viewT(double sinAzm, double cosAzm, double sinAlt, double cosAlt,
		double sinRol, double cosRol, Camera p, vect3 v)
{
	vect3 t;
	
	t = rotZrad(sinRol, cosRol, rotXrad(sinAlt, cosAlt, rotZrad(sinAzm, cosAzm, translate(-p.x, -p.y, -p.z, v))));
	t.w = 1.0;

	return t;
}


vect3 sun2view(double sinAzm, double cosAzm, double sinAlt, double cosAlt,
	double sinRol, double cosRol, vect3 v)
{
	vect3 t;

	t = rotZrad(sinRol, cosRol, rotXrad(sinAlt, cosAlt, rotZrad(sinAzm, cosAzm, v)));

	return t;
}


mat4x4 multiplyMxM(mat4x4 p, mat4x4 q)
{
	mat4x4 t;

	t._00 = p._00 * q._00 + p._01 * q._10 + p._02 * q._20 + p._03 * q._30;
	t._01 = p._00 * q._01 + p._01 * q._11 + p._02 * q._21 + p._03 * q._31;
	t._02 = p._00 * q._02 + p._01 * q._12 + p._02 * q._22 + p._03 * q._32;
	t._03 = p._00 * q._03 + p._01 * q._13 + p._02 * q._23 + p._03 * q._33;

	t._10 = p._10 * q._00 + p._11 * q._10 + p._12 * q._20 + p._13 * q._30;
	t._11 = p._10 * q._01 + p._11 * q._11 + p._12 * q._21 + p._13 * q._31;
	t._12 = p._10 * q._02 + p._11 * q._12 + p._12 * q._22 + p._13 * q._32;
	t._13 = p._10 * q._03 + p._11 * q._13 + p._12 * q._23 + p._13 * q._33;

	t._20 = p._20 * q._00 + p._21 * q._10 + p._22 * q._20 + p._23 * q._30;
	t._21 = p._20 * q._01 + p._21 * q._11 + p._22 * q._21 + p._23 * q._31;
	t._22 = p._20 * q._02 + p._21 * q._12 + p._22 * q._22 + p._23 * q._32;
	t._23 = p._20 * q._03 + p._21 * q._13 + p._22 * q._23 + p._23 * q._33;

	t._30 = p._30 * q._00 + p._31 * q._10 + p._32 * q._20 + p._33 * q._30;
	t._31 = p._30 * q._01 + p._31 * q._11 + p._32 * q._21 + p._33 * q._31;
	t._32 = p._30 * q._02 + p._31 * q._12 + p._32 * q._22 + p._33 * q._32;
	t._33 = p._30 * q._03 + p._31 * q._13 + p._32 * q._23 + p._33 * q._33;

	return t;
}


coord2 world2Plan(double x, double y, double scale)
{
	coord2 temp;
	temp.x = (int)(SCREEN_WIDTH / 2 + x * scale);
	temp.y = (int)(SCREEN_HEIGHT / 2 - y * scale);

	return temp;
}


coord2 view2screen(vect3 vertex, int width, int height, double hR, double vR)
{
	coord2 pixel;

	pixel.x = (int)(width * 0.5 + (vertex.x / vertex.z) * width * 0.5 * hR * 0.95);
	pixel.y = (int)(height * 0.5 - (vertex.y / vertex.z) * width * 0.5 * vR * 0.95);
	pixel.z = vertex.z;

	return pixel;
}


int GetYMax3(coord2* p)
{
	int yMax = p[0].y;
	for (int i = 1; i < 3; i++)
	{
		if (p[i].y > yMax) { yMax = p[i].y; }
	}
	return yMax;
}


int GetYMin3(coord2* p)
{
	int yMin = p[0].y;
	for (int i = 1; i < 3; i++)
	{
		if (p[i].y < yMin) { yMin = p[i].y; }
	}
	return yMin;
}


bool onScreen(coord2 test, int w, int h)
{
	return (test.x >= 0 && test.x < w && test.y >= 0 && test.y < h);
}


void fillTriangleZ(triangle2d t, Uint32* pixels, double* z, int width, int height, Uint32 colour)
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
	double startZ, endZ, zCurrent;
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
			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < width) && (hg >= 0 && hg < height))
				{
					zCurrent = startZ + ((endZ - startZ) / (double)wd) * (double)i;		//Z-buffering happens in this block
					if (zCurrent < z[hg * width + i])
					{
						pixels[hg * width + i] = colour;
						z[hg * width + i] = zCurrent;
					}
				}
			}
		}
	}
}


void drawCrosshair(Uint32* pixels, int w, int h, int hole, int size, Uint32 colour)
{
	for (int i = 0; i < (size - hole); i++)
	{
		pixels[(h / 2 - 1) * w + w / 2 + hole + i] = colour;
		pixels[(h / 2) * w + w / 2 + hole + i] = colour;

		pixels[(h / 2 + hole + i) * w + w / 2 - 1] = colour;
		pixels[(h / 2 + hole + i) * w + w / 2] = colour;
	}

	for (int i = size - hole; i > 0; i--)
	{
		pixels[(h / 2 - 1) * w - w / 2 - hole - i] = colour;
		pixels[(h / 2) * w - w / 2 - hole - i] = colour;

		pixels[(h / 2 - hole - i) * w + w / 2 - 1] = colour;
		pixels[(h / 2 - hole - i) * w + w / 2] = colour;
	}
}


void initFrustum(double fovH, double fovV, double zNear, double zFar, box* f)
{
	f->nearPlane.N = { 0.0,            0.0,            1.0,            0.0 };
	f->nearPlane.P = { 0.0,            0.0,          zNear,            1.0 };
	f->farPlane.N = { 0.0,            0.0,           -1.0,            0.0 };
	f->farPlane.P = { 0.0,            0.0,           zFar,            1.0 };
	f->leftPlane.N = { cos(fovH / 2),  0.0,  sin(fovH / 2), 	        0.0 };
	f->leftPlane.P = { 0.0,            0.0,            0.0001,            1.0 };
	f->rightPlane.N = { -cos(fovH / 2), 0.0,  sin(fovH / 2),            0.0 };
	f->rightPlane.P = { 0.0,            0.0,            0.0001,            1.0 };
	f->topPlane.N = { 0.0, cos(fovV / 2),  sin(fovV / 2),            0.0 };
	f->topPlane.P = { 0.0,            0.0,            0.0001,            1.0 };
	f->bottomPlane.N = { 0.0,  -cos(fovV / 2),  sin(fovV / 2),            0.0 };
	f->bottomPlane.P = { 0.0,            0.0,            0.0001,            1.0 };
}


int clipToFrustum(vect3* vertList, box f)
{
	plane currentPlane;
	int nVert = 3;
	currentPlane = f.nearPlane;
	clipPoly(&nVert, vertList, currentPlane);
	currentPlane = f.topPlane;
	clipPoly(&nVert, vertList, currentPlane);
	currentPlane = f.bottomPlane;
	clipPoly(&nVert, vertList, currentPlane);
	currentPlane = f.leftPlane;
	clipPoly(&nVert, vertList, currentPlane);
	currentPlane = f.rightPlane;
	clipPoly(&nVert, vertList, currentPlane);
	currentPlane = f.farPlane;
	clipPoly(&nVert, vertList, currentPlane);


	return nVert;
}


void clipPoly(int* nVert, vect3* vertList, plane clippingPlane)
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
			clipEdge(clippingPlane, vertList[i], vertList[i + 1], &nResult, vTemp);
		}
		else
		{
			clipEdge(clippingPlane, vertList[i], vertList[0], &nResult, vTemp);
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
	*nVert = nResult;
}


int sign(double a)
{
	int t;
	if (a > 0) { t = 1; }
	else if (a == 0) { t = 0; }
	else { t = -1; }

	return t;
}


void clipEdge(plane p, vect3 startV, vect3 endV, int* nResult, vect3* vTemp)
{
	double t;
	vect3 a = subVectors(startV, p.P);
	double sStart = dotProduct(a, p.N);
	vect3 b = subVectors(endV, p.P);
	double sEnd = dotProduct(b, p.N);
	if (sign(sStart) != sign(sEnd))
	{
		vect3 d = subVectors(endV, startV);
		double dist = dotProduct(d, p.N);
		if (sign(sStart) == -1)
		{
			if (dist)
			{
				t = (dist - sEnd) / dist;

				vTemp[*nResult].x = startV.x + t * (endV.x - startV.x);
				vTemp[*nResult].y = startV.y + t * (endV.y - startV.y);
				vTemp[*nResult].z = startV.z + t * (endV.z - startV.z);
				(*nResult)++;
			}
		}
		if (sign(sEnd) == -1)
		{
			if (dist)
			{
				t = (-dist - sStart) / dist;

				vTemp[*nResult].x = endV.x - t * (startV.x - endV.x);
				vTemp[*nResult].y = endV.y - t * (startV.y - endV.y);
				vTemp[*nResult].z = endV.z - t * (startV.z - endV.z);
				(*nResult)++;
			}
			else
			{

				vTemp[*nResult].x = endV.x;
				vTemp[*nResult].y = endV.y;
				vTemp[*nResult].z = endV.z;
				(*nResult)++;
			}
		}
		else
		{

			vTemp[*nResult].x = endV.x;
			vTemp[*nResult].y = endV.y;
			vTemp[*nResult].z = endV.z;
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
			(*nResult)++;
		}
	}
}