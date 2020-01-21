#include "Room.h"


Room::Room()
{
	position = { 0.0f, 0.0f, 0.0f, 1.0f };
	rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
}


Room::Room(float pX, float pY, float pZ, float sX, float sY, float sZ):
	sizeX(sX), sizeY(sY), sizeZ(sZ)
{
	position = { pX, pY, pZ, 1.0f };
	rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
}


Room::~Room()
{
	if (openingOnBottom) { delete[] openingBottom; }
	if (openingOnTop) { delete[] openingTop; }
	if (openingOnBack) { delete[] openingBack; }
	if (openingOnFront) { delete[] openingFront; }
	if (openingOnLeft) { delete[] openingLeft; }
	if (openingOnRight) { delete[] openingRight; }
}


void Room::setPosition(float  px, float py, float pz)
{
	position	= { px, py, pz, 1.0 };
}


void Room::setDimension(float sx, float sy, float sz)
{
	sizeX		= sx;
	sizeY		= sy;
	sizeZ		= sz;
}


void Room::setRotation(float rx, float ry, float rz)
{
	rotation.x	= rx;
	rotation.y	= ry;
	rotation.z	= rz;
}


void Room::setColour(Uint32 bottom, Uint32 top, Uint32 back, Uint32 front, Uint32 left, Uint32 right)
{
	colBottom	= bottom;
	colTop		= top;
	colBack		= back;
	colFront	= front;
	colLeft		= left;
	colRight	= right;
}


void Room::setTexture(int bottom, int top, int back, int front, int left, int right)
{
	txtBottom	= bottom;
	txtTop		= top;
	txtBack		= back;
	txtFront	= front;
	txtLeft		= left;
	txtRight	= right;
}


void Room::setTextureScale(float bottom, float top, float back, float front, float left, float right)
{
	txUBottom	= bottom;
	txUTop		= top;
	txUBack		= back;
	txUFront	= front;
	txULeft		= left;
	txURight	= right;
}


void Room::setSideOn(bool bottom, bool top, bool back, bool front, bool left, bool right)
{
	isBottomOn	= bottom;
	isTopOn		= top;
	isBackOn	= back;
	isFrontOn	= front;
	isLeftOn	= left;
	isRightOn	= right;
}


