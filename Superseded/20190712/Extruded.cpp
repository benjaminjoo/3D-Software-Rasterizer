//#include <stdio.h>
#include <math.h>
#include "Extruded.h"



Extruded::Extruded()
{
	offsetX = 0.0;
	offsetY = 0.0;

	resProfile = 12;
	resPath = 12;

	nProfile = 2;
	nPath = 4;

	pProfile = new polyNode[2];
	pPath = new polyNode[4];

	c = 255;
}


Extruded::Extruded(double offX, double offY, int resPrf, int resPth, int nPrf, int nPth, polyNode* pPrf, polyNode* pPth, Uint32 col)
{
	offsetX = offX;
	offsetY = offY;

	resProfile = resPrf;
	resPath = resPth;

	nProfile = nPrf;
	nPath = nPth;

	pProfile = pPrf;
	pPath = pPth;

	c = col;
}


Extruded::~Extruded()
{
}


Uint32 Extruded::getColour()
{
	return c;
}


int Extruded::numberOfProfileNodes()
{
	int nP = 0;

	for (int i = 0; i < nProfile; i++)
	{
		switch (pProfile[i].s)
		{
		case 1:
			nP++;
			break;
		case 4001:
			double a = pProfile[i].y;
			int nStep = abs(int(double(resProfile) / 360.0 * a));
			nP += nStep;
			break;
		}
	}

	return nP;
}


int Extruded::numberOfPathNodes()
{
	int nP = 0;

	for (int i = 0; i < nPath; i++)
	{
		switch (pPath[i].s)
		{
		case 1:
			nP++;
			break;
		case 4001:
			double a = pPath[i].y;
			int nStep = abs(int(double(resPath) / 360.0 * a));
			nP += nStep;
			break;
		}
	}

	return nP;
}


int Extruded::getTotalPoly()
{
	int nProfileNodes = this->numberOfProfileNodes();
	int nPathNodes = this->numberOfPathNodes();

	return (nPathNodes - 3) * (nProfileNodes - 1) * 2;
}


int Extruded::numberOfProfileArcs()
{
	int nA = 0;

	for (int i = 0; i < nProfile; i++)
	{
		if (pProfile[i].s == 900) { nA++; }
	}

	return nA;
}


int Extruded::numberOfProfileFlats()
{
	int nF = 0;

	for (int i = 0; i < nProfile; i++)
	{
		if (pProfile[i].s != 900) { nF++; }
	}

	return nF;
}


int Extruded::numberOfPathArcs()
{
	int nA = 0;

	for (int i = 1; i < nPath - 1; i++)
	{
		if (pPath[i].s == 900) { nA++; }
	}

	return nA;
}


int Extruded::numberOfPathFlats()
{
	int nF = 0;

	for (int i = 1; i < nPath - 1; i++)
	{
		if (pPath[i].s != 900) { nF++; }
	}

	return nF;
}


int* Extruded::arcDirectionsProfile()
{
	int cC = 0;

	int nArc = this->numberOfProfileArcs();

	int* directions = new int[nArc];

	for (int i = 0; i < nProfile; i++)
	{
		if (pProfile[i].s == 4001)
		{
			if (cC < nArc)
			{
				directions[cC] = sign(pProfile[i].y);
				cC++;
			}
		}
	}

	return directions;
	delete[] directions;
}


int* Extruded::arcDirectionsPath()
{
	int cC = 0;

	int nArc = this->numberOfPathArcs();

	int* directions = new int[nArc];

	for (int i = 0; i < nPath; i++)
	{
		if (pPath[i].s == 4001)
		{
			if (cC < nArc)
			{
				directions[cC] = sign(pPath[i].y);
				cC++;
			}
		}
	}

	return directions;
	delete[] directions;
}


polyNode* Extruded::arcCentresProfile()
{
	int cC = 0;

	int nArc = this->numberOfProfileArcs();

	polyNode* centres = new polyNode[nArc];

	for (int i = 0; i < nProfile; i++)
	{
		if (pProfile[i].s == 900)
		{
			if (cC < nArc)
			{
				centres[cC] = pProfile[i];
				cC++;
			}
		}
	}

	return centres;
	delete[] centres;
}


