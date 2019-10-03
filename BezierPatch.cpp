#include "BezierPatch.h"



BezierPatch::BezierPatch(int L, int T, Uint32 C)
{
	lod			= L;

	ctrlP[0]	= {0.0f, 0.0f, 0.0f, 1.0f};
	ctrlP[1]	= {1.0f, 0.0f, 0.0f, 1.0f};
	ctrlP[2]	= {2.0f, 0.0f, 0.0f, 1.0f};
	ctrlP[3]	= {0.0f, 1.0f, 0.0f, 1.0f};
	ctrlP[4]	= {1.0f, 1.0f, 0.0f, 1.0f};
	ctrlP[5]	= {2.0f, 1.0f, 0.0f, 1.0f};
	ctrlP[6]	= {0.0f, 2.0f, 0.0f, 1.0f};
	ctrlP[7]	= {1.0f, 2.0f, 0.0f, 1.0f};
	ctrlP[8]	= {2.0f, 2.0f, 0.0f, 1.0f};

	int nVert	= this->getTotalVert();
	vertices	= new vect3[nVert];

	isInsideOut = false;

	colour		= C;
	texture		= T;
}


BezierPatch::BezierPatch(int resol, vect3* P)
{
	lod			= resol;

	ctrlP[0]	= P[0];
	ctrlP[1]	= P[1];
	ctrlP[2]	= P[2];
	ctrlP[3]	= P[3];
	ctrlP[4]	= P[4];
	ctrlP[5]	= P[5];
	ctrlP[6]	= P[6];
	ctrlP[7]	= P[7];
	ctrlP[8]	= P[8];

	int nVert	= this->getTotalVert();
	vertices	= new vect3[nVert];

	isInsideOut = false;

	colour		= 255;
	texture		= 1;
}


BezierPatch::BezierPatch(int resol, vect3 a, vect3 b, vect3 c, vect3 d, vect3 e, vect3 f, vect3 g, vect3 h, vect3 i)
{
	lod			= resol;

	ctrlP[0]	= a;
	ctrlP[1]	= b;
	ctrlP[2]	= c;
	ctrlP[3]	= d;
	ctrlP[4]	= e;
	ctrlP[5]	= f;
	ctrlP[6]	= g;
	ctrlP[7]	= h;
	ctrlP[8]	= i;

	int nVert	= this->getTotalVert();
	vertices	= new vect3[nVert];

	isInsideOut = false;

	colour		= 255;
	texture		= 1;
}


BezierPatch::~BezierPatch()
{
	delete[] vertices;
}


