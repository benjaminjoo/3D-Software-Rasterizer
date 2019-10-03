#include "Sphere.h"
#include "Definitions.h"
#include <stdio.h>

Sphere::Sphere()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;

	radius = 1.0;

	resol = 12;

	gouraud = true;

	c = 255;
}

Sphere::Sphere(double cx, double cy, double cz, int n, Uint32 colour)
{
	x = cx;
	y = cy;
	z = cz;

	radius = 1.0;

	resol = n;

	gouraud = true;

	c = colour;
}

Sphere::Sphere(double cx, double cy, double cz, double r, int n, bool g, Uint32 colour)
{
	x = cx;
	y = cy;
	z = cz;

	radius = r;

	resol = n;

	gouraud = g;

	c = colour;
}

Sphere::~Sphere()
{

}

int Sphere::getTotalVert()
{
	return 2 * resol * (resol - 1) + 2;
}

int Sphere::getTotalPoly()
{
	return 4 * resol * (resol - 1);
}

bool Sphere::isGouraudOn()
{
	return gouraud;
}

vect3* Sphere::getVertexData()
{
	double angleV, angleH, stepV, stepH;
	stepV = 180.0 / resol;
	stepH = 360.0 / (double(resol) * 2);
	int cnt = 0;
	int total;
	total = 2 * resol * (resol - 1) + 2;
	vect3 * s = new vect3[total];
	if (s != NULL)
	{
		if (cnt < total)
		{
			s[cnt].x = x;				//South Pole
			s[cnt].y = y;
			s[cnt].z = z - radius;
			s[cnt].w = 1.0;
			cnt++;			
		}
		for (int i = 1; i < resol; i++)
		{
			for (int j = 0; j < resol * 2; j++)
			{
				angleV = (-90.0 + i * stepV) * PI / 180.0;
				angleH = (j * stepH) * PI / 180.0;
				if (cnt < total)
				{
					if (cnt < total)
					{
						s[cnt].x = x + radius * cos(angleV) * cos(angleH);
						s[cnt].y = y + radius * cos(angleV) * sin(angleH);
						s[cnt].z = z + radius * sin(angleV);
						s[cnt].w = 1.0;
						cnt++;
					}
				}
			}
		}
		if (cnt < total)
		{
			s[cnt].x = x;				//North Pole
			s[cnt].y = y;
			s[cnt].z = z + radius;
			s[cnt].w = 1.0;
			cnt++;
		}
	}

	return s;

	delete[] s;
}


triangle3dV* Sphere::getTriangleData()
{
	int totalVert, totalPoly;
	totalVert = 2 * resol * (resol - 1) + 2;
	totalPoly = 4 * resol * (resol - 1);
	vect3 centreV = { x, y, z, 1.0 };
	vect3 * s = new vect3[totalVert];
	triangle3dV * t = new triangle3dV[totalPoly];
	if (s != NULL) { s = this->getVertexData(); }

	int tCount = 0;

	//Adding triangles forming caps, North Pole first

	for (int i = 1; i < resol * 2 + 1; i++)
	{
		if (i < resol * 2)
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[0];		t[tCount].B = s[i + 1];	t[tCount].C = s[i];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp); t[tCount].N.w = 0.0;
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));
				tCount++;
			}
		}
		else
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[0];		t[tCount].B = s[1];		t[tCount].C = s[i];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp); t[tCount].N.w = 0.0;
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));
				tCount++;
			}
		}
	}

	//Adding triangles forming bands, going from south to north

	for (int j = 0; j < resol - 2; j++)				//incrementing 'j' represents vertical rising
	{
		for (int i = 0; i < resol * 2; i++)		//incrementing 'j' represents horizontal rotation
		{
			if (i < resol * 2 - 1)
			{
				if (tCount < totalPoly)
				{
					t[tCount].A = s[1 + (j * resol * 2) + i];
					t[tCount].B = s[1 + (j * resol * 2) + i + 1];
					t[tCount].C = s[1 + ((j + 1) * resol * 2) + i];
					//vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
					vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
					t[tCount].N = unitVector(temp); t[tCount].N.w = 0.0;
					t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
					t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
					t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));
					tCount++;
				}
				if (tCount < totalPoly)
				{
					t[tCount].A = s[1 + (j * resol * 2) + i + 1];
					t[tCount].B = s[1 + ((j + 1) * resol * 2) + i + 1];
					t[tCount].C = s[1 + ((j + 1) * resol * 2) + i];
					//vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
					vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
					t[tCount].N = unitVector(temp); t[tCount].N.w = 0.0;
					t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
					t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
					t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));
					tCount++;
				}
			}
			else
			{
				if (tCount < totalPoly)
				{
					t[tCount].A = s[1 + (j * resol * 2) + i];
					t[tCount].B = s[1 + (j * resol * 2)];
					t[tCount].C = s[1 + ((j + 1) * resol * 2) + i];
					//vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
					vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
					t[tCount].N = unitVector(temp); t[tCount].N.w = 0.0;
					t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
					t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
					t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));
					tCount++;
				}
				if (tCount < totalPoly)
				{
					t[tCount].A = s[1 + (j * resol * 2)];
					t[tCount].B = s[1 + (j * resol * 2) + i + 1];
					t[tCount].C = s[1 + ((j + 1) * resol * 2) + i];
					//vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
					vect3 temp = crossProduct(subVectors(t[tCount].A, t[tCount].B), subVectors(t[tCount].A, t[tCount].C));
					t[tCount].N = unitVector(temp); t[tCount].N.w = 0.0;
					t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
					t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
					t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));
					tCount++;
				}
			}
		}
	}

	//Adding triangles forming caps, South Pole

	for (int i = 0; i < resol * 2; i++)
	{
		if (i < resol * 2 - 1)
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[(resol - 2) * 2 * resol + 1 + i];
				t[tCount].B = s[(resol - 2) * 2 * resol + 1 + i + 1];
				t[tCount].C = s[(resol - 1) * 2 * resol + 1];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp); t[tCount].N.w = 0.0;
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));
				tCount++;
			}
		}
		else
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[(resol - 2) * 2 * resol + 1 + i];
				t[tCount].B = s[(resol - 2) * 2 * resol + 1];
				t[tCount].C = s[(resol - 1) * 2 * resol + 1];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp); t[tCount].N.w = 0.0;
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));
				tCount++;
			}
		}
	}

	return t;

	delete[] s;
	delete[] t;
}


Uint32 Sphere::getColour()
{
	return c;
}


/*unsigned char Sphere::getRedValue()
{
	return ((unsigned char)(c >> 16 & 0xFF));
}


unsigned char Sphere::getGreenValue()
{
	return ((unsigned char)(c >> 8 & 0xFF));
}


unsigned char Sphere::getBlueValue()
{
	return ((unsigned char)(c & 0xFF));
}*/



