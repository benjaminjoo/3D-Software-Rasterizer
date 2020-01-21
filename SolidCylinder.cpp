#include "SolidCylinder.h"



SolidCylinder::SolidCylinder()
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


SolidCylinder::SolidCylinder(float px, float py, float pz, Uint32 c)
{
	scale			= { 1.0f, 1.0f, 1.0f, 1.0f };
	position		= { px, py, pz, 1.0f };
	rotation		= { 0.0f, 0.0f, 0.0f, 1.0f };

	colour			= c;
	texture			= 0;
	txU				= 1.0f;

	castsShadows	= false;

	radius			= 1.0f;
	height			= 2.0f;
	resol			= 24;

	nPoly = getTotalPoly();
	mesh = new triangle3dV[nPoly];
	getTriangleData(mesh);
}


SolidCylinder::SolidCylinder(float sx, float sy, float sz, float px, float py, float pz, float rx, float ry, float rz,
								Uint32 c, int t, float r, float h, int res)
{
	scale			= { sx, sy, sz, 1.0f };
	position		= { px, py, pz, 1.0f };
	rotation		= { rx, ry, rz, 1.0f };

	colour			= c;
	texture			= t;
	txU				= 2.5;

	castsShadows	= false;

	radius			= r;
	height			= h;
	resol			= res;

	nPoly = getTotalPoly();
	mesh = new triangle3dV[nPoly];
	getTriangleData(mesh);
}


SolidCylinder::SolidCylinder(float sx, float sy, float sz, float px, float py, float pz, float rx, float ry, float rz,
	Uint32 c, int t, float r, float h, int res, bool dynamic)
{
	scale = { sx, sy, sz, 1.0f };
	position = { px, py, pz, 1.0f };
	rotation = { rx, ry, rz, 1.0f };

	colour = c;
	texture = t;
	txU = 2.5;

	castsShadows = false;

	radius = r;
	height = h;
	resol = res;

	isDynamic = dynamic;

	nPoly = getTotalPoly();
	mesh = new triangle3dV[nPoly];
	getTriangleData(mesh);
}


SolidCylinder::~SolidCylinder()
{
}


int	SolidCylinder::getTotalVert()
{
	return 4 * resol;
}


int	SolidCylinder::getTotalPoly()
{
	return 4 * resol + 2 * (2 * resol - 2);
}


void SolidCylinder::getVertexData(vect3* s)
{
	float angleH, stepH;
	stepH = 360.0f / (float(resol) * 2);
	int cnt = 0;
	int total;
	total = 4 * resol;
	if (s != nullptr)
	{
		for (int i = 0; i < 2 * resol; i++)
		{
			angleH = (i * stepH) * PI / 180.0f;
			if (cnt < total)
			{
				s[cnt].x = radius * cos(angleH);
				s[cnt].y = radius * sin(angleH);
				s[cnt].z = 0.0f;
				s[cnt].w = 1.0f;
				cnt++;
			}
		}
		for (int j = 0; j < 2 * resol; j++)
		{
			angleH = (j * stepH) * PI / 180.0f;
			if (cnt < total)
			{
				s[cnt].x = radius * cos(angleH);
				s[cnt].y = radius * sin(angleH);
				s[cnt].z = height;
				s[cnt].w = 1.0f;
				cnt++;
			}
		}
	}
}