void BezierPatch::setControlPoint(unsigned int p, vect3 V)
{
	if (p < this->getTotalVert())
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

void BezierPatch::getSpineCoord(vect3 frontP, vect3 midP, vect3 backP, vect3* V)
{
	vect3 midDist		= subVectors(midP, halfwayPoint(frontP, backP));
	vect3 midCtrl		= addVectors(midP, midDist);
	vect3 inTangent		= subVectors(midCtrl, frontP);
	vect3 outTangent	= subVectors(backP, midCtrl);
	for (auto i = 0; i < lod + 1; i++)
	{
		vect3 stringStart	= addVectors(frontP,	scaleVector((1.0f / (double)lod) * i, inTangent	));
		vect3 stringEnd		= addVectors(midCtrl,	scaleVector((1.0f / (double)lod) * i, outTangent	));
		vect3 stringVect	= subVectors(stringEnd, stringStart);
		V[i] = addVectors(stringStart, scaleVector((1.0f / (double)lod) * i, stringVect));
	}
}


void BezierPatch::getTangent(vect3 frontP, vect3 midP, vect3 backP, vect3* T)
{
	vect3 midDist		= subVectors(midP, halfwayPoint(frontP, backP));
	vect3 midCtrl		= addVectors(midP, midDist);
	vect3 inTangent		= subVectors(midCtrl, frontP);
	vect3 outTangent	= subVectors(backP, midCtrl);
	for (auto i = 0; i < lod + 1; i++)
	{
		vect3 stringStart	= addVectors(frontP,	scaleVector((1.0f / (double)lod) * i, inTangent	));
		vect3 stringEnd		= addVectors(midCtrl,	scaleVector((1.0f / (double)lod) * i, outTangent));		
		T[i] = unitVector(subVectors(stringEnd, stringStart));
	}
}


vect3 BezierPatch::getTangent(vect3 frontP, vect3 midP, vect3 backP, int i)
{
	vect3 midDist = subVectors(midP, halfwayPoint(frontP, backP));
	vect3 midCtrl = addVectors(midP, midDist);
	vect3 inTangent = subVectors(midCtrl, frontP);
	vect3 outTangent = subVectors(backP, midCtrl);

	vect3 stringStart	= addVectors(frontP,	scaleVector((1.0f / (double)lod) * i, inTangent	));
	vect3 stringEnd		= addVectors(midCtrl,	scaleVector((1.0f / (double)lod) * i, outTangent));

	return unitVector(subVectors(stringEnd, stringStart));
}


void BezierPatch::getVertexData(point3* pointCloud)
{
	/*Calculate Lower Horizontal Spine*/
	vect3* lowerH = new vect3[lod + 1];
	this->getSpineCoord(ctrlP[0], ctrlP[1], ctrlP[2], lowerH);
	/*Calculate Upper Horizontal Spine*/
	vect3* upperH = new vect3[lod + 1];
	this->getSpineCoord(ctrlP[6], ctrlP[7], ctrlP[8],upperH);

	/*Calculate Left Vertical Spine*/
	vect3* leftV = new vect3[lod + 1];
	this->getSpineCoord(ctrlP[0], ctrlP[3], ctrlP[6], leftV);
	/*Calculate Right Vertical Spine*/
	vect3* rightV = new vect3[lod + 1];
	this->getSpineCoord(ctrlP[2], ctrlP[5], ctrlP[8], rightV);

	vect3* middleH = new vect3[lod + 1];
	/*Calculate Middle Horizontal Spine*/
	this->getSpineCoord(leftV[lod / 2], ctrlP[4], rightV[lod / 2], middleH);
	/*Calculate Middle Vertical Spine*/
	vect3* middleV = new vect3[lod + 1];
	this->getSpineCoord(lowerH[lod / 2], ctrlP[4], upperH[lod / 2], middleV);

	for (auto j = 0; j < lod + 1; j++)
	{
		vect3* currentSpine = new vect3[lod + 1];
		this->getSpineCoord(leftV[j], middleV[j], rightV[j], currentSpine);
		for (auto i = 0; i < lod + 1; i++)
		{
			vertices[j * (lod + 1) + i] = currentSpine[i];
			point3 tempP;
			tempP.P = currentSpine[i];
			tempP.colour = 255;
			pointCloud[j * (lod + 1) + i] = tempP;
		}
		delete[] currentSpine;
	}

	delete[] lowerH;
	delete[] upperH;
	delete[] leftV;
	delete[] rightV;
	delete[] middleH;
	delete[] middleV;
}


void BezierPatch::getVertexData_(vect3* s, vect3* t)
{
	/*Calculate Lower Horizontal Spine*/
	vect3* lowerH = new vect3[lod + 1];
	this->getSpineCoord(ctrlP[0], ctrlP[1], ctrlP[2], lowerH);
	/*Calculate Upper Horizontal Spine*/
	vect3* upperH = new vect3[lod + 1];
	this->getSpineCoord(ctrlP[6], ctrlP[7], ctrlP[8], upperH);

	/*Calculate Left Vertical Spine*/
	vect3* leftV = new vect3[lod + 1];
	this->getSpineCoord(ctrlP[0], ctrlP[3], ctrlP[6], leftV);
	/*Calculate Right Vertical Spine*/
	vect3* rightV = new vect3[lod + 1];
	this->getSpineCoord(ctrlP[2], ctrlP[5], ctrlP[8], rightV);

	vect3* middleH = new vect3[lod + 1];
	/*Calculate Middle Horizontal Spine*/
	this->getSpineCoord(leftV[lod / 2], ctrlP[4], rightV[lod / 2], middleH);
	/*Calculate Middle Vertical Spine*/
	vect3 * middleV = new vect3[lod + 1];
	this->getSpineCoord(lowerH[lod / 2], ctrlP[4], upperH[lod / 2], middleV);
	/*Calculating vertex co-ordinates*/
	for (auto j = 0; j < lod + 1; j++)
	{
		vect3* currentSpine = new vect3[lod + 1];
		this->getSpineCoord(leftV[j], middleV[j], rightV[j], currentSpine);
		for (auto i = 0; i < lod + 1; i++)
		{
			vertices[j * (lod + 1) + i] = currentSpine[i];
			s[j * (lod + 1) + i]		= currentSpine[i];
		}
		delete[] currentSpine;
	}
	/*Calculating surface normals*/
	for (auto j = 0; j < lod + 1; j++)
	{
		for (auto i = 0; i < lod + 1; i++)
		{
			vect3 tangentH = getTangent(leftV[j],	middleV[j], rightV[j], i);
			vect3 tangentV = getTangent(lowerH[i],	middleH[i], upperH[i], j);
			//vect3 tangent  = crossProduct(tangentH, tangentV);
			t[j * (lod + 1) + i] = unitVector(crossProduct(tangentH, tangentV));
		}
	}

	delete[] lowerH;
	delete[] upperH;
	delete[] leftV;
	delete[] rightV;
	delete[] middleH;
	delete[] middleV;
}


void BezierPatch::getTriangleData_(triangle3dV* t)
{
	int totalVert, totalPoly;
	totalVert	= this->getTotalVert();
	totalPoly	= this->getTotalPoly();
	vect3* s	= new vect3[totalVert];
	vect3* tang	= new vect3[totalVert];
	this->getVertexData_(s, tang);

	int tCount = 0;

	for (int j = 0; j < lod; j++)
	{
		for (int i = 0; i < lod; i++)
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[j * (lod + 1) + i];
				t[tCount].B = s[(j + 1) * (lod + 1) + i + 1];
				t[tCount].C = s[j * (lod + 1) + i + 1];

				t[tCount].An = tang[j * (lod + 1) + i];
				t[tCount].Bn = tang[(j + 1) * (lod + 1) + i + 1];
				t[tCount].Cn = tang[j * (lod + 1) + i + 1];
			
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].C), subVectors(t[tCount].A, t[tCount].C));
				t[tCount].N = unitVector(temp);
				//t[tCount].N = t[tCount].An = t[tCount].Bn = t[tCount].Cn = unitVector(temp);

				/*
				vect3 temp = { 0.0, 0.0, 0.0, 1.0 };
				if (!isInsideOut)
				{
					temp = crossProduct(subVectors(t[tCount].B, t[tCount].C), subVectors(t[tCount].A, t[tCount].C));
				}
				else
				{
					temp = crossProduct(subVectors(t[tCount].C, t[tCount].B), subVectors(t[tCount].C, t[tCount].A));
				}
				t[tCount].N = t[tCount].An = t[tCount].Bn = t[tCount].Cn = unitVector(temp);
				*/

				t[tCount].At.u = (1.0 / lod) * i;			t[tCount].At.v = (1.0 / lod) * j;
				t[tCount].Bt.u = (1.0 / lod) * (i + 1);		t[tCount].Bt.v = (1.0 / lod) * (j + 1);
				t[tCount].Ct.u = (1.0 / lod) * (i + 1);		t[tCount].Ct.v = (1.0 / lod) * j;

				t[tCount].colour	= colour;
				t[tCount].texture	= texture;

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

				vect3 temp = crossProduct(subVectors(t[tCount].A, t[tCount].B), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp);
				//t[tCount].N = t[tCount].An = t[tCount].Bn = t[tCount].Cn = unitVector(temp);

				/*
				vect3 temp = { 0.0, 0.0, 0.0, 1.0 };
				if (!isInsideOut)
				{
					temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].B, t[tCount].C));
				}
				else
				{
					temp = crossProduct(subVectors(t[tCount].A, t[tCount].B), subVectors(t[tCount].C, t[tCount].B));
				}
				t[tCount].N = t[tCount].An = t[tCount].Bn = t[tCount].Cn = unitVector(temp);
				*/

				t[tCount].At.u = (1.0 / lod) * i;			t[tCount].At.v = (1.0 / lod) * j;
				t[tCount].Bt.u = (1.0 / lod) * i;			t[tCount].Bt.v = (1.0 / lod) * (j + 1);
				t[tCount].Ct.u = (1.0 / lod) * (i + 1);		t[tCount].Ct.v = (1.0 / lod) * (j + 1);

				t[tCount].colour = colour;
				t[tCount].texture = texture;

				tCount++;
			}
		}
	}

	delete[] s;

	transformMesh(this->getTotalPoly(), t, scale.x, scale.y, scale.z, position.x, position.y, position.z, rotation.x, rotation.y, rotation.z);
}


vect3 BezierPatch::getPosition()
{
	return position;
}


void BezierPatch::constructShadowVolume(vect3)
{

}

