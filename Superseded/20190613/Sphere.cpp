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
}

Sphere::Sphere(double cx, double cy, double cz, int n)
{
	x = cx;
	y = cy;
	z = cz;

	radius = 1.0;

	resol = n;
}

Sphere::Sphere(double cx, double cy, double cz, double r, int n)
{
	x = cx;
	y = cy;
	z = cz;

	radius = r;

	resol = n;
}

Sphere::~Sphere()
{

}

int Sphere::getTotal()
{
	return resol * (resol - 1);
}

vect3* Sphere::getVertexData()
{
	double angleV, angleH, stepV, stepH;
	stepV = 180.0 / resol;
	stepH = 360.0 / resol;
	int cnt = 0;
	int total;
	total = resol * (resol - 1);
	vect3 * s = new vect3[total];
	if (s != NULL)
	{
		for (int i = 1; i < resol; i++)
		{
			for (int j = 0; j < resol; j++)
			{
				angleV = (-90.0 + i * stepV) * PI / 180.0;
				angleH = (j * stepH) * PI / 180.0;
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
	printf("%d vertices added...\n", cnt);
	return s;
}
