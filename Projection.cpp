#include "Projection.h"



Projection::Projection()
{
	std::cout << "+";
}


Projection::~Projection()
{
	std::cout << "-";
}


int Projection::GetYMax3(coord2* p)
{
	int yMax = p[0].y;
	for (int i = 1; i < 3; i++)
		if (p[i].y > yMax)
			yMax = p[i].y;
	return yMax;
}


int Projection::GetYMin3(coord2* p)
{
	int yMin = p[0].y;
	for (int i = 1; i < 3; i++)
		if (p[i].y < yMin)
			yMin = p[i].y;
	return yMin;
}


float Projection::getMax(int n, float* v)
{
	float max = v[0];
	for (int i = 0; i < n; i++)
		if (v[i] > max)
			max = v[i];
	return max;
}


float Projection::getMin(int n, float* v)
{
	float min = v[0];
	for (int i = 0; i < n; i++)
		if (v[i] > min)
			min = v[i];
	return min;
}


void Projection::clampValue(float* value, float lower, float upper)
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


vect3 Projection::rotXrad(const float& sinA, const float& cosA, const vect3& v)
{
	vect3 t;

	mat4x4 m = { 1.0f,          0.0f,         0.0f,          0.0f,
						0.0f,          cosA,         sinA,          0.0f,
						0.0f,         -sinA,         cosA,          0.0f,
						0.0f,          0.0f,         0.0f,          1.0f };

	t = multiplyVxM(m, v);

	return t;
}


vect3 Projection::rotYrad(const float& sinA, const float& cosA, const vect3& v)
{
	vect3 t;

	mat4x4 m = { cosA,          0,     -sinA,          0,
						0,          1,         0,          0,
					 sinA,          0,      cosA,          0,
						0,          0,         0,          1 };

	t = multiplyVxM(m, v);

	return t;
}


vect3 Projection::rotZrad(const float& sinA, const float& cosA, const vect3& v)
{
	vect3 t;

	mat4x4 m = { cosA,       sinA,         0.0f,          0.0f,
					-sinA,       cosA,         0.0f,          0.0f,
					 0.0f,       0.0f,         1.0f,          0.0f,
					 0.0f,       0.0f,         0.0f,          1.0f };

	t = multiplyVxM(m, v);

	return t;
}


triangle3dV Projection::rotXT(float angle, triangle3dV T)
{
	float sin_a = sin(angle);
	float cos_a = cos(angle);

	T.A = rotXrad(sin_a, cos_a, T.A);
	T.B = rotXrad(sin_a, cos_a, T.B);
	T.C = rotXrad(sin_a, cos_a, T.C);
	T.An = rotXrad(sin_a, cos_a, T.An);
	T.Bn = rotXrad(sin_a, cos_a, T.Bn);
	T.Cn = rotXrad(sin_a, cos_a, T.Cn);
	T.N = rotXrad(sin_a, cos_a, T.N);

	return T;
}


triangle3dV Projection::rotYT(float angle, triangle3dV T)
{
	float sin_a = sin(angle);
	float cos_a = cos(angle);

	T.A = rotYrad(sin_a, cos_a, T.A);
	T.B = rotYrad(sin_a, cos_a, T.B);
	T.C = rotYrad(sin_a, cos_a, T.C);
	T.An = rotYrad(sin_a, cos_a, T.An);
	T.Bn = rotYrad(sin_a, cos_a, T.Bn);
	T.Cn = rotYrad(sin_a, cos_a, T.Cn);
	T.N = rotYrad(sin_a, cos_a, T.N);

	return T;
}


triangle3dV Projection::rotZT(float angle, triangle3dV T)
{
	float sin_a = sin(angle);
	float cos_a = cos(angle);

	T.A = rotZrad(sin_a, cos_a, T.A);
	T.B = rotZrad(sin_a, cos_a, T.B);
	T.C = rotZrad(sin_a, cos_a, T.C);
	T.An = rotZrad(sin_a, cos_a, T.An);
	T.Bn = rotZrad(sin_a, cos_a, T.Bn);
	T.Cn = rotZrad(sin_a, cos_a, T.Cn);
	T.N = rotZrad(sin_a, cos_a, T.N);

	return T;
}


vect3 Projection::translate(float x, float y, float z, vect3 v)
{
	vect3 t;

	float deltaX = x;
	float deltaY = y;
	float deltaZ = z;

	mat4x4 m = { 1.0f,          0.0f,         0.0f,     deltaX,
						0.0f,          1.0f,         0.0f,     deltaY,
						0.0f,          0.0f,         1.0f,     deltaZ,
						0.0f,          0.0f,         0.0f,          1.0f };

	t = multiplyMxV(m, v);

	return t;
}


triangle3dV Projection::translateT(float x, float y, float z, triangle3dV T)
{
	T.A = translate(x, y, z, T.A);
	T.B = translate(x, y, z, T.B);
	T.C = translate(x, y, z, T.C);

	return T;
}


vect3 Projection::scale(float scaleX, float scaleY, float scaleZ, vect3 v)
{
	vect3 t;

	mat4x4 m = { scaleX,          0.0f,         0.0f,          0.0f,
				   0.0f,		scaleY,         0.0f,          0.0f,
				   0.0f,		  0.0f,       scaleZ,          0.0f,
				   0.0f,          0.0f,         0.0f,          1.0f };
	t = multiplyMxV(m, v);

	return t;
}


