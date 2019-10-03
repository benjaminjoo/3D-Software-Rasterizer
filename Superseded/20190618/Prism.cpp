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


triangle3d* Prism::getTriangleData()
{
	vect3* p = new vect3[8];
	triangle3d* t = new triangle3d[12];

	p[0].x = x;		p[0].y = y;		p[0].z = z;		p[0].w = 1.0;
	p[1].x = x + a;	p[1].y = y;		p[1].z = z;		p[1].w = 1.0;
	p[2].x = x + a;	p[2].y = y + b; p[2].z = z;		p[2].w = 1.0;
	p[3].x = x;		p[3].y = y + b; p[3].z = z;		p[3].w = 1.0;
	p[4].x = x;		p[4].y = y;		p[4].z = z + c;	p[4].w = 1.0;
	p[5].x = x + a;	p[5].y = y;		p[5].z = z + c;	p[5].w = 1.0;
	p[6].x = x + a;	p[6].y = y + b;	p[6].z = z + c;	p[6].w = 1.0;
	p[7].x = x;		p[7].y = y + b; p[7].z = z + c; p[7].w = 1.0;

	t[0].A = p[0];	t[0].B = p[3];	t[0].C = p[1];		t[0].N = { 0,  0, -1,  0 };
	t[1].A = p[1];	t[1].B = p[3];	t[1].C = p[2];		t[1].N = { 0,  0, -1,  0 };

	t[2].A = p[0];	t[2].B = p[5];	t[2].C = p[4];		t[2].N = { 0, -1,  0,  0 };
	t[3].A = p[0];	t[3].B = p[1];	t[3].C = p[5];		t[3].N = { 0, -1,  0,  0 };

	t[4].A = p[3];	t[4].B = p[7];	t[4].C = p[6];		t[4].N = { 0,  1,  0,  0 };
	t[5].A = p[3];	t[5].B = p[6];	t[5].C = p[2];		t[5].N = { 0,  1,  0,  0 };

	t[6].A = p[4];	t[6].B = p[5];	t[6].C = p[7];		t[6].N = { 0,  0,  1,  0 };
	t[7].A = p[5];	t[7].B = p[6];	t[7].C = p[7];		t[7].N = { 0,  0,  1,  0 };

	t[8].A = p[0];	t[8].B = p[7];	t[8].C = p[3];		t[8].N = { -1,  0,  0,  0 };
	t[9].A = p[0];	t[9].B = p[4];	t[9].C = p[7];		t[9].N = { -1,  0,  0,  0 };

	t[10].A = p[1];	t[10].B = p[6];	t[10].C = p[5];		t[10].N = { 1,  0,  0,  0 };
	t[11].A = p[1];	t[11].B = p[2];	t[11].C = p[6];		t[11].N = { 1,  0,  0,  0 };

	return t;
}


Uint32 Prism::getColour()
{
	return col;
}


unsigned char Prism::getRedValue()
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
}