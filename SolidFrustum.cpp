#include "SolidFrustum.h"



SolidFrustum::SolidFrustum()
{
	nPoly = getTotalPoly();
	mesh = new triangle3dV[nPoly];
	getTriangleData(mesh);
}


SolidFrustum::SolidFrustum(float px, float py, float pz, float znear, float zfar, float fov, float asp, Uint32 c):
	zNear(znear), zFar(zfar), fovH(fov), aspRatio(asp)
{
	position = { px, py, pz, 1.0 };
	colour = c;

	nPoly = getTotalPoly();
	mesh = new triangle3dV[nPoly];
	getTriangleData(mesh);
}


SolidFrustum::~SolidFrustum()
{
}


int	SolidFrustum::getTotalVert()
{
	return 8;
}


int SolidFrustum::getTotalPoly()
{
	return 12;
}


void SolidFrustum::getTriangleData(triangle3dV* t)
{
	vect3* p = new vect3[8];

	float wNear	= zNear	* tan(fovH * 0.5f);
	float wFar		= zFar	* tan(fovH * 0.5f);
	float hNear	= wNear * aspRatio;
	float hFar		= wFar	* aspRatio;

	p[0].x = wNear;			p[0].y = hNear;			p[0].z = zNear;			p[0].w = 1.0f;
	p[1].x = wNear;			p[1].y = -hNear;		p[1].z = zNear;			p[1].w = 1.0f;
	p[2].x = -wNear;		p[2].y = -hNear;		p[2].z = zNear;			p[2].w = 1.0f;
	p[3].x = -wNear;		p[3].y = hNear;			p[3].z = zNear;			p[3].w = 1.0f;
	p[4].x = wFar;			p[4].y = hFar;			p[4].z = zFar;			p[4].w = 1.0f;
	p[5].x = wFar;			p[5].y = -hFar;			p[5].z = zFar;			p[5].w = 1.0f;
	p[6].x = -wFar;			p[6].y = -hFar;			p[6].z = zFar;			p[6].w = 1.0f;
	p[7].x = -wFar;			p[7].y = hFar;			p[7].z = zFar;			p[7].w = 1.0f;

	t[0].A = p[0];
	t[0].B = p[1];
	t[0].C = p[2];
	t[0].An = t[0].Bn = t[0].Cn = t[0].N = { 0.0f,  0.0f, -1.0f,  0.0f };

	t[1].A = p[2];
	t[1].B = p[3];
	t[1].C = p[0];
	t[1].An = t[1].Bn = t[1].Cn = t[1].N = { 0.0f,  0.0f, -1.0f,  0.0f };

	t[2].A = p[0];
	t[2].B = p[4];
	t[2].C = p[5];
	t[2].An = t[2].Bn = t[2].Cn = t[2].N = unitVector((t[2].C - t[2].B) ^ (t[2].A - t[2].B));

	t[3].A = p[1];
	t[3].B = p[0];
	t[3].C = p[5];
	t[3].An = t[3].Bn = t[3].Cn = t[3].N = unitVector((t[3].C - t[3].B) ^ (t[3].A - t[3].B));

	t[4].A = p[1];
	t[4].B = p[5];
	t[4].C = p[6];
	t[4].An = t[4].Bn = t[4].Cn = t[4].N = unitVector((t[4].C - t[4].B) ^ (t[4].A - t[4].B));

	t[5].A = p[2];
	t[5].B = p[1];
	t[5].C = p[6];
	t[5].An = t[5].Bn = t[5].Cn = t[5].N = unitVector((t[5].C - t[5].B) ^ (t[5].A - t[5].B));

	t[6].A = p[2];
	t[6].B = p[6];
	t[6].C = p[7];
	t[6].An = t[6].Bn = t[6].Cn = t[6].N = unitVector((t[6].C - t[6].B) ^ (t[6].A - t[6].B));

	t[7].A = p[3];
	t[7].B = p[2];
	t[7].C = p[7];
	t[7].An = t[7].Bn = t[7].Cn = t[7].N = unitVector((t[7].C - t[7].B) ^ (t[7].A - t[7].B));

	t[8].A = p[3];
	t[8].B = p[7];
	t[8].C = p[4];
	t[8].An = t[8].Bn = t[8].Cn = t[8].N = unitVector((t[8].C - t[8].B) ^ (t[8].A - t[8].B));

	t[9].A = p[0];
	t[9].B = p[3];
	t[9].C = p[4];
	t[9].An = t[9].Bn = t[9].Cn = t[9].N = unitVector((t[9].C - t[9].B) ^ (t[9].A - t[9].B));

	t[10].A = p[5];
	t[10].B = p[4];
	t[10].C = p[7];
	t[10].An = t[10].Bn = t[10].Cn = t[10].N = { 0.0f, 0.0f, 1.0f, 0.0f };

	t[11].A = p[7];
	t[11].B = p[6];
	t[11].C = p[5];
	t[11].An = t[11].Bn = t[11].Cn = t[11].N = { 0.0f, 0.0f, 1.0f, 0.0f };



	t[0].At.u = txU;	t[0].At.v = txU;	t[0].Bt.u = txU;	t[0].Bt.v = txU;	t[0].Ct.u = txU;	t[0].Ct.v = txU;
	t[1].At.u = txU;	t[1].At.v = txU;	t[1].Bt.u = txU;	t[1].Bt.v = txU;	t[1].Ct.u = txU;	t[1].Ct.v = txU;
	t[2].At.u = txU;	t[2].At.v = txU;	t[2].Bt.u = txU;	t[2].Bt.v = txU;	t[2].Ct.u = txU;	t[2].Ct.v = txU;
	t[3].At.u = txU;	t[3].At.v = txU;	t[3].Bt.u = txU;	t[3].Bt.v = txU;	t[3].Ct.u = txU;	t[3].Ct.v = txU;
	t[4].At.u = txU;	t[4].At.v = txU;	t[4].Bt.u = txU;	t[4].Bt.v = txU;	t[4].Ct.u = txU;	t[4].Ct.v = txU;
	t[5].At.u = txU;	t[5].At.v = txU;	t[5].Bt.u = txU;	t[5].Bt.v = txU;	t[5].Ct.u = txU;	t[5].Ct.v = txU;
	t[6].At.u = txU;	t[6].At.v = txU;	t[6].Bt.u = txU;	t[6].Bt.v = txU;	t[6].Ct.u = txU;	t[6].Ct.v = txU;
	t[7].At.u = txU;	t[7].At.v = txU;	t[7].Bt.u = txU;	t[7].Bt.v = txU;	t[7].Ct.u = txU;	t[7].Ct.v = txU;
	t[8].At.u = txU;	t[8].At.v = txU;	t[8].Bt.u = txU;	t[8].Bt.v = txU;	t[8].Ct.u = txU;	t[8].Ct.v = txU;
	t[9].At.u = txU;	t[9].At.v = txU;	t[9].Bt.u = txU;	t[9].Bt.v = txU;	t[9].Ct.u = txU;	t[9].Ct.v = txU;
	t[10].At.u = txU;	t[10].At.v = txU;	t[10].Bt.u = txU;	t[10].Bt.v = txU;	t[10].Ct.u = txU;	t[10].Ct.v = txU;
	t[11].At.u = txU;	t[11].At.v = txU;	t[11].Bt.u = txU;	t[11].Bt.v = txU;	t[11].Ct.u = txU;	t[11].Ct.v = txU;

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

	nPoly = getTotalPoly();

	Projector->transformMesh(nPoly, t, scale.x, scale.y, scale.z,
										position.x, position.y, position.z,
										rotation.x, rotation.y, rotation.z);
}
