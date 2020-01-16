#include "Projection.h"



Projection::Projection()
{
	std::cout << "Projection toolbox created..." << std::endl;
}


Projection::~Projection()
{
	std::cout << "Projection toolbox destroyed..." << std::endl;
}


int Projection::GetYMax3(coord2* p)
{
	int yMax = p[0].y;
	for (int i = 1; i < 3; i++)
		if (p[i].y > yMax)
			yMax = p[i].y;
	return yMax;
}


int Projection::GetYMin3(coord2 * p)
{
	int yMin = p[0].y;
	for (int i = 1; i < 3; i++)
		if (p[i].y < yMin)
			yMin = p[i].y;
	return yMin;
}


void Projection::clampValue(double* value, double lower, double upper)
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


transform3d Projection::getTransformation(double azm, double alt, double rol)
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


vect3 Projection::rotXrad(const double& sinA, const double& cosA, const vect3& v)
{
	vect3 t;

	mat4x4 m = {		1,          0,         0,          0,
						0,       cosA,      sinA,          0,
						0,      -sinA,      cosA,          0,
						0,          0,         0,          1 };

	t = multiplyVxM(m, v);

	return t;
}


vect3 Projection::rotYrad(const double& sinA, const double& cosA, const vect3& v)
{
	vect3 t;

	mat4x4 m = {	 cosA,          0,     -sinA,          0,
						0,          1,         0,          0,
					 sinA,          0,      cosA,          0,
						0,          0,         0,          1 };

	t = multiplyVxM(m, v);

	return t;
}


vect3 Projection::rotZrad(const double& sinA, const double& cosA, const vect3& v)
{
	vect3 t;

	mat4x4 m = {	 cosA,       sinA,         0,          0,
					-sinA,       cosA,         0,          0,
						0,          0,         1,          0,
						0,          0,         0,          1 };

	t = multiplyVxM(m, v);

	return t;
}


triangle3dV Projection::rotXT(double angle, triangle3dV T)
{
	double sin_a = sin(angle);
	double cos_a = cos(angle);

	T.A = rotXrad(sin_a, cos_a, T.A);
	T.B = rotXrad(sin_a, cos_a, T.B);
	T.C = rotXrad(sin_a, cos_a, T.C);
	T.An = rotXrad(sin_a, cos_a, T.An);
	T.Bn = rotXrad(sin_a, cos_a, T.Bn);
	T.Cn = rotXrad(sin_a, cos_a, T.Cn);
	T.N = rotXrad(sin_a, cos_a, T.N);

	return T;
}


triangle3dV Projection::rotYT(double angle, triangle3dV T)
{
	double sin_a = sin(angle);
	double cos_a = cos(angle);

	T.A = rotYrad(sin_a, cos_a, T.A);
	T.B = rotYrad(sin_a, cos_a, T.B);
	T.C = rotYrad(sin_a, cos_a, T.C);
	T.An = rotYrad(sin_a, cos_a, T.An);
	T.Bn = rotYrad(sin_a, cos_a, T.Bn);
	T.Cn = rotYrad(sin_a, cos_a, T.Cn);
	T.N = rotYrad(sin_a, cos_a, T.N);

	return T;
}


triangle3dV Projection::rotZT(double angle, triangle3dV T)
{
	double sin_a = sin(angle);
	double cos_a = cos(angle);

	T.A = rotZrad(sin_a, cos_a, T.A);
	T.B = rotZrad(sin_a, cos_a, T.B);
	T.C = rotZrad(sin_a, cos_a, T.C);
	T.An = rotZrad(sin_a, cos_a, T.An);
	T.Bn = rotZrad(sin_a, cos_a, T.Bn);
	T.Cn = rotZrad(sin_a, cos_a, T.Cn);
	T.N = rotZrad(sin_a, cos_a, T.N);

	return T;
}


vect3 Projection::translate(double x, double y, double z, vect3 v)
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


triangle3dV Projection::translateT(double x, double y, double z, triangle3dV T)
{
	T.A = translate(x, y, z, T.A);
	T.B = translate(x, y, z, T.B);
	T.C = translate(x, y, z, T.C);

	return T;
}


vect3 Projection::scale(double scaleX, double scaleY, double scaleZ, vect3 v)
{
	vect3 t;

	mat4x4 m = { scaleX,          0,         0,          0,
						0,     scaleY,         0,          0,
						0,          0,    scaleZ,          0,
						0,          0,         0,          1 };
	t = multiplyMxV(m, v);

	return t;
}


triangle3dV Projection::scaleT(double x, double y, double z, triangle3dV T)
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


void Projection::rotateMesh(int n, triangle3dV* object, double rx, double ry, double rz)
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


