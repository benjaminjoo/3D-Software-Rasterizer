#include "Prism.h"
#include "Definitions.h"


Prism::Prism()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;

	a = 1.0;
	b = 1.0;
	c = 1.0;

	resol = 10;
}

Prism::Prism(double cx, double cy, double cz, int n)
{
	x = cx;
	y = cy;
	z = cz;

	a = 1.0;
	b = 1.0;
	c = 1.0;

	resol = n;
}

Prism::Prism(double cx, double cy, double cz, double ea, double eb, double ec, int n)
{
	x = cx;
	y = cy;
	z = cz;

	a = ea;
	b = eb;
	c = ec;

	resol = n;
}

Prism::~Prism()
{
}

int Prism::getTotal()
{
	return (resol + 1) * (resol + 1) * (resol + 1) - (resol - 1) * (resol - 1) * (resol - 1);
}

vect3* Prism::getVertexData()
{
	double stepA, stepB, stepC;
	stepA = a / (double)resol;
	stepB = b / (double)resol;
	stepC = c / (double)resol;
	int cnt = 0;
	int total;
	total = (resol + 1) * (resol + 1) * (resol + 1) - (resol - 1) * (resol - 1) * (resol - 1);
	vect3 * s = new vect3[total];
	if (s != NULL)
	{
		for (int k = 0; k < resol + 1; k++)
		{
			for (int j = 0; j < resol + 1; j++)
			{
				for (int i = 0; i < resol + 1; i++)
				{
					if (i == 0 || i == resol||
						j == 0 || j == resol||
						k == 0 || k == resol)
					{
						if (cnt < total)
						{
							s[cnt].x = x + i * stepA;
							s[cnt].y = y + j * stepB;
							s[cnt].z = z + k * stepC;
							s[cnt].w = 1.0;
							cnt++;
						}
					}
				}
			}
		}

	}
	return s;
}