triangle3dV Projection::scaleT(float x, float y, float z, triangle3dV T)
{
	T.A = scale(x, y, z, T.A);
	T.B = scale(x, y, z, T.B);
	T.C = scale(x, y, z, T.C);
	T.An = scale(sign(x), sign(y), sign(z), T.An);
	T.Bn = scale(sign(x), sign(y), sign(z), T.Bn);
	T.Cn = scale(sign(x), sign(y), sign(z), T.Cn);
	T.N = scale(sign(x), sign(y), sign(z), T.N);

	return T;
}


vect3 Projection::multiplyVxM(const mat4x4& m, const vect3& v)
{
	vect3 t;

	t.x = v.x * m._00 + v.y * m._10 + v.z * m._20 + v.w * m._30;
	t.y = v.x * m._01 + v.y * m._11 + v.z * m._21 + v.w * m._31;
	t.z = v.x * m._02 + v.y * m._12 + v.z * m._22 + v.w * m._32;
	t.w = v.x * m._03 + v.y * m._13 + v.z * m._23 + v.w * m._33;

	return t;
}


vect3 Projection::multiplyMxV(const mat4x4& m, const vect3& v)
{
	vect3 t;

	t.x = v.x * m._00 + v.y * m._01 + v.z * m._02 + v.w * m._03;
	t.y = v.x * m._10 + v.y * m._11 + v.z * m._12 + v.w * m._13;
	t.z = v.x * m._20 + v.y * m._21 + v.z * m._22 + v.w * m._23;
	t.w = v.x * m._30 + v.y * m._31 + v.z * m._32 + v.w * m._33;

	return t;
}


mat4x4 Projection::multiplyMxM(const mat4x4& p, const mat4x4& q)
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


void Projection::movePoly(vect3 m, triangle3dV& poly)
{
	poly.A = addVectors(poly.A, m);
	poly.B = addVectors(poly.B, m);
	poly.C = addVectors(poly.C, m);
}


void Projection::rotateMesh(int n, triangle3dV* object, float rx, float ry, float rz)
{
	for (int i = 0; i < n; i++)
	{
		triangle3dV temp = object[i];

		temp = rotXT(rx, temp);
		temp = rotYT(ry, temp);
		temp = rotZT(rz, temp);

		object[i] = temp;
	}
}


void Projection::transformMesh(int n, triangle3dV* object, float scX, float scY, float scZ, float mvX, float mvY, float mvZ,
	float rX, float rY, float rZ)
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


mat4x4 Projection::getTranslation(vect3 mv)
{
	mat4x4 result;

	result = { 1.0f,          0.0f,         0.0f,		 mv.x,
				0.0f,          1.0f,         0.0f,		 mv.y,
				0.0f,          0.0f,         1.0f,		 mv.z,
				0.0f,          0.0f,         0.0f,       1.0f };

	return result;
}


mat4x4 Projection::getRotation(axis t, float a)
{
	mat4x4 result = { 1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f };

	float sinA = sin(a);
	float cosA = cos(a);

	switch (t)
	{
	case axis::x:
	{
		result = { 1.0f,       0.0f,       0.0f,       0.0f,
					0.0f,       cosA,       sinA,       0.0f,
					0.0f,      -sinA,       cosA,       0.0f,
					0.0f,       0.0f,       0.0f,       1.0f };
	}
	break;
	case axis::y:
	{
		result = { cosA,       0.0f,      -sinA,       0.0f,
					0.0f,       1.0f,       0.0f,       0.0f,
					sinA,       0.0f,       cosA,       0.0f,
					0.0f,       0.0f,       0.0f,       1.0f };
	}
	break;
	case axis::z:
	{
		result = { cosA,       sinA,       0.0f,       0.0f,
				   -sinA,       cosA,       0.0f,       0.0f,
					0.0f,       0.0f,       1.0f,       0.0f,
					0.0f,       0.0f,       0.0f,       1.0f };
	}
	break;
	}

	return result;
}


void Projection::object2worldT(const vect3& sc, const vect3& mv, const vect3& rt, triangle3dV& T)
{
	T = Projection::rotXT(rt.x, T);
	T = Projection::rotYT(rt.y, T);
	T = Projection::rotZT(rt.z, T);
	T = translateT(mv.x, mv.y, mv.z, T);
}


void Projection::world2view(triangle3dV& tr, mat4x4& rot, mat4x4& mov)
{
	tr.A = rot * mov * tr.A;
	tr.B = rot * mov * tr.B;
	tr.C = rot * mov * tr.C;
	tr.An = rot * tr.An;
	tr.Bn = rot * tr.Bn;
	tr.Cn = rot * tr.Cn;
	tr.N = rot * tr.N;
}


point3 Projection::world2viewP(point3 p, mat4x4& rot, mat4x4& mov)
{
	point3 tempPoint;

	tempPoint.P = rot * mov * p.P;
	tempPoint.P.w = 1.0f;
	tempPoint.colour = p.colour;

	return tempPoint;
}


vect3 Projection::screen2view(coord2 pixel, std::shared_ptr<Canvas> screen, float h_ratio, float v_ratio)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

	vect3 vertex;
	float t = 1.0f / pixel.z;

	vertex.x = float(pixel.x) - float(w * 0.5f) * t / (w * 0.475f * h_ratio);
	vertex.y = float(h * 0.5f) - float(pixel.y) * t / (w * 0.475f * v_ratio);
	vertex.z = t;
	vertex.w = 1.0f;

	return vertex;
}