void Projection::transformMesh(int n, triangle3dV* object, double scX, double scY, double scZ, double mvX, double mvY, double mvZ,
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


void Projection::object2worldT(const vect3& sc, const vect3& mv, const vect3& rt, triangle3dV& T)
{
	T = Projection::rotXT(rt.x, T);
	T = Projection::rotYT(rt.y, T);
	T = Projection::rotZT(rt.z, T);
	T = translateT(mv.x, mv.y, mv.z, T);
}


void Projection::world2view(triangle3dV& tr, transform3d T, double x, double y, double z)
{
	tr.A = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr.A))));
	tr.B = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr.B))));
	tr.C = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr.C))));
	tr.An = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, tr.An)));
	tr.Bn = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, tr.Bn)));
	tr.Cn = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, tr.Cn)));

	tr.N = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, tr.N)));
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


vect3 Projection::world2view(transform3d T, vect3 tr, double x, double y, double z)
{
	vect3 v;

	v = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, tr))));

	return v;
}


point3 Projection::world2viewP(point3 p, mat4x4& rot, mat4x4& mov)
{
	point3 tempPoint;

	//tempPoint.P = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, translate(-x, -y, -z, p.P))));
	tempPoint.P = rot * mov * p.P;
	tempPoint.P.w = 1.0;
	tempPoint.colour = p.colour;

	return tempPoint;
}


vect3 Projection::sun2view(transform3d T, vect3 v)
{
	vect3 t;

	t = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, v)));

	return t;
}


vect3 Projection::screen2view(coord2 pixel, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

	vect3 vertex;
	double t = 1.0f / pixel.z;

	vertex.x = double(pixel.x) - double(w / 2) * t / (w * 0.475 * h_ratio);
	vertex.y = double(h / 2) - double(pixel.y) * t / (w * 0.475 * v_ratio);
	vertex.z = t;
	vertex.w = 1.0f;

	return vertex;
}


void Projection::illuminatePoly(LightSource L, triangle3dV* T, const triangle3dV& P, const projectionStyle& style)
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


Uint32 Projection::modifyColour(const Uint32& inputColour, const double& illumination)
{
	bool red = true, green = true, blue = true;
	byte a = 0, r, g, b, r2fill = 0, g2fill = 0, b2fill = 0;
	double illSurplus = 0.0;

	if (illumination <= 1.0)
	{
		r = (byte)(double(inputColour >> 16 & 255) * illumination);
		g = (byte)(double(inputColour >> 8 & 255) * illumination);
		b = (byte)(double(inputColour & 255) * illumination);
	}
	else if (illumination > 1.0)
	{
		illSurplus = illumination - 1.0;

		r = (byte)(double(inputColour >> 16 & 255) * illumination);
		g = (byte)(double(inputColour >> 8 & 255) * illumination);
		b = (byte)(double(inputColour & 255) * illumination);

		r2fill = 255 - r;	g2fill = 255 - g;	b2fill = 255 - b;

		double s = 1.0f / (MAX_ILLUMINATION - 1.0) * illSurplus;

		r += byte(double(r2fill) * s);
		g += byte(double(g2fill) * s);
		b += byte(double(b2fill) * s);
	}

	return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}


textCoord Projection::getUVCoord(const vect3& startV, const vect3& endV, const textCoord& startC, const textCoord& endC, const vect3& testV)
{
	textCoord testC;

	double a = dotProduct(subVectors(testV, startV), subVectors(endV, startV));
	double b = dotProduct(subVectors(endV, startV), subVectors(endV, startV));
	double d = (b != 0.0) ? (a / b) : (0.0);

	testC.u = startC.u + d * (endC.u - startC.u);
	testC.v = startC.v + d * (endC.v - startC.v);

	return testC;
}


void Projection::fillTriangleSolidColour(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

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
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (double)dy);
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
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (double)dy);
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
			deltaZ = (endZ - startZ) / (double)span;
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
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (double)dy);
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
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (double)dy);
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
			deltaZ = (endZ - startZ) / (double)span;
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


void Projection::fillTriangleGouraudShaded(const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

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

	double deltaZ, deltaIll;

	byte r, g, b;
	r = ((byte)(t.h >> 16 & 0xFF));
	g = ((byte)(t.h >> 8 & 0xFF));
	b = ((byte)(t.h & 0xFF));

	double corr = 0.0;

	coord2 currentP, startP, endP;
	vect3 startV, endV;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z; dIllum = t.illumB - t.illumA;
			yy = (double)hg - (double)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * ((yy + corr) / (double)dy);
			illum = t.illumA + dIllum * (yy / (double)dy);
			wd = t.a.x + std::round(xx);
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
			wd = t.b.x + std::round(xx);
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
			wd = t.c.x + std::round(xx);
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
			startV = screen2view(startP, screen, h_ratio, v_ratio);
			endV = screen2view(endP, screen, h_ratio, v_ratio);

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					if (1 / zCurrent < screen->depthBuffer[hg * w + i])
					{
						screen->pixelBuffer[hg * w + i] = getColour(0, (byte)(r * illCurrent), (byte)(g * illCurrent), (byte)(b * illCurrent));
						screen->depthBuffer[hg * w + i] = 1 / zCurrent;
					}
					zCurrent += deltaZ;
					illCurrent += deltaIll;
				}
			}
		}
	}
}


