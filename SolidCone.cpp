#include "SolidCone.h"



SolidCone::SolidCone()
{
	scale			= { 1.0f, 1.0f, 1.0f, 1.0f };
	position		= { 0.0f, 0.0f, 0.0f, 1.0f };
	rotation		= { 0.0f, 0.0f, 0.0f, 1.0f };

	colour			= 255;
	texture			= 0;
	txU				= 1.0f;

	castsShadows	= false;

	radius			= 1.0f;
	height			= 1.0f;
	resol			= 24;

	nPoly = getTotalPoly();
	mesh = new triangle3dV[nPoly];
	getTriangleData(mesh);
}


SolidCone::SolidCone(float px, float py, float pz, Uint32 c)
{
	scale			= { 1.0f, 1.0f, 1.0f, 1.0f };
	position		= { px, py, pz, 1.0f };
	rotation		= { 0.0f, 0.0f, 0.0f, 1.0f };

	colour			= c;
	texture			= 0;
	txU				= 1.0f;	

	castsShadows	= false;

	radius			= 1.0f;
	height			= 1.0f;
	resol			= 24;

	nPoly = getTotalPoly();
	mesh = new triangle3dV[nPoly];
	getTriangleData(mesh);
}


SolidCone::SolidCone(float sx, float sy, float sz, float px, float py, float pz, float rx, float ry, float rz,
	Uint32 c, int t, float r, float h, int res)
{
	scale			= { sx, sy, sz, 1.0f };
	position		= { px, py, pz, 1.0f };
	rotation		= { rx, ry, rz, 1.0f };

	colour			= c;
	texture			= t;
	txU				= 1.0f;

	castsShadows	= false;

	radius			= r;
	height			= h;
	resol			= res;

	nPoly = getTotalPoly();
	mesh = new triangle3dV[nPoly];
	getTriangleData(mesh);
}

SolidCone::~SolidCone()
{
}


int	SolidCone::getTotalVert()
{
	return 2 * resol + 1;
}


int	SolidCone::getTotalPoly()
{
	return 4 * resol - 2;
}


void SolidCone::getVertexData(vect3* s)
{
	float angleH, stepH;
	stepH = 360.0f / (float(resol) * 2);
	int cnt = 0;
	int total;
	total = 2 * resol + 1;
	if (s != nullptr)
	{
		for (int i = 0; i < 2 * resol; i++)
		{
			angleH = PI / resol * i;
			if (cnt < total)
			{
				s[cnt].x = radius * cos(angleH);
				s[cnt].y = radius * sin(angleH);
				s[cnt].z = 0.0f;
				s[cnt].w = 1.0f;
				cnt++;
			}
		}
		if (cnt < total)
		{
			s[cnt].x = 0.0f;
			s[cnt].y = 0.0f;
			s[cnt].z = height;
			s[cnt].w = 1.0f;
			cnt++;
		}
	}
}


void SolidCone::getTriangleData(triangle3dV* t)
{
	int totalVert, totalPoly;
	totalVert = 2 * resol + 1;
	totalPoly = 4 * resol - 2;
	vect3* s = new vect3[totalVert];
	this->getVertexData(s);
	vect3 vectR = { 0.0f, 0.0f, -(radius * radius) / height, 1.0f };

	int tCount = 0;

	float uvStepHor;
	float perimeter = 2 * radius * PI;
	uvStepHor = txU / (2 * float(resol));

	//Adding polygons forming the base

	for (int i = 0; i < 2 * resol - 2; i++)
	{
		if (tCount < totalPoly)
		{
			t[tCount].A = s[0];		t[tCount].B = s[i + 2];	t[tCount].C = s[i + 1];
			vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
			t[tCount].N = unitVector(temp);
			t[tCount].An = t[tCount].Bn = t[tCount].Cn = { 0.0f, 0.0f, -1.0f, 0.0f };
			t[tCount].texture = texture;
			t[tCount].colour = colour;
			tCount++;
		}
	}

	//Adding polygons forming side surface
	float lSide = sqrt(radius * radius + height * height);

	for (int i = 0; i < 2 * resol; i++)
	{
		if (i < resol * 2 - 1)
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[i];		t[tCount].B = s[i + 1];	t[tCount].C = s[2 * resol];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp);
				t[tCount].An = unitVector(subVectors(t[tCount].A, vectR));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, vectR));
				t[tCount].Cn = { (t[tCount].An.x + t[tCount].Bn.x) / 2,
									(t[tCount].An.y + t[tCount].Bn.y) / 2,
									(t[tCount].An.z + t[tCount].Bn.z) / 2,
									0.0f };

				t[tCount].texture = texture;
				t[tCount].colour = colour;

				t[tCount].At.u = uvStepHor * (float(i) + 0.0f);		t[tCount].At.v = 0.0f;
				t[tCount].Bt.u = uvStepHor * (float(i) + 1.0f);		t[tCount].Bt.v = 0.0f;
				t[tCount].Ct.u = uvStepHor * (float(i) + 0.5f);		t[tCount].Ct.v = txU;

				tCount++;
			}
		}
		else
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[i];		t[tCount].B = s[0];		t[tCount].C = s[2 * resol];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp);
				t[tCount].An = unitVector(subVectors(t[tCount].A, vectR));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, vectR));
				t[tCount].Cn = { (t[tCount].An.x + t[tCount].Bn.x) / 2,
									(t[tCount].An.y + t[tCount].Bn.y) / 2,
									(t[tCount].An.z + t[tCount].Bn.z) / 2,
									0.0f };

				t[tCount].texture = texture;
				t[tCount].colour = colour;

				t[tCount].At.u = uvStepHor * (float(i) + 0.0f);		t[tCount].At.v = 0.0f;
				t[tCount].Bt.u = txU;								t[tCount].Bt.v = 0.0f;
				t[tCount].Ct.u = uvStepHor * (float(i) + 0.5f);		t[tCount].Ct.v = txU;

				tCount++;
			}
		}
	}

	delete[] s;

	Projector->transformMesh(totalPoly, t, scale.x, scale.y, scale.z,
											position.x, position.y, position.z,
											rotation.x, rotation.y, rotation.z);
}