polyNode* Extruded::arcCentresPath()
{
	int cC = 0;

	int nArc = this->numberOfPathArcs();

	polyNode* centres = new polyNode[nArc];

	for (int i = 0; i < nPath; i++)
	{
		if (pPath[i].s == 900)
		{
			if (cC < nArc)
			{
				centres[cC] = pPath[i];
				cC++;
			}
		}
	}

	return centres;
	delete[] centres;
}


polyNode* Extruded::tessellateProfile()
{
	int nNodes = this->numberOfProfileNodes();

	polyNode* spine = new polyNode[nNodes];

	int cP = 0;
	int cA = 0;

	for (int i = 0; i < nProfile; i++)
	{
		switch (pProfile[i].s)
		{
		case 1:
			if (cP < nNodes)
			{
				spine[cP] = pProfile[i];
				spine[cP].s = -1;
				cP++;
			}
			break;
		case 4001:
			double sx, sy, cx, cy, r, a, aStart, aStep;
			sx = pProfile[i - 2].x;
			sy = pProfile[i - 2].y;
			cx = pProfile[i - 1].x;
			cy = pProfile[i - 1].y;
			r = distancePN(pProfile[i - 1], pProfile[i - 2]);
			a = pProfile[i].y;
			int nStep = abs(int(double(resProfile) / 360.0 * a));
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


polyNode* Extruded::tessellatePath()
{
	int nNodes = this->numberOfPathNodes();

	polyNode* spine = new polyNode[nNodes];

	int cP = 0;
	int cA = 0;

	for (int i = 0; i < nPath; i++)
	{
		switch (pPath[i].s)
		{
		case 1:
			if (cP < nNodes)
			{
				spine[cP] = pPath[i];
				spine[cP].s = -1;
				cP++;
			}
			break;
		case 4001:
			double sx, sy, cx, cy, r, a, aStart, aStep;
			sx = pPath[i - 2].x;
			sy = pPath[i - 2].y;
			cx = pPath[i - 1].x;
			cy = pPath[i - 1].y;
			r = distancePN(pPath[i - 1], pPath[i - 2]);
			a = pPath[i].y;
			int nStep = abs(int(double(resPath) / 360.0 * a));
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


double* Extruded::getPathBisectorAng()
{
	int nBisectors = this->numberOfPathNodes() - 2;
	double* bisectors = new double[nBisectors];
	polyNode* path = this->tessellatePath();

	polyNode currentNormal, nextNormal, biSector;
	double ang;
	double angNormal, angBisector;
	for (int i = 0; i < nBisectors; i++)
	{
		currentNormal = unitNode(getNormal(path[i + 1], path[i]));
		nextNormal = unitNode(getNormal(path[i + 2], path[i + 1]));
		biSector = unitNode(addNodes(currentNormal, nextNormal));

		if (currentNormal.y > 0)
		{
			angNormal = acos(dotNodes(currentNormal, { 1.0, 0.0, 1 }));
		}
		else
		{
			angNormal = -acos(dotNodes(currentNormal, { 1.0, 0.0, 1 }));
		}
		//printf("Normal #%d angle: %.4f\n", i, angNormal * 180.0 / PI);
		if (biSector.y > 0)
		{
			angBisector = acos(dotNodes(biSector, { 1.0, 0.0, 1 }));
		}
		else
		{
			angBisector = -acos(dotNodes(biSector, { 1.0, 0.0, 1 }));
		}
		//printf("Bisector #%d angle: %.4f\n", i, angBisector * 180.0 / PI);


		ang = angBisector - angNormal;

		if (i < nBisectors) { bisectors[i] = acos(dotNodes(currentNormal, biSector)); }
	}

	return bisectors;
	delete[] bisectors;
	delete[] path;
}


double* Extruded::getSinB()
{
	int nBisectors = this->numberOfPathNodes() - 2;
	double* sinB = new double[nBisectors];
	polyNode* path = this->tessellatePath();

	polyNode currentNormal, nextNormal, biSector;
	for (int i = 0; i < nBisectors; i++)
	{
		currentNormal = unitNode(getNormal(path[i + 1], path[i]));
		nextNormal = unitNode(getNormal(path[i + 2], path[i + 1]));
		biSector = unitNode(addNodes(currentNormal, nextNormal));
		sinB[i] = biSector.y;
		//printf("Sin B: %.4f\n", sinB[i]);
	}

	return sinB;
	delete[] sinB;
	delete[] path;
}


double* Extruded::getCosB()
{
	int nBisectors = this->numberOfPathNodes() - 2;
	double* cosB = new double[nBisectors];
	polyNode* path = this->tessellatePath();

	polyNode currentNormal, nextNormal, biSector;
	for (int i = 0; i < nBisectors; i++)
	{
		currentNormal = unitNode(getNormal(path[i + 1], path[i]));
		nextNormal = unitNode(getNormal(path[i + 2], path[i + 1]));
		biSector = unitNode(addNodes(currentNormal, nextNormal));
		cosB[i] = biSector.x;
		//printf("Cos B: %.4f\n", cosB[i]);
	}

	return cosB;
	delete[] cosB;
	delete[] path;
}


polyNode* Extruded::get2dNormals()
{
	int nProfileNodes = this->numberOfProfileNodes();
	//int nPathNodes = this->numberOfPathNodes() - 2;

	polyNode* profileP = this->tessellateProfile();
	//polyNode* pathP = this->tessellatePath();

	polyNode* arcCProfile = this->arcCentresProfile();
	//polyNode* arcCPath = this->arcCentresPath();

	int* dirProfile = this->arcDirectionsProfile();
	//int* dirPath = this->arcDirectionsPath();

	int nSides = nProfileNodes - 1;
	int nNormals = nSides * 2;

	polyNode* n = new polyNode[nNormals];

	for (int i = 0; i < nSides; i++)
	{
		polyNode nodeLower, nodeUpper;
		nodeLower = profileP[i];
		nodeUpper = profileP[i + 1];

		if (nodeLower.s >= 0 || nodeUpper.s >= 0)
		{
			int curveNo = nodeLower.s >= 0 ? nodeLower.s : nodeUpper.s;
			if (2 * i + 1 < nNormals)
			{
				if (dirProfile[curveNo] == 1)
				{
					n[2 * i] = unitNode(subNodes(profileP[i], arcCProfile[curveNo]));
					n[2 * i + 1] = unitNode(subNodes(profileP[i + 1], arcCProfile[curveNo]));
				}
				else if (dirProfile[curveNo == -1])
				{
					n[2 * i] = unitNode(subNodes(arcCProfile[curveNo], profileP[i]));
					n[2 * i + 1] = unitNode(subNodes(arcCProfile[curveNo], profileP[i + 1]));
				}
			}
		}
		else
		{
			if (2 * i + 1 < nNormals) { n[2 * i] = n[2 * i + 1] = unitNode(getNormal(profileP[i + 1], profileP[i])); }
		}
	}

	return n;
	delete[] n;
	delete[] profileP;
	delete[] arcCProfile;
	delete[] dirProfile;
}


vect3* Extruded::getVertexData()
{
	int nProfileNodes = this->numberOfProfileNodes();
	int nPathNodes = this->numberOfPathNodes() - 2;
	int nVert = nProfileNodes * nPathNodes;

	vect3* v = new vect3[nVert];

	polyNode* profileP = this->tessellateProfile();
	polyNode* pathP = this->tessellatePath();
	double* sinB = this->getSinB();
	double* cosB = this->getCosB();
	double* bisector = this->getPathBisectorAng();

	double r;
	int cV = 0;
	for (int j = 0; j < nPathNodes; j++)
	{
		for (int i = 0; i < nProfileNodes; i++)
		{
			if (cos(bisector[j]) != 0.0)
			{
				r = (offsetX + profileP[i].x) / cos(bisector[j]);
			}
			else
			{
				r = offsetX + profileP[i].x;
			}
			if (cV < nVert)
			{
				v[cV].x = pathP[j + 1].x + r * cosB[j];
				v[cV].y = pathP[j + 1].y + r * sinB[j];
				v[cV].z = offsetY + profileP[i].y;
				v[cV].w = 1.0;
				cV++;
			}
		}
	}

	return v;
	delete[] v;
	delete[] profileP, pathP, sinB, cosB, bisector;
}


triangle3dV* Extruded::getTriangleData()
{
	int nProfileNodes = this->numberOfProfileNodes();
	int nPathNodes = this->numberOfPathNodes() - 2;
	vect3* v = this->getVertexData();
	int nTriangle = this->getTotalPoly();
	triangle3dV* t = new triangle3dV[nTriangle];
	int cT = 0;
	vect3 temp;

	for (int j = 0; j < nPathNodes - 1; j++)
	{
		for (int i = 0; i < nProfileNodes - 1; i++)
		{
			if (cT < nTriangle)
			{
				t[cT].A = v[j * nProfileNodes + i + 1];
				t[cT].B = v[j * nProfileNodes + i];
				t[cT].C = v[(j + 1) * nProfileNodes + i];
				temp = crossProduct(subVectors(t[cT].B, t[cT].A), subVectors(t[cT].C, t[cT].B));
				t[cT].N = unitVector(temp);
				t[cT].An = t[cT].Bn = t[cT].Cn = t[cT].N;
				cT++;
			}
			if (cT < nTriangle)
			{
				t[cT].A = v[j * nProfileNodes + i + 1];
				t[cT].B = v[(j + 1) * nProfileNodes + i];
				t[cT].C = v[(j + 1) * nProfileNodes + i + 1];
				temp = crossProduct(subVectors(t[cT].B, t[cT].A), subVectors(t[cT].C, t[cT].B));
				t[cT].N = unitVector(temp);
				t[cT].An = t[cT].Bn = t[cT].Cn = t[cT].N;
				cT++;
			}
		}
	}

	return t;

	delete[] t;
	delete[] v;
}


triangle3dV* Extruded::getTriangleDataN()
{
	int nTriangle = this->getTotalPoly();
	int nProfileNodes = this->numberOfProfileNodes();
	int nPathNodes = this->numberOfPathNodes() - 2;
	triangle3dV* t = this->getTriangleData();

	polyNode* pathP = this->tessellatePath();
	int* dirPath = this->arcDirectionsPath();
	polyNode* arcCPath = this->arcCentresPath();

	polyNode* n = this->get2dNormals();

	int cT = 0;	//Polygon counter
	//int cC = 0;	//Path curve counter
	double rLower, rUpper;
	polyNode nodeFront, nodeBack;
	polyNode normalFront, normalBack;
	double angFront = 0, angBack = 0;

	for (int j = 1; j < nPathNodes - 1; j++)
	{
		nodeFront = pathP[j];
		nodeBack = pathP[j + 1];
		if (nodeFront.s >= 0 || nodeBack.s >= 0)
		//If current path node is on a CURVED segment
		{
			int curveNo = nodeFront.s >= 0 ? nodeFront.s : nodeBack.s;
			if (dirPath[curveNo] == 1)
			//If path bends LEFT
			{
				normalFront = unitNode(subNodes(nodeFront, arcCPath[curveNo]));
				normalBack = unitNode(subNodes(nodeBack, arcCPath[curveNo]));

				if (normalFront.y > 0)
				{
					angFront = acos(dotNodes(normalFront, { 1.0, 0.0, 1 }));
				}
				else
				{
					angFront = -acos(dotNodes(normalFront, { 1.0, 0.0, 1 }));
				}
				if (normalBack.y >= 0)
				{
					angBack = acos(dotNodes(normalBack, { 1.0, 0.0, 1 }));
				}
				else
				{
					angBack = -acos(dotNodes(normalBack, { 1.0, 0.0, 1 }));
				}
				//printf("Curve No. %d Front Angle: %.4f\tBack Angle: %.4f\n", curveNo, angFront * 180.0 / PI, angBack * 180.0 / PI);

			}
			if (dirPath[curveNo] == -1)
			//If path bends RIGHT
			{
				normalFront = unitNode(subNodes(arcCPath[curveNo], nodeFront));
				normalBack = unitNode(subNodes(arcCPath[curveNo], nodeBack));

				if (normalFront.y > 0)
				{
					angFront = acos(dotNodes(normalFront, { 1.0, 0.0, 1 }));
				}
				else
				{
					angFront = -acos(dotNodes(normalFront, { 1.0, 0.0, 1 }));
				}
				if (normalBack.y >= 0)
				{
					angBack = acos(dotNodes(normalBack, { 1.0, 0.0, 1 }));
				}
				else
				{
					angBack = -acos(dotNodes(normalBack, { 1.0, 0.0, 1 }));
				}
				//printf("Curve No. %d Front Angle: %.4f\tBack Angle: %.4f\n", curveNo, angFront * 180.0 / PI, angBack * 180.0 / PI);

			}
			for (int i = 0; i < nProfileNodes - 1; i++)
			{
				rLower = n[i].x;	rUpper = n[i + 1].x;

				if (cT < nTriangle)
				{
					t[cT].An.x = cos(angFront);
					t[cT].An.y = sin(angFront);
					t[cT].An.z = n[i + 1].y;
					t[cT].An.w = 0.0;

					t[cT].Bn.x = cos(angFront);
					t[cT].Bn.y = sin(angFront);
					t[cT].Bn.z = n[i].y;
					t[cT].Bn.w = 0.0;

					t[cT].Cn.x = cos(angBack);
					t[cT].Cn.y = sin(angBack);
					t[cT].Cn.z = n[i].y;
					t[cT].Cn.w = 0.0;

					/*t[cT].An.x = rUpper * cos(angFront);
					t[cT].An.y = rUpper * sin(angFront);
					t[cT].An.z = n[i + 1].y;
					t[cT].An.w = 0.0;

					t[cT].Bn.x = rLower * cos(angFront);
					t[cT].Bn.y = rLower * sin(angFront);
					t[cT].Bn.z = n[i].y;
					t[cT].Bn.w = 0.0;

					t[cT].Cn.x = rLower * cos(angBack);
					t[cT].Cn.y = rLower * sin(angBack);
					t[cT].Cn.z = n[i].y;
					t[cT].Cn.w = 0.0;*/

					cT++;
				}
				if (cT < nTriangle)
				{
					t[cT].An.x = cos(angFront);
					t[cT].An.y = sin(angFront);
					t[cT].An.z = n[i + 1].y;
					t[cT].An.w = 0.0;

					t[cT].Bn.x = cos(angBack);
					t[cT].Bn.y = sin(angBack);
					t[cT].Bn.z = n[i].y;
					t[cT].Bn.w = 0.0;

					t[cT].Cn.x = cos(angBack);
					t[cT].Cn.y = sin(angBack);
					t[cT].Cn.z = n[i + 1].y;
					t[cT].Cn.w = 0.0;

					/*t[cT].An.x = rUpper * cos(angFront);
					t[cT].An.y = rUpper * sin(angFront);
					t[cT].An.z = n[i + 1].y;
					t[cT].An.w = 0.0;

					t[cT].Bn.x = rLower * cos(angBack);
					t[cT].Bn.y = rLower * sin(angBack);
					t[cT].Bn.z = n[i].y;
					t[cT].Bn.w = 0.0;

					t[cT].Cn.x = rUpper * cos(angBack);
					t[cT].Cn.y = rUpper * sin(angBack);
					t[cT].Cn.z = n[i + 1].y;
					t[cT].Cn.w = 0.0;*/

					cT++;
				}
			}
		}
		else
		//If current path node is on FLAT segment
		{
			for (int i = 0; i < nProfileNodes - 1; i++)
			{

			}
		}
	}

	return t;

	delete[] n;
	delete[] t;
	delete[] pathP; dirPath, arcCPath;
}