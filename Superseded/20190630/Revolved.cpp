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
	//int nStep = abs(int(double(resolH) / 360.0 * (aEnd - aStart)));
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


vect3* Revolved::getArcCentres()
{
	double aShell = aEnd - aStart;
	int nStep = this->numberOfStepsH();
	double aStep = aShell / nStep;
	int nArc = this->numberOfArcs();
	int nCentres = nStep * nArc;

	vect3* nc = new vect3[nCentres];

	polyNode* pc = this->arcCentres();

	double r, a;
	int cC = 0;
	for (int j = 0; j < nArc; j++)
	{
		for (int i = 0; i < nStep; i++)
		{
			r = offset + pc[j].x;
			a = aStart + i * aStep;
			if (cC < nCentres)
			{
				nc[cC].x = r * cos(a * PI / 180.0);
				nc[cC].y = r * sin(a * PI / 180.0);
				nc[cC].z = pc[j].y;
				nc[cC].w = 1.0;
				cC++;
			}
		}
	}

	return nc;

	delete[] pc;
	delete[] nc;
}


triangle3dV* Revolved::getTriangleData()
{
	int nNodes = this->numberOfNodes();
	double aShell = aEnd - aStart;
	int nStep = this->numberOfStepsH();
	double aStep = aShell / nStep;
	int nVert = nStep * nNodes;
	int nArc = this->numberOfArcs();

	vect3* v = this->getVertexData();

	vect3* ac = this->getArcCentres();

	int* dir = this->arcDirections();

	polyNode* p = this->tessellatePolyline();

	int nTriangle = this->getTotalPoly();

	triangle3dV* t = new triangle3dV[nTriangle];

	int cT = 0;
	int cC = 0;

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
					/*if (p[j].s >= 0 || p[j + 1].s >= 0)
					{
						int lowerArcNo = p[j].s;
						int upperArcNo = p[j + 1].s;
						int direction;
						vect3 centreA = ac[resolH * lowerArcNo + i];
						vect3 centreB = ac[resolH * lowerArcNo + i + 1];
						vect3 centreC = ac[resolH * upperArcNo + i];
						if (lowerArcNo < 0) { direction = dir[upperArcNo]; }
						if (upperArcNo < 0) { direction = dir[lowerArcNo]; }
						if (upperArcNo >= 0 && lowerArcNo >= 0) { direction = dir[lowerArcNo]; }
						if (direction > 0)
						{
							t[cT].An = unitVector(subVectors(t[cT].A, centreA));
							t[cT].Bn = unitVector(subVectors(t[cT].B, centreB));
							t[cT].Cn = unitVector(subVectors(t[cT].C, centreC));
						}
						else
						{
							t[cT].An = unitVector(subVectors(centreA, t[cT].A));
							t[cT].Bn = unitVector(subVectors(centreB, t[cT].B));
							t[cT].Cn = unitVector(subVectors(centreC, t[cT].C));
						}
					}
					else
					{
						polyNode lowerNode = p[j];
						polyNode upperNode = p[j + 1];
						polyNode tempNormal = getNormal(upperNode, lowerNode);

						double angA = aStart + i * aStep;
						double angB = aStart + (double(i) + 1) * aStep;
						double rad = offset + tempNormal.x;

						temp.x = rad * cos(angA * PI / 180.0);
						temp.y = rad * sin(angA * PI / 180.0);
						temp.z = tempNormal.y;
						temp.w = 0.0;

						t[cT].An = unitVector(temp);

						temp.x = rad * cos(angB * PI / 180.0);
						temp.y = rad * sin(angB * PI / 180.0);
						temp.z = tempNormal.y;
						temp.w = 0.0;

						t[cT].Bn = unitVector(temp);

						t[cT].Cn = t[cT].An;
					}*/
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
					/*if (p[j].s >= 0 || p[j + 1].s >= 0)
					{
						int lowerArcNo = p[j].s;
						int upperArcNo = p[j + 1].s;
						int direction;
						vect3 centreA = ac[resolH * lowerArcNo + i + 1];
						vect3 centreB = ac[resolH * upperArcNo + i + 1];
						vect3 centreC = ac[resolH * upperArcNo + i];
						if (lowerArcNo < 0) { direction = dir[upperArcNo]; }
						if (upperArcNo < 0) { direction = dir[lowerArcNo]; }
						if (upperArcNo >= 0 && lowerArcNo >= 0) { direction = dir[lowerArcNo]; }
						if (direction > 0)
						{
							t[cT].An = unitVector(subVectors(t[cT].A, centreA));
							t[cT].Bn = unitVector(subVectors(t[cT].B, centreB));
							t[cT].Cn = unitVector(subVectors(t[cT].C, centreC));
						}
						else
						{
							t[cT].An = unitVector(subVectors(centreA, t[cT].A));
							t[cT].Bn = unitVector(subVectors(centreB, t[cT].B));
							t[cT].Cn = unitVector(subVectors(centreC, t[cT].C));
						}
					}
					else
					{
						polyNode lowerNode = p[j];
						polyNode upperNode = p[j + 1];
						polyNode tempNormal = getNormal(lowerNode, upperNode);

						double angA = aStart + i * aStep;
						double angB = aStart + (double(i) + 1) * aStep;
						double rad = offset + tempNormal.x;

						temp.x = rad * cos(angB * PI / 180.0);
						temp.y = rad * sin(angB * PI / 180.0);
						temp.z = tempNormal.y;
						temp.w = 0.0;

						t[cT].An = unitVector(temp);

						t[cT].Bn = t[cT].An;

						temp.x = rad * cos(angA * PI / 180.0);
						temp.y = rad * sin(angA * PI / 180.0);
						temp.z = tempNormal.y;
						temp.w = 0.0;

						t[cT].Cn = unitVector(temp);
					}*/
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
					/*if (p[j].s >= 0 || p[j + 1].s >= 0)
					{
						int lowerArcNo = p[j].s;
						int upperArcNo = p[j + 1].s;
						int direction;
						vect3 centreA = ac[resolH * lowerArcNo + i];
						vect3 centreB = ac[resolH * lowerArcNo];
						vect3 centreC = ac[resolH * upperArcNo + i];
						if (lowerArcNo < 0) { direction = dir[upperArcNo]; }
						if (upperArcNo < 0) { direction = dir[lowerArcNo]; }
						if (upperArcNo >= 0 && lowerArcNo >= 0) { direction = dir[lowerArcNo]; }
						if (direction > 0)
						{
							t[cT].An = unitVector(subVectors(t[cT].A, centreA));
							t[cT].Bn = unitVector(subVectors(t[cT].B, centreB));
							t[cT].Cn = unitVector(subVectors(t[cT].C, centreC));
						}
						else
						{
							t[cT].An = unitVector(subVectors(centreA, t[cT].A));
							t[cT].Bn = unitVector(subVectors(centreB, t[cT].B));
							t[cT].Cn = unitVector(subVectors(centreC, t[cT].C));
						}
					}
					else
					{
						polyNode lowerNode = p[j];
						polyNode upperNode = p[j + 1];
						polyNode tempNormal = getNormal(upperNode, lowerNode);

						double angA = aStart + i * aStep;
						double angB = aStart + (double(i) + 1) * aStep;
						double rad = offset + tempNormal.x;

						temp.x = rad * cos(angA * PI / 180.0);
						temp.y = rad * sin(angA * PI / 180.0);
						temp.z = tempNormal.y;
						temp.w = 0.0;

						t[cT].An = unitVector(temp);

						temp.x = rad * cos(angB * PI / 180.0);
						temp.y = rad * sin(angB * PI / 180.0);
						temp.z = tempNormal.y;
						temp.w = 0.0;

						t[cT].Bn = unitVector(temp);

						t[cT].Cn = t[cT].An;
					}*/
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
					/*if (p[j].s >= 0 || p[j + 1].s >= 0)
					{
						int lowerArcNo = p[j].s;
						int upperArcNo = p[j + 1].s;
						int direction;
						vect3 centreA = ac[resolH * lowerArcNo];
						vect3 centreB = ac[resolH * upperArcNo];
						vect3 centreC = ac[resolH * upperArcNo + i];
						if (lowerArcNo < 0) { direction = dir[upperArcNo]; }
						if (upperArcNo < 0) { direction = dir[lowerArcNo]; }
						if (upperArcNo >= 0 && lowerArcNo >= 0) { direction = dir[lowerArcNo]; }
						if (direction > 0)
						{
							t[cT].An = unitVector(subVectors(t[cT].A, centreA));
							t[cT].Bn = unitVector(subVectors(t[cT].B, centreB));
							t[cT].Cn = unitVector(subVectors(t[cT].C, centreC));
						}
						else
						{
							t[cT].An = unitVector(subVectors(centreA, t[cT].A));
							t[cT].Bn = unitVector(subVectors(centreB, t[cT].B));
							t[cT].Cn = unitVector(subVectors(centreC, t[cT].C));
						}
					}
					else
					{
						polyNode lowerNode = p[j];
						polyNode upperNode = p[j + 1];
						polyNode tempNormal = getNormal(lowerNode, upperNode);

						double angA = aStart + i * aStep;
						double angB = aStart + (double(i) + 1) * aStep;
						double rad = offset + tempNormal.x;

						temp.x = rad * cos(angB * PI / 180.0);
						temp.y = rad * sin(angB * PI / 180.0);
						temp.z = tempNormal.y;
						temp.w = 0.0;

						t[cT].An = unitVector(temp);

						t[cT].Bn = t[cT].An;

						temp.x = rad * cos(angA * PI / 180.0);
						temp.y = rad * sin(angA * PI / 180.0);
						temp.z = tempNormal.y;
						temp.w = 0.0;

						t[cT].Cn = unitVector(temp);
					}*/
					cT++;
				}
			}
		}	
	}
	printf("%d polygons generated...\n", cT);
	return t;
	delete[] v;
	delete[] ac;
	delete[] dir;
	delete[] p;
	delete[] t;	
}