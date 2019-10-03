#include <stdio.h>
#include <math.h>
#include "Revolved.h"
#include "Definitions.h"


Revolved::Revolved()
{
	offset		= 1.0;
	aStart		= 0.0;
	aEnd		= 360.0;

	resolH		= 24;
	resolV		= 24;

	n			= 2;
	*polyLine	= { (0.0, 0.0, 1), (0.0, 2.0, 1) };

	c			= 255;
}


Revolved::Revolved(double cx, double cy, double cz, double off, double aS, double aE, int rH, int rV, int nP, polyNode* pLine, Uint32 col)
{
	offset		= off;
	aStart		= aS;
	aEnd		= aE;

	resolH		= rH;
	resolV		= rV;

	n			= nP;
	polyLine	= pLine;

	c			= col;
}


Revolved::~Revolved()
{
}


Uint32 Revolved::getColour()
{
	return c;
}


int Revolved::numberOfStepsH()
{
	return abs(int(double(resolH) / 360.0 * (aEnd - aStart)));
}


int Revolved::getTotalPoly()
{
	int nNodes = this->numberOfNodes();
	int nStep = this->numberOfStepsH();
	int nVert = nStep * nNodes;

	return nStep * 2 * (nNodes - 1);
}


int Revolved::numberOfNodes()
{
	int nP = 0;
	
	for (int i = 0; i < n; i++)
	{
		switch (polyLine[i].s)
		{
		case 1:
			nP++;
			break;
		case 4001:
			double a = polyLine[i].y;
			int nStep = abs(int(double(resolV) / 360.0 * a));			
			nP += nStep;
			break;
		}
	}

	return nP;
}


int Revolved::numberOfArcs()
{
	int nA = 0;
	
	for(int i = 0; i < n; i++)
	{
		if (polyLine[i].s == 900) { nA++; }
	}
    
	return nA;
}


int Revolved::numberOfFlats()
{
	int nF = 0;

	for (int i = 0; i < n; i++)
	{
		if (polyLine[i].s != 900) { nF++; }
	}

	return nF;
}


int* Revolved::arcDirections()
{
	int cC = 0;

	int nArc = this->numberOfArcs();

	int* directions = new int[nArc];

	for (int i = 0; i < n; i++)
	{
		if (polyLine[i].s == 4001)
		{
			if (cC < nArc)
			{
				directions[cC] = sign(polyLine[i].y);
				cC++;
			}
		}
	}

	return directions;
	delete[] directions;
}


polyNode* Revolved::arcCentres()
{
	int cC = 0;

	int nArc = this->numberOfArcs();

	polyNode* centres = new polyNode[nArc];

	for (int i = 0; i < n; i++)
	{
		if (polyLine[i].s == 900)
		{
			if(cC < nArc)
			{
				centres[cC] = polyLine[i];
				cC++;
			}
		}
	}

	return centres;
	delete[] centres;
}


polyNode* Revolved::tessellatePolyline()
{
	int nNodes = this->numberOfNodes();

	polyNode* spine = new polyNode[nNodes];

	int cP = 0;
	int cA = 0;

	for (int i = 0; i < n; i++)
	{
		switch (polyLine[i].s)
		{
		case 1:
			if (cP < nNodes)
			{
				spine[cP] = polyLine[i];
				spine[cP].s = -1;
				cP++;
			}
			break;
		case 4001:
			double sx, sy, cx, cy, r, a, aStart, aStep;
			sx = polyLine[i - 2].x;
			sy = polyLine[i - 2].y;
			cx = polyLine[i - 1].x;
			cy = polyLine[i - 1].y;
			r = distancePN(polyLine[i - 1], polyLine[i - 2]);
			a = polyLine[i].y;
			int nStep = abs(int(double(resolV) / 360.0 * a));	
			aStart = atan2(sy - cy, sx - cx);
			aStep = a / (double(nStep) + 1) * PI / 180.0;
			polyNode p;
			for (int j = 1; j < nStep + 1; j++)
			{
				p.x = cx + r * cos(aStart + j * aStep);
				p.y = cy + r * sin(aStart + j * aStep);
				p.s = cA;
				if (cP < nNodes) { spine[cP] = p; }
				cP++;
			}
			cA++;
			break;
		}
	}

	return spine;

	delete[] spine;
}


