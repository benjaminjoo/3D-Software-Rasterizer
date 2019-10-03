#include "Definitions.h"


float quickSqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
	x2 = number * 0.5F;
	y = number;
	i = *(long*)& y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)& i;
	y = y * (threehalfs - (x2 * y * y));

	return 1 / y;
}


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


double distancePN(polyNode a, polyNode b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}


Uint32 getColour(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
{
	return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}


Uint32 modifyColour(Uint32 inputColour, double illumination)
{
	byte a, r, g, b;

	a = 0;
	r = (byte)((inputColour >> 16 & 255) * illumination);
	g = (byte)((inputColour >> 8 & 255) * illumination);
	b = (byte)((inputColour & 255) * illumination);

	return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}


polyNode subNodes(polyNode a, polyNode b)
{
	polyNode t;

	t.x = a.x - b.x;
	t.y = a.y - b.y;
	t.s = a.s;

	return t;
}

polyNode addNodes(polyNode a, polyNode b)
{
	polyNode t;

	t.x = a.x + b.x;
	t.y = a.y + b.y;
	t.s = a.s;

	return t;
}

polyNode unitNode(polyNode a)
{
	polyNode t;

	double length = sqrt(a.x * a.x + a.y * a.y);
	t.x = a.x / length;
	t.y = a.y / length;
	t.s = a.s;

	return t;
}

double dotNodes(polyNode a, polyNode b)
{
	return a.x * b.x + a.y * b.y;
}

polyNode getNormal(polyNode a, polyNode b)
{
	polyNode tempNode = subNodes(a, b);
	polyNode tempNormal;
	tempNormal.x = tempNode.y;
	tempNormal.y = -tempNode.x;
	tempNormal.s = 0;

	return tempNormal;
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


vect3 rotX(double a, vect3 v)
{
	vect3 t;

	mat4x4 m = {		1,          0,         0,          0,
						0,     cos(a),    sin(a),          0,
						0,    -sin(a),    cos(a),          0,
						0,          0,         0,          1 };
	t = multiplyVxM(m, v);

	return t;
}


vect3 rotY(double a, vect3 v)
{
	vect3 t;

	mat4x4 m = {	cos(a),          0,   -sin(a),          0,
						0,          1,         0,          0,
				   sin(a),          0,    cos(a),          0,
						0,          0,         0,          1 };
	t = multiplyVxM(m, v);

	return t;
}


vect3 rotZ(double a, vect3 v)
{
	vect3 t;

	mat4x4 m = {   cos(a),     sin(a),         0,          0,
				  -sin(a),     cos(a),         0,          0,
						0,          0,         1,          0,
						0,          0,         0,          1 };
	t = multiplyVxM(m, v);

	return t;
}


vect3 scale(double scaleX, double scaleY, double scaleZ, vect3 v)
{
	vect3 t;

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


triangle3dV rotXT(double angle, triangle3dV T)
{
	T.A		= rotX(angle, T.A);
	T.B		= rotX(angle, T.B);
	T.C		= rotX(angle, T.C);
	T.An	= rotX(angle, T.An);
	T.Bn	= rotX(angle, T.Bn);
	T.Cn	= rotX(angle, T.Cn);
	T.N		= rotX(angle, T.N);

	return T;
}


triangle3dV rotYT(double angle, triangle3dV T)
{
	T.A		= rotY(angle, T.A);
	T.B		= rotY(angle, T.B);
	T.C		= rotY(angle, T.C);
	T.An	= rotY(angle, T.An);
	T.Bn	= rotY(angle, T.Bn);
	T.Cn	= rotY(angle, T.Cn);
	T.N		= rotY(angle, T.N);

	return T;
}


triangle3dV rotZT(double angle, triangle3dV T)
{
	T.A		= rotZ(angle, T.A);
	T.B		= rotZ(angle, T.B);
	T.C		= rotZ(angle, T.C);
	T.An	= rotZ(angle, T.An);
	T.Bn	= rotZ(angle, T.Bn);
	T.Cn	= rotZ(angle, T.Cn);
	T.N		= rotZ(angle, T.N);

	return T;
}


triangle3dV scaleT(double x, double y, double z, triangle3dV T)
{
	T.A		= scale(x, y, z, T.A);
	T.B		= scale(x, y, z, T.B);
	T.C		= scale(x, y, z, T.C);
	T.An	= scale(sign(x), sign(y), sign(z), T.An);
	T.Bn	= scale(sign(x), sign(y), sign(z), T.Bn);
	T.Cn	= scale(sign(x), sign(y), sign(z), T.Cn);
	T.N		= scale(sign(x), sign(y), sign(z), T.N);

	return T;
}


triangle3dV translateT(double x, double y, double z, triangle3dV T)
{
	T.A		= translate(x, y, z, T.A);
	T.B		= translate(x, y, z, T.B);
	T.C		= translate(x, y, z, T.C);
	T.An	= translate(x, y, z, T.An);
	T.Bn	= translate(x, y, z, T.Bn);
	T.Cn	= translate(x, y, z, T.Cn);
	T.N		= translate(x, y, z, T.N);

	return T;
}


triangle3dV* transformObject(int nPoly, triangle3dV* object, double scX, double scY, double scZ, double mvX, double mvY, double mvZ,
	double rX, double rY, double rZ)
{
	triangle3dV* target = new triangle3dV[nPoly];

	for (int i = 0; i < nPoly; i++)
	{
		triangle3dV temp = object[i];

		temp = scaleT(scX, scY, scZ, temp);
		temp = translateT(mvX, mvY, mvZ, temp);
		temp = rotXT(rX, temp);
		temp = rotYT(rY, temp);
		temp = rotZT(rZ, temp);

		//temp.texture = object[i].texture;
		
		target[i] = temp;
	}

	return target;
	delete[] target;
}


void transformMesh(int n, triangle3dV* object, double scX, double scY, double scZ, double mvX, double mvY, double mvZ,
	double rX, double rY, double rZ)
{
	for (int i = 0; i < n; i++)
	{
		triangle3dV temp = object[i];

		temp = scaleT(scX, scY, scZ, temp);
		temp = translateT(mvX, mvY, mvZ, temp);
		temp = rotXT(rX, temp);
		temp = rotYT(rY, temp);
		temp = rotZT(rZ, temp);

		object[i] = temp;
	}
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


int sign(double a)
{
	int t;
	if (a > 0) { t = 1; }
	else if (a == 0) { t = 0; }
	else { t = -1; }

	return t;
}


int roundInt(double a)
{
	double t = a - (int)a;
	if (a >= 0.5)
	{
		return (int)a + 1;
	}
	else
	{
		return (int)a;
	}
}


double pow10(int n)
{
	double result = 1.0;

	if (n > 0)
	{
		for (int i = 0; i < n; i++)
		{
			result *= 10;
		}
	}
	else if (n < 0)
	{
		for (int i = 0; i > n; i--)
		{
			result /= 10;
		}
	}

	return result;
}


int pow2(int n)
{
	int result = 1;

	for (int i = 0; i < n; i++)
	{
		result *= 2;
	}

	return result;
}


int* getFractionals(double number, int nDecimals)
{
	int dec;
	if (number > 0)
	{
		dec = int((number - (int)(number)) * pow10(nDecimals));
	}
	else dec = 0;

	int* decimalDigits = new int(nDecimals);

	for (int i = 0; i < nDecimals; i++)
	{
		if (i < nDecimals)
		{
			decimalDigits[i] = (char)(dec / pow10(nDecimals - 1 - i));
			int rem = dec % (int)(pow10(nDecimals - 1 - i));
			dec = rem;
		}
	}

	return decimalDigits;
}


int* getIntegers(double number, int* n)
{
	int rawInt = (int)(number);
	int nDigits = 0;
	while (rawInt > 0)
	{
		rawInt /= 10;
		nDigits++;
	}

	int* intDigits = new int[nDigits];
	rawInt = (int)(number);

	for (int i = nDigits - 1; i >= 0; i--)
	{
		intDigits[i] = rawInt % 10;
		rawInt /= 10;
	}

	*n = nDigits;
	return intDigits;
	delete[] intDigits;
}