void Projection::illuminatePoly(LightSource Light, vect3& View, triangle3dV* viewT, const triangle3dV& worldT, const projectionStyle& style, const float& min)
{
	float illumAll, illumA, illumB, illumC;
	illumAll = illumA = illumB = illumC = 0.0f;
	vect3 n;

	if (style == projectionStyle::sunlight ||
		style == projectionStyle::gouraud_shaded ||
		style == projectionStyle::blinn_phong)
	{
		n = worldT.An;
		illumA = Light.getIllumination(n);
		clampValue(&illumA, min, 1.0f);
		viewT->A.w = illumA;

		n = worldT.Bn;
		illumB = Light.getIllumination(n);
		clampValue(&illumB, min, 1.0f);
		viewT->B.w = illumB;

		n = worldT.Cn;
		illumC = Light.getIllumination(n);
		clampValue(&illumC, min, 1.0f);
		viewT->C.w = illumC;
	}
	else
	{
		n = worldT.N;
		illumAll = Light.getIllumination(n);
		clampValue(&illumAll, min, 1.0f);
		viewT->A.w = viewT->B.w = viewT->C.w = illumAll;
	}
}


Uint32 Projection::modifyColour(const Uint32& inputColour, const float& illumination)
{
	//bool red = true, green = true, blue = true;
	byte a = 0, r, g, b, r2fill = 0, g2fill = 0, b2fill = 0;
	float illSurplus = 0.0f;

	if (illumination <= 1.0f)
	{
		r = (byte)(float(inputColour >> 16 & 255)* illumination);
		g = (byte)(float(inputColour >> 8 & 255)* illumination);
		b = (byte)(float(inputColour & 255) * illumination);
	}
	else if (illumination > 1.0f)
	{
		illSurplus = illumination - 1.0f;

		r = (byte)(float(inputColour >> 16 & 255)* illumination);
		g = (byte)(float(inputColour >> 8 & 255)* illumination);
		b = (byte)(float(inputColour & 255) * illumination);

		r2fill = 255 - r;	g2fill = 255 - g;	b2fill = 255 - b;

		float s = 1.0f / (MAX_ILLUMINATION - 1.0f) * illSurplus;

		r += byte(float(r2fill) * s);
		g += byte(float(g2fill) * s);
		b += byte(float(b2fill) * s);
	}

	return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}


textCoord Projection::getUVCoord(const vect3& startV, const vect3& endV, const textCoord& startC, const textCoord& endC, const vect3& testV)
{
	textCoord testC;

	float a = dotProduct(subVectors(testV, startV), subVectors(endV, startV));
	float b = dotProduct(subVectors(endV, startV), subVectors(endV, startV));
	float d = (b != 0.0f) ? (a / b) : (0.0f);

	testC.u = startC.u + d * (endC.u - startC.u);
	testC.v = startC.v + d * (endC.v - startC.v);

	return testC;
}


void Projection::fillTriangleSolidColour(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, float h_ratio, float v_ratio)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

	Uint32 pixelColour = t.h;
	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;
	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int halfW = (int)(w * 0.5f);
	int halfH = (int)(h * 0.5f);
	int dx, dy;
	float xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	float zLimit[2] = { 0.0f, 0.0f };

	vect3 nullVect = { 0.0f, 0.0f, 0.0f, 0.0f };
	vect3 sides[4] = { nullVect, nullVect, nullVect, nullVect };
	vect3 sideL[2] = { nullVect, nullVect };
	vect3 sideR[2] = { nullVect, nullVect };
	textCoord nullCoord = { 0.0f, 0.0f };
	textCoord uvSides[4] = { nullCoord, nullCoord, nullCoord, nullCoord };
	textCoord uvSideL[2] = { nullCoord, nullCoord };
	textCoord uvSideR[2] = { nullCoord, nullCoord };

	int endIndex;
	int startX, endX;
	float startZ, endZ, zCurrent;
	float hCorr = w * 0.475f * h_ratio;
	float vCorr = w * 0.475f * v_ratio;
	float deltaZ;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z;
			yy = (float)hg - (float)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (float)dy);
			wd = t.a.x + std::round(xx);
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
			yy = (float)hg - (float)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (float)dy);
			wd = t.b.x + std::round(xx);
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
			yy = (float)hg - (float)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (float)dy);
			wd = t.c.x + std::round(xx);
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
			deltaZ = (endZ - startZ) / (float)span;
			zCurrent = startZ;

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					if (1 / zCurrent < screen->depthBuffer[hg * w + i])
					{
						screen->pixelBuffer[hg * w + i] = pixelColour;
						screen->depthBuffer[hg * w + i] = 1 / zCurrent;
					}
					zCurrent += deltaZ;
				}
			}
		}
	}
}


