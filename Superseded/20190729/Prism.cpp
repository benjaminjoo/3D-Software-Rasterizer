#include "Prism.h"
#include "Definitions.h"


Prism::Prism()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;

	a = 1.0;
	b = 1.0;
	c = 2.0;

	col = 255;
}


Prism::Prism(double cx, double cy, double cz, double hg, Uint32 colour)
{
	x = cx;
	y = cy;
	z = cz;

	a = 1.0;
	b = 1.0;
	c = hg;

	col = colour;
}


Prism::Prism(double cx, double cy, double cz, double ea, double eb, double ec, Uint32 colour)
{
	x = cx;
	y = cy;
	z = cz;

	a = ea;
	b = eb;
	c = ec;

	col = colour;
}


Prism::~Prism()
{
}


int Prism::getTotalVert()
{
	return 8;
}

int Prism::getTotalPoly()
{
	return 12;
}


triangle3dV* Prism::getTriangleData()
{
	vect3* p = new vect3[8];
	triangle3dV* t = new triangle3dV[12];

	p[0].x = x;		p[0].y = y;		p[0].z = z;		p[0].w = 1.0;
	p[1].x = x + a;	p[1].y = y;		p[1].z = z;		p[1].w = 1.0;
	p[2].x = x + a;	p[2].y = y + b; p[2].z = z;		p[2].w = 1.0;
	p[3].x = x;		p[3].y = y + b; p[3].z = z;		p[3].w = 1.0;
	p[4].x = x;		p[4].y = y;		p[4].z = z + c;	p[4].w = 1.0;
	p[5].x = x + a;	p[5].y = y;		p[5].z = z + c;	p[5].w = 1.0;
	p[6].x = x + a;	p[6].y = y + b;	p[6].z = z + c;	p[6].w = 1.0;
	p[7].x = x;		p[7].y = y + b; p[7].z = z + c; p[7].w = 1.0;

	t[0].A = p[0];	t[0].B = p[3];	t[0].C = p[1];		t[0].An = t[0].Bn = t[0].Cn = t[0].N = { 0,  0, -1,  0 };
	t[1].A = p[1];	t[1].B = p[3];	t[1].C = p[2];		t[1].An = t[1].Bn = t[1].Cn = t[1].N = { 0,  0, -1,  0 };

	t[2].A = p[0];	t[2].B = p[5];	t[2].C = p[4];		t[2].An = t[2].Bn = t[2].Cn = t[2].N = { 0, -1,  0,  0 };
	t[3].A = p[0];	t[3].B = p[1];	t[3].C = p[5];		t[3].An = t[3].Bn = t[3].Cn = t[3].N = { 0, -1,  0,  0 };

	t[4].A = p[3];	t[4].B = p[7];	t[4].C = p[6];		t[4].An = t[4].Bn = t[4].Cn = t[4].N = { 0,  1,  0,  0 };
	t[5].A = p[3];	t[5].B = p[6];	t[5].C = p[2];		t[5].An = t[5].Bn = t[5].Cn = t[5].N = { 0,  1,  0,  0 };

	t[6].A = p[4];	t[6].B = p[5];	t[6].C = p[7];		t[6].An = t[6].Bn = t[6].Cn = t[6].N = { 0,  0,  1,  0 };
	t[7].A = p[5];	t[7].B = p[6];	t[7].C = p[7];		t[7].An = t[7].Bn = t[7].Cn = t[7].N = { 0,  0,  1,  0 };

	t[8].A = p[0];	t[8].B = p[7];	t[8].C = p[3];		t[8].An = t[8].Bn = t[8].Cn = t[8].N = { -1,  0,  0,  0 };
	t[9].A = p[0];	t[9].B = p[4];	t[9].C = p[7];		t[9].An = t[9].Bn = t[9].Cn = t[9].N = { -1,  0,  0,  0 };

	t[10].A = p[1];	t[10].B = p[6];	t[10].C = p[5];		t[10].An = t[10].Bn = t[10].Cn = t[10].N = { 1,  0,  0,  0 };
	t[11].A = p[1];	t[11].B = p[2];	t[11].C = p[6];		t[11].An = t[11].Bn = t[11].Cn = t[11].N = { 1,  0,  0,  0 };

	return t;
}


Uint32 Prism::getColour()
{
	return col;
}


/*unsigned char Prism::getRedValue()
{
	return ((unsigned char)(col >> 16 & 0xFF));
}


unsigned char Prism::getGreenValue()
{
	return ((unsigned char)(col >> 8 & 0xFF));
}


unsigned char Prism::getBlueValue()
{
	return ((unsigned char)(col & 0xFF));
}*/