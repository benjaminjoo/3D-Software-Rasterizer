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


vect3 sun2view(double sinAzm, double cosAzm, double sinAlt, double cosAlt,
	double sinRol, double cosRol, vect3 v)
{
	vect3 t;

	t = rotZrad(sinRol, cosRol, rotXrad(sinAlt, cosAlt, rotZrad(sinAzm, cosAzm, v)));

	return t;
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


/*void initFrustum(double fovH, double fovV, double zNear, double zFar, box* f)
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
}*/


int sign(double a)
{
	int t;
	if (a > 0) { t = 1; }
	else if (a == 0) { t = 0; }
	else { t = -1; }

	return t;
}