void Projection::fillTriangleFlatShaded(const triangle2dG& t, std::shared_ptr<Canvas> screen)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int dx, dy;
	float xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	float zLimit[2] = { 0, 0 };
	int endIndex;
	int startX, endX;
	float startZ, endZ, zCurrent, illCurrent;

	illCurrent = t.illumA;
	float deltaZ;

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
			yy = (float)hg - (float)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (float)dy);
			wd = t.a.x + std::round(xx);
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
			yy = (float)hg - (float)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (float)dy);
			wd = t.b.x + std::round(xx);
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
			yy = (float)hg - (float)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (float)dy);
			wd = t.c.x + std::round(xx);
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
			deltaZ = (endZ - startZ) / (float)span;
			zCurrent = startZ;
			int hgw = hg * w;
			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					if (1 / zCurrent < screen->depthBuffer[hg * w + i])
					{
						screen->pixelBuffer[hgw + i] = pixelColour;
						screen->depthBuffer[hgw + i] = 1 / zCurrent;
					}
					zCurrent += deltaZ;
				}
			}
		}
	}
}


void Projection::fillTriangleGouraudShaded(const triangle2dG& t, std::shared_ptr<Canvas> screen, float h_ratio, float v_ratio)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int dx, dy;
	float dIllum, illum;
	float xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	float zLimit[2] = { 0.0f, 0.0f };
	float illLimit[2] = { 0.0f, 0.0f };
	int endIndex;
	int startX, endX;
	float startZ, endZ, zCurrent, startIll, endIll, illCurrent;

	float deltaZ, deltaIll;

	byte r, g, b;
	r = ((byte)(t.h >> 16 & 0xFF));
	g = ((byte)(t.h >> 8 & 0xFF));
	b = ((byte)(t.h & 0xFF));

	float corr = 0.0f;

	coord2 currentP, startP, endP;
	vect3 startV, endV;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z; dIllum = t.illumB - t.illumA;
			yy = (float)hg - (float)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * ((yy + corr) / (float)dy);
			illum = t.illumA + dIllum * (yy / (float)dy);
			wd = t.a.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0f;
				endIndex++;
			}
		}
		//Side B-C:
		if ((t.b.y <= hg && t.c.y > hg) || (t.c.y <= hg && t.b.y > hg))
		{
			dx = t.c.x - t.b.x; dy = t.c.y - t.b.y; dz = t.c.z - t.b.z; dIllum = t.illumC - t.illumB;
			yy = (float)hg - (float)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * ((yy + corr) / (float)dy);
			illum = t.illumB + dIllum * (yy / (float)dy);
			wd = t.b.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0f;
				endIndex++;
			}
		}
		//Side C-A:
		if ((t.c.y <= hg && t.a.y > hg) || (t.a.y <= hg && t.c.y > hg))
		{
			dx = t.a.x - t.c.x; dy = t.a.y - t.c.y; dz = t.a.z - t.c.z; dIllum = t.illumA - t.illumC;
			yy = (float)hg - (float)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * ((yy + corr) / (float)dy);
			illum = t.illumC + dIllum * (yy / (float)dy);
			wd = t.c.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0f;
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
			deltaZ = (endZ - startZ) / (float)span;
			deltaIll = ((endIll - startIll) / (float)span) / 100.0f;
			zCurrent = startZ;
			illCurrent = startIll / 100.0f;

			startP.x = startX;	startP.y = hg;	startP.z = startZ;
			endP.x = endX;		endP.y = hg;	endP.z = endZ;
			startV = screen2view(startP, screen, h_ratio, v_ratio);
			endV = screen2view(endP, screen, h_ratio, v_ratio);

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					if (1 / zCurrent < screen->depthBuffer[hg * w + i])
					{
						if (illCurrent > 1.0f)
						{
							screen->pixelBuffer[hg * w + i] = 0x00ffffff;
						}
						else
						{
							screen->pixelBuffer[hg * w + i] = getColour(0, (byte)(r * illCurrent), (byte)(g * illCurrent), (byte)(b * illCurrent));
						}
						screen->depthBuffer[hg * w + i] = 1 / zCurrent;
					}
					zCurrent += deltaZ;
					illCurrent += deltaIll;
				}
			}
		}
	}
}


