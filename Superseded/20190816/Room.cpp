#include "Room.h"


Room::Room()
{
	posX		= 0.0;
	posY		= 0.0;
	posZ		= 0.0;

	sizeX		= 5.0;
	sizeY		= 4.0;
	sizeZ		= 3.0;

	rotX		= 0.0;
	rotY		= 0.0;
	rotZ		= 0.0;

	colBottom	= 255;
	colTop		= 255;
	colBack		= 255;
	colFront	= 255;
	colLeft		= 255;
	colRight	= 255;

	txtBottom	= 0;
	txtTop		= 0;
	txtBack		= 0;
	txtFront	= 0;
	txtLeft		= 0;
	txtRight	= 0;

	txUBottom	= 1.0;
	txUTop		= 1.0;
	txUBack		= 1.0;
	txUFront	= 1.0;
	txULeft		= 1.0;
	txURight	= 1.0;

	isBottomOn	= true;
	isTopOn		= true;
	isBackOn	= true;
	isFrontOn	= true;
	isLeftOn	= true;
	isRightOn	= true;

	openingOnBottom		= false;
	openingOnTop		= false;
	openingOnBack		= false;
	openingOnFront		= false;
	openingOnLeft		= false;
	openingOnRight		= false;

	openingBottom		= NULL;
	openingTop			= NULL;
	openingBack			= NULL;
	openingFront		= NULL;
	openingLeft			= NULL;
	openingRight		= NULL;
}


