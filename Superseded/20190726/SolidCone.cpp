#include "SolidCone.h"



SolidCone::SolidCone()
{
	scaleX		= 1.0;
	scaleY		= 1.0;
	scaleZ		= 1.0;

	posX		= 0.0;
	posY		= 0.0;
	posZ		= 0.0;

	rotX		= 0.0;
	rotY		= 0.0;
	rotZ		= 0.0;

	colour		= 255;
	texture		= 0;

	radius		= 1.0;
	height		= 1.0;
	resol		= 24;
}


SolidCone::SolidCone(double px, double py, double pz, Uint32 c)
{
	scaleX		= 1.0;
	scaleY		= 1.0;
	scaleZ		= 1.0;

	posX		= px;
	posY		= py;
	posZ		= pz;

	rotX		= 0.0;
	rotY		= 0.0;
	rotZ		= 0.0;

	colour		= c;
	texture		= 0;

	radius		= 1.0;
	height		= 1.0;
	resol		= 24;
}


SolidCone::SolidCone(double sx, double sy, double sz, double px, double py, double pz, double rx, double ry, double rz,
	Uint32 c, int t, double r, double h, int res)
{
	scaleX		= sx;
	scaleY		= sy;
	scaleZ		= sz;

	posX		= px;
	posY		= py;
	posZ		= pz;

	rotX		= rx;
	rotY		= ry;
	rotZ		= rz;

	colour		= c;
	texture		= t;

	radius		= r;
	height		= h;
	resol		= res;
}

SolidCone::~SolidCone()
{
}


int	SolidCone::getTotalVert()
{
	return 2 * resol + 1;
}


int SolidCone::getTextureID()
{
	return texture;
}


int	SolidCone::getTotalPoly()
{
	return 4 * resol - 2;
}


vect3* SolidCone::getVertexData()
{
	double angleH, stepH;
	stepH = 360.0 / (double(resol) * 2);
	int cnt = 0;
	int total;
	total = 2 * resol + 1;
	vect3* s = new vect3[total];
	if (s != NULL)
	{
		for (int i = 0; i < 2 * resol; i++)
		{
			angleH = (i * stepH) * PI / 180.0;
			if (cnt < total)
			{
				s[cnt].x = radius * cos(angleH);
				s[cnt].y = radius * sin(angleH);
				s[cnt].z = 0.0;
				s[cnt].w = 1.0;
				cnt++;
			}
		}
		if (cnt < total)
		{
			s[cnt].x = 0.0;
			s[cnt].y = 0.0;
			s[cnt].z = height;
			s[cnt].w = 1.0;
			cnt++;
		}
	}

	return s;

	delete[] s;
}


triangle3dV* SolidCone::getTriangleData()
{
	int totalVert, totalPoly;
	totalVert = 2 * resol + 1;
	totalPoly = 4 * resol - 2;
	vect3* s = new vect3[totalVert];
	vect3 vectR = { 0.0, 0.0, - (radius * radius) / height, 1.0 };
	triangle3dV* t = new triangle3dV[totalPoly];
	if (s != NULL) { s = this->getVertexData(); }

	int tCount = 0;

	//Adding polygons forming the base

	for (int i = 0; i < 2 * resol - 2; i++)
	{
		if (tCount < totalPoly)
		{
			t[tCount].A = s[0];		t[tCount].B = s[i + 2];	t[tCount].C = s[i + 1];
			vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
			t[tCount].N = unitVector(temp);
			t[tCount].An = t[tCount].Bn = t[tCount].Cn = { 0.0, 0.0, -1.0, 0.0 };
			t[tCount].texture = texture;
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
				t[tCount].A = s[i];		t[tCount].B = s[i + 1];	t[tCount].C = s[2 * resol];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp);
				t[tCount].An = unitVector(subVectors(t[tCount].A, vectR));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, vectR));
				t[tCount].Cn = { (t[tCount].An.x + t[tCount].Bn.x) / 2,
									(t[tCount].An.y + t[tCount].Bn.y) / 2,
									(t[tCount].An.z + t[tCount].Bn.z) / 2,
									0.0 };
				t[tCount].texture = texture;
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
									0.0 };
				t[tCount].texture = texture;
				tCount++;
			}
		}
	}

	return transformObject(this->getTotalPoly(), t, scaleX, scaleY, scaleZ, posX, posY, posZ, rotX, rotY, rotZ);

	delete[] s;
	delete[] t;
}


Uint32 SolidCone::getColour()
{
	return colour;
}