void Projection::fillTriangleCheckerboard(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, float h_ratio, float v_ratio)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int halfW = std::round(w * 0.5f);
	int halfH = std::round(h * 0.5f);
	int dx, dy;
	float dIllum, illum;
	float xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	float zLimit[2] = { 0.0f, 0.0f };
	float illLimit[2] = { 0.0f, 0.0f };

	vect3 nullVect = { 0.0f, 0.0f, 0.0f, 0.0f };
	vect3 sides[4] = { nullVect, nullVect, nullVect, nullVect };
	vect3 sideL[2] = { nullVect, nullVect };
	vect3 sideR[2] = { nullVect, nullVect };
	textCoord nullCoord = { 0.0f, 0.0f };
	textCoord uvSides[4] = { nullCoord, nullCoord, nullCoord, nullCoord };
	textCoord uvSideL[2] = { nullCoord, nullCoord };
	textCoord uvSideR[2] = { nullCoord, nullCoord };

	int endIndex;
	int startX, endX;
	float startZ, endZ, zCurrent, startIll, endIll, illCurrent;
	float invertStartPz, invertEndPz, invertCurrentPz;
	float hCorr = w * 0.475f * h_ratio;
	float vCorr = w * 0.475f * v_ratio;
	float deltaZ, deltaIll;

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
			yy = (float)hg - (float)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (float)dy);
			illum = t.illumA + dIllum * (yy / (float)dy);
			wd = t.a.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0f;

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
			yy = (float)hg - (float)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (float)dy);
			illum = t.illumB + dIllum * (yy / (float)dy);
			wd = t.b.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0f;

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
			yy = (float)hg - (float)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (float)dy);
			illum = t.illumC + dIllum * (yy / (float)dy);
			wd = t.c.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0f;

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
			deltaZ = (endZ - startZ) / (float)span;
			deltaIll = ((endIll - startIll) / (float)span) / 100.0f;
			zCurrent = startZ;
			illCurrent = startIll / 100.0f;

			startP.x = startX;	startP.y = hg;	startP.z = startZ;
			endP.x = endX;		endP.y = hg;	endP.z = endZ;

			invertStartPz = 1 / startP.z;
			startVert.x = (startP.x - float(halfW)) * invertStartPz / hCorr;
			startVert.y = (float(halfH) - startP.y) * invertStartPz / vCorr;
			startVert.z = invertStartPz;

			invertEndPz = 1 / endP.z;
			endVert.x = (endP.x - float(halfW)) * invertEndPz / hCorr;
			endVert.y = (float(halfH) - endP.y) * invertEndPz / vCorr;
			endVert.z = invertEndPz;


			startUV = getUVCoord(sideL[0], sideL[1], uvSideL[0], uvSideL[1], startVert);
			endUV = getUVCoord(sideR[0], sideR[1], uvSideR[0], uvSideR[1], endVert);


			currentP.y = hg;

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					if (1 / zCurrent < screen->depthBuffer[hg * w + i])
					{
						currentP.x = i;
						currentP.z = zCurrent;

						invertCurrentPz = 1 / currentP.z;
						currentVert.x = (currentP.x - float(halfW)) * invertCurrentPz / hCorr;
						currentVert.y = (float(halfH) - currentP.y) * invertCurrentPz / vCorr;
						currentVert.z = invertCurrentPz;

						sampleUV = getUVCoord(startVert, endVert, startUV, endUV, currentVert);

						sampleXnew = (int)(64.0f * sampleUV.u);
						sampleYnew = (int)(64.0f * sampleUV.v);

						Uint32 blue = getColour(0, 0, 0, (byte)(255 * illCurrent));
						Uint32 white = getColour(0, (byte)(255 * illCurrent), (byte)(255 * illCurrent), (byte)(255 * illCurrent));

						if (int(sampleXnew / 16) % 2)
						{
							if (int(sampleYnew / 16) % 2)
							{
								screen->pixelBuffer[hg * w + i] = white;
							}
							else
							{
								screen->pixelBuffer[hg * w + i] = blue;
							}
						}
						else
						{
							if (int(sampleYnew / 16) % 2)
							{
								screen->pixelBuffer[hg * w + i] = blue;
							}
							else
							{
								screen->pixelBuffer[hg * w + i] = white;
							}
						}
						screen->depthBuffer[hg * w + i] = 1 / zCurrent;
					}
					zCurrent += deltaZ;
					illCurrent += deltaIll;
				}
			}
		}
	}
}


void Projection::fillTriangleDepthVisualised(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, float zNear, float h_ratio, float v_ratio)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int dx, dy;
	float xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	float zLimit[2] = { 0.0f, 0.0f };

	int endIndex;
	int startX, endX;
	float startZ, endZ, zCurrent;
	float invertCurrentZ;
	float deltaZ;
	coord2 currentP, startP, endP;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z;
			yy = (float)hg - (float)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (float)dy);
			wd = t.a.x + std::round(xx);
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
			yy = (float)hg - (float)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (float)dy);
			wd = t.b.x + std::round(xx);
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
			yy = (float)hg - (float)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (float)dy);
			wd = t.c.x + std::round(xx);
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
			deltaZ = (endZ - startZ) / (float)span;
			zCurrent = startZ;

			currentP.y = hg;

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					invertCurrentZ = 1 / zCurrent;
					if (invertCurrentZ < screen->depthBuffer[hg * w + i])
					{
						screen->pixelBuffer[hg * w + i] = getColour(0, (byte)(255.0f * zCurrent), (byte)(255.0f * zCurrent), (byte)(255.0f * zCurrent));
						screen->depthBuffer[hg * w + i] = invertCurrentZ;
					}
					zCurrent += deltaZ;
				}
			}
		}
	}
}