void SolidCylinder::getTriangleData(triangle3dV* t)
{
	int totalVert, totalPoly;
	totalVert = 4 * resol;
	totalPoly = 4 * resol + 2 * (2 * resol - 2);
	vect3 * s = new vect3[totalVert];
	vect3 centreB = { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3 centreT = { 0.0f, 0.0f, height, 1.0f };
	this->getVertexData(s);

	int tCount = 0;

	float uvStepHor;
	float perimeter = 2 * radius * PI;
	uvStepHor = (perimeter / txU) / (2 * float(resol));

	//Adding polygons forming the base

	for (int i = 0; i < 2 * resol - 2; i++)
	{
		if (tCount < totalPoly)
		{
			t[tCount].A = s[0];		t[tCount].B = s[i + 2];	t[tCount].C = s[i + 1];
			//vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
			//t[tCount].N = unitVector(temp);
			t[tCount].An = t[tCount].Bn = t[tCount].Cn = t[tCount].N = { 0.0f, 0.0f, -1.0f, 0.0f };

			t[tCount].texture = 0;
			t[tCount].colour = colour;

			tCount++;
		}
	}

	//Adding polygons forming side surface

	for (int i = 0; i < 2 * resol; i++)
	{
		if (i < resol * 2 - 1)
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[i];		t[tCount].B = s[i + 1];	t[tCount].C = s[i + 2 * resol];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
				t[tCount].N = unitVector(temp);
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreB));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreB));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreT));

				t[tCount].texture = texture;
				t[tCount].colour = colour;

				t[tCount].At.u = uvStepHor * (float(i) + 0.0f);		t[tCount].At.v = 0.0f;
				t[tCount].Bt.u = uvStepHor * (float(i) + 1.0f);		t[tCount].Bt.v = 0.0f;
				t[tCount].Ct.u = uvStepHor * (float(i) + 0.0f);		t[tCount].Ct.v = height / txU;

				tCount++;
			}
		}
		else
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[i];		t[tCount].B = s[0];		t[tCount].C = s[i + 2 * resol];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
				t[tCount].N = unitVector(temp);
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreB));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreB));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreT));

				t[tCount].texture = texture;
				t[tCount].colour = colour;

				t[tCount].At.u = uvStepHor * (float(i) + 0.0f);		t[tCount].At.v = 0.0f;
				t[tCount].Bt.u = perimeter / txU;						t[tCount].Bt.v = 0.0f;
				t[tCount].Ct.u = uvStepHor * (float(i) + 0.0f);		t[tCount].Ct.v = height / txU;

				tCount++;
			}
		}
	}

	for (int i = 0; i < 2 * resol; i++)
	{
		if (i < resol * 2 - 1)
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[i + 1];
				t[tCount].B = s[2 * resol + i + 1];
				t[tCount].C = s[2 * resol + i];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
				t[tCount].N = unitVector(temp);
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreB));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreT));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreT));

				t[tCount].texture = texture;
				t[tCount].colour = colour;

				t[tCount].At.u = uvStepHor * (float(i) + 1.0f);		t[tCount].At.v = 0.0f;
				t[tCount].Bt.u = uvStepHor * (float(i) + 1.0f);		t[tCount].Bt.v = height / txU;
				t[tCount].Ct.u = uvStepHor * (float(i) + 0.0f);		t[tCount].Ct.v = height / txU;

				tCount++;
			}
		}
		else
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[0];
				t[tCount].B = s[2 * resol];
				t[tCount].C = s[4 * resol - 1];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
				t[tCount].N = unitVector(temp);
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreB));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreT));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreT));

				t[tCount].texture = texture;
				t[tCount].colour = colour;

				t[tCount].At.u = perimeter / txU;					t[tCount].At.v = 0.0f;
				t[tCount].Bt.u = perimeter / txU;					t[tCount].Bt.v = height / txU;
				t[tCount].Ct.u = uvStepHor * (float(i) + 0.0f);		t[tCount].Ct.v = height / txU;

				tCount++;
			}
		}
	}

	//Adding polygons forming the top

	for (int i = 0; i < 2 * resol - 2; i++)
	{
		if (tCount < totalPoly)
		{
			t[tCount].A = s[2 * resol];
			t[tCount].B = s[2 * resol + i + 1];
			t[tCount].C = s[2 * resol + i + 2];
			//vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
			//t[tCount].N = unitVector(temp);
			t[tCount].An = t[tCount].Bn = t[tCount].Cn = t[tCount].N = { 0.0f, 0.0f, 1.0f, 0.0f };

			t[tCount].texture = 0;
			t[tCount].colour = colour;

			tCount++;
		}
	}

	delete[] s;

	nPoly = totalPoly;

	if (!isDynamic)
	{
		Projector->transformMesh(nPoly, t, scale.x, scale.y, scale.z,
											position.x, position.y, position.z,
											rotation.x, rotation.y, rotation.z);
	}
}
