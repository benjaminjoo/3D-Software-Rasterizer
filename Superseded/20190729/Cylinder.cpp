#include "Cylinder.h"
#include "Definitions.h"
#include <stdio.h>


Cylinder::Cylinder()
{
	x		= 0.0;
	y		= 0.0;
	z		= 0.0;

	radius	= 1.0;
	height	= 2.0;

	resol	= 12;

	gouraud = true;

	c		= 255;
}


Cylinder::Cylinder(double cx, double cy, double cz, double h, int res, Uint32 colour)
{
	x		= cx;
	y		= cy;
	z		= cz;

	radius	= 1.0;
	height	= h;

	resol	= res;

	gouraud = true;

	c		= colour;
}


Cylinder::Cylinder(double cx, double cy, double cz, double r, double h, int res, bool g, Uint32 colour)
{
	x		= cx;
	y		= cy;
	z		= cz;

	radius	= r;
	height	= h;

	resol	= res;

	gouraud = g;

	c		= colour;
}


Cylinder::~Cylinder()
{
}


int Cylinder::getTotalVert()
{
	return 4 * resol;
}

int Cylinder::getTotalPoly()
{
	return 4 * resol + 2 * (2 * resol - 2);
}


vect3* Cylinder::getVertexData()
{
	double angleH, stepH;
	stepH = 360.0 / (double(resol) * 2);
	int cnt = 0;
	int total;
	total = 4 * resol;
	vect3 * s = new vect3[total];
	if (s != NULL)
	{
		for (int i = 0; i < 2 * resol; i++)
		{
			angleH = (i * stepH) * PI / 180.0;
			if (cnt < total)
			{
				s[cnt].x = x + radius * cos(angleH);
				s[cnt].y = y + radius * sin(angleH);
				s[cnt].z = z;
				s[cnt].w = 1.0;
				cnt++;
			}
		}
		for (int j = 0; j < 2 * resol; j++)
		{
			angleH = (j * stepH) * PI / 180.0;
			if (cnt < total)
			{
				s[cnt].x = x + radius * cos(angleH);
				s[cnt].y = y + radius * sin(angleH);
				s[cnt].z = z + height;
				s[cnt].w = 1.0;
				cnt++;
			}
		}
	}

	return s;
}


triangle3dV* Cylinder::getTriangleData()
{
	int totalVert, totalPoly;
	totalVert = 4 * resol;
	totalPoly = 4 * resol + 2 * (2 * resol - 2);
	vect3 * s = new vect3[totalVert];
	vect3 centreB = { x, y, z, 1.0 };
	vect3 centreT = { x, y, z + height, 1.0 };
	triangle3dV * t = new triangle3dV[totalPoly];
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
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp);
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreB));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreB));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreT));
				tCount++;
			}
		}
		else
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[i];		t[tCount].B = s[0];		t[tCount].C = s[i + 2 * resol];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp);
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreB));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreB));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreT));
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
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp);
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreB));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreT));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreT));
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
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp);
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreB));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreT));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreT));
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
			vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
			t[tCount].N = unitVector(temp);
			t[tCount].An = t[tCount].Bn = t[tCount].Cn = { 0.0, 0.0, 1.0, 0.0 };
			tCount++;
		}
	}

	return t;

	delete[] s;
	delete[] t;
}


Uint32 Cylinder::getColour()
{
	return c;
}


/*unsigned char Cylinder::getRedValue()
{
	return ((unsigned char)(c >> 16 & 0xFF));
}


unsigned char Cylinder::getGreenValue()
{
	return ((unsigned char)(c >> 8 & 0xFF));
}


unsigned char Cylinder::getBlueValue()
{
	return ((unsigned char)(c & 0xFF));
}*/