polyNode* Revolved::get2dNormals()
{
	int nNodes = this->numberOfNodes();
	polyNode* p = this->tessellatePolyline();
	polyNode* arcC = this->arcCentres();
	int* dir = this->arcDirections();

	int nSides = nNodes - 1;
	int nNormals = nSides * 2;

	polyNode* n = new polyNode[nNormals];

	for (int i = 0; i < nSides; i++)
	{
		polyNode nodeLower, nodeUpper;
		nodeLower = p[i];
		nodeUpper = p[i + 1];
		if (nodeLower.s >= 0 || nodeUpper.s >= 0)
		{
			int curveNo = nodeLower.s >= 0 ? nodeLower.s : nodeUpper.s;			
			if (2 * i + 1 < nNormals)
			{
				if (dir[curveNo] == 1)
				{
					n[2 * i]		= unitNode(subNodes(p[i], arcC[curveNo]));
					n[2 * i + 1]	= unitNode(subNodes(p[i + 1], arcC[curveNo]));
				}
				else if (dir[curveNo == -1])
				{
					n[2 * i]		= unitNode(subNodes(arcC[curveNo], p[i]));
					n[2 * i + 1]	= unitNode(subNodes(arcC[curveNo], p[i + 1]));
				}
			}
		}
		else
		{
			if (2 * i + 1 < nNormals) { n[2 * i] = n[2 * i + 1] = unitNode(getNormal(p[i + 1], p[i])); }
		}
	}

	return n;
	delete[] n;
	delete[] p;
	delete[] arcC;
	delete[] dir;
}


vect3* Revolved::getVertexData()
{
	double aShell = aEnd - aStart;
	int nStep = this->numberOfStepsH();
	double aStep = aShell / nStep;
	int nNodes = this->numberOfNodes();
	polyNode* p;
	p = this->tessellatePolyline();
	int nVert = nStep * nNodes;

	vect3 * v = new vect3[nVert];

	double r, a;
	int cV = 0;
	for (int j = 0; j < nNodes; j++)
	{
		for (int i = 0; i < nStep; i++)
		{
			r = offset + p[j].x;
			a = aStart + i * aStep;
			if (cV < nVert)
			{
				v[cV].x = r * cos(a * PI / 180.0);
				v[cV].y = r * sin(a * PI / 180.0);
				v[cV].z = p[j].y;
				v[cV].w = 1.0;
				cV++;
			}
		}
	}

	return v;
	delete[] p;
	delete[] v;
}


triangle3dV* Revolved::getTriangleDataN()
{
	double aShell = aEnd - aStart;
	int nStep = this->numberOfStepsH();
	double aStep = aShell / nStep;

	int nTriangle = this->getTotalPoly();
	int nNodes = this->numberOfNodes();
	triangle3dV* t = this->getTriangleData();

	polyNode * n = this->get2dNormals();
	double rLower, rUpper, a, b;
	int cT = 0;

	for (int j = 0; j < nNodes - 1; j++)
	{
		for (int i = 0; i < resolH; i++)
		{
			rLower = n[2 * j].x;
			rUpper = n[2 * j + 1].x;
			a = aStart + i * aStep;
			b = a + aStep;
			if (i < resolH - 1)
			{
				if (cT < nTriangle)
				{
					t[cT].An.x = rLower * cos(a * PI / 180.0);
					t[cT].An.y = rLower * sin(a * PI / 180.0);
					t[cT].An.z = n[2 * j].y;
					t[cT].An.w = 0.0;

					t[cT].Bn.x = rLower * cos(b * PI / 180.0);
					t[cT].Bn.y = rLower * sin(b * PI / 180.0);
					t[cT].Bn.z = n[2 * j].y;
					t[cT].Bn.w = 0.0;

					t[cT].Cn.x = rUpper * cos(a * PI / 180.0);
					t[cT].Cn.y = rUpper * sin(a * PI / 180.0);
					t[cT].Cn.z = n[2 * j + 1].y;
					t[cT].Cn.w = 0.0;

					cT++;
				}
				if (cT < nTriangle)
				{
					t[cT].An.x = rLower * cos(b * PI / 180.0);
					t[cT].An.y = rLower * sin(b * PI / 180.0);
					t[cT].An.z = n[2 * j].y;
					t[cT].An.w = 0.0;

					t[cT].Bn.x = rUpper * cos(b * PI / 180.0);
					t[cT].Bn.y = rUpper * sin(b * PI / 180.0);
					t[cT].Bn.z = n[2 * j + 1].y;
					t[cT].Bn.w = 0.0;

					t[cT].Cn.x = rUpper * cos(a * PI / 180.0);
					t[cT].Cn.y = rUpper * sin(a * PI / 180.0);
					t[cT].Cn.z = n[2 * j + 1].y;
					t[cT].Cn.w = 0.0;

					cT++;
				}
			}
			else
			{
				if (cT < nTriangle)
				{
					t[cT].An.x = rLower * cos(a * PI / 180.0);
					t[cT].An.y = rLower * sin(a * PI / 180.0);
					t[cT].An.z = n[2 * j].y;
					t[cT].An.w = 0.0;

					t[cT].Bn.x = rLower * cos(b * PI / 180.0);
					t[cT].Bn.y = rLower * sin(b * PI / 180.0);
					t[cT].Bn.z = n[2 * j].y;
					t[cT].Bn.w = 0.0;

					t[cT].Cn.x = rUpper * cos(a * PI / 180.0);
					t[cT].Cn.y = rUpper * sin(a * PI / 180.0);
					t[cT].Cn.z = n[2 * j + 1].y;
					t[cT].Cn.w = 0.0;

					cT++;
				}
				if (cT < nTriangle)
				{
					t[cT].An.x = rLower * cos(b * PI / 180.0);
					t[cT].An.y = rLower * sin(b * PI / 180.0);
					t[cT].An.z = n[2 * j].y;
					t[cT].An.w = 0.0;

					t[cT].Bn.x = rUpper * cos(b * PI / 180.0);
					t[cT].Bn.y = rUpper * sin(b * PI / 180.0);
					t[cT].Bn.z = n[2 * j + 1].y;
					t[cT].Bn.w = 0.0;

					t[cT].Cn.x = rUpper * cos(a * PI / 180.0);
					t[cT].Cn.y = rUpper * sin(a * PI / 180.0);
					t[cT].Cn.z = n[2 * j + 1].y;
					t[cT].Cn.w = 0.0;

					cT++;
				}
			}
		}
	}

	return t;

	delete[] n;
	delete[] t;
}


