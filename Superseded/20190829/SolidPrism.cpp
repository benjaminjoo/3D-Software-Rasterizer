#include "SolidPrism.h"



SolidPrism::SolidPrism()
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
	txU			= 1.0;

	edgeX		= 1.0;
	edgeY		= 2.0;
	edgeZ		= 3.0;
}


SolidPrism::SolidPrism(double px, double py, double pz, Uint32 c, double ex, double ey, double ez)
{
	scaleX		= 1.0;
	scaleY		= 1.0;
	scaleZ		= 1.0;

	posX		= px;
	posY		= py;
	posZ		= pz;

	rotX		= 0.0;
	rotY		= 0.0;
	rotZ		= 0.0;

	colour		= 255;
	texture		= 0;
	txU			= 1.0;

	edgeX		= ex;
	edgeY		= ey;
	edgeZ		= ez;
}


SolidPrism::SolidPrism(double sx, double sy, double sz, double px, double py, double pz,
						double rx, double ry, double rz, Uint32 c, int t, double ex, double ey, double ez)
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

	colour		= c;
	texture		= t;
	txU			= 2.5;

	edgeX		= ex;
	edgeY		= ey;
	edgeZ		= ez;
}


SolidPrism::~SolidPrism()
{
}


int	SolidPrism::getTotalVert()
{
	return 8;
}


int	SolidPrism::getTotalPoly()
{
	return 12;
}


triangle3dV* SolidPrism::getTriangleData()
{
	vect3* p = new vect3[8];
	triangle3dV* t = new triangle3dV[12];

	p[0].x = 0;			p[0].y = 0;			p[0].z = 0;			p[0].w = 1.0;
	p[1].x = edgeX;		p[1].y = 0;			p[1].z = 0;			p[1].w = 1.0;
	p[2].x = edgeX;		p[2].y = edgeY;		p[2].z = 0;			p[2].w = 1.0;
	p[3].x = 0;			p[3].y = edgeY;		p[3].z = 0;			p[3].w = 1.0;
	p[4].x = 0;			p[4].y = 0;			p[4].z = edgeZ;		p[4].w = 1.0;
	p[5].x = edgeX;		p[5].y = 0;			p[5].z = edgeZ;		p[5].w = 1.0;
	p[6].x = edgeX;		p[6].y = edgeY;		p[6].z = edgeZ;		p[6].w = 1.0;
	p[7].x = 0;			p[7].y = edgeY;		p[7].z = edgeZ;		p[7].w = 1.0;

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




	t[0].At.u = 0.0;			t[0].At.v = edgeY / txU;
	t[0].Bt.u = 0.0;			t[0].Bt.v = 0.0;
	t[0].Ct.u = edgeX / txU;	t[0].Ct.v = edgeY / txU;

	t[1].At.u = edgeX / txU;	t[1].At.v = edgeY / txU;
	t[1].Bt.u = 0.0;			t[1].Bt.v = 0.0;
	t[1].Ct.u = edgeX / txU;	t[1].Ct.v = 0.0;

	t[2].At.u = 0.0;			t[2].At.v = 0.0;
	t[2].Bt.u = edgeX / txU;	t[2].Bt.v = edgeZ / txU;
	t[2].Ct.u = 0.0;			t[2].Ct.v = edgeZ / txU;

	t[3].At.u = 0.0;			t[3].At.v = 0.0;
	t[3].Bt.u = edgeX / txU;	t[3].Bt.v = 0.0;
	t[3].Ct.u = edgeX / txU;	t[3].Ct.v = edgeZ / txU;

	t[4].At.u = edgeX / txU;	t[4].At.v = 0.0;
	t[4].Bt.u = edgeX / txU;	t[4].Bt.v = edgeZ / txU;
	t[4].Ct.u = 0.0;			t[4].Ct.v = edgeZ / txU;

	t[5].At.u = edgeX / txU;	t[5].At.v = 0.0;
	t[5].Bt.u = 0.0;			t[5].Bt.v = edgeZ / txU;
	t[5].Ct.u = 0.0;			t[5].Ct.v = 0.0;

	t[6].At.u = 0.0;			t[6].At.v = 0.0;
	t[6].Bt.u = edgeX / txU;	t[6].Bt.v = 0.0;
	t[6].Ct.u = 0.0;			t[6].Ct.v = edgeY / txU;

	t[7].At.u = edgeX / txU;	t[7].At.v = 0.0;
	t[7].Bt.u = edgeX / txU;	t[7].Bt.v = edgeY / txU;
	t[7].Ct.u = 0.0;			t[7].Ct.v = edgeY / txU;

	t[8].At.u = edgeY / txU;	t[8].At.v = 0.0;
	t[8].Bt.u = 0.0;			t[8].Bt.v = edgeZ / txU;
	t[8].Ct.u = 0.0;			t[8].Ct.v = 0.0;

	t[9].At.u = edgeY / txU;	t[9].At.v = 0.0;
	t[9].Bt.u = edgeY / txU;	t[9].Bt.v = edgeZ / txU;
	t[9].Ct.u = 0.0;			t[9].Ct.v = edgeZ / txU;

	t[10].At.u = 0.0;			t[10].At.v = 0.0;
	t[10].Bt.u = edgeY / txU;	t[10].Bt.v = edgeZ / txU;
	t[10].Ct.u = 0.0;			t[10].Ct.v = edgeZ / txU;

	t[11].At.u = 0.0;			t[11].At.v = 0.0;
	t[11].Bt.u = edgeY / txU;	t[11].Bt.v = 0.0;
	t[11].Ct.u = edgeY / txU;	t[11].Ct.v = edgeZ / txU;


	t[0].texture = texture;
	t[1].texture = texture;
	t[2].texture = texture;
	t[3].texture = texture;
	t[4].texture = texture;
	t[5].texture = texture;
	t[6].texture = texture;
	t[7].texture = texture;
	t[8].texture = texture;
	t[9].texture = texture;
	t[10].texture = texture;
	t[11].texture = texture;


	t[0].colour = colour;
	t[1].colour = colour;
	t[2].colour = colour;
	t[3].colour = colour;
	t[4].colour = colour;
	t[5].colour = colour;
	t[6].colour = colour;
	t[7].colour = colour;
	t[8].colour = colour;
	t[9].colour = colour;
	t[10].colour = colour;
	t[11].colour = colour;


	return transformObject(this->getTotalPoly(), t, scaleX, scaleY, scaleZ, posX, posY, posZ, rotX, rotY, rotZ);

	delete[] p;
	delete[] t;
}


void SolidPrism::getTriangleData_(triangle3dV* t)
{
	vect3* p = new vect3[8];

	p[0].x = 0;			p[0].y = 0;			p[0].z = 0;			p[0].w = 1.0;
	p[1].x = edgeX;		p[1].y = 0;			p[1].z = 0;			p[1].w = 1.0;
	p[2].x = edgeX;		p[2].y = edgeY;		p[2].z = 0;			p[2].w = 1.0;
	p[3].x = 0;			p[3].y = edgeY;		p[3].z = 0;			p[3].w = 1.0;
	p[4].x = 0;			p[4].y = 0;			p[4].z = edgeZ;		p[4].w = 1.0;
	p[5].x = edgeX;		p[5].y = 0;			p[5].z = edgeZ;		p[5].w = 1.0;
	p[6].x = edgeX;		p[6].y = edgeY;		p[6].z = edgeZ;		p[6].w = 1.0;
	p[7].x = 0;			p[7].y = edgeY;		p[7].z = edgeZ;		p[7].w = 1.0;

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




	t[0].At.u = 0.0;			t[0].At.v = edgeY / txU;
	t[0].Bt.u = 0.0;			t[0].Bt.v = 0.0;
	t[0].Ct.u = edgeX / txU;	t[0].Ct.v = edgeY / txU;

	t[1].At.u = edgeX / txU;	t[1].At.v = edgeY / txU;
	t[1].Bt.u = 0.0;			t[1].Bt.v = 0.0;
	t[1].Ct.u = edgeX / txU;	t[1].Ct.v = 0.0;

	t[2].At.u = 0.0;			t[2].At.v = 0.0;
	t[2].Bt.u = edgeX / txU;	t[2].Bt.v = edgeZ / txU;
	t[2].Ct.u = 0.0;			t[2].Ct.v = edgeZ / txU;

	t[3].At.u = 0.0;			t[3].At.v = 0.0;
	t[3].Bt.u = edgeX / txU;	t[3].Bt.v = 0.0;
	t[3].Ct.u = edgeX / txU;	t[3].Ct.v = edgeZ / txU;

	t[4].At.u = edgeX / txU;	t[4].At.v = 0.0;
	t[4].Bt.u = edgeX / txU;	t[4].Bt.v = edgeZ / txU;
	t[4].Ct.u = 0.0;			t[4].Ct.v = edgeZ / txU;

	t[5].At.u = edgeX / txU;	t[5].At.v = 0.0;
	t[5].Bt.u = 0.0;			t[5].Bt.v = edgeZ / txU;
	t[5].Ct.u = 0.0;			t[5].Ct.v = 0.0;

	t[6].At.u = 0.0;			t[6].At.v = 0.0;
	t[6].Bt.u = edgeX / txU;	t[6].Bt.v = 0.0;
	t[6].Ct.u = 0.0;			t[6].Ct.v = edgeY / txU;

	t[7].At.u = edgeX / txU;	t[7].At.v = 0.0;
	t[7].Bt.u = edgeX / txU;	t[7].Bt.v = edgeY / txU;
	t[7].Ct.u = 0.0;			t[7].Ct.v = edgeY / txU;

	t[8].At.u = edgeY / txU;	t[8].At.v = 0.0;
	t[8].Bt.u = 0.0;			t[8].Bt.v = edgeZ / txU;
	t[8].Ct.u = 0.0;			t[8].Ct.v = 0.0;

	t[9].At.u = edgeY / txU;	t[9].At.v = 0.0;
	t[9].Bt.u = edgeY / txU;	t[9].Bt.v = edgeZ / txU;
	t[9].Ct.u = 0.0;			t[9].Ct.v = edgeZ / txU;

	t[10].At.u = 0.0;			t[10].At.v = 0.0;
	t[10].Bt.u = edgeY / txU;	t[10].Bt.v = edgeZ / txU;
	t[10].Ct.u = 0.0;			t[10].Ct.v = edgeZ / txU;

	t[11].At.u = 0.0;			t[11].At.v = 0.0;
	t[11].Bt.u = edgeY / txU;	t[11].Bt.v = 0.0;
	t[11].Ct.u = edgeY / txU;	t[11].Ct.v = edgeZ / txU;


	t[0].texture = texture;
	t[1].texture = texture;
	t[2].texture = texture;
	t[3].texture = texture;
	t[4].texture = texture;
	t[5].texture = texture;
	t[6].texture = texture;
	t[7].texture = texture;
	t[8].texture = texture;
	t[9].texture = texture;
	t[10].texture = texture;
	t[11].texture = texture;


	t[0].colour = colour;
	t[1].colour = colour;
	t[2].colour = colour;
	t[3].colour = colour;
	t[4].colour = colour;
	t[5].colour = colour;
	t[6].colour = colour;
	t[7].colour = colour;
	t[8].colour = colour;
	t[9].colour = colour;
	t[10].colour = colour;
	t[11].colour = colour;

	delete[] p;

	transformMesh(this->getTotalPoly(), t, scaleX, scaleY, scaleZ, posX, posY, posZ, rotX, rotY, rotZ);
}
