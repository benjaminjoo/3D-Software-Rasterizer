#include "Bullet.h"



Bullet::Bullet()
{
	nPoly = getTotalPoly();
	mesh = new triangle3dV[nPoly];
	getTriangleData(mesh);
}


Bullet::Bullet(double px, double py, double pz, double d, double l, double v, Uint32 c):
	dia(d), len(l), muzzleVelocity(v)
{
	position	= { px, py, pz, 1.0 };
	colour		= c;

	nPoly = getTotalPoly();
	mesh = new triangle3dV[nPoly];
	getTriangleData(mesh);
}


Bullet::~Bullet()
{
}


int	Bullet::getTotalVert()
{
	return 4;
}


int	Bullet::getTotalPoly()
{
	return 4;
}


void Bullet::getTriangleData(triangle3dV* t)
{
	vect3* p = new vect3[4];

	p[0].x = 0.0f;							p[0].y = 0.0f;			p[0].z = 0.0f;							p[0].w = 1.0f;
	p[1].x = 0.0f;							p[1].y = -len;			p[1].z = dia * 0.5f;					p[1].w = 1.0f;
	p[2].x = dia * 0.5f * cos(PI / 6);		p[2].y = -len;			p[2].z = -dia * 0.5f * sin(PI / 6);		p[2].w = 1.0f;
	p[3].x = -dia * 0.5f * cos(PI / 6);		p[3].y = -len;			p[3].z = -dia * 0.5f * sin(PI / 6);		p[3].w = 1.0f;

	t[0].A = p[1];
	t[0].B = p[2];
	t[0].C = p[3];
	t[0].An = t[0].Bn = t[0].Cn = t[0].N = { 0.0f,  -1.0f, 0.0f,  0.0f };

	t[1].A = p[0];
	t[1].B = p[2];
	t[1].C = p[1];
	t[1].An = t[1].Bn = t[1].Cn = t[1].N = unitVector(crossProduct(subVectors(p[1], p[0]), subVectors(p[2], p[0])));

	t[2].A = p[0];
	t[2].B = p[3];
	t[2].C = p[2];
	t[2].An = t[2].Bn = t[2].Cn = t[2].N = unitVector(crossProduct(subVectors(p[2], p[0]), subVectors(p[3], p[0])));

	t[3].A = p[0];
	t[3].B = p[1];
	t[3].C = p[3];
	t[3].An = t[3].Bn = t[3].Cn = t[3].N = unitVector(crossProduct(subVectors(p[3], p[0]), subVectors(p[1], p[0])));

	t[0].At.u = txU;	t[0].At.v = txU;	t[0].Bt.u = txU;	t[0].Bt.v = txU;	t[0].Ct.u = txU;	t[0].Ct.v = txU;
	t[1].At.u = txU;	t[1].At.v = txU;	t[1].Bt.u = txU;	t[1].Bt.v = txU;	t[1].Ct.u = txU;	t[1].Ct.v = txU;
	t[2].At.u = txU;	t[2].At.v = txU;	t[2].Bt.u = txU;	t[2].Bt.v = txU;	t[2].Ct.u = txU;	t[2].Ct.v = txU;
	t[3].At.u = txU;	t[3].At.v = txU;	t[3].Bt.u = txU;	t[3].Bt.v = txU;	t[3].Ct.u = txU;	t[3].Ct.v = txU;

	t[0].texture = texture;
	t[1].texture = texture;
	t[2].texture = texture;
	t[3].texture = texture;

	t[0].colour = colour;
	t[1].colour = colour;
	t[2].colour = colour;
	t[3].colour = colour;

	delete[] p;
}


double Bullet::getMuzzleVelocity()
{
	return muzzleVelocity;
}


