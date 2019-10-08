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


Uint32 getColour(const unsigned char a, const unsigned char r, const unsigned char g, const unsigned char b)
{
	return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}


Uint32 modifyColour(const Uint32& inputColour, const double& illumination)
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
	else if(illumination > 1.0)
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


bool checkPolygonForSplitting(int n, vect2* V, polyNode p, polyNode q)
{
	bool needsSplitting = false;
	polyNode normal = getNormal(p, q);
	polyNode currentStart, currentEnd;
	double sStart, sEnd;
	polyNode P, Q;
	for (int i = 0; i < n - 1; i++)
	{
		P.x = V[i].x;
		P.y = V[i].y;
		P.s = 0;
		Q.x = V[i + 1].x;
		Q.y = V[i + 1].y;
		Q.s = 0;
		currentStart = subNodes(P, p);
		currentEnd = subNodes(Q, p);
		sStart = dotNodes(currentStart, normal);
		sEnd = dotNodes(currentEnd, normal);
		if (sign(sStart) && sign(sEnd))
		{
			if (sign(sStart) != sign(sEnd)) { needsSplitting = true; }
		}
	}
	return needsSplitting;
}


bool checkPolygonForSplitting(polygon4uv V, polyNode p, polyNode q)
{
	bool needsSplitting = false;
	polyNode normal = getNormal(p, q);
	polyNode currentStart, currentEnd;
	double sStart, sEnd;
	polyNode P, Q;
	for (int i = 0; i < 3; i++)
	{
		P.x = V.points[i].x;
		P.y = V.points[i].y;
		P.s = 0;
		Q.x = V.points[i + 1].x;
		Q.y = V.points[i + 1].y;
		Q.s = 0;
		currentStart = subNodes(P, p);
		currentEnd = subNodes(Q, p);
		sStart = dotNodes(currentStart, normal);
		sEnd = dotNodes(currentEnd, normal);
		if (sign(sStart) && sign(sEnd))
		{
			if (sign(sStart) != sign(sEnd)) { needsSplitting = true; }
		}
	}
	return needsSplitting;
}