Room::Room(double pX, double pY, double pZ, double sX, double sY, double sZ, double rX, double rY, double rZ, Uint32* col, int* txt)
{
	posX		= pX;
	posY		= pY;
	posZ		= pZ;

	sizeX		= sX;
	sizeY		= sY;
	sizeZ		= sZ;

	rotX		= rX;
	rotY		= rY;
	rotZ		= rZ;

	colBottom	= 255;
	colTop		= 255;
	colBack		= 255;
	colFront	= 255;
	colLeft		= 255;
	colRight	= 255;

	txtBottom	= 0;
	txtTop		= 0;
	txtBack		= 0;
	txtFront	= 0;
	txtLeft		= 0;
	txtRight	= 0;

	txUBottom	= 1.0;
	txUTop		= 1.0;
	txUBack		= 1.0;
	txUFront	= 1.0;
	txULeft		= 1.0;
	txURight	= 1.0;

	isBottomOn	= true;
	isTopOn		= true;
	isBackOn	= true;
	isFrontOn	= true;
	isLeftOn	= true;
	isRightOn	= true;

	openingOnBottom		= false;
	openingOnTop		= false;
	openingOnBack		= false;
	openingOnFront		= false;
	openingOnLeft		= false;
	openingOnRight		= false;

	openingBottom		= NULL;
	openingTop			= NULL;
	openingBack			= NULL;
	openingFront		= NULL;
	openingLeft			= NULL;
	openingRight		= NULL;
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


void Room::setPosition(double  x, double y, double z)
{
	posX		= x;
	posY		= y;
	posZ		= z;
}


void Room::setDimension(double x, double y, double z)
{
	sizeX		= x;
	sizeY		= y;
	sizeZ		= z;
}


void Room::setRotation(double x, double y, double z)
{
	rotX		= x;
	rotY		= y;
	rotZ		= z;
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


void Room::setTextureScale(double bottom, double top, double back, double front, double left, double right)
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


void Room::addOpening(Side currentSide, double bottomLeftX, double bottomLeftY, double width, double height)
{
	switch (currentSide)
	{
	case Bottom:
		openingOnBottom = true;
		openingBottom = new vect3[4];
		if (bottomLeftX < 0.0) { bottomLeftX = 0.0; }
		if (bottomLeftY < 0.0) { bottomLeftY = 0.0; }
		if (bottomLeftX >= sizeX) { bottomLeftX = sizeX * 0.25; }
		if (bottomLeftY >= sizeY) { bottomLeftY = sizeY * 0.25; }
		if (bottomLeftX + width > sizeX) { width = sizeX - bottomLeftX; }
		if (bottomLeftY + height > sizeY) { height = sizeY - bottomLeftY; }
		openingBottom[0].x = bottomLeftX;
		openingBottom[0].y = bottomLeftY;
		openingBottom[0].z = 0.0;
		openingBottom[0].w = 1.0;
		openingBottom[1].x = bottomLeftX + width;
		openingBottom[1].y = bottomLeftY;
		openingBottom[1].z = 0.0;
		openingBottom[1].w = 1.0;
		openingBottom[2].x = bottomLeftX + width;
		openingBottom[2].y = bottomLeftY + height;
		openingBottom[2].z = 0.0;
		openingBottom[2].w = 1.0;
		openingBottom[3].x = bottomLeftX;
		openingBottom[3].y = bottomLeftY + height;
		openingBottom[3].z = 0.0;
		openingBottom[3].w = 1.0;
		break;
	case Top:
		openingOnTop = true;
		openingTop = new vect3[4];
		if (bottomLeftX < 0.0) { bottomLeftX = 0.0; }
		if (bottomLeftY < 0.0) { bottomLeftY = 0.0; }
		if (bottomLeftX >= sizeX) { bottomLeftX = sizeX * 0.25; }
		if (bottomLeftY >= sizeY) { bottomLeftY = sizeY * 0.25; }
		if (bottomLeftX + width > sizeX) { width = sizeX - bottomLeftX; }
		if (bottomLeftY + height > sizeY) { height = sizeY - bottomLeftY; }
		openingTop[0].x = bottomLeftX;
		openingTop[0].y = bottomLeftY;
		openingTop[0].z = sizeZ;
		openingTop[0].w = 1.0;
		openingTop[1].x = bottomLeftX + width;
		openingTop[1].y = bottomLeftY;
		openingTop[1].z = sizeZ;
		openingTop[1].w = 1.0;
		openingTop[2].x = bottomLeftX + width;
		openingTop[2].y = bottomLeftY + height;
		openingTop[2].z = sizeZ;
		openingTop[2].w = 1.0;
		openingTop[3].x = bottomLeftX;
		openingTop[3].y = bottomLeftY + height;
		openingTop[3].z = sizeZ;
		openingTop[3].w = 1.0;
		break;
	case Back:
		openingOnBack = true;
		openingBack = new vect3[4];
		if (bottomLeftX < 0.0) { bottomLeftX = 0.0; }
		if (bottomLeftY < 0.0) { bottomLeftY = 0.0; }
		if (bottomLeftX >= sizeX) { bottomLeftX = sizeX * 0.25; }
		if (bottomLeftY >= sizeZ) { bottomLeftY = sizeZ * 0.25; }
		if (bottomLeftX + width > sizeX) { width = sizeX - bottomLeftX; }
		if (bottomLeftY + height > sizeZ) { height = sizeZ - bottomLeftY; }
		openingBack[0].x = bottomLeftX;
		openingBack[0].y = sizeY;
		openingBack[0].z = bottomLeftY;
		openingBack[0].w = 1.0;
		openingBack[1].x = bottomLeftX + width;
		openingBack[1].y = sizeY;
		openingBack[1].z = bottomLeftY;
		openingBack[1].w = 1.0;
		openingBack[2].x = bottomLeftX + width;
		openingBack[2].y = sizeY;
		openingBack[2].z = bottomLeftY + height;
		openingBack[2].w = 1.0;
		openingBack[3].x = bottomLeftX;
		openingBack[3].y = sizeY;
		openingBack[3].z = bottomLeftY + height;
		openingBack[3].w = 1.0;
		break;
	case Front:
		openingOnFront = true;
		openingFront = new vect3[4];
		if (bottomLeftX < 0.0) { bottomLeftX = 0.0; }
		if (bottomLeftY < 0.0) { bottomLeftY = 0.0; }
		if (bottomLeftX >= sizeX) { bottomLeftX = sizeX * 0.25; }
		if (bottomLeftY >= sizeZ) { bottomLeftY = sizeZ * 0.25; }
		if (bottomLeftX + width > sizeX) { width = sizeX - bottomLeftX; }
		if (bottomLeftY + height > sizeZ) { height = sizeZ - bottomLeftY; }
		openingFront[0].x = bottomLeftX;
		openingFront[0].y = 0.0;
		openingFront[0].z = bottomLeftY;
		openingFront[0].w = 1.0;
		openingFront[1].x = bottomLeftX + width;
		openingFront[1].y = 0.0;
		openingFront[1].z = bottomLeftY;
		openingFront[1].w = 1.0;
		openingFront[2].x = bottomLeftX + width;
		openingFront[2].y = 0.0;
		openingFront[2].z = bottomLeftY + height;
		openingFront[2].w = 1.0;
		openingFront[3].x = bottomLeftX;
		openingFront[3].y = 0.0;
		openingFront[3].z = bottomLeftY + height;
		openingFront[3].w = 1.0;
		break;
	case Left:
		openingOnLeft = true;
		openingLeft = new vect3[4];
		if (bottomLeftX < 0.0) { bottomLeftX = 0.0; }
		if (bottomLeftY < 0.0) { bottomLeftY = 0.0; }
		if (bottomLeftX >= sizeY) { bottomLeftX = sizeY * 0.25; }
		if (bottomLeftY >= sizeZ) { bottomLeftY = sizeZ * 0.25; }
		if (bottomLeftX + width > sizeY) { width = sizeY - bottomLeftX; }
		if (bottomLeftY + height > sizeZ) { height = sizeZ - bottomLeftY; }
		openingLeft[0].x = 0.0;
		openingLeft[0].y = bottomLeftX;
		openingLeft[0].z = bottomLeftY;
		openingLeft[0].w = 1.0;
		openingLeft[1].x = 0.0;
		openingLeft[1].y = bottomLeftX + width;
		openingLeft[1].z = bottomLeftY;
		openingLeft[1].w = 1.0;
		openingLeft[2].x = 0.0;
		openingLeft[2].y = bottomLeftX + width;
		openingLeft[2].z = bottomLeftY + height;
		openingLeft[2].w = 1.0;
		openingLeft[3].x = 0.0;
		openingLeft[3].y = bottomLeftX;
		openingLeft[3].z = bottomLeftY + height;
		openingLeft[3].w = 1.0;
		break;
	case Right:
		openingOnRight = true;
		openingRight = new vect3[4];
		if (bottomLeftX < 0.0) { bottomLeftX = 0.0; }
		if (bottomLeftY < 0.0) { bottomLeftY = 0.0; }
		if (bottomLeftX >= sizeY) { bottomLeftX = sizeY * 0.25; }
		if (bottomLeftY >= sizeZ) { bottomLeftY = sizeZ * 0.25; }
		if (bottomLeftX + width > sizeY) { width = sizeY - bottomLeftX; }
		if (bottomLeftY + height > sizeZ) { height = sizeZ - bottomLeftY; }
		openingRight[0].x = sizeX;
		openingRight[0].y = bottomLeftX;
		openingRight[0].z = bottomLeftY;
		openingRight[0].w = 1.0;
		openingRight[1].x = sizeX;
		openingRight[1].y = bottomLeftX + width;
		openingRight[1].z = bottomLeftY;
		openingRight[1].w = 1.0;
		openingRight[2].x = sizeX;
		openingRight[2].y = bottomLeftX + width;
		openingRight[2].z = bottomLeftY + height;
		openingRight[2].w = 1.0;
		openingRight[3].x = sizeX;
		openingRight[3].y = bottomLeftX;
		openingRight[3].z = bottomLeftY + height;
		openingRight[3].w = 1.0;
		break;
	}

}


int Room::getTotalVert()
{
	return 8;
}


int Room::getTotalPoly()
{
	int polyCount = 0;
	polyCount = isBottomOn * 2 + isTopOn * 2 + isBackOn * 2 + isFrontOn * 2 + isLeftOn * 2 + isRightOn * 2;

	if (isBottomOn)
	{
		if (openingOnBottom)
		{
			/*if (openingBottom[0].x > 0.0 && openingBottom[1].x < sizeX)
			{

			}*/
			//polyCount += 2;
		}
		else
		{
			//polyCount += 2;
		}
	}


	return polyCount;
}


triangle3dV* Room::getTriangleData()
{
	int vCount = this->getTotalVert();
	int polyCount = this->getTotalPoly();
	vect3* p = new vect3[vCount];
	triangle3dV* t = new triangle3dV[polyCount];

	/*p[0].x = 0;			p[0].y = 0;			p[0].z = 0;			p[0].w = 1.0;	//Bottom	-	Front	-	Left
	p[1].x = sizeX;		p[1].y = 0;			p[1].z = 0;			p[1].w = 1.0;	//Bottom	-	Front	-	Right
	p[2].x = sizeX;		p[2].y = sizeY;		p[2].z = 0;			p[2].w = 1.0;	//Bottom	-	Back	-	Right
	p[3].x = 0;			p[3].y = sizeY;		p[3].z = 0;			p[3].w = 1.0;	//Bottom	-	Back	-	Left
	p[4].x = 0;			p[4].y = 0;			p[4].z = sizeZ;		p[4].w = 1.0;	//Top		-	Front	-	Left
	p[5].x = sizeX;		p[5].y = 0;			p[5].z = sizeZ;		p[5].w = 1.0;	//Top		-	Front	-	Right
	p[6].x = sizeX;		p[6].y = sizeY;		p[6].z = sizeZ;		p[6].w = 1.0;	//Top		-	Back	-	Right
	p[7].x = 0;			p[7].y = sizeY;		p[7].z = sizeZ;		p[7].w = 1.0;	//Top		-	Back	-	Left*/

	int vC = 0;

	if (vC < vCount)
	{
		p[vC].x = 0;			p[vC].y = 0;			p[vC].z = 0;			p[vC].w = 1.0;	//Bottom	-	Front	-	Left
		vC++;
	}
	if (vC < vCount)
	{
		p[vC].x = sizeX;		p[vC].y = 0;			p[vC].z = 0;			p[vC].w = 1.0;	//Bottom	-	Front	-	Right
		vC++;
	}
	if (vC < vCount)
	{
		p[vC].x = sizeX;		p[vC].y = sizeY;		p[vC].z = 0;			p[vC].w = 1.0;	//Bottom	-	Back	-	Right
		vC++;
	}
	if (vC < vCount)
	{
		p[vC].x = 0;			p[vC].y = sizeY;		p[vC].z = 0;			p[vC].w = 1.0;	//Bottom	-	Back	-	Left
		vC++;
	}
	if (vC < vCount)
	{
		p[vC].x = 0;			p[vC].y = 0;			p[vC].z = sizeZ;		p[vC].w = 1.0;	//Top		-	Front	-	Left
		vC++;
	}
	if (vC < vCount)
	{
		p[vC].x = sizeX;		p[vC].y = 0;			p[vC].z = sizeZ;		p[vC].w = 1.0;	//Top		-	Front	-	Right
		vC++;
	}
	if (vC < vCount)
	{
		p[vC].x = sizeX;		p[vC].y = sizeY;		p[vC].z = sizeZ;		p[vC].w = 1.0;	//Top		-	Back	-	Right
		vC++;
	}
	if (vC < vCount)
	{
		p[vC].x = 0;			p[vC].y = sizeY;		p[vC].z = sizeZ;		p[vC].w = 1.0;	//Top		-	Back	-	Left
		vC++;
	}
	
	

	int tC = 0;

	if (openingOnBottom)
	{

	}
	else
	{
		t[0].A = p[0];	t[0].B = p[3];	t[0].C = p[1];		t[0].An = t[0].Bn = t[0].Cn = t[0].N = { 0.0,  0.0,  1.0,  0.0 };		//BOTTOM
		t[1].A = p[1];	t[1].B = p[3];	t[1].C = p[2];		t[1].An = t[1].Bn = t[1].Cn = t[1].N = { 0.0,  0.0,  1.0,  0.0 };
	}

	if (openingOnTop)
	{

	}
	else
	{
		t[6].A = p[4];	t[6].B = p[5];	t[6].C = p[7];		t[6].An = t[6].Bn = t[6].Cn = t[6].N = { 0.0,  0.0, -1.0,  0.0 };		//TOP
		t[7].A = p[5];	t[7].B = p[6];	t[7].C = p[7];		t[7].An = t[7].Bn = t[7].Cn = t[7].N = { 0.0,  0.0, -1.0,  0.0 };
	}

	if (openingOnBack)
	{

	}
	else
	{
		t[4].A = p[3];	t[4].B = p[7];	t[4].C = p[6];		t[4].An = t[4].Bn = t[4].Cn = t[4].N = { 0.0, -1.0,  0.0,  0.0 };		//BACK
		t[5].A = p[3];	t[5].B = p[6];	t[5].C = p[2];		t[5].An = t[5].Bn = t[5].Cn = t[5].N = { 0.0, -1.0,  0.0,  0.0 };
	}

	if (openingOnFront)
	{

	}
	else
	{
		t[2].A = p[0];	t[2].B = p[5];	t[2].C = p[4];		t[2].An = t[2].Bn = t[2].Cn = t[2].N = { 0.0,  1.0,  0.0,  0.0 };		//FRONT
		t[3].A = p[0];	t[3].B = p[1];	t[3].C = p[5];		t[3].An = t[3].Bn = t[3].Cn = t[3].N = { 0.0,  1.0,  0.0,  0.0 };
	}

	if (openingOnLeft)
	{

	}
	else
	{
		t[8].A = p[0];	t[8].B = p[7];	t[8].C = p[3];		t[8].An = t[8].Bn = t[8].Cn = t[8].N = { 1.0,  0.0,  0.0,  0.0 };		//LEFT
		t[9].A = p[0];	t[9].B = p[4];	t[9].C = p[7];		t[9].An = t[9].Bn = t[9].Cn = t[9].N = { 1.0,  0.0,  0.0,  0.0 };
	}

	if (openingOnRight)
	{

	}
	else
	{
		t[10].A = p[1];	t[10].B = p[6];	t[10].C = p[5];		t[10].An = t[10].Bn = t[10].Cn = t[10].N = { -1.0,  0.0,  0.0,  0.0 };	//RIGHT
		t[11].A = p[1];	t[11].B = p[2];	t[11].C = p[6];		t[11].An = t[11].Bn = t[11].Cn = t[11].N = { -1.0,  0.0,  0.0,  0.0 };
	}


	t[0].At.u = 0.0;					t[0].At.v = sizeY / txUBottom;		//BOTTOM
	t[0].Bt.u = 0.0;					t[0].Bt.v = 0.0;
	t[0].Ct.u = sizeX / txUBottom;		t[0].Ct.v = sizeY / txUBottom;

	t[1].At.u = sizeX / txUBottom;		t[1].At.v = sizeY / txUBottom;
	t[1].Bt.u = 0.0;					t[1].Bt.v = 0.0;
	t[1].Ct.u = sizeX / txUBottom;		t[1].Ct.v = 0.0;

	t[2].At.u = 0.0;					t[2].At.v = 0.0;					//FRONT
	t[2].Bt.u = sizeX / txUFront;		t[2].Bt.v = sizeZ / txUFront;
	t[2].Ct.u = 0.0;					t[2].Ct.v = sizeZ / txUFront;

	t[3].At.u = 0.0;					t[3].At.v = 0.0;
	t[3].Bt.u = sizeX / txUFront;		t[3].Bt.v = 0.0;
	t[3].Ct.u = sizeX / txUFront;		t[3].Ct.v = sizeZ / txUFront;

	t[4].At.u = sizeX / txUBack;		t[4].At.v = 0.0;					//BACK
	t[4].Bt.u = sizeX / txUBack;		t[4].Bt.v = sizeZ / txUBack;
	t[4].Ct.u = 0.0;					t[4].Ct.v = sizeZ / txUBack;

	t[5].At.u = sizeX / txUBack;		t[5].At.v = 0.0;
	t[5].Bt.u = 0.0;					t[5].Bt.v = sizeZ / txUBack;
	t[5].Ct.u = 0.0;					t[5].Ct.v = 0.0;

	t[6].At.u = 0.0;					t[6].At.v = 0.0;					//TOP
	t[6].Bt.u = sizeX / txUTop;			t[6].Bt.v = 0.0;
	t[6].Ct.u = 0.0;					t[6].Ct.v = sizeY / txUTop;

	t[7].At.u = sizeX / txUTop;			t[7].At.v = 0.0;
	t[7].Bt.u = sizeX / txUTop;			t[7].Bt.v = sizeY / txUTop;
	t[7].Ct.u = 0.0;					t[7].Ct.v = sizeY / txUTop;

	t[8].At.u = sizeY / txULeft;		t[8].At.v = 0.0;					//LEFT
	t[8].Bt.u = 0.0;					t[8].Bt.v = sizeZ / txULeft;
	t[8].Ct.u = 0.0;					t[8].Ct.v = 0.0;

	t[9].At.u = sizeY / txULeft;		t[9].At.v = 0.0;
	t[9].Bt.u = sizeY / txULeft;		t[9].Bt.v = sizeZ / txULeft;
	t[9].Ct.u = 0.0;					t[9].Ct.v = sizeZ / txULeft;

	t[10].At.u = 0.0;					t[10].At.v = 0.0;					//RIGHT
	t[10].Bt.u = sizeY / txURight;		t[10].Bt.v = sizeZ / txURight;
	t[10].Ct.u = 0.0;					t[10].Ct.v = sizeZ / txURight;

	t[11].At.u = 0.0;					t[11].At.v = 0.0;
	t[11].Bt.u = sizeY / txURight;		t[11].Bt.v = 0.0;
	t[11].Ct.u = sizeY / txURight;		t[11].Ct.v = sizeZ / txURight;


	t[0].texture	= txtBottom;
	t[1].texture	= txtBottom;
	t[2].texture	= txtFront;
	t[3].texture	= txtFront;
	t[4].texture	= txtBack;
	t[5].texture	= txtBack;
	t[6].texture	= txtTop;
	t[7].texture	= txtTop;
	t[8].texture	= txtLeft;
	t[9].texture	= txtLeft;
	t[10].texture	= txtRight;
	t[11].texture	= txtRight;


	t[0].colour		= colBottom;
	t[1].colour		= colBottom;
	t[2].colour		= colFront;
	t[3].colour		= colFront;
	t[4].colour		= colBack;
	t[5].colour		= colBack;
	t[6].colour		= colTop;
	t[7].colour		= colTop;
	t[8].colour		= colLeft;
	t[9].colour		= colLeft;
	t[10].colour	= colRight;
	t[11].colour	= colRight;


	return t;

	delete[] p;
	delete[] t;
}