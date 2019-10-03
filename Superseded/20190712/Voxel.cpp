#include "Voxel.h"



Voxel::Voxel()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;

	posX = 0;
	posY = 0;
	posZ = 0;

	s = 1.0;

	c = 255;

	front	= true;
	back	= true;
	left	= true;
	right	= true;
	top		= true;
	bottom	= true;
}


Voxel::Voxel(double cx, double cy, double cz, int px, int py, int pz, Uint32 colour)
{
	x = cx;
	y = cy;
	z = cz;

	posX = px;
	posY = py;
	posZ = pz;

	s = 1.0;

	c = colour;

	front	= true;
	back	= true;
	left	= true;
	right	= true;
	top		= true;
	bottom	= true;
}


Voxel::Voxel(double cx, double cy, double cz, int px, int py, int pz, double edge, Uint32 colour)
{
	x = cx;
	y = cy;
	z = cz;

	posX = px;
	posY = py;
	posZ = pz;

	s = edge;

	c = colour;

	front	= true;
	back	= true;
	left	= true;
	right	= true;
	top		= true;
	bottom	= true;
}


Voxel::~Voxel()
{
}


int Voxel::getTotalVert()
{
	return 8;
}


int Voxel::getTotalPoly()
{
	return 12;
}


triangle3dV* Voxel::getTriangleData()
{
	vect3* p = new vect3[8];
	int numPoly = this->countFaces() * 2;
	triangle3dV* t = new triangle3dV[numPoly];

	p[0].x = x;		p[0].y = y;		p[0].z = z;		p[0].w = 1.0;
	p[1].x = x + s;	p[1].y = y;		p[1].z = z;		p[1].w = 1.0;
	p[2].x = x + s;	p[2].y = y + s; p[2].z = z;		p[2].w = 1.0;
	p[3].x = x;		p[3].y = y + s; p[3].z = z;		p[3].w = 1.0;
	p[4].x = x;		p[4].y = y;		p[4].z = z + s;	p[4].w = 1.0;
	p[5].x = x + s;	p[5].y = y;		p[5].z = z + s;	p[5].w = 1.0;
	p[6].x = x + s;	p[6].y = y + s;	p[6].z = z + s;	p[6].w = 1.0;
	p[7].x = x;		p[7].y = y + s; p[7].z = z + s; p[7].w = 1.0;

	int i = 0;

	if (bottom && i < numPoly)
	{
		t[i].A = p[0];	t[i].B = p[3];	t[i].C = p[1];		t[i].An = t[i].Bn = t[i].Cn = t[i].N = { 0,  0, -1,  0 };	i++;
		t[i].A = p[1];	t[i].B = p[3];	t[i].C = p[2];		t[i].An = t[i].Bn = t[i].Cn = t[i].N = { 0,  0, -1,  0 };	i++;
	}

	if (front && i < numPoly)
	{
		t[i].A = p[0];	t[i].B = p[5];	t[i].C = p[4];		t[i].An = t[i].Bn = t[i].Cn = t[i].N = { 0, -1,  0,  0 };	i++;
		t[i].A = p[0];	t[i].B = p[1];	t[i].C = p[5];		t[i].An = t[i].Bn = t[i].Cn = t[i].N = { 0, -1,  0,  0 };	i++;
	}

	if (back && i < numPoly)
	{
		t[i].A = p[3];	t[i].B = p[7];	t[i].C = p[6];		t[i].An = t[i].Bn = t[i].Cn = t[i].N = { 0,  1,  0,  0 };	i++;
		t[i].A = p[3];	t[i].B = p[6];	t[i].C = p[2];		t[i].An = t[i].Bn = t[i].Cn = t[i].N = { 0,  1,  0,  0 };	i++;
	}

	if (top && i < numPoly)
	{
		t[i].A = p[4];	t[i].B = p[5];	t[i].C = p[7];		t[i].An = t[i].Bn = t[i].Cn = t[i].N = { 0,  0,  1,  0 };	i++;
		t[i].A = p[5];	t[i].B = p[6];	t[i].C = p[7];		t[i].An = t[i].Bn = t[i].Cn = t[i].N = { 0,  0,  1,  0 };	i++;
	}

	if (left && i < numPoly)
	{
		t[i].A = p[0];	t[i].B = p[7];	t[i].C = p[3];		t[i].An = t[i].Bn = t[i].Cn = t[i].N = { -1,  0,  0,  0 };	i++;
		t[i].A = p[0];	t[i].B = p[4];	t[i].C = p[7];		t[i].An = t[i].Bn = t[i].Cn = t[i].N = { -1,  0,  0,  0 };	i++;
	}

	if (right && i < numPoly)
	{
		t[i].A = p[1];	t[i].B = p[6];	t[i].C = p[5];		t[i].An = t[i].Bn = t[i].Cn = t[i].N = { 1,  0,  0,  0 };	i++;
		t[i].A = p[1];	t[i].B = p[2];	t[i].C = p[6];		t[i].An = t[i].Bn = t[i].Cn = t[i].N = { 1,  0,  0,  0 };	i++;
	}

	return t;
	delete[] p;
	delete[] t;
}


Uint32 Voxel::getColour()
{
	return c;
}


unsigned char Voxel::getRedValue()
{
	return ((unsigned char)(c >> 16 & 0xFF));
}


unsigned char Voxel::getGreenValue()
{
	return ((unsigned char)(c >> 8 & 0xFF));
}


unsigned char Voxel::getBlueValue()
{
	return ((unsigned char)(c & 0xFF));
}


int Voxel::countFaces()
{
	int t = 0;

	if (front)t++;
	if (back)t++;
	if (left)t++;
	if (right)t++;
	if (top)t++;
	if (bottom)t++;

	//return t;
	return 12;
}


void Voxel::setFront(bool state)
{
	front = state;
}


void Voxel::setBack(bool state)
{
	back = state;
}


void Voxel::setLeft(bool state)
{
	left = state;
}


void Voxel::setRight(bool state)
{
	right = state;
}


void Voxel::setTop(bool state)
{
	top = state;
}


void Voxel::setBottom(bool state)
{
	bottom = state;
}


void Voxel::updateX(double newX)
{
	x = newX;
}


void Voxel::updateY(double newY)
{
	y = newY;
}


void Voxel::updateZ(double newZ)
{
	z = newZ;
}


void Voxel::updateC(Uint32 colour)
{
	c = colour;
}


void Voxel::updateS(double size)
{
	s = size;
}