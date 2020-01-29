#include "Lamp.h"

Lamp::Lamp(float cx, float cy, float cz, float az, float al, float rl,
	float fov, float nr, float fr, int width, int height) :
	x(cx),
	y(cy),
	z(cz),
	azm(az),
	alt(al),
	rol(rl),
	fovH(fov),
	zNear(nr),
	zFar(fr),
	w(width),
	h(height)
{
	std::cout << "Lamp constructor called" << std::endl;

	Frustum.initFrustum(this->getFovH(), this->getFovV(), zNear, zFar);

	hRatio = this->getHRatio();
	vRatio = this->getVRatio();

	this->clearVertexList();

	depthBuffer = new float[w * h];
}


Lamp::~Lamp()
{
	delete[] depthBuffer;
}


void Lamp::update()
{

}


float Lamp::getIllumination(vect3& P, vect3& N)
{
	if (pointInsideFrustum(P))
		return 1.0f;
	else
		return 0.0f;
}


float Lamp::getBlinnSpecular(vect3& P, vect3& N, vect3& V, float& shine)
{
	return 0.0f;
}


float Lamp::getFovH()
{
	return fovH;
}


float Lamp::getFovV()
{
	return (atanf(tanf(fovH * 0.5f) / (w / h))) * 2.0f;
}


float Lamp::getHRatio()
{
	return 1 / (tan(fovH / 2.0f));
}


float Lamp::getVRatio()
{
	return 1 / (tanf(atanf(tanf(fovH * 0.5f) / (w / h) * 2.0f)));
}


void Lamp::clearVertexList()
{
	for (int v = 0; v < MAXCLIPVERTS; v++)
		vertexList[v] = { 0.0f, 0.0f, 0.0f, 0.0f };
}


int Lamp::GetYMax3(coord2* p)
{
	int yMax = p[0].y;
	for (int i = 1; i < 3; i++)
		if (p[i].y > yMax)
			yMax = p[i].y;
	return yMax;
}


int Lamp::GetYMin3(coord2* p)
{
	int yMin = p[0].y;
	for (int i = 1; i < 3; i++)
		if (p[i].y < yMin)
			yMin = p[i].y;
	return yMin;
}


bool Lamp::polyFacingLamp(const triangle3dV& worldT)
{
	vect3 lightVector = subVectors(worldT.A, { x, y, z, 1.0 });
	return dotProduct(worldT.N, lightVector) < 0.0f ? true : false;
}


mat4x4 Lamp::getTranslation(vect3 mv)
{
	mat4x4 result;

	result = { 1.0f,          0.0f,         0.0f,		 mv.x,
				0.0f,          1.0f,         0.0f,		 mv.y,
				0.0f,          0.0f,         1.0f,		 mv.z,
				0.0f,          0.0f,         0.0f,       1.0f };

	return result;
}


