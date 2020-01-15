#include "BezierPatch.h"


BezierPatch::BezierPatch(unsigned int L, int T, Uint32 C)
{
	lod = L;

	ctrlP[0] = { 0.0f, 0.0f, 0.0f, 1.0f };
	ctrlP[1] = { 1.0f, 0.0f, 0.0f, 1.0f };
	ctrlP[2] = { 2.0f, 0.0f, 0.0f, 1.0f };
	ctrlP[3] = { 0.0f, 1.0f, 0.0f, 1.0f };
	ctrlP[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
	ctrlP[5] = { 2.0f, 1.0f, 0.0f, 1.0f };
	ctrlP[6] = { 0.0f, 2.0f, 0.0f, 1.0f };
	ctrlP[7] = { 1.0f, 2.0f, 0.0f, 1.0f };
	ctrlP[8] = { 2.0f, 2.0f, 0.0f, 1.0f };

	std::unique_ptr<vect3[]> vertices(new vect3[this->getTotalVert()]);

	isInsideOut = false;

	colour = C;
	texture = T;
}


BezierPatch::BezierPatch(unsigned int resol, vect3* P)
{
	lod = resol;

	ctrlP[0] = P[0];
	ctrlP[1] = P[1];
	ctrlP[2] = P[2];
	ctrlP[3] = P[3];
	ctrlP[4] = P[4];
	ctrlP[5] = P[5];
	ctrlP[6] = P[6];
	ctrlP[7] = P[7];
	ctrlP[8] = P[8];

	std::unique_ptr<vect3[]> vertices(new vect3[this->getTotalVert()]);

	isInsideOut = false;

	colour = 255;
	texture = 1;
}


BezierPatch::BezierPatch(unsigned int resol, vect3 a, vect3 b, vect3 c, vect3 d, vect3 e, vect3 f, vect3 g, vect3 h, vect3 i)
{
	lod = resol;

	ctrlP[0] = a;
	ctrlP[1] = b;
	ctrlP[2] = c;
	ctrlP[3] = d;
	ctrlP[4] = e;
	ctrlP[5] = f;
	ctrlP[6] = g;
	ctrlP[7] = h;
	ctrlP[8] = i;

	std::unique_ptr<vect3[]> vertices(new vect3[this->getTotalVert()]);

	isInsideOut = false;

	colour = 255;
	texture = 1;
}


BezierPatch::~BezierPatch()
{
}


void BezierPatch::setControlPoint(unsigned int p, vect3 V)
{
	if (p < 9)
	{
		ctrlP[p] = V;
	}
}


void BezierPatch::turnInsideOut()
{
	isInsideOut = isInsideOut ? false : true;
}


int BezierPatch::getTotalVert()
{
	return (lod + 1) * (lod + 1);
}


int	BezierPatch::getTotalPoly()
{
	return lod * lod * 2;
}

/*
void BezierPatch::getSpineCoord(vect3 frontP, vect3 midP, vect3 backP, vect3* V)
{
	vect3 inTangent		= subVectors(midP, frontP);
	vect3 outTangent	= subVectors(backP, midP);
	for (auto i = 0; i < lod + 1; i++)
	{
		vect3 stringStart	= addVectors(frontP,	scaleVector((1.0f / (double)lod) * i, inTangent	));
		vect3 stringEnd		= addVectors(midP,		scaleVector((1.0f / (double)lod) * i, outTangent	));
		vect3 stringVect	= subVectors(stringEnd, stringStart);
		V[i] = addVectors(stringStart, scaleVector((1.0f / (double)lod) * i, stringVect));
	}
}
*/

void BezierPatch::getSpineCoord(vect3 frontP, vect3 midP, vect3 backP, vect3 * V)
{
	vect3 midDist = midP - halfwayPoint(frontP, backP);
	vect3 midCtrl = midP + midDist;
	vect3 inTangent = midCtrl - frontP;
	vect3 outTangent = backP - midCtrl;
	for (unsigned int i = 0; i < lod + 1; i++)
	{
		vect3 stringStart = frontP + inTangent * ((1.0f / (double)lod) * i);
		vect3 stringEnd = midCtrl + outTangent * ((1.0f / (double)lod) * i);
		vect3 stringVect = stringEnd - stringStart;
		V[i] = stringStart + stringVect * ((1.0f / (double)lod) * i);
	}
}


void BezierPatch::getSpineCoord(vect3 frontP, vect3 midP, vect3 backP, std::shared_ptr<vect3[]> V)
{
	vect3 midDist = midP - halfwayPoint(frontP, backP);
	vect3 midCtrl = midP + midDist;
	vect3 inTangent = midCtrl - frontP;
	vect3 outTangent = backP - midCtrl;
	for (unsigned int i = 0; i < lod + 1; i++)
	{
		vect3 stringStart = frontP + inTangent * ((1.0f / (double)lod) * i);
		vect3 stringEnd = midCtrl + outTangent * ((1.0f / (double)lod) * i);
		vect3 stringVect = stringEnd - stringStart;
		V[i] = stringStart + stringVect * ((1.0f / (double)lod) * i);
	}
}


void BezierPatch::getTangent(vect3 frontP, vect3 midP, vect3 backP, vect3 * T)
{
	vect3 midDist = midP - halfwayPoint(frontP, backP);
	vect3 midCtrl = midP + midDist;
	vect3 inTangent = midCtrl - frontP;
	vect3 outTangent = backP - midCtrl;
	for (unsigned int i = 0; i < lod + 1; i++)
	{
		vect3 stringStart = frontP + inTangent * ((1.0f / (double)lod) * i);
		vect3 stringEnd = midCtrl + outTangent * ((1.0f / (double)lod) * i);
		T[i] = unitVector(stringEnd - stringStart);
	}
}


vect3 BezierPatch::getTangent(vect3 frontP, vect3 midP, vect3 backP, int i)
{
	vect3 midDist = midP - halfwayPoint(frontP, backP);
	vect3 midCtrl = midP + midDist;
	vect3 inTangent = midCtrl - frontP;
	vect3 outTangent = backP - midCtrl;

	vect3 stringStart = frontP + inTangent * ((1.0f / (double)lod) * i);
	vect3 stringEnd = midCtrl + outTangent * ((1.0f / (double)lod) * i);

	return unitVector(stringEnd - stringStart);
}


void BezierPatch::getVertexData(point3 * pointCloud)
{
	int nVert = this->getTotalVert();
	std::unique_ptr<vect3[]> vertices(new vect3[nVert]);

	/*Calculate Lower Horizontal Spine*/
	std::shared_ptr<vect3[]> lowerH(new vect3[lod + 1]);
	this->getSpineCoord(ctrlP[0], ctrlP[1], ctrlP[2], lowerH);
	/*Calculate Upper Horizontal Spine*/
	std::shared_ptr<vect3[]> upperH(new vect3[lod + 1]);
	this->getSpineCoord(ctrlP[6], ctrlP[7], ctrlP[8], upperH);

	/*Calculate Left Vertical Spine*/
	std::shared_ptr<vect3[]> leftV(new vect3[lod + 1]);
	this->getSpineCoord(ctrlP[0], ctrlP[3], ctrlP[6], leftV);
	/*Calculate Right Vertical Spine*/
	std::shared_ptr<vect3[]> rightV(new vect3[lod + 1]);
	this->getSpineCoord(ctrlP[2], ctrlP[5], ctrlP[8], rightV);

	/*Calculate Middle Horizontal Spine*/
	std::shared_ptr<vect3[]> middleH(new vect3[lod + 1]);
	this->getSpineCoord(leftV[lod / 2], ctrlP[4], rightV[lod / 2], middleH);
	/*Calculate Middle Vertical Spine*/
	std::shared_ptr<vect3[]> middleV(new vect3[lod + 1]);
	this->getSpineCoord(lowerH[lod / 2], ctrlP[4], upperH[lod / 2], middleV);

	for (unsigned int j = 0; j < lod + 1; j++)
	{
		std::shared_ptr<vect3[]> currentSpine(new vect3[lod + 1]);
		this->getSpineCoord(leftV[j], middleV[j], rightV[j], currentSpine);
		for (unsigned int i = 0; i < lod + 1; i++)
		{
			vertices[j * (lod + 1) + i] = currentSpine[i];
			point3 tempP;
			tempP.P = currentSpine[i];
			tempP.colour = 255;
			pointCloud[j * (lod + 1) + i] = tempP;
		}
	}
}


void BezierPatch::getVertexData_(std::shared_ptr<vect3[]> s, std::shared_ptr<vect3[]> t)
{
	int nVert = this->getTotalVert();
	std::unique_ptr<vect3[]> vertices(new vect3[nVert]);

	/*Calculate Lower Horizontal Spine*/
	std::shared_ptr<vect3[]> lowerH(new vect3[lod + 1]);
	this->getSpineCoord(ctrlP[0], ctrlP[1], ctrlP[2], lowerH);
	/*Calculate Upper Horizontal Spine*/
	std::shared_ptr<vect3[]> upperH(new vect3[lod + 1]);
	this->getSpineCoord(ctrlP[6], ctrlP[7], ctrlP[8], upperH);

	/*Calculate Left Vertical Spine*/
	std::shared_ptr<vect3[]> leftV(new vect3[lod + 1]);
	this->getSpineCoord(ctrlP[0], ctrlP[3], ctrlP[6], leftV);
	/*Calculate Right Vertical Spine*/
	std::shared_ptr<vect3[]> rightV(new vect3[lod + 1]);
	this->getSpineCoord(ctrlP[2], ctrlP[5], ctrlP[8], rightV);


	/*Calculate Middle Horizontal Spine*/
	std::shared_ptr<vect3[]> middleH(new vect3[lod + 1]);
	this->getSpineCoord(leftV[lod / 2], ctrlP[4], rightV[lod / 2], middleH);
	/*Calculate Middle Vertical Spine*/
	std::shared_ptr<vect3[]> middleV(new vect3[lod + 1]);
	this->getSpineCoord(lowerH[lod / 2], ctrlP[4], upperH[lod / 2], middleV);
	/*Calculating vertex co-ordinates*/
	for (unsigned int j = 0; j < lod + 1; j++)
	{
		std::shared_ptr<vect3[]> currentSpine(new vect3[lod + 1]);
		this->getSpineCoord(leftV[j], middleV[j], rightV[j], currentSpine);
		for (unsigned int i = 0; i < lod + 1; i++)
		{
			vertices[j * (lod + 1) + i] = currentSpine[i];
			s[j * (lod + 1) + i] = currentSpine[i];
		}
	}
	/*Calculating surface normals*/
	for (unsigned int j = 0; j < lod + 1; j++)
	{
		for (unsigned int i = 0; i < lod + 1; i++)
		{
			vect3 tangentH = getTangent(leftV[j], middleV[j], rightV[j], i);
			vect3 tangentV = getTangent(lowerH[i], middleH[i], upperH[i], j);
			t[j * (lod + 1) + i] = unitVector(tangentH ^ tangentV);
		}
	}
}


void BezierPatch::getTriangleData_(triangle3dV * t)
{
	int totalVert, totalPoly;
	totalVert = this->getTotalVert();
	totalPoly = this->getTotalPoly();
	std::shared_ptr<vect3[]> s(new vect3[totalVert]);
	std::shared_ptr<vect3[]> tang(new vect3[totalVert]);
	this->getVertexData_(s, tang);

	int tCount = 0;

	for (unsigned int j = 0; j < lod; j++)
	{
		for (unsigned int i = 0; i < lod; i++)
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[j * (lod + 1) + i];
				t[tCount].B = s[(j + 1) * (lod + 1) + i + 1];
				t[tCount].C = s[j * (lod + 1) + i + 1];

				t[tCount].An = tang[j * (lod + 1) + i];
				t[tCount].Bn = tang[(j + 1) * (lod + 1) + i + 1];
				t[tCount].Cn = tang[j * (lod + 1) + i + 1];

				vect3 temp = (t[tCount].B - t[tCount].C) ^ (t[tCount].A - t[tCount].C);
				t[tCount].N = unitVector(temp);

				t[tCount].At.u = (1.0f / double(lod)) * double(i);				t[tCount].At.v = (1.0f / double(lod)) * double(j);
				t[tCount].Bt.u = (1.0f / double(lod)) * (double(i) + 1.0f);		t[tCount].Bt.v = (1.0f / double(lod)) * (double(j) + 1.0f);
				t[tCount].Ct.u = (1.0f / double(lod)) * (double(i) + 1.0f);		t[tCount].Ct.v = (1.0f / double(lod)) * double(j);

				t[tCount].colour = colour;
				t[tCount].texture = texture;

				tCount++;
			}

			if (tCount < totalPoly)
			{
				t[tCount].A = s[j * (lod + 1) + i];
				t[tCount].B = s[(j + 1) * (lod + 1) + i];
				t[tCount].C = s[(j + 1) * (lod + 1) + i + 1];

				t[tCount].An = tang[j * (lod + 1) + i];
				t[tCount].Bn = tang[(j + 1) * (lod + 1) + i];
				t[tCount].Cn = tang[(j + 1) * (lod + 1) + i + 1];

				vect3 temp = (t[tCount].A - t[tCount].B) ^ (t[tCount].C - t[tCount].B);
				t[tCount].N = unitVector(temp);

				t[tCount].At.u = (1.0f / double(lod)) * double(i);			t[tCount].At.v = (1.0f / double(lod)) * double(j);
				t[tCount].Bt.u = (1.0f / double(lod)) * double(i);			t[tCount].Bt.v = (1.0f / double(lod)) * (double(j) + 1.0f);
				t[tCount].Ct.u = (1.0f / double(lod)) * (double(i) + 1.0f);	t[tCount].Ct.v = (1.0f / double(lod)) * (double(j) + 1.0f);

				t[tCount].colour = colour;
				t[tCount].texture = texture;

				tCount++;
			}
		}
	}
}


void BezierPatch::constructShadowVolume(vect3)
{

}