void Room::addOpening(Side currentSide, float bottomLeftX, float bottomLeftY, float width, float height)
{
	switch (currentSide)
	{
	case Bottom:
		openingOnBottom = true;
		openingBottom = new vect3[4];
		if (bottomLeftX < 0.0f) { bottomLeftX = 0.0f; }
		if (bottomLeftY < 0.0f) { bottomLeftY = 0.0f; }
		if (bottomLeftX >= sizeX) { bottomLeftX = sizeX * 0.25f; }
		if (bottomLeftY >= sizeY) { bottomLeftY = sizeY * 0.25f; }
		if (bottomLeftX + width > sizeX) { width = sizeX - bottomLeftX; }
		if (bottomLeftY + height > sizeY) { height = sizeY - bottomLeftY; }
		openingBottom[0].x = bottomLeftX;
		openingBottom[0].y = bottomLeftY;
		openingBottom[0].z = 0.0f;
		openingBottom[0].w = 1.0f;
		openingBottom[1].x = bottomLeftX + width;
		openingBottom[1].y = bottomLeftY;
		openingBottom[1].z = 0.0f;
		openingBottom[1].w = 1.0f;
		openingBottom[2].x = bottomLeftX + width;
		openingBottom[2].y = bottomLeftY + height;
		openingBottom[2].z = 0.0f;
		openingBottom[2].w = 1.0f;
		openingBottom[3].x = bottomLeftX;
		openingBottom[3].y = bottomLeftY + height;
		openingBottom[3].z = 0.0f;
		openingBottom[3].w = 1.0f;
		break;
	case Top:
		openingOnTop = true;
		openingTop = new vect3[4];
		if (bottomLeftX < 0.0f) { bottomLeftX = 0.0f; }
		if (bottomLeftY < 0.0f) { bottomLeftY = 0.0f; }
		if (bottomLeftX >= sizeX) { bottomLeftX = sizeX * 0.25f; }
		if (bottomLeftY >= sizeY) { bottomLeftY = sizeY * 0.25f; }
		if (bottomLeftX + width > sizeX) { width = sizeX - bottomLeftX; }
		if (bottomLeftY + height > sizeY) { height = sizeY - bottomLeftY; }
		openingTop[0].x = bottomLeftX;
		openingTop[0].y = bottomLeftY;
		openingTop[0].z = sizeZ;
		openingTop[0].w = 1.0f;
		openingTop[1].x = bottomLeftX + width;
		openingTop[1].y = bottomLeftY;
		openingTop[1].z = sizeZ;
		openingTop[1].w = 1.0f;
		openingTop[2].x = bottomLeftX + width;
		openingTop[2].y = bottomLeftY + height;
		openingTop[2].z = sizeZ;
		openingTop[2].w = 1.0f;
		openingTop[3].x = bottomLeftX;
		openingTop[3].y = bottomLeftY + height;
		openingTop[3].z = sizeZ;
		openingTop[3].w = 1.0f;
		break;
	case Back:
		openingOnBack = true;
		openingBack = new vect3[4];
		if (bottomLeftX < 0.0f) { bottomLeftX = 0.0f; }
		if (bottomLeftY < 0.0f) { bottomLeftY = 0.0f; }
		if (bottomLeftX >= sizeX) { bottomLeftX = sizeX * 0.25f; }
		if (bottomLeftY >= sizeZ) { bottomLeftY = sizeZ * 0.25f; }
		if (bottomLeftX + width > sizeX) { width = sizeX - bottomLeftX; }
		if (bottomLeftY + height > sizeZ) { height = sizeZ - bottomLeftY; }
		openingBack[0].x = bottomLeftX;
		openingBack[0].y = sizeY;
		openingBack[0].z = bottomLeftY;
		openingBack[0].w = 1.0f;
		openingBack[1].x = bottomLeftX + width;
		openingBack[1].y = sizeY;
		openingBack[1].z = bottomLeftY;
		openingBack[1].w = 1.0f;
		openingBack[2].x = bottomLeftX + width;
		openingBack[2].y = sizeY;
		openingBack[2].z = bottomLeftY + height;
		openingBack[2].w = 1.0f;
		openingBack[3].x = bottomLeftX;
		openingBack[3].y = sizeY;
		openingBack[3].z = bottomLeftY + height;
		openingBack[3].w = 1.0f;
		break;
	case Front:
		openingOnFront = true;
		openingFront = new vect3[4];
		if (bottomLeftX < 0.0f) { bottomLeftX = 0.0f; }
		if (bottomLeftY < 0.0f) { bottomLeftY = 0.0f; }
		if (bottomLeftX >= sizeX) { bottomLeftX = sizeX * 0.25f; }
		if (bottomLeftY >= sizeZ) { bottomLeftY = sizeZ * 0.25f; }
		if (bottomLeftX + width > sizeX) { width = sizeX - bottomLeftX; }
		if (bottomLeftY + height > sizeZ) { height = sizeZ - bottomLeftY; }
		openingFront[0].x = bottomLeftX;
		openingFront[0].y = 0.0f;
		openingFront[0].z = bottomLeftY;
		openingFront[0].w = 1.0f;
		openingFront[1].x = bottomLeftX + width;
		openingFront[1].y = 0.0f;
		openingFront[1].z = bottomLeftY;
		openingFront[1].w = 1.0f;
		openingFront[2].x = bottomLeftX + width;
		openingFront[2].y = 0.0f;
		openingFront[2].z = bottomLeftY + height;
		openingFront[2].w = 1.0f;
		openingFront[3].x = bottomLeftX;
		openingFront[3].y = 0.0f;
		openingFront[3].z = bottomLeftY + height;
		openingFront[3].w = 1.0f;
		break;
	case Left:
		openingOnLeft = true;
		openingLeft = new vect3[4];
		if (bottomLeftX < 0.0f) { bottomLeftX = 0.0f; }
		if (bottomLeftY < 0.0f) { bottomLeftY = 0.0f; }
		if (bottomLeftX >= sizeY) { bottomLeftX = sizeY * 0.25f; }
		if (bottomLeftY >= sizeZ) { bottomLeftY = sizeZ * 0.25f; }
		if (bottomLeftX + width > sizeY) { width = sizeY - bottomLeftX; }
		if (bottomLeftY + height > sizeZ) { height = sizeZ - bottomLeftY; }
		openingLeft[0].x = 0.0f;
		openingLeft[0].y = bottomLeftX;
		openingLeft[0].z = bottomLeftY;
		openingLeft[0].w = 1.0f;
		openingLeft[1].x = 0.0f;
		openingLeft[1].y = bottomLeftX + width;
		openingLeft[1].z = bottomLeftY;
		openingLeft[1].w = 1.0f;
		openingLeft[2].x = 0.0f;
		openingLeft[2].y = bottomLeftX + width;
		openingLeft[2].z = bottomLeftY + height;
		openingLeft[2].w = 1.0f;
		openingLeft[3].x = 0.0f;
		openingLeft[3].y = bottomLeftX;
		openingLeft[3].z = bottomLeftY + height;
		openingLeft[3].w = 1.0f;
		break;
	case Right:
		openingOnRight = true;
		openingRight = new vect3[4];
		if (bottomLeftX < 0.0f) { bottomLeftX = 0.0f; }
		if (bottomLeftY < 0.0f) { bottomLeftY = 0.0f; }
		if (bottomLeftX >= sizeY) { bottomLeftX = sizeY * 0.25f; }
		if (bottomLeftY >= sizeZ) { bottomLeftY = sizeZ * 0.25f; }
		if (bottomLeftX + width > sizeY) { width = sizeY - bottomLeftX; }
		if (bottomLeftY + height > sizeZ) { height = sizeZ - bottomLeftY; }
		openingRight[0].x = sizeX;
		openingRight[0].y = bottomLeftX;
		openingRight[0].z = bottomLeftY;
		openingRight[0].w = 1.0f;
		openingRight[1].x = sizeX;
		openingRight[1].y = bottomLeftX + width;
		openingRight[1].z = bottomLeftY;
		openingRight[1].w = 1.0f;
		openingRight[2].x = sizeX;
		openingRight[2].y = bottomLeftX + width;
		openingRight[2].z = bottomLeftY + height;
		openingRight[2].w = 1.0f;
		openingRight[3].x = sizeX;
		openingRight[3].y = bottomLeftX;
		openingRight[3].z = bottomLeftY + height;
		openingRight[3].w = 1.0f;
		break;
	}
}


void Room::addBottomPoly(triangle3dV T)
{
	bottomPolyContainer.push_back(T);
}


void Room::addTopPoly(triangle3dV T)
{
	topPolyContainer.push_back(T);
}


void Room::addBackPoly(triangle3dV T)
{
	backPolyContainer.push_back(T);
}


void Room::addFrontPoly(triangle3dV T)
{
	frontPolyContainer.push_back(T);
}


void Room::addLeftPoly(triangle3dV T)
{
	leftPolyContainer.push_back(T);
}


void Room::addRightPoly(triangle3dV T)
{
	rightPolyContainer.push_back(T);
}


int Room::getTotalVert()
{
	return 8;
}


int Room::getTotalPoly()
{
	return	bottomPolyContainer.size() + topPolyContainer.size() + backPolyContainer.size() +
			frontPolyContainer.size() + leftPolyContainer.size() + rightPolyContainer.size();
}


void Room::breakUpFace(Side currentSide, polygon4uv* rootPolyPtr, polyNode* pHole, vect3 N, Uint32 colour, int texture)
{
	polyNode splittingA, splittingB;
	for(int i = 0; i < 4; i++)
	{
		if (i < 3)
		{
			splittingA = pHole[i];
			splittingB = pHole[i + 1];
		}
		else
		{
			splittingA = pHole[i];
			splittingB = pHole[0];
		}

		this->splitPolygon(currentSide, rootPolyPtr, splittingA, splittingB);
	}
	this->traversePolygonTree(currentSide, rootPolyPtr, N, colour, texture);
}


