#include "SolidRevolved.h"



SolidRevolved::SolidRevolved()
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

	n			= 2;
	*polyLine	= { (0.0, 0.0, 1), (0.0, 2.0, 1) };

	offset		= 1.0;

	aStart		= 0.0;
	aEnd		= 360.0;

	resolH		= 24;
	resolV		= 24;
}


SolidRevolved::SolidRevolved(double sx, double sy, double sz, double px, double py, double pz, double rx, double ry, double rz,
								Uint32 col, int t, int nP, polyNode* pLine, double off, double aS, double aE, int rH, int rV )
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

	colour		= col;
	texture		= t;

	n			= nP;
	polyLine	= pLine;

	offset		= off;

	aStart		= aS;
	aEnd		= aE;

	resolH		= rH;
	resolV		= rV;
}


SolidRevolved::~SolidRevolved()
{
}


int	SolidRevolved::numberOfStepsH()
{
	return abs(int(double(resolH) / 360.0 * (aEnd - aStart)));
}


int SolidRevolved::getTotalVert()
{
	return 0;
}


int SolidRevolved::getTotalPoly()
{
	int nNodes = this->numberOfNodes();
	int nStep = this->numberOfStepsH();
	int nVert = nStep * nNodes;

	return nStep * 2 * (nNodes - 1);
}


int SolidRevolved::numberOfNodes()
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


int SolidRevolved::numberOfArcs()
{
	int nA = 0;

	for (int i = 0; i < n; i++)
	{
		if (polyLine[i].s == 900) { nA++; }
	}

	return nA;
}


int SolidRevolved::numberOfFlats()
{
	int nF = 0;

	for (int i = 0; i < n; i++)
	{
		if (polyLine[i].s != 900) { nF++; }
	}

	return nF;
}


int* SolidRevolved::arcDirections()
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


polyNode* SolidRevolved::arcCentres()
{
	int cC = 0;

	int nArc = this->numberOfArcs();

	polyNode* centres = new polyNode[nArc];

	for (int i = 0; i < n; i++)
	{
		if (polyLine[i].s == 900)
		{
			if (cC < nArc)
			{
				centres[cC] = polyLine[i];
				cC++;
			}
		}
	}

	return centres;
	delete[] centres;
}


polyNode* SolidRevolved::tessellatePolyline()
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


polyNode* SolidRevolved::get2dNormals()
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
					n[2 * i] = unitNode(subNodes(p[i], arcC[curveNo]));
					n[2 * i + 1] = unitNode(subNodes(p[i + 1], arcC[curveNo]));
				}
				else if (dir[curveNo == -1])
				{
					n[2 * i] = unitNode(subNodes(arcC[curveNo], p[i]));
					n[2 * i + 1] = unitNode(subNodes(arcC[curveNo], p[i + 1]));
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


vect3* SolidRevolved::getVertexData()
{
	double aShell = aEnd - aStart;
	int nStep = this->numberOfStepsH();
	double aStep = aShell / nStep;
	int nNodes = this->numberOfNodes();
	polyNode* p;
	p = this->tessellatePolyline();
	int nVert = nStep * nNodes;

	vect3* v = new vect3[nVert];

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


triangle3dV* SolidRevolved::getTriangleDataFlat()
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
					t[cT].texture = texture;
					t[cT].colour = colour;
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
					t[cT].texture = texture;
					t[cT].colour = colour;
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
					t[cT].texture = texture;
					t[cT].colour = colour;
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
					t[cT].texture = texture;
					t[cT].colour = colour;
					cT++;
				}
			}
		}
	}

	return t;
	delete[] v;
	delete[] t;
}


triangle3dV* SolidRevolved::getTriangleData()
{
	double aShell = aEnd - aStart;
	int nStep = this->numberOfStepsH();
	double aStep = aShell / nStep;

	int nTriangle = this->getTotalPoly();
	int nNodes = this->numberOfNodes();
	triangle3dV* t = this->getTriangleDataFlat();

	polyNode* n = this->get2dNormals();
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

	return transformObject(this->getTotalPoly(), t, scaleX, scaleY, scaleZ, posX, posY, posZ, rotX, rotY, rotZ);

	delete[] n;
	delete[] t;
}


void SolidRevolved::getTriangleData_(triangle3dV* t)
{
	double aShell = aEnd - aStart;
	int nStep = this->numberOfStepsH();
	double aStep = aShell / nStep;

	int nTriangle = this->getTotalPoly();
	int nNodes = this->numberOfNodes();

	polyNode* n = this->get2dNormals();
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

	delete[] n;
}