mat4x4 Lamp::getRotation(axis t, float a)
{
	mat4x4 result = { 1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f };

	float sinA = sinf(a);
	float cosA = cosf(a);

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


void Lamp::object2world(mat4x4& MR, mat4x4& R, triangle3dV& T)
{
	T.A = MR * T.A;
	T.B = MR * T.B;
	T.C = MR * T.C;
	T.An = R * T.An;
	T.Bn = R * T.Bn;
	T.Cn = R * T.Cn;
	T.N = R * T.N;
}


void Lamp::world2view(mat4x4& RM, mat4x4& R, triangle3dV& T)
{
	T.A = RM * T.A;
	T.B = RM * T.B;
	T.C = RM * T.C;
	T.An = R * T.An;
	T.Bn = R * T.Bn;
	T.Cn = R * T.Cn;
	T.N = R * T.N;
}


inline coord2 Lamp::view2screen(const vect3& vertex, const float& hR, const float& vR)
{
	coord2 pixel;

	float s = float(w) * 0.475f; //w* 0.5f* 0.95f

	pixel.x = (int)(w * 0.5f + static_cast<float>(vertex.x / vertex.z)* s* hR);
	pixel.y = (int)(h * 0.5f - static_cast<float>(vertex.y / vertex.z)* s* vR);
	pixel.z = 1 / vertex.z;

	return pixel;
}


bool Lamp::pointInsideFrustum(vect3& V)
{
	plane currentPlane;

	currentPlane = Frustum.getNearPlane();
	if (pointBehindPlane(currentPlane, V))
		return false;

	currentPlane = Frustum.getTopPlane();
	if (pointBehindPlane(currentPlane, V))
		return false;

	currentPlane = Frustum.getBottomPlane();
	if (pointBehindPlane(currentPlane, V))
		return false;

	currentPlane = Frustum.getLeftPlane();
	if (pointBehindPlane(currentPlane, V))
		return false;

	currentPlane = Frustum.getRightPlane();
	if (pointBehindPlane(currentPlane, V))
		return false;

	currentPlane = Frustum.getFarPlane();
	if (pointBehindPlane(currentPlane, V))
		return false;

	return true;
}


bool Lamp::pointBehindPlane(const plane& p, vect3& V)
{
	vect3 a = V - p.P;
	float sa = a * p.N;
	if (sa <= 0.0f)
		return true;
	else
		return false;
}


int Lamp::clipToFrustum(const triangle3dV& viewT, vect3* vertList)
{
	memset(vertList, 0, MAXCLIPVERTS * sizeof(float));

	vertList[0] = viewT.A;
	vertList[1] = viewT.B;
	vertList[2] = viewT.C;

	int nVert = 3;
	plane currentPlane;

	currentPlane = Frustum.getNearPlane();
	this->clipPoly(&nVert, vertList, currentPlane);
	currentPlane = Frustum.getTopPlane();
	this->clipPoly(&nVert, vertList, currentPlane);
	currentPlane = Frustum.getBottomPlane();
	this->clipPoly(&nVert, vertList, currentPlane);
	currentPlane = Frustum.getLeftPlane();
	this->clipPoly(&nVert, vertList, currentPlane);
	currentPlane = Frustum.getRightPlane();
	this->clipPoly(&nVert, vertList, currentPlane);
	currentPlane = Frustum.getFarPlane();
	this->clipPoly(&nVert, vertList, currentPlane);

	return nVert;
}


inline void Lamp::clipPoly(int* nVert, vect3* vertList, plane clippingPlane)
{
	int nResult = 0;
	vect3 vTemp[MAXCLIPVERTS];

	for (int i = 0; i < *nVert; i++)
	{
		if (i < (*nVert) - 1)
			this->clipEdge(clippingPlane, vertList[i], vertList[i + 1], &nResult, vTemp);
		else
			this->clipEdge(clippingPlane, vertList[i], vertList[0], &nResult, vTemp);
	}
	for (int j = 0; j < nResult; j++)
		vertList[j] = vTemp[j];

	*nVert = nResult;
}


inline void Lamp::clipEdge(const plane& p, const vect3& startV, const vect3& endV, int* nResult, vect3* vTemp)
{
	float t;
	vect3 a = { startV.x - p.P.x, startV.y - p.P.y, startV.z - p.P.z, 1.0 };
	float sStart = a.x * p.N.x + a.y * p.N.y + a.z * p.N.z;
	vect3 b = { endV.x - p.P.x, endV.y - p.P.y, endV.z - p.P.z, 1.0 };
	float sEnd = b.x * p.N.x + b.y * p.N.y + b.z * p.N.z;
	if (sign(sStart) != sign(sEnd))
	{
		vect3 d = { endV.x - startV.x, endV.y - startV.y, endV.z - startV.z, 1.0f };
		float dist = d.x * p.N.x + d.y * p.N.y + d.z * p.N.z;

		if (sStart < 0)
			if (dist)
			{
				t = (dist - sEnd) / dist;

				vTemp[*nResult].x = startV.x + t * (endV.x - startV.x);
				vTemp[*nResult].y = startV.y + t * (endV.y - startV.y);
				vTemp[*nResult].z = startV.z + t * (endV.z - startV.z);
				vTemp[*nResult].w = startV.w + t * (endV.w - startV.w);

				(*nResult)++;
			}
		if (sEnd < 0)
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
		if (sign(sStart) > 0 && sign(sEnd) > 0)
		{

			vTemp[*nResult].x = endV.x;
			vTemp[*nResult].y = endV.y;
			vTemp[*nResult].z = endV.z;
			vTemp[*nResult].w = endV.w;

			(*nResult)++;
		}
}


void Lamp::scanMesh(const int& nPoly, triangle3dV* mesh, mat4x4& rot, mat4x4& mov)
{
	mat4x4 RM = rot * mov;
	mat4x4 M = mov;

	for (int i = 0; i < nPoly; i++)
	{
		triangle3dV worldT = mesh[i];

		if (polyFacingLamp(worldT))
			scanPolygon(RM, M, worldT);
	}
}


void Lamp::scanMesh(const int& nPoly, triangle3dV* mesh, mat4x4& rot, mat4x4& mov, vect3 mv, vect3 rt)
{
	mat4x4 rotX = getRotation(axis::x, rt.x);

	mat4x4 rotY = getRotation(axis::y, -rt.y);

	mat4x4 rotZ = getRotation(axis::z, -rt.z);

	mat4x4 R = rotZ * rotY * rotX;

	mat4x4 MOV = getTranslation(mv);

	mat4x4 MR = MOV * R;

	mat4x4 RM = rot * mov;
	mat4x4 M = mov;

	for (int i = 0; i < nPoly; i++)
	{
		triangle3dV worldT = mesh[i];

		this->object2world(MR, R, worldT);

		if (polyFacingLamp(worldT))
			scanPolygon(RM, M, worldT);
	}
}


void Lamp::scanPolygon(mat4x4& RM, mat4x4& R, triangle3dV& viewT)
{
	this->world2view(RM, R, viewT);

	int nVert = this->clipToFrustum(viewT, vertexList);

	this->projectPoly(nVert);
}


void Lamp::projectPoly(int n)
{
	triangle2dG screenT;

	for (int i = 0; i < n - 2; i++)
	{
		screenT.a = this->view2screen(vertexList[0], hRatio, vRatio);
		screenT.b = this->view2screen(vertexList[1 + i], hRatio, vRatio);
		screenT.c = this->view2screen(vertexList[2 + i], hRatio, vRatio);

		this->fillTriangleDepth(screenT);
	}
}


void Lamp::fillTriangleDepth(const triangle2dG& t)
{
	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int dx, dy;
	float xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	float zLimit[2] = { 0.0f, 0.0f };

	int endIndex, startX, endX;
	float startZ, endZ, zCurrent, invertCurrentZ, deltaZ;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x;
			dy = t.b.y - t.a.y;
			dz = t.b.z - t.a.z;

			yy = (float)hg - (float)t.a.y;
			xx = dx * (yy / dy);
			zz = t.a.z + dz * (yy / (float)dy);

			wd = t.a.x + static_cast<int>(std::round(xx));
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
			dx = t.c.x - t.b.x;
			dy = t.c.y - t.b.y;
			dz = t.c.z - t.b.z;

			yy = (float)hg - (float)t.b.y;
			xx = dx * (yy / dy);
			zz = t.b.z + dz * (yy / (float)dy);

			wd = t.b.x + static_cast<int>(std::round(xx));
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
			dx = t.a.x - t.c.x;
			dy = t.a.y - t.c.y;
			dz = t.a.z - t.c.z;

			yy = (float)hg - (float)t.c.y;
			xx = dx * (yy / dy);
			zz = t.c.z + dz * (yy / (float)dy);

			wd = t.c.x + static_cast<int>(std::round(xx));
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