void Room::splitPolygon(Side currentSide, polygon4uv* polyPtr, polyNode p, polyNode q)
{
	handedness branch;
	int leftCounter, rightCounter, totalCounter;
	vect2 edgeStart, edgeEnd, intersectionPoint = { 0.0f, 0.0f, 0.0f, 0.0f };
	
	if (polyPtr->leftChild == nullptr && polyPtr->rightChild == nullptr)	//If polygon is childless
	{
		//Check whether polygon is split by current cutting edge
		if (checkPolygonForSplitting(*polyPtr, p, q))
		{
			//If so, split polygon into leftChild and rightChild
			polyPtr->leftChild = new polygon4uv;
			polyPtr->leftChild->leftChild = nullptr;
			polyPtr->leftChild->rightChild = nullptr;
			//printf("Polygon is split into leftChild...");

			polyPtr->rightChild = new polygon4uv;
			polyPtr->rightChild->leftChild = nullptr;
			polyPtr->rightChild->rightChild = nullptr;
			//printf("...and rightChild.\n");
			
			branch = left;
			leftCounter = 0;
			rightCounter = 0;
			totalCounter = 0;

			for (int i = 0 ; i < 4; i++)
			{
				if (i < 3)
				{
					edgeStart = polyPtr->points[i];
					edgeEnd = polyPtr->points[i + 1];
				}
				else
				{
					edgeStart = polyPtr->points[i];
					edgeEnd = polyPtr->points[0];
				}
				if (iSect2dLine(edgeStart, edgeEnd, p, q, &intersectionPoint))
				{
					//printf("Edge is split...\n");
					if (branch == left)
					{
						polyPtr->leftChild->points[leftCounter] = intersectionPoint;
						leftCounter++;
						branch = right;
						polyPtr->rightChild->points[rightCounter] = intersectionPoint;
						rightCounter++;
						polyPtr->rightChild->points[rightCounter] = edgeEnd;
						rightCounter++;
					}
					else if (branch == right)
					{
						polyPtr->rightChild->points[rightCounter] = intersectionPoint;
						rightCounter++;
						branch = left;
						polyPtr->leftChild->points[leftCounter] = intersectionPoint;
						leftCounter++;
						polyPtr->leftChild->points[leftCounter] = edgeEnd;
						leftCounter++;
					}
				}
				else
				{
					//printf("Edge doesn't need to be split...\n");
					if (branch == left)
					{
						polyPtr->leftChild->points[leftCounter] = edgeEnd;
						leftCounter++;
					}
					else if (branch == right)
					{
						polyPtr->rightChild->points[rightCounter] = edgeEnd;
						rightCounter++;
					}
				}
			}
		}
	}
	else if(polyPtr->leftChild != nullptr && polyPtr->rightChild != nullptr)
	{
		//Pass children to polygon splitting function
		splitPolygon(currentSide, polyPtr->leftChild, p, q);
		splitPolygon(currentSide, polyPtr->rightChild, p, q);
	}
	
}


bool Room::checkForHole(Side currentSide, polygon4uv* poly, vect3* hole)
{
	float holeCentreX = 0.0f, holeCentreY = 0.0f;
	float xMin, xMax, yMin, yMax;
	xMin = poly->points[0].x;
	xMax = poly->points[0].x;
	yMin = poly->points[0].y;
	yMax = poly->points[0].y;
	for (int i = 0; i < 4; i++)
	{
		if (poly->points[i].x < xMin) { xMin = poly->points[i].x; }
		if (poly->points[i].x > xMax) { xMax = poly->points[i].x; }
		if (poly->points[i].y < yMin) { yMin = poly->points[i].y; }
		if (poly->points[i].y > yMax) { yMax = poly->points[i].y; }
	}
	switch (currentSide)
	{
	case Bottom:
	case Top:
		holeCentreX = (hole[0].x + hole[2].x) / 2;
		holeCentreY = (hole[0].y + hole[2].y) / 2;
		break;
	case Back:
	case Front:
		holeCentreX = (hole[0].x + hole[2].x) / 2;
		holeCentreY = (hole[0].z + hole[2].z) / 2;
		break;
	case Left:
	case Right:
		holeCentreX = (hole[0].y + hole[2].y) / 2;
		holeCentreY = (hole[0].z + hole[2].z) / 2;
		break;
	}

	if (xMin < holeCentreX && holeCentreX < xMax && yMin < holeCentreY && holeCentreY < yMax)
	{
		return false;
	}
	else
	{
		return true;
	}
}


