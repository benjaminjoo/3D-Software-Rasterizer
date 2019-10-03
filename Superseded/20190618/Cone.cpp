#include "Cone.h"
#include "Definitions.h"
#include <stdio.h>


Cone::Cone()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;

	radius = 1.0;
	height = 2.0;

	resol = 12;

	c = 255;
}


Cone::Cone(double cx, double cy, double cz, double h, int res, Uint32 colour)
{
	x = cx;
	y = cy;
	z = cz;

	radius = 1.0;
	height = h;

	resol = res;

	c = colour;
}


Cone::Cone(double cx, double cy, double cz, double r, double h, int res, Uint32 colour)
{
	x = cx;
	y = cy;
	z = cz;

	radius = r;
	height = h;

	resol = res;

	c = colour;
}


Cone::~Cone()
{
}


int Cone::getTotalVert()
{
	return 2 * resol + 1;
}

int Cone::getTotalPoly()
{
	return 4 * resol - 2;
}


vect3 * Cone::getVertexData()
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
				s[cnt].x = x + radius * cos(angleH);
				s[cnt].y = y + radius * sin(angleH);
				s[cnt].z = z;
				s[cnt].w = 1.0;
				cnt++;
			}
		}
		if (cnt < total)
		{
			s[cnt].x = x;
			s[cnt].y = y;
			s[cnt].z = z + height;
			s[cnt].w = 1.0;
			cnt++;
		}
	}
	printf("%d vertices added...\n", cnt);
	return s;
}


triangle3d* Cone::getTriangleData()
{
	int totalVert, totalPoly;
	totalVert = 2 * resol + 1;
	totalPoly = 4 * resol - 2;
	vect3 * s = new vect3[totalVert];
	triangle3d * t = new triangle3d[totalPoly];
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
				tCount++;
			}
		}
	}
	printf("%d polygons created...\n", tCount);
	return t;
}


Uint32 Cone::getColour()
{
	return c;
}


unsigned char Cone::getRedValue()
{
	return ((unsigned char)(c >> 16 & 0xFF));
}


unsigned char Cone::getGreenValue()
{
	return ((unsigned char)(c >> 8 & 0xFF));
}


unsigned char Cone::getBlueValue()
{
	return ((unsigned char)(c & 0xFF));
}