#include "Torus.h"
#include "Definitions.h"
#include <stdio.h>

Torus::Torus()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;

	radiusH = 3.0;
	radiusV = 1.0;

	resolH = 24;
	resolV = 24;

	c = 255;
}

Torus::Torus(double cx, double cy, double cz, Uint32 colour)
{
	x = cx;
	y = cy;
	z = cz;

	radiusH = 3.0;
	radiusV = 1.0;

	resolH = 24;
	resolV = 24;

	c = colour;
}

Torus::Torus(double cx, double cy, double cz, double rH, double rV, int nH, int nV, Uint32 colour)
{
	x = cx;
	y = cy;
	z = cz;

	radiusH = rH;
	radiusV = rV;

	resolH = nH;
	resolV = nV;

	c = colour;
}

Torus::~Torus()
{

}

int Torus::getTotalVert()
{
	return resolH * resolV;
}

int Torus::getTotalPoly()
{
	return resolH * resolV;
}

vect3* Torus::getVertexData()
{
	double angCurrentH, angCurrentV, stepV, stepH;
	stepV = (360.0 / (double)resolV) * PI / 180.0;
	stepH = (720.0 / (double)resolH) * PI / 180.0;
	int cnt = 0;
	int total;
	total = resolH * resolV;
	vect3* s = new vect3[total];
	if (s != NULL)
	{
		double cxCurrentH, cyCurrentH;
		for (int h = 0; h < resolH; h++)
		{
			angCurrentH = h * stepH;
			cxCurrentH = x + radiusH * cos(angCurrentH);
			cyCurrentH = y + radiusH * sin(angCurrentH);
			for (int v = 0; v < resolV; v++)
			{
				angCurrentV = v * stepV;
				if (cnt < total)
				{
					s[cnt].x = cxCurrentH + radiusV * cos(angCurrentV) * cos(angCurrentH);
					s[cnt].y = cyCurrentH + radiusV * cos(angCurrentV) * sin(angCurrentH);
					s[cnt].z = z + radiusV * sin(angCurrentV);
					s[cnt].w = 1.0;
					cnt++;
				}
			}
		}
	}
	printf("%d vertices added...\n", cnt);
	return s;
}


triangle3d* Torus::getTriangleData()
{
	int totalVert, totalPoly;
	totalVert = resolH * resolV;
	totalPoly = resolH * resolV;
	vect3* s = new vect3[totalVert];
	triangle3d* t = new triangle3d[totalPoly];
	if (s != NULL) { s = this->getVertexData(); }

	int tCount = 0;
	int p, q, r;

	if (t != NULL)
	{
		for (int h = 1; h < resolH + 1; h++)
		{
			if (h < resolH)
			{
				for (int v = 0; v < resolV; v++)
				{
					if (v < resolV - 1)
					{
						p = (h - 1) * resolV + v;
						q = (h - 1) * resolV + v + 1;
						r = h * resolV + v;
						if (tCount < totalPoly)
						{
							t[tCount].A = s[r];
							t[tCount].B = s[q];
							t[tCount].C = s[p];
							vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
							t[tCount].N = unitVector(temp);
							tCount++;
						}
					}
					else
					{
						p = (h - 1) * resolV + v;
						q = (h - 1) * resolV;
						r = h * resolV + v;
						if (tCount < totalPoly)
						{
							t[tCount].A = s[r];
							t[tCount].B = s[q];
							t[tCount].C = s[p];
							vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
							t[tCount].N = unitVector(temp);
							tCount++;
						}
					}
				}
				for (int v = 0; v < resolV; v++)
				{
					if (v < resolV - 1)
					{
						p = (h - 1) * resolV + v + 1;
						q = h * resolV + v + 1;
						r = h * resolV + v;
						if (tCount < totalPoly)
						{
							t[tCount].A = s[r];
							t[tCount].B = s[q];
							t[tCount].C = s[p];
							vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
							t[tCount].N = unitVector(temp);
							tCount++;
						}
					}
					else
					{
						p = (h - 1) * resolV;
						q = h * resolV;
						r = h * resolV + v;
						if (tCount < totalPoly)
						{
							t[tCount].A = s[r];
							t[tCount].B = s[q];
							t[tCount].C = s[p];
							vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
							t[tCount].N = unitVector(temp);
							tCount++;
						}
					}
				}
			}
			else
			{
				for (int v = 0; v < resolV; v++)
				{
					if (v < resolV - 1)
					{
						p = (h - 1) * resolV + v;
						q = (h - 1) * resolV + v + 1;
						r = v;
						if (tCount < totalPoly)
						{
							t[tCount].A = s[r];
							t[tCount].B = s[q];
							t[tCount].C = s[p];
							vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
							t[tCount].N = unitVector(temp);
							tCount++;
						}
					}
					else
					{
						p = (h - 1) * resolV + v;
						q = (h - 1) * resolV;
						r = v;
						if (tCount < totalPoly)
						{
							t[tCount].A = s[r];
							t[tCount].B = s[q];
							t[tCount].C = s[p];
							vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
							t[tCount].N = unitVector(temp);
							tCount++;
						}
					}
				}
				for (int v = 0; v < resolV; v++)
				{
					if (v < resolV - 1)
					{
						p = (h - 1) * resolV + v;
						q = (h - 1) * resolV;
						r = h * resolV + v;
						if (tCount < totalPoly)
						{
							t[tCount].A = s[r];
							t[tCount].B = s[q];
							t[tCount].C = s[p];
							vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
							t[tCount].N = unitVector(temp);
							tCount++;
						}
					}
					else
					{
						p = (h - 1) * resolV;
						q = 0;
						r = resolV - 1;
						if (tCount < totalPoly)
						{
							t[tCount].A = s[r];
							t[tCount].B = s[q];
							t[tCount].C = s[p];
							vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
							t[tCount].N = unitVector(temp);
							tCount++;
						}
					}
				}
			}
		}
	}
	printf("%d polygons created...\n", tCount);
	return t;

}


Uint32 Torus::getColour()
{
	return c;
}


unsigned char Torus::getRedValue()
{
	return ((unsigned char)(c >> 16 & 0xFF));
}


unsigned char Torus::getGreenValue()
{
	return ((unsigned char)(c >> 8 & 0xFF));
}


unsigned char Torus::getBlueValue()
{
	return ((unsigned char)(c & 0xFF));
}