void Projection::fillTriangleSunlight(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, float h_ratio, float v_ratio, txt* texture)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int halfW = std::round(w * 0.5f);
	int halfH = std::round(h * 0.5f);
	int dx, dy;
	float dIllum, illum;
	float xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	float zLimit[2] = { 0.0f, 0.0f };
	float illLimit[2] = { 0.0f, 0.0f };

	vect3 nullVect = { 0.0f, 0.0f, 0.0f, 0.0f };
	vect3 sides[4] = { nullVect, nullVect, nullVect, nullVect };
	vect3 sideL[2] = { nullVect, nullVect };
	vect3 sideR[2] = { nullVect, nullVect };
	textCoord nullCoord = { 0.0f, 0.0f };
	textCoord uvSides[4] = { nullCoord, nullCoord, nullCoord, nullCoord };
	textCoord uvSideL[2] = { nullCoord, nullCoord };
	textCoord uvSideR[2] = { nullCoord, nullCoord };

	int endIndex;
	int startX, endX;
	float startZ, endZ, zCurrent, startIll, endIll, illCurrent;
	float invertStartPz, invertEndPz, invertCurrentZ;
	float hCorr = w * 0.475f * h_ratio;
	float vCorr = w * 0.475f * v_ratio;
	float deltaZ, deltaIll;

	coord2 currentP, startP, endP;
	vect3 currentVert, startVert, endVert;
	textCoord startUV, endUV, sampleUV;
	int textureWidth = texture->w;
	int textureHeight = texture->h;
	int sampleXold = 0, sampleYold = 0, sampleXnew = 0, sampleYnew = 0;
	Uint32 finalPixel;

	for (int hg = yMin; hg < yMax; hg++)
	{
		int hgw = hg * w;
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z; dIllum = t.illumB - t.illumA;
			yy = (float)hg - (float)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (float)dy);
			illum = t.illumA + dIllum * (yy / (float)dy);
			wd = t.a.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0f;

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
			yy = (float)hg - (float)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (float)dy);
			illum = t.illumB + dIllum * (yy / (float)dy);
			wd = t.b.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0f;

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
			yy = (float)hg - (float)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (float)dy);
			illum = t.illumC + dIllum * (yy / (float)dy);
			wd = t.c.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0f;

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
			deltaZ = (endZ - startZ) / (float)span;
			deltaIll = ((endIll - startIll) / (float)span) / 100.0f;
			zCurrent = startZ;
			illCurrent = startIll / 100.0f;

			startP.x = startX;	startP.y = hg;	startP.z = startZ;
			endP.x = endX;		endP.y = hg;	endP.z = endZ;

			invertStartPz = 1 / startP.z;
			startVert.x = (startP.x - float(halfW)) * invertStartPz / hCorr;
			startVert.y = (float(halfH) - startP.y) * invertStartPz / vCorr;
			startVert.z = invertStartPz;

			invertEndPz = 1 / endP.z;
			endVert.x = (endP.x - float(halfW)) * invertEndPz / hCorr;
			endVert.y = (float(halfH) - endP.y) * invertEndPz / vCorr;
			endVert.z = invertEndPz;

			startUV = getUVCoord(sideL[0], sideL[1], uvSideL[0], uvSideL[1], startVert);
			endUV = getUVCoord(sideR[0], sideR[1], uvSideR[0], uvSideR[1], endVert);

			currentP.y = hg;

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					invertCurrentZ = 1 / zCurrent;
					if (invertCurrentZ < screen->depthBuffer[hgw + i])
					{
						currentP.x = i;
						currentP.z = zCurrent;

						currentVert.x = (currentP.x - float(halfW)) * invertCurrentZ / hCorr;
						currentVert.y = (float(halfH) - currentP.y) * invertCurrentZ / vCorr;
						currentVert.z = invertCurrentZ;

						sampleUV = getUVCoord(startVert, endVert, startUV, endUV, currentVert);

						sampleXnew = ((int)(textureWidth * sampleUV.u)) % textureWidth;
						sampleYnew = ((int)(textureHeight * sampleUV.v)) % textureHeight;

						if (sampleXnew < 0) { sampleXnew = textureWidth + sampleXnew % textureWidth; }
						if (sampleYnew < 0) { sampleYnew = textureHeight + sampleYnew % textureHeight; }

						if ((sampleXnew != sampleXold) || (sampleYnew != sampleYold))
						{
							finalPixel = texture->pixels[sampleYnew * textureWidth + sampleXnew];
						}

						sampleXold = sampleXnew;
						sampleYold = sampleYnew;

						screen->pixelBuffer[hgw + i] = modifyColour(finalPixel, illCurrent);

						screen->depthBuffer[hgw + i] = invertCurrentZ;
					}
					zCurrent += deltaZ;
					illCurrent += deltaIll;
				}
			}
		}
	}
}


void Projection::fillTriangleTorchlight(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, float h_ratio, float v_ratio, txt* texture,
	const float& lightIntensity, const float& maxIllumination)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int halfW = std::round(w * 0.5f);
	int halfH = std::round(h * 0.5f);
	int dx, dy;
	float xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	float zLimit[2] = { 0.0f, 0.0f };

	vect3 nullVect = { 0.0f, 0.0f, 0.0f, 0.0f };
	vect3 sides[4] = { nullVect, nullVect, nullVect, nullVect };
	vect3 sideL[2] = { nullVect, nullVect };
	vect3 sideR[2] = { nullVect, nullVect };
	textCoord nullCoord = { 0.0f, 0.0f };
	textCoord uvSides[4] = { nullCoord, nullCoord, nullCoord, nullCoord };
	textCoord uvSideL[2] = { nullCoord, nullCoord };
	textCoord uvSideR[2] = { nullCoord, nullCoord };

	int endIndex;
	int startX, endX;
	float startZ, endZ, zCurrent, illCurrent;
	float invertStartPz, invertEndPz, invertCurrentZ;
	float hCorr = w * 0.475f * h_ratio;
	float vCorr = w * 0.475f * v_ratio;
	float deltaZ;

	coord2 currentP, startP, endP;
	vect3 currentVert, startVert, endVert;
	textCoord startUV, endUV, sampleUV;
	int sampleXold = 0, sampleYold = 0, sampleXnew = 0, sampleYnew = 0;
	Uint32 finalPixel;
	float distSquared;
	vect3 lightVector;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z;
			yy = (float)hg - (float)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (float)dy);
			wd = t.a.x + std::round(xx);
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
			yy = (float)hg - (float)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (float)dy);
			wd = t.b.x + std::round(xx);
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
			yy = (float)hg - (float)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (float)dy);
			wd = t.c.x + std::round(xx);
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
			deltaZ = (endZ - startZ) / (float)span;
			zCurrent = startZ;

			startP.x = startX;	startP.y = hg;	startP.z = startZ;
			endP.x = endX;		endP.y = hg;	endP.z = endZ;

			invertStartPz = 1 / startP.z;
			startVert.x = (startP.x - float(halfW)) * invertStartPz / hCorr;
			startVert.y = (float(halfH) - startP.y) * invertStartPz / vCorr;
			startVert.z = invertStartPz;

			invertEndPz = 1 / endP.z;
			endVert.x = (endP.x - float(halfW)) * invertEndPz / hCorr;
			endVert.y = (float(halfH) - endP.y) * invertEndPz / vCorr;
			endVert.z = invertEndPz;

			startUV = getUVCoord(sideL[0], sideL[1], uvSideL[0], uvSideL[1], startVert);
			endUV = getUVCoord(sideR[0], sideR[1], uvSideR[0], uvSideR[1], endVert);

			currentP.y = hg;

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					invertCurrentZ = 1 / zCurrent;
					if (invertCurrentZ < screen->depthBuffer[hg * w + i])
					{
						currentP.x = i;
						currentP.z = zCurrent;

						currentVert.x = (currentP.x - float(halfW)) * invertCurrentZ / hCorr;
						currentVert.y = (float(halfH) - currentP.y) * invertCurrentZ / vCorr;
						currentVert.z = invertCurrentZ;

						illCurrent = 1.0f;
						lightVector = unitVector(currentVert);

						distSquared = distanceSquared(currentVert, { 0.0f, 0.0f, 0.0f, 1.0f });
						if (distSquared >= 0.0f)
						{
							illCurrent -= 1.0f / dotProductSquared({ 0.0f, 0.0f, 1.0f, 1.0f }, lightVector);
							illCurrent += lightIntensity / distSquared;
						}

						clampValue(&illCurrent, 0.025f, maxIllumination);

						sampleUV = getUVCoord(startVert, endVert, startUV, endUV, currentVert);

						sampleXnew = ((int)(texture->w * sampleUV.u)) % texture->w;
						sampleYnew = ((int)(texture->h * sampleUV.v)) % texture->h;

						if (sampleYnew < 0) { sampleYnew = texture->h + sampleYnew % texture->h; }
						if (sampleXnew < 0) { sampleXnew = texture->w + sampleXnew % texture->w; }

						if ((sampleXnew != sampleXold) || (sampleYnew != sampleYold))
						{
							finalPixel = texture->pixels[sampleYnew * texture->w + sampleXnew];
						}

						sampleXold = sampleXnew;
						sampleYold = sampleYnew;

						screen->pixelBuffer[hg * w + i] = modifyColour(finalPixel, illCurrent);
						screen->depthBuffer[hg * w + i] = invertCurrentZ;
					}
					zCurrent += deltaZ;
				}
			}
		}
	}
}


void Projection::fillTriangleTorchlightSolidColour(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, float h_ratio, float v_ratio,
	const float& lightIntensity, const float& maxIllumination)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int halfW = std::round(w * 0.5);
	int halfH = std::round(h * 0.5);
	int dx, dy;
	float xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	float zLimit[2] = { 0.0f, 0.0f };

	vect3 nullVect = { 0.0f, 0.0f, 0.0f, 0.0f };
	vect3 sides[4] = { nullVect, nullVect, nullVect, nullVect };
	vect3 sideL[2] = { nullVect, nullVect };
	vect3 sideR[2] = { nullVect, nullVect };

	int endIndex;
	int startX, endX;
	float startZ, endZ, zCurrent, illCurrent;
	float invertCurrentZ;
	float hCorr = w * 0.475f * h_ratio;
	float vCorr = w * 0.475f * v_ratio;
	float deltaZ;

	coord2 currentP, startP, endP;
	vect3 currentVert;
	int sampleXold = 0, sampleYold = 0, sampleXnew = 0, sampleYnew = 0;
	float distSquared;
	vect3 lightVector;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z;
			yy = (float)hg - (float)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (float)dy);
			wd = t.a.x + std::round(xx);
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
			yy = (float)hg - (float)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (float)dy);
			wd = t.b.x + std::round(xx);
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
			yy = (float)hg - (float)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (float)dy);
			wd = t.c.x + std::round(xx);
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
			deltaZ = (endZ - startZ) / (float)span;
			zCurrent = startZ;

			currentP.y = hg;

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					invertCurrentZ = 1 / zCurrent;
					if (invertCurrentZ < screen->depthBuffer[hg * w + i])
					{
						currentP.x = i;
						currentP.z = zCurrent;

						currentVert.x = (currentP.x - float(halfW)) * invertCurrentZ / hCorr;
						currentVert.y = (float(halfH) - currentP.y) * invertCurrentZ / vCorr;
						currentVert.z = invertCurrentZ;

						illCurrent = 1.0f;
						lightVector = unitVector(currentVert);

						distSquared = distanceSquared(currentVert, { 0.0f, 0.0f, 0.0f, 1.0f });
						if (distSquared >= 0.0f)
						{
							illCurrent -= 1.0f / dotProductSquared({ 0.0f, 0.0f, 1.0f, 1.0f }, lightVector);
							illCurrent += lightIntensity / distSquared;
						}

						clampValue(&illCurrent, 0.025f, maxIllumination);

						screen->pixelBuffer[hg * w + i] = modifyColour(t.h, illCurrent);
						screen->depthBuffer[hg * w + i] = invertCurrentZ;
					}
					zCurrent += deltaZ;
				}
			}
		}
	}
}


void Projection::fillTriangleBlinnPhong(const triangle2dG& t, float* spL, std::shared_ptr<Canvas> screen, float h_ratio, float v_ratio)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int dx, dy;
	float dIllum, illum;
	float dSpec, spec;
	float xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	float zLimit[2] = { 0.0f, 0.0f };
	float illLimit[2] = { 0.0f, 0.0f };
	float specLimit[2] = { 0.0f, 0.0f };
	int endIndex;
	int startX, endX;
	float startZ, endZ, zCurrent, startIll, endIll, illCurrent, startSpec, endSpec, specCurrent;

	float deltaZ, deltaIll, deltaSpec;

	byte r, g, b;
	r = ((byte)(t.h >> 16 & 0xFF));
	g = ((byte)(t.h >> 8 & 0xFF));
	b = ((byte)(t.h & 0xFF));

	float corr = 0.0f;

	coord2 currentP, startP, endP;
	vect3 startV, endV;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z; dIllum = t.illumB - t.illumA; dSpec = spL[1] - spL[0];
			yy = (float)hg - (float)t.a.y; xx = dx * (yy / dy);
			zz = t.a.z + dz * (yy / (float)dy);
			illum = t.illumA + dIllum * (yy / (float)dy);
			spec = spL[0] + dSpec * (yy / (float)dy);
			wd = t.a.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum;
				specLimit[endIndex] = spec;
				endIndex++;
			}
		}
		//Side B-C:
		if ((t.b.y <= hg && t.c.y > hg) || (t.c.y <= hg && t.b.y > hg))
		{
			dx = t.c.x - t.b.x; dy = t.c.y - t.b.y; dz = t.c.z - t.b.z; dIllum = t.illumC - t.illumB; dSpec = spL[2] - spL[1];
			yy = (float)hg - (float)t.b.y; xx = dx * (yy / dy);
			zz = t.b.z + dz * (yy / (float)dy);
			illum = t.illumB + dIllum * (yy / (float)dy);
			spec = spL[1] + dSpec * (yy / (float)dy);
			wd = t.b.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum;
				specLimit[endIndex] = spec;
				endIndex++;
			}
		}
		//Side C-A:
		if ((t.c.y <= hg && t.a.y > hg) || (t.a.y <= hg && t.c.y > hg))
		{
			dx = t.a.x - t.c.x; dy = t.a.y - t.c.y; dz = t.a.z - t.c.z; dIllum = t.illumA - t.illumC; dSpec = spL[0] - spL[2];
			yy = (float)hg - (float)t.c.y; xx = dx * (yy / dy);
			zz = t.c.z + dz * (yy / (float)dy);
			illum = t.illumC + dIllum * (yy / (float)dy);
			spec = spL[2] + dSpec * (yy / (float)dy);
			wd = t.c.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum;
				specLimit[endIndex] = spec;
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
				startSpec = specLimit[0];
				endSpec = specLimit[1];
			}
			else
			{
				startX = lineEnd[1];
				endX = lineEnd[0];
				startZ = zLimit[1];
				endZ = zLimit[0];
				startIll = illLimit[1];
				endIll = illLimit[0];
				startSpec = specLimit[1];
				endSpec = specLimit[0];
			}
			int span = abs(endX - startX + 1);
			deltaZ = (endZ - startZ) / (float)span;
			deltaIll = ((endIll - startIll) / (float)span);
			deltaSpec = ((endSpec - startSpec) / (float)span);
			zCurrent = startZ;
			illCurrent = startIll;
			specCurrent = startSpec;

			startP.x = startX;	startP.y = hg;	startP.z = startZ;
			endP.x = endX;		endP.y = hg;	endP.z = endZ;
			startV = screen2view(startP, screen, h_ratio, v_ratio);
			endV = screen2view(endP, screen, h_ratio, v_ratio);

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					if (1 / zCurrent < screen->depthBuffer[hg * w + i])
					{
						float rgb[3];

						rgb[0] = (static_cast<float>(r)* illCurrent) + (255.0f * specCurrent);
						rgb[1] = (static_cast<float>(g)* illCurrent) + (255.0f * specCurrent);
						rgb[2] = (static_cast<float>(b)* illCurrent) + (255.0f * specCurrent);

						float rgbMax = getMax(3, rgb);
						if (rgbMax > 255.0f)
						{
							float scale = 1.0f / getMax(3, rgb);

							rgb[0] = (rgb[0] * scale) * 255.0f;
							rgb[1] = (rgb[1] * scale) * 255.0f;
							rgb[2] = (rgb[2] * scale) * 255.0f;
						}

						Uint32 finalPixel = getColour(0, static_cast<unsigned char>(rgb[0]),
							static_cast<unsigned char>(rgb[1]),
							static_cast<unsigned char>(rgb[2]));

						screen->pixelBuffer[hg * w + i] = finalPixel;
						screen->depthBuffer[hg * w + i] = 1 / zCurrent;
					}
					zCurrent += deltaZ;
					illCurrent += deltaIll;
					specCurrent += deltaSpec;
				}
			}
		}
	}
}