void Room::traversePolygonTree(Side currentSide, polygon4uv* polyTree, vect3 N, Uint32 colour, int texture)
{
	if (polyTree->leftChild == nullptr && polyTree->rightChild == nullptr)
	{
		//printf("\nPOLYGON IS SAVED...\n\n");
		triangle3dV T;

		T.An = T.Bn = T.Cn = T.N = N;
		T.colour	= colour;
		T.texture	= texture;

		switch (currentSide)
		{
		case Bottom:

			if (this->checkForHole(Bottom, polyTree, openingBottom))
			{
				T.A.x	= polyTree->points[0].x;	T.B.x	= polyTree->points[1].x;	T.C.x	= polyTree->points[2].x;
				T.A.y	= polyTree->points[0].y;	T.B.y	= polyTree->points[1].y;	T.C.y	= polyTree->points[2].y;
				T.A.z	= 0.0f;						T.B.z	= 0.0f;						T.C.z	= 0.0f;
				T.A.w	= 1.0f;						T.B.w	= 1.0f;						T.C.w	= 1.0f;
				T.At.u	= polyTree->points[0].u;	T.Bt.u	= polyTree->points[1].u;	T.Ct.u	= polyTree->points[2].u;
				T.At.v	= polyTree->points[0].v;	T.Bt.v	= polyTree->points[1].v;	T.Ct.v	= polyTree->points[2].v;

				this->addBottomPoly(T);

				T.A.x	= polyTree->points[0].x;	T.B.x	= polyTree->points[2].x;	T.C.x	= polyTree->points[3].x;
				T.A.y	= polyTree->points[0].y;	T.B.y	= polyTree->points[2].y;	T.C.y	= polyTree->points[3].y;
				T.A.z	= 0.0f;						T.B.z	= 0.0f;						T.C.z	= 0.0f;
				T.A.w	= 1.0f;						T.B.w	= 1.0f;						T.C.w	= 1.0f;
				T.At.u	= polyTree->points[0].u;	T.Bt.u	= polyTree->points[2].u;	T.Ct.u	= polyTree->points[3].u;
				T.At.v	= polyTree->points[0].v;	T.Bt.v	= polyTree->points[2].v;	T.Ct.v	= polyTree->points[3].v;

				this->addBottomPoly(T);
			}

			break;

		case Top:

			if (this->checkForHole(Top, polyTree, openingTop))
			{
				T.A.x	= polyTree->points[0].x;	T.B.x	= polyTree->points[1].x;	T.C.x	= polyTree->points[2].x;
				T.A.y	= polyTree->points[0].y;	T.B.y	= polyTree->points[1].y;	T.C.y	= polyTree->points[2].y;
				T.A.z	= sizeZ;					T.B.z	= sizeZ;					T.C.z	= sizeZ;
				T.A.w	= 1.0f;						T.B.w	= 1.0f;						T.C.w	= 1.0f;
				T.At.u	= polyTree->points[0].u;	T.Bt.u	= polyTree->points[1].u;	T.Ct.u	= polyTree->points[2].u;
				T.At.v	= polyTree->points[0].v;	T.Bt.v	= polyTree->points[1].v;	T.Ct.v	= polyTree->points[2].v;

				this->addTopPoly(T);

				T.A.x	= polyTree->points[0].x;	T.B.x	= polyTree->points[2].x;	T.C.x	= polyTree->points[3].x;
				T.A.y	= polyTree->points[0].y;	T.B.y	= polyTree->points[2].y;	T.C.y	= polyTree->points[3].y;
				T.A.z	= sizeZ;					T.B.z	= sizeZ;					T.C.z	= sizeZ;
				T.A.w	= 1.0f;						T.B.w	= 1.0f;						T.C.w	= 1.0f;
				T.At.u	= polyTree->points[0].u;	T.Bt.u	= polyTree->points[2].u;	T.Ct.u	= polyTree->points[3].u;
				T.At.v	= polyTree->points[0].v;	T.Bt.v	= polyTree->points[2].v;	T.Ct.v	= polyTree->points[3].v;

				this->addTopPoly(T);
			}

			break;

		case Back:

			if (this->checkForHole(Back, polyTree, openingBack))
			{
				T.A.x	= polyTree->points[0].x;	T.B.x	= polyTree->points[1].x;	T.C.x	= polyTree->points[2].x;
				T.A.z	= polyTree->points[0].y;	T.B.z	= polyTree->points[1].y;	T.C.z	= polyTree->points[2].y;
				T.A.y	= sizeY;					T.B.y	= sizeY;					T.C.y	= sizeY;
				T.A.w	= 1.0f;						T.B.w	= 1.0f;						T.C.w	= 1.0f;
				T.At.u	= polyTree->points[0].u;	T.Bt.u	= polyTree->points[1].u;	T.Ct.u	= polyTree->points[2].u;
				T.At.v	= polyTree->points[0].v;	T.Bt.v	= polyTree->points[1].v;	T.Ct.v	= polyTree->points[2].v;

				this->addBackPoly(T);

				T.A.x	= polyTree->points[0].x;	T.B.x	= polyTree->points[2].x;	T.C.x	= polyTree->points[3].x;
				T.A.z	= polyTree->points[0].y;	T.B.z	= polyTree->points[2].y;	T.C.z	= polyTree->points[3].y;
				T.A.y	= sizeY;					T.B.y	= sizeY;					T.C.y	= sizeY;
				T.A.w	= 1.0f;						T.B.w	= 1.0f;						T.C.w	= 1.0f;
				T.At.u	= polyTree->points[0].u;	T.Bt.u	= polyTree->points[2].u;	T.Ct.u	= polyTree->points[3].u;
				T.At.v	= polyTree->points[0].v;	T.Bt.v	= polyTree->points[2].v;	T.Ct.v	= polyTree->points[3].v;

				this->addBackPoly(T);
			}

			break;

		case Front:

			if (this->checkForHole(Front, polyTree, openingFront))
			{
				T.A.x	= polyTree->points[0].x;	T.B.x	= polyTree->points[1].x;	T.C.x	= polyTree->points[2].x;
				T.A.z	= polyTree->points[0].y;	T.B.z	= polyTree->points[1].y;	T.C.z	= polyTree->points[2].y;
				T.A.y	= 0.0f;						T.B.y	= 0.0f;						T.C.y	= 0.0f;
				T.A.w	= 1.0f;						T.B.w	= 1.0f;						T.C.w	= 1.0f;
				T.At.u	= polyTree->points[0].u;	T.Bt.u	= polyTree->points[1].u;	T.Ct.u	= polyTree->points[2].u;
				T.At.v	= polyTree->points[0].v;	T.Bt.v	= polyTree->points[1].v;	T.Ct.v	= polyTree->points[2].v;

				this->addFrontPoly(T);

				T.A.x	= polyTree->points[0].x;	T.B.x	= polyTree->points[2].x;	T.C.x	= polyTree->points[3].x;
				T.A.z	= polyTree->points[0].y;	T.B.z	= polyTree->points[2].y;	T.C.z	= polyTree->points[3].y;
				T.A.y	= 0.0f;						T.B.y	= 0.0f;						T.C.y	= 0.0f;
				T.A.w	= 1.0f;						T.B.w	= 1.0f;						T.C.w	= 1.0f;
				T.At.u	= polyTree->points[0].u;	T.Bt.u	= polyTree->points[2].u;	T.Ct.u	= polyTree->points[3].u;
				T.At.v	= polyTree->points[0].v;	T.Bt.v	= polyTree->points[2].v;	T.Ct.v	= polyTree->points[3].v;

				this->addFrontPoly(T);
			}

			break;

		case Left:

			if (this->checkForHole(Left, polyTree, openingLeft))
			{
				T.A.y	= polyTree->points[0].x;	T.B.y	= polyTree->points[1].x;	T.C.y	= polyTree->points[2].x;
				T.A.z	= polyTree->points[0].y;	T.B.z	= polyTree->points[1].y;	T.C.z	= polyTree->points[2].y;
				T.A.x	= 0.0f;						T.B.x	= 0.0f;						T.C.x	= 0.0f;
				T.A.w	= 1.0f;						T.B.w	= 1.0f;						T.C.w	= 1.0f;
				T.At.u	= polyTree->points[0].u;	T.Bt.u	= polyTree->points[1].u;	T.Ct.u	= polyTree->points[2].u;
				T.At.v	= polyTree->points[0].v;	T.Bt.v	= polyTree->points[1].v;	T.Ct.v	= polyTree->points[2].v;

				this->addLeftPoly(T);

				T.A.y	= polyTree->points[0].x;	T.B.y	= polyTree->points[2].x;	T.C.y	= polyTree->points[3].x;
				T.A.z	= polyTree->points[0].y;	T.B.z	= polyTree->points[2].y;	T.C.z	= polyTree->points[3].y;
				T.A.x	= 0.0f;						T.B.x	= 0.0f;						T.C.x	= 0.0f;
				T.A.w	= 1.0f;						T.B.w	= 1.0f;						T.C.w	= 1.0f;
				T.At.u	= polyTree->points[0].u;	T.Bt.u	= polyTree->points[2].u;	T.Ct.u	= polyTree->points[3].u;
				T.At.v	= polyTree->points[0].v;	T.Bt.v	= polyTree->points[2].v;	T.Ct.v	= polyTree->points[3].v;

				this->addLeftPoly(T);
			}

			break;

		case Right:

			if (this->checkForHole(Right, polyTree, openingRight))
			{
				T.A.y	= polyTree->points[0].x;	T.B.y	= polyTree->points[1].x;	T.C.y	= polyTree->points[2].x;
				T.A.z	= polyTree->points[0].y;	T.B.z	= polyTree->points[1].y;	T.C.z	= polyTree->points[2].y;
				T.A.x	= sizeX;					T.B.x	= sizeX;					T.C.x	= sizeX;
				T.A.w	= 1.0f;						T.B.w	= 1.0f;						T.C.w	= 1.0f;
				T.At.u	= polyTree->points[0].u;	T.Bt.u	= polyTree->points[1].u;	T.Ct.u	= polyTree->points[2].u;
				T.At.v	= polyTree->points[0].v;	T.Bt.v	= polyTree->points[1].v;	T.Ct.v	= polyTree->points[2].v;

				this->addRightPoly(T);

				T.A.y	= polyTree->points[0].x;	T.B.y	= polyTree->points[2].x;	T.C.y	= polyTree->points[3].x;
				T.A.z	= polyTree->points[0].y;	T.B.z	= polyTree->points[2].y;	T.C.z	= polyTree->points[3].y;
				T.A.x	= sizeX;					T.B.x	= sizeX;					T.C.x	= sizeX;
				T.A.w	= 1.0f;						T.B.w	= 1.0f;						T.C.w	= 1.0f;
				T.At.u	= polyTree->points[0].u;	T.Bt.u	= polyTree->points[2].u;	T.Ct.u	= polyTree->points[3].u;
				T.At.v	= polyTree->points[0].v;	T.Bt.v	= polyTree->points[2].v;	T.Ct.v	= polyTree->points[3].v;

				this->addRightPoly(T);
			}

			break;

		}
	}
	else if (polyTree->leftChild != nullptr && polyTree->rightChild != nullptr)
	{
		traversePolygonTree(currentSide, polyTree->leftChild, N, colour, texture);
		traversePolygonTree(currentSide, polyTree->rightChild, N, colour, texture);
	}
}