triangle3dV* Revolved::getTriangleData()
{
	int nNodes = this->numberOfNodes();
	vect3* v = this->getVertexData();
	int nTriangle = this->getTotalPoly();
	triangle3dV* t = new triangle3dV[nTriangle];
	int cT = 0;
	vect3 temp;

	for (int j = 0; j < nNodes - 1; j++)
	{
		for (int i = 0; i < resolH; i++)
		{
			if (i < resolH - 1)
			{
				if (cT < nTriangle)
				{
					t[cT].A = v[j * resolH + i];
					t[cT].B = v[j * resolH + i + 1];
					t[cT].C = v[(j + 1) * resolH + i];
					temp = crossProduct(subVectors(t[cT].B, t[cT].A), subVectors(t[cT].C, t[cT].B));
					t[cT].N = unitVector(temp);
					t[cT].An = t[cT].Bn = t[cT].Cn = t[cT].N;
					cT++;
				}
				if (cT < nTriangle)
				{
					t[cT].A = v[j * resolH + i + 1];
					t[cT].B = v[(j + 1) * resolH + i + 1];
					t[cT].C = v[(j + 1) * resolH + i];
					temp = crossProduct(subVectors(t[cT].B, t[cT].A), subVectors(t[cT].C, t[cT].B));
					t[cT].N = unitVector(temp);
					t[cT].An = t[cT].Bn = t[cT].Cn = t[cT].N;
					cT++;
				}
			}
			else
			{
				if (cT < nTriangle)
				{
					t[cT].A = v[j * resolH + i];
					t[cT].B = v[j * resolH];
					t[cT].C = v[(j + 1) * resolH + i];
					temp = crossProduct(subVectors(t[cT].B, t[cT].A), subVectors(t[cT].C, t[cT].B));
					t[cT].N = unitVector(temp);
					t[cT].An = t[cT].Bn = t[cT].Cn = t[cT].N;
					cT++;
				}
				if (cT < nTriangle)
				{
					t[cT].A = v[j * resolH];
					t[cT].B = v[(j + 1) * resolH];
					t[cT].C = v[(j + 1) * resolH + i];
					temp = crossProduct(subVectors(t[cT].B, t[cT].A), subVectors(t[cT].C, t[cT].B));
					t[cT].N = unitVector(temp);
					t[cT].An = t[cT].Bn = t[cT].Cn = t[cT].N;
					cT++;
				}
			}
		}
	}
	printf("%d polygons generated...\n", cT);
	return t;
	delete[] v;
	delete[] t;
}