bool iSect2dLine(vect2 a, vect2 b, polyNode p, polyNode q, vect2* r)
{
	polyNode a_, b_;
	a_.x = a.x; a_.y = a.y; a_.s = 0;
	b_.x = b.x; b_.y = b.y; b_.s = 0;
	polyNode ap = subNodes(a_, p);
	polyNode bp = subNodes(b_, p);

	polyNode normal = getNormal(p, q);

	double sA = dotNodes(ap, normal);
	double sB = dotNodes(bp, normal);

	double t;

	if (sign(sA) != sign(sB))
	{
		polyNode d = subNodes(b_, a_);
		double dist = dotNodes(d, normal);

		if (sA < 0.0)
		{
			if (dist)
			{
				t = (dist - sB) / dist;

				r->x = a.x + t * (b.x - a.x);
				r->y = a.y + t * (b.y - a.y);
				r->u = a.u + t * (b.u - a.u);
				r->v = a.v + t * (b.v - a.v);
			}
		}
		if (sB < 0.0)
		{
			if (dist)
			{
				t = (-dist - sA) / dist;

				r->x = b.x - t * (a.x - b.x);
				r->y = b.y - t * (a.y - b.y);
				r->u = b.u - t * (a.u - b.u);
				r->v = b.v - t * (a.v - b.v);
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}


void printCoord3(vect3 V)
{
	printf("x: %4.4f\ty: %4.4f\tz: %4.4f\n", V.x, V.y, V.z);
}


vect3 invertVector(vect3 a)
{
	vect3 temp;

	temp.x = -a.x;
	temp.y = -a.y;
	temp.z = -a.z;
	temp.x = 1.0;

	return temp;
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


worldCoord worldCoord::operator + (const worldCoord& p)
{
	return { x + p.x, y + p.y, z + p.z };
}


worldCoord worldCoord::operator - (const worldCoord& p)
{
	return { x - p.x, y - p.y, z - p.z };
}


worldCoord worldCoord::operator += (const worldCoord& p)
{
	return { x += p.x, y += p.y, z += p.z };
}


worldCoord worldCoord::operator -= (const worldCoord& p)
{
	return { x -= p.x, y -= p.y, z -= p.z };
}


worldCoord worldCoord::operator ^ (const worldCoord& p)
{
	return { y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x };
}


double worldCoord::operator * (const worldCoord& p)
{
	return x * p.x + y * p.y + z * p.z;
}


worldCoord addVectors2(worldCoord a, worldCoord b)
{
	worldCoord temp;

	temp.x = a.x + b.x;
	temp.y = a.y + b.y;
	temp.z = a.z + b.z;

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


worldCoord subVectors2(worldCoord a, worldCoord b)
{
	worldCoord temp;

	temp.x = a.x - b.x;
	temp.y = a.y - b.y;
	temp.z = a.z - b.z;

	return temp;
}


vect3 halfwayPoint(vect3 a, vect3 b)
{
	vect3 temp;

	temp.x = (a.x + b.x) * 0.5;
	temp.y = (a.y + b.y) * 0.5;
	temp.z = (a.z + b.z) * 0.5;
	temp.w = 1.0;

	return temp;
}


double distanceSquared(vect3 a, vect3 b)
{
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}


vect3 unitVector(vect3 v)
{
	vect3 temp;

	double length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

	temp.x = v.x / length;
	temp.y = v.y / length;
	temp.z = v.z / length;
	temp.w = 1.0;

	return temp;
}


worldCoord unitVector2(worldCoord v)
{
	worldCoord temp;

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
	temp.w = 1.0;	//0.0

	return temp;
}


double dotProduct(vect3 a, vect3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}


double dotProduct2(worldCoord a, worldCoord b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}


worldCoord rotate2(worldCoord target, Side currentView, worldCoord origin, double angle)
{
	worldCoord temp		= subVectors2(target, origin);
	worldCoord result	= { 0.0f, 0.0f, 0.0f };

	if (currentView == Top)
	{
		result.x = temp.x * cos(angle) - temp.y * sin(angle);
		result.y = temp.y * cos(angle) + temp.x * sin(angle);
		result.z = target.z;
	}
	else if (currentView == Front)
	{
		result.x = temp.x * cos(angle) - temp.z * sin(angle);
		result.y = target.y;
		result.z = temp.z * cos(angle) + temp.x * sin(angle);
	}
	else if (currentView == Right)
	{
		result.x = target.x;
		result.y = temp.y * cos(angle) - temp.z * sin(angle);
		result.z = temp.z * cos(angle) + temp.y * sin(angle);
	}

	result.x += origin.x;
	result.y += origin.y;
	result.z += origin.z;

	return result;
}


double dotProductSquared(vect3 a, vect3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z) * (a.x * b.x + a.y * b.y + a.z * b.z);
}


double distPoint2Plane(vect3 P, triangle3dV T)
{
	return abs(dotProduct(subVectors(T.A, P), T.N));
}


double distPoint2Line(worldCoord P, Side view, line3 L)
{
	worldCoord normal = rotate2(unitVector2(subVectors2(L.vert[1], L.vert[0])), view, { 0.0f, 0.0f, 0.0f }, PI * 0.5f);
	switch (view)
	{
	case Top:
		normal.z = 0.0f;
		break;
	case Front:
		normal.y = 0.0f;
		break;
	case Right:
		normal.x = 0.0f;
		break;
	}
	return abs(dotProduct2(subVectors2(L.vert[0], P), normal));
}


bool pointIsAroundLine(worldCoord P, Side view, line3 L)
{
	worldCoord A = L.vert[0], B = L.vert[1];
	switch (view)
	{
	case Top:
		{
			A.z = 0.0f;
			B.z = 0.0f;
			P.z = 0.0f;
		}
		break;
	case Front:
		{
			A.y = 0.0f;
			B.y = 0.0f;
			P.y = 0.0f;
		}
		break;
	case Right:
		{
			A.x = 0.0f;
			B.x = 0.0f;
			P.x = 0.0f;
		}
		break;
	}
	double sA = dotProduct2(subVectors2(B, A), subVectors2(A, P));
	double sB = dotProduct2(subVectors2(B, A), subVectors2(B, P));

	return (sign(sA) != sign(sB)) ? true : false;
}


vect3 crossProduct(vect3 a, vect3 b)
{
	vect3 t;

	t.x = a.y * b.z - a.z * b.y;
	t.y = a.z * b.x - a.x * b.z;
	t.z = a.x * b.y - a.y * b.x;

	t.w = 1.0;

	return t;
}


worldCoord crossProduct(worldCoord a, worldCoord b)
{
	worldCoord t;

	t.x = a.y * b.z - a.z * b.y;
	t.y = a.z * b.x - a.x * b.z;
	t.z = a.x * b.y - a.y * b.x;

	return t;
}


vect3 midPoint(vect3 a, vect3 b)
{
	vect3 temp;

	temp.x = (a.x + b.x) / 2;
	temp.y = (a.y + b.y) / 2;
	temp.z = (a.z + b.z) / 2;
	temp.w = 1.0;

	return temp;
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


vect3 scaleVector(double scale, vect3 v)
{
	vect3 t;

	mat4x4 m = {	scale,          0,         0,          0,
						0,		scale,         0,          0,
						0,          0,	   scale,          0,
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
		temp = rotXT(rX, temp);
		temp = rotYT(rY, temp);
		temp = rotZT(rZ, temp);
		temp = translateT(mvX, mvY, mvZ, temp);

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


/*double getMinN(int n, double* list)
{
	double minimum = list[0];
	for (int i = 0; i < n; i++)
	{
		if (list[i] < minimum) { minimum = list[i]; }
	}
	return minimum;
}*/


template <class T>
T getMinN(int n, T* list)
{
	T minimum = list[0];
	for (auto i = 0; i < n; i++)
	{
		if (list[i] < minimum) { minimum = list[i]; }
	}
	return minimum;
}



vect3 getXYZmin(int n, triangle3dV* T, double padding)
{
	vect3 xyzMin;
	xyzMin.x = T[0].A.x;
	xyzMin.y = T[0].A.y;
	xyzMin.z = T[0].A.z;
	xyzMin.w = 1.0;
	for (int i = 0; i < n; i++)
	{
		if (T[i].A.x < xyzMin.x) { xyzMin.x = T[i].A.x; }	
		if (T[i].A.y < xyzMin.y) { xyzMin.y = T[i].A.y; }
		if (T[i].A.z < xyzMin.z) { xyzMin.z = T[i].A.z; }
		if (T[i].B.x < xyzMin.x) { xyzMin.x = T[i].B.x; }
		if (T[i].B.y < xyzMin.y) { xyzMin.y = T[i].B.y; }
		if (T[i].B.z < xyzMin.z) { xyzMin.z = T[i].B.z; }
		if (T[i].C.x < xyzMin.x) { xyzMin.x = T[i].C.x; }
		if (T[i].C.y < xyzMin.y) { xyzMin.y = T[i].C.y; }
		if (T[i].C.z < xyzMin.z) { xyzMin.z = T[i].C.z; }
	}
	xyzMin.x -= padding;
	xyzMin.y -= padding;
	xyzMin.z -= padding;
	return xyzMin;
}


vect3 getXYZmax(int n, triangle3dV* T, double padding)
{
	vect3 xyzMax;
	xyzMax.x = T[0].A.x;
	xyzMax.y = T[0].A.y;
	xyzMax.z = T[0].A.z;
	xyzMax.w = 1.0;
	for (int i = 0; i < n; i++)
	{
		if (T[i].A.x > xyzMax.x) { xyzMax.x = T[i].A.x; }
		if (T[i].A.y > xyzMax.y) { xyzMax.y = T[i].A.y; }
		if (T[i].A.z > xyzMax.z) { xyzMax.z = T[i].A.z; }
		if (T[i].B.x > xyzMax.x) { xyzMax.x = T[i].B.x; }
		if (T[i].B.y > xyzMax.y) { xyzMax.y = T[i].B.y; }
		if (T[i].B.z > xyzMax.z) { xyzMax.z = T[i].B.z; }
		if (T[i].C.x > xyzMax.x) { xyzMax.x = T[i].C.x; }
		if (T[i].C.y > xyzMax.y) { xyzMax.y = T[i].C.y; }
		if (T[i].C.z > xyzMax.z) { xyzMax.z = T[i].C.z; }
	}
	xyzMax.x += padding;
	xyzMax.y += padding;
	xyzMax.z += padding;
	return xyzMax;
}


bool checkBoundingBoxForCollision(vect3 p, boundingBox B)
{
	if ((p.x >= B.minExt.x && p.x <= B.maxExt.x) &&
		(p.y >= B.minExt.y && p.y <= B.maxExt.y) &&
		(p.z >= B.minExt.z && p.z <= B.maxExt.z))
	{
		return true;
	}
	else
	{
		return false;
	}
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

	int* decimalDigits = new int[nDecimals];

	for (int i = 0; i < nDecimals; i++)
	{
		if (i < nDecimals)
		{
			decimalDigits[i] = (int)(dec / pow10(nDecimals - 1 - i));
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


void freePointerToPolyArray(triangle3dV** ptr, int nDimensions)
{
	for (int i = 0; i < nDimensions; i++)
	{
		if (ptr[i] != nullptr)
		{
			delete[] ptr[i];
		}		
	}
	if (ptr != nullptr)
	{
		delete[] ptr;
	}	
}