void Room::calculateMesh()
{
	vect3* p = new vect3[8];

	int vC = 0;

	if (vC < 8)
	{
		p[vC].x = 0.0f;			p[vC].y = 0.0f;			p[vC].z = 0.0f;			p[vC].w = 1.0f;	//Bottom	-	Front	-	Left
		vC++;
	}
	if (vC < 8)
	{
		p[vC].x = sizeX;		p[vC].y = 0.0f;			p[vC].z = 0.0f;			p[vC].w = 1.0f;	//Bottom	-	Front	-	Right
		vC++;
	}
	if (vC < 8)
	{
		p[vC].x = sizeX;		p[vC].y = sizeY;		p[vC].z = 0.0f;			p[vC].w = 1.0f;	//Bottom	-	Back	-	Right
		vC++;
	}
	if (vC < 8)
	{
		p[vC].x = 0.0f;			p[vC].y = sizeY;		p[vC].z = 0.0f;			p[vC].w = 1.0f;	//Bottom	-	Back	-	Left
		vC++;
	}
	if (vC < 8)
	{
		p[vC].x = 0.0f;			p[vC].y = 0.0f;			p[vC].z = sizeZ;		p[vC].w = 1.0f;	//Top		-	Front	-	Left
		vC++;
	}
	if (vC < 8)
	{
		p[vC].x = sizeX;		p[vC].y = 0.0f;			p[vC].z = sizeZ;		p[vC].w = 1.0f;	//Top		-	Front	-	Right
		vC++;
	}
	if (vC < 8)
	{
		p[vC].x = sizeX;		p[vC].y = sizeY;		p[vC].z = sizeZ;		p[vC].w = 1.0f;	//Top		-	Back	-	Right
		vC++;
	}
	if (vC < 8)
	{
		p[vC].x = 0.0f;			p[vC].y = sizeY;		p[vC].z = sizeZ;		p[vC].w = 1.0f;	//Top		-	Back	-	Left
		vC++;
	}

	triangle3dV T;
	polygon4uv currentPoly;
	vect2* pPoly = new vect2[4];
	polyNode* pHole = new polyNode[4];
	vect3 normal;
	for (int i = 0; i < 4; i++)
	{
		pPoly[i] = { 0.0f, 0.0f, 0.0f, 0.0f };
		pHole[i] = { 0.0f, 0.0f, 0 };
	}

	if (isBottomOn)
	{
		normal = { 0.0f,  0.0f,  1.0f,  0.0f };

		if (openingOnBottom)
		{
			pPoly[0].x = p[0].x;	pPoly[0].y = p[0].y;	pPoly[0].u = 0.0f;					pPoly[0].v = 0.0f;
			pPoly[1].x = p[1].x;	pPoly[1].y = p[1].y;	pPoly[1].u = sizeX / txUBottom;		pPoly[1].v = 0.0f;
			pPoly[2].x = p[2].x;	pPoly[2].y = p[2].y;	pPoly[2].u = sizeX / txUBottom;		pPoly[2].v = sizeY / txUBottom;
			pPoly[3].x = p[3].x;	pPoly[3].y = p[3].y;	pPoly[3].u = 0.0f;					pPoly[3].v = sizeY / txUBottom;

			currentPoly.points[0]	= pPoly[0];
			currentPoly.points[1]	= pPoly[1];
			currentPoly.points[2]	= pPoly[2];
			currentPoly.points[3]	= pPoly[3];
			currentPoly.leftChild	= nullptr;
			currentPoly.rightChild	= nullptr;

			if (openingBottom != nullptr)
			{
				pHole[0].x = openingBottom[0].x;	pHole[0].y = openingBottom[0].y;
				pHole[1].x = openingBottom[1].x;	pHole[1].y = openingBottom[1].y;
				pHole[2].x = openingBottom[2].x;	pHole[2].y = openingBottom[2].y;
				pHole[3].x = openingBottom[3].x;	pHole[3].y = openingBottom[3].y;
			}

			this->breakUpFace(Bottom, &currentPoly, pHole, normal, colBottom, txtBottom);
		}
		else
		{

			T.A = p[0];	T.B = p[3];	T.C = p[1];		T.An = T.Bn = T.Cn = T.N = normal;

			T.At.u = 0.0f;					T.At.v = sizeY / txUBottom;
			T.Bt.u = 0.0f;					T.Bt.v = 0.0f;
			T.Ct.u = sizeX / txUBottom;		T.Ct.v = sizeY / txUBottom;

			T.texture = txtBottom;
			T.colour = colBottom;

			this->addBottomPoly(T);

			T.A = p[1];	T.B = p[3];	T.C = p[2];		T.An = T.Bn = T.Cn = T.N = normal;

			T.At.u = sizeX / txUBottom;		T.At.v = sizeY / txUBottom;
			T.Bt.u = 0.0f;					T.Bt.v = 0.0f;
			T.Ct.u = sizeX / txUBottom;		T.Ct.v = 0.0f;

			this->addBottomPoly(T);
		}
	}

	if (isFrontOn)
	{
		normal = { 0.0f,  1.0f,  0.0f,  0.0f };

		if (openingOnFront)
		{
			pPoly[0].x = p[1].x;	pPoly[0].y = p[1].z;	pPoly[0].u = 0.0f;					pPoly[0].v = 0.0f;
			pPoly[1].x = p[0].x;	pPoly[1].y = p[0].z;	pPoly[1].u = sizeX / txUFront;		pPoly[1].v = 0.0f;
			pPoly[2].x = p[4].x;	pPoly[2].y = p[4].z;	pPoly[2].u = sizeX / txUFront;		pPoly[2].v = sizeZ / txUFront;
			pPoly[3].x = p[5].x;	pPoly[3].y = p[5].z;	pPoly[3].u = 0.0f;					pPoly[3].v = sizeZ / txUFront;

			currentPoly.points[0]	= pPoly[0];
			currentPoly.points[1]	= pPoly[1];
			currentPoly.points[2]	= pPoly[2];
			currentPoly.points[3]	= pPoly[3];
			currentPoly.leftChild	= nullptr;
			currentPoly.rightChild	= nullptr;

			if (openingFront != nullptr)
			{
				pHole[0].x = openingFront[0].x;		pHole[0].y = openingFront[0].z;
				pHole[1].x = openingFront[1].x;		pHole[1].y = openingFront[1].z;
				pHole[2].x = openingFront[2].x;		pHole[2].y = openingFront[2].z;
				pHole[3].x = openingFront[3].x;		pHole[3].y = openingFront[3].z;
			}

			this->breakUpFace(Front, &currentPoly, pHole, normal, colFront, txtFront);
		}
		else
		{
			T.A = p[0];	T.B = p[5];	T.C = p[4];		T.An = T.Bn = T.Cn = T.N = normal;

			T.At.u = 0.0f;					T.At.v = 0.0f;
			T.Bt.u = sizeX / txUFront;		T.Bt.v = sizeZ / txUFront;
			T.Ct.u = 0.0f;					T.Ct.v = sizeZ / txUFront;

			T.texture = txtFront;
			T.colour = colFront;

			this->addFrontPoly(T);

			T.A = p[0];	T.B = p[1];	T.C = p[5];		T.An = T.Bn = T.Cn = T.N = normal;

			T.At.u = 0.0f;					T.At.v = 0.0f;
			T.Bt.u = sizeX / txUFront;		T.Bt.v = 0.0f;
			T.Ct.u = sizeX / txUFront;		T.Ct.v = sizeZ / txUFront;

			this->addFrontPoly(T);
		}
	}

	if (isBackOn)
	{
		normal = { 0.0f, -1.0f,  0.0f,  0.0f };

		if (openingOnBack)
		{
			pPoly[0].x = p[3].x;	pPoly[0].y = p[3].z;	pPoly[0].u = 0.0f;					pPoly[0].v = 0.0f;
			pPoly[1].x = p[2].x;	pPoly[1].y = p[2].z;	pPoly[1].u = sizeX / txUBack;		pPoly[1].v = 0.0f;
			pPoly[2].x = p[6].x;	pPoly[2].y = p[6].z;	pPoly[2].u = sizeX / txUBack;		pPoly[2].v = sizeZ / txUBack;
			pPoly[3].x = p[7].x;	pPoly[3].y = p[7].z;	pPoly[3].u = 0.0f;					pPoly[3].v = sizeZ / txUBack;

			currentPoly.points[0]	= pPoly[0];
			currentPoly.points[1]	= pPoly[1];
			currentPoly.points[2]	= pPoly[2];
			currentPoly.points[3]	= pPoly[3];
			currentPoly.leftChild	= nullptr;
			currentPoly.rightChild	= nullptr;

			if (openingBack != nullptr)
			{
				pHole[0].x = openingBack[0].x;		pHole[0].y = openingBack[0].z;
				pHole[1].x = openingBack[1].x;		pHole[1].y = openingBack[1].z;
				pHole[2].x = openingBack[2].x;		pHole[2].y = openingBack[2].z;
				pHole[3].x = openingBack[3].x;		pHole[3].y = openingBack[3].z;
			}

			this->breakUpFace(Back, &currentPoly, pHole, normal, colBack, txtBack);
		}
		else
		{
			T.A = p[3];	T.B = p[7];	T.C = p[6];		T.An = T.Bn = T.Cn = T.N = normal;

			T.At.u = sizeX / txUBack;		T.At.v = 0.0f;
			T.Bt.u = sizeX / txUBack;		T.Bt.v = sizeZ / txUBack;
			T.Ct.u = 0.0f;					T.Ct.v = sizeZ / txUBack;

			T.texture = txtBack;
			T.colour = colBack;

			this->addBackPoly(T);

			T.A = p[3];	T.B = p[6];	T.C = p[2];		T.An = T.Bn = T.Cn = T.N = normal;

			T.At.u = sizeX / txUBack;		T.At.v = 0.0f;
			T.Bt.u = 0.0f;					T.Bt.v = sizeZ / txUBack;
			T.Ct.u = 0.0f;					T.Ct.v = 0.0f;

			this->addBackPoly(T);
		}
	}

	if (isTopOn)
	{
		normal = { 0.0f, 0.0f, -1.0f, 0.0f };

		if (openingOnTop)
		{
			pPoly[0].x = p[7].x;	pPoly[0].y = p[7].y;	pPoly[0].u = 0.0f;					pPoly[0].v = 0.0f;
			pPoly[1].x = p[6].x;	pPoly[1].y = p[6].y;	pPoly[1].u = sizeX / txUTop;		pPoly[1].v = 0.0f;
			pPoly[2].x = p[5].x;	pPoly[2].y = p[5].y;	pPoly[2].u = sizeX / txUTop;		pPoly[2].v = sizeY / txUTop;
			pPoly[3].x = p[4].x;	pPoly[3].y = p[4].y;	pPoly[3].u = 0.0f;					pPoly[3].v = sizeY / txUTop;

			currentPoly.points[0]	= pPoly[0];
			currentPoly.points[1]	= pPoly[1];
			currentPoly.points[2]	= pPoly[2];
			currentPoly.points[3]	= pPoly[3];
			currentPoly.leftChild	= nullptr;
			currentPoly.rightChild	= nullptr;

			if (openingTop != nullptr)
			{
				pHole[0].x = openingTop[0].x;		pHole[0].y = openingTop[0].y;
				pHole[1].x = openingTop[1].x;		pHole[1].y = openingTop[1].y;
				pHole[2].x = openingTop[2].x;		pHole[2].y = openingTop[2].y;
				pHole[3].x = openingTop[3].x;		pHole[3].y = openingTop[3].y;
			}

			this->breakUpFace(Top, &currentPoly, pHole, normal, colTop, txtTop);
		}
		else
		{
			T.A = p[4];	T.B = p[5];	T.C = p[7];		T.An = T.Bn = T.Cn = T.N = normal;

			T.At.u = 0.0f;					T.At.v = 0.0f;
			T.Bt.u = sizeX / txUTop;		T.Bt.v = 0.0f;
			T.Ct.u = 0.0f;					T.Ct.v = sizeY / txUTop;

			T.texture = txtTop;
			T.colour = colTop;

			this->addTopPoly(T);

			T.A = p[5];	T.B = p[6];	T.C = p[7];		T.An = T.Bn = T.Cn = T.N = normal;

			T.At.u = sizeX / txUTop;		T.At.v = 0.0f;
			T.Bt.u = sizeX / txUTop;		T.Bt.v = sizeY / txUTop;
			T.Ct.u = 0.0f;					T.Ct.v = sizeY / txUTop;

			this->addTopPoly(T);
		}
	}

	if (isLeftOn)
	{
		normal = { 1.0f,  0.0f,  0.0f,  0.0f };

		if (openingOnLeft)
		{
			pPoly[0].x = p[0].y;	pPoly[0].y = p[0].z;	pPoly[0].u = 0.0f;					pPoly[0].v = 0.0f;
			pPoly[1].x = p[3].y;	pPoly[1].y = p[3].z;	pPoly[1].u = sizeY / txULeft;		pPoly[1].v = 0.0f;
			pPoly[2].x = p[7].y;	pPoly[2].y = p[7].z;	pPoly[2].u = sizeY / txULeft;		pPoly[2].v = sizeZ / txULeft;
			pPoly[3].x = p[4].y;	pPoly[3].y = p[4].z;	pPoly[3].u = 0.0f;					pPoly[3].v = sizeZ / txULeft;

			currentPoly.points[0]	= pPoly[0];
			currentPoly.points[1]	= pPoly[1];
			currentPoly.points[2]	= pPoly[2];
			currentPoly.points[3]	= pPoly[3];
			currentPoly.leftChild	= nullptr;
			currentPoly.rightChild	= nullptr;

			if (openingLeft != nullptr)
			{
				pHole[0].x = openingLeft[0].y;		pHole[0].y = openingLeft[0].z;
				pHole[1].x = openingLeft[1].y;		pHole[1].y = openingLeft[1].z;
				pHole[2].x = openingLeft[2].y;		pHole[2].y = openingLeft[2].z;
				pHole[3].x = openingLeft[3].y;		pHole[3].y = openingLeft[3].z;
			}

			this->breakUpFace(Left, &currentPoly, pHole, normal, colLeft, txtLeft);
		}
		else
		{
			T.A = p[0];	T.B = p[7];	T.C = p[3];		T.An = T.Bn = T.Cn = T.N = normal;

			T.At.u = sizeY / txULeft;		T.At.v = 0.0f;
			T.Bt.u = 0.0f;					T.Bt.v = sizeZ / txULeft;
			T.Ct.u = 0.0f;					T.Ct.v = 0.0f;

			T.texture = txtLeft;
			T.colour = colLeft;

			this->addLeftPoly(T);

			T.A = p[0];	T.B = p[4];	T.C = p[7];		T.An = T.Bn = T.Cn = T.N = normal;

			T.At.u = sizeY / txULeft;		T.At.v = 0.0f;
			T.Bt.u = sizeY / txULeft;		T.Bt.v = sizeZ / txULeft;
			T.Ct.u = 0.0f;					T.Ct.v = sizeZ / txULeft;

			this->addLeftPoly(T);
		}
	}

	if (isRightOn)
	{
		normal = { -1.0f,  0.0f,  0.0f,  0.0f };

		if (openingOnRight)
		{
			pPoly[0].x = p[2].y;	pPoly[0].y = p[2].z;	pPoly[0].u = 0.0f;					pPoly[0].v = 0.0f;
			pPoly[1].x = p[1].y;	pPoly[1].y = p[1].z;	pPoly[1].u = sizeY / txURight;		pPoly[1].v = 0.0f;
			pPoly[2].x = p[5].y;	pPoly[2].y = p[5].z;	pPoly[2].u = sizeY / txURight;		pPoly[2].v = sizeZ / txURight;
			pPoly[3].x = p[6].y;	pPoly[3].y = p[6].z;	pPoly[3].u = 0.0f;					pPoly[3].v = sizeZ / txURight;

			currentPoly.points[0]	= pPoly[0];
			currentPoly.points[1]	= pPoly[1];
			currentPoly.points[2]	= pPoly[2];
			currentPoly.points[3]	= pPoly[3];
			currentPoly.leftChild	= nullptr;
			currentPoly.rightChild	= nullptr;

			if (openingRight != nullptr)
			{
				pHole[0].x = openingRight[0].y;		pHole[0].y = openingRight[0].z;
				pHole[1].x = openingRight[1].y;		pHole[1].y = openingRight[1].z;
				pHole[2].x = openingRight[2].y;		pHole[2].y = openingRight[2].z;
				pHole[3].x = openingRight[3].y;		pHole[3].y = openingRight[3].z;
			}	

			this->breakUpFace(Right, &currentPoly, pHole, normal, colRight, txtRight);
		}
		else
		{
			T.A = p[1];	T.B = p[6];	T.C = p[5];		T.An = T.Bn = T.Cn = T.N = normal;

			T.At.u = 0.0f;					T.At.v = 0.0f;
			T.Bt.u = sizeY / txURight;		T.Bt.v = sizeZ / txURight;
			T.Ct.u = 0.0f;					T.Ct.v = sizeZ / txURight;

			T.texture = txtRight;
			T.colour = colRight;

			this->addRightPoly(T);
			
			T.A = p[1];	T.B = p[2];	T.C = p[6];		T.An = T.Bn = T.Cn = T.N = normal;

			T.At.u = 0.0f;					T.At.v = 0.0f;
			T.Bt.u = sizeY / txURight;		T.Bt.v = 0.0f;
			T.Ct.u = sizeY / txURight;		T.Ct.v = sizeZ / txURight;

			this->addRightPoly(T);
		}
	}


	delete[] p;
	delete[] pPoly, pHole;

	nPoly = getTotalPoly();
	mesh = new triangle3dV[nPoly];
	getTriangleData(mesh);
}


void Room::getTriangleData(triangle3dV* t)
{
	int tCount = this->getTotalPoly();

	int tC = 0;

	for (unsigned int i = 0; i < bottomPolyContainer.size(); i++, tC++)
	{
		if (tC < tCount)
		{
			t[tC] = bottomPolyContainer[i];
			t[tC].A.x += position.x;	t[tC].A.y += position.y;	t[tC].A.z += position.z;
			t[tC].B.x += position.x;	t[tC].B.y += position.y;	t[tC].B.z += position.z;
			t[tC].C.x += position.x;	t[tC].C.y += position.y;	t[tC].C.z += position.z;
		}
	}

	for (unsigned int i = 0; i < topPolyContainer.size(); i++, tC++)
	{
		if (tC < tCount)
		{
			t[tC] = topPolyContainer[i];
			t[tC].A.x += position.x;	t[tC].A.y += position.y;	t[tC].A.z += position.z;
			t[tC].B.x += position.x;	t[tC].B.y += position.y;	t[tC].B.z += position.z;
			t[tC].C.x += position.x;	t[tC].C.y += position.y;	t[tC].C.z += position.z;
		}
	}

	for (unsigned int i = 0; i < backPolyContainer.size(); i++, tC++)
	{
		if (tC < tCount)
		{
			t[tC] = backPolyContainer[i];
			t[tC].A.x += position.x;	t[tC].A.y += position.y;	t[tC].A.z += position.z;
			t[tC].B.x += position.x;	t[tC].B.y += position.y;	t[tC].B.z += position.z;
			t[tC].C.x += position.x;	t[tC].C.y += position.y;	t[tC].C.z += position.z;
		}
	}

	for (unsigned int i = 0; i < frontPolyContainer.size(); i++, tC++)
	{
		if (tC < tCount)
		{
			t[tC] = frontPolyContainer[i];
			t[tC].A.x += position.x;	t[tC].A.y += position.y;	t[tC].A.z += position.z;
			t[tC].B.x += position.x;	t[tC].B.y += position.y;	t[tC].B.z += position.z;
			t[tC].C.x += position.x;	t[tC].C.y += position.y;	t[tC].C.z += position.z;
		}
	}

	for (unsigned int i = 0; i < leftPolyContainer.size(); i++, tC++)
	{
		if (tC < tCount)
		{
			t[tC] = leftPolyContainer[i];
			t[tC].A.x += position.x;	t[tC].A.y += position.y;	t[tC].A.z += position.z;
			t[tC].B.x += position.x;	t[tC].B.y += position.y;	t[tC].B.z += position.z;
			t[tC].C.x += position.x;	t[tC].C.y += position.y;	t[tC].C.z += position.z;
		}
	}

	for (unsigned int i = 0; i < rightPolyContainer.size(); i++, tC++)
	{
		if (tC < tCount)
		{
			t[tC] = rightPolyContainer[i];
			t[tC].A.x += position.x;	t[tC].A.y += position.y;	t[tC].A.z += position.z;
			t[tC].B.x += position.x;	t[tC].B.y += position.y;	t[tC].B.z += position.z;
			t[tC].C.x += position.x;	t[tC].C.y += position.y;	t[tC].C.z += position.z;
		}
	}

	nPoly = tCount;

	Projector->transformMesh(nPoly, t, scale.x, scale.y, scale.z,
										position.x, position.y, position.z,
										rotation.x, rotation.y, rotation.z);
}