void Projection::fillTriangleCheckerboard(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio)
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
			wd = t.a.x + std::round(xx);
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
			wd = t.b.x + std::round(xx);
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
			wd = t.c.x + std::round(xx);
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
					if (1 / zCurrent < screen->depthBuffer[hg * w + i])
					{
						currentP.x = i;
						currentP.z = zCurrent;

						invertCurrentPz = 1 / currentP.z;
						currentVert.x = (currentP.x - double(halfW)) * invertCurrentPz / hCorr;
						currentVert.y = (double(halfH) - currentP.y) * invertCurrentPz / vCorr;
						currentVert.z = invertCurrentPz;

						sampleUV = getUVCoord(startVert, endVert, startUV, endUV, currentVert);

						sampleXnew = (int)(64.0 * sampleUV.u);
						sampleYnew = (int)(64.0 * sampleUV.v);

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


void Projection::fillTriangleDepthVisualised(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double zNear, double h_ratio, double v_ratio)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int dx, dy;
	double xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	double zLimit[2] = { 0.0, 0.0 };

	int endIndex;
	int startX, endX;
	double startZ, endZ, zCurrent;
	double invertCurrentZ;
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
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (double)dy);
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
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (double)dy);
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
			deltaZ = (endZ - startZ) / (double)span;
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


void Projection::fillTriangleSunlight(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio, txt* texture)
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
	double invertStartPz, invertEndPz, invertCurrentZ;
	double hCorr = w * 0.475 * h_ratio;
	double vCorr = w * 0.475 * v_ratio;
	double deltaZ, deltaIll;

	coord2 currentP, startP, endP;
	vect3 currentVert, startVert, endVert;
	textCoord startUV, endUV, sampleUV;
	int sampleXold = 0, sampleYold = 0, sampleXnew = 0, sampleYnew = 0;
	Uint32 finalPixel;

	byte a = 0, r2fill = 0, g2fill = 0, b2fill = 0;
	double illSurplus = 0.0;

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
			wd = t.a.x + std::round(xx);
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
			wd = t.b.x + std::round(xx);
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
			wd = t.c.x + std::round(xx);
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
					if (invertCurrentZ < screen->depthBuffer[hgw + i])
					{
						currentP.x = i;
						currentP.z = zCurrent;

						currentVert.x = (currentP.x - double(halfW)) * invertCurrentZ / hCorr;
						currentVert.y = (double(halfH) - currentP.y) * invertCurrentZ / vCorr;
						currentVert.z = invertCurrentZ;

						sampleUV = getUVCoord(startVert, endVert, startUV, endUV, currentVert);

						sampleXnew = ((int)(texture->w * sampleUV.u)) % texture->w;
						sampleYnew = ((int)(texture->h * sampleUV.v)) % texture->h;

						if (sampleXnew < 0) { sampleXnew = texture->w + sampleXnew % texture->w; }
						if (sampleYnew < 0) { sampleYnew = texture->h + sampleYnew % texture->h; }

						if ((sampleXnew != sampleXold) || (sampleYnew != sampleYold))
						{
							finalPixel = texture->pixels[sampleYnew * texture->w + sampleXnew];
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


void Projection::fillTriangleTorchlight(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio, txt* texture,
	const double& lightIntensity, const double& maxIllumination)
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
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (double)dy);
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
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (double)dy);
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
					if (invertCurrentZ < screen->depthBuffer[hg * w + i])
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


void Projection::fillTriangleTorchlightSolidColour(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio,
	const double& lightIntensity, const double& maxIllumination)
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
	double invertCurrentZ;
	double hCorr = w * 0.475 * h_ratio;
	double vCorr = w * 0.475 * v_ratio;
	double deltaZ;

	coord2 currentP, startP, endP;
	vect3 currentVert;
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
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (double)dy);
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
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (double)dy);
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
			deltaZ = (endZ - startZ) / (double)span;
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

						screen->pixelBuffer[hg * w + i] = modifyColour(t.h, illCurrent);
						screen->depthBuffer[hg * w + i] = invertCurrentZ;
					}
					zCurrent += deltaZ;
				}
			}
		}
	}
}
