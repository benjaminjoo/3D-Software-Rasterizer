#include "SolidCone.h"



SolidCone::SolidCone()
{
	scaleX		= 1.0;
	scaleY		= 1.0;
	scaleZ		= 1.0;

	//posX		= 0.0;
	//posY		= 0.0;
	//posZ		= 0.0;

	position	= { 0.0, 0.0, 0.0, 1.0 };

	rotX		= 0.0;
	rotY		= 0.0;
	rotZ		= 0.0;

	colour		= 255;
	texture		= 0;
	txU			= 1.0;

	radius		= 1.0;
	height		= 1.0;
	resol		= 24;
}


SolidCone::SolidCone(double px, double py, double pz, Uint32 c)
{
	scaleX		= 1.0;
	scaleY		= 1.0;
	scaleZ		= 1.0;

	//posX		= px;
	//posY		= py;
	//posZ		= pz;

	position	= { 0.0, 0.0, 0.0, 1.0 };

	rotX		= 0.0;
	rotY		= 0.0;
	rotZ		= 0.0;

	colour		= c;
	texture		= 0;
	txU			= 1.0;	

	radius		= 1.0;
	height		= 1.0;
	resol		= 24;
}


SolidCone::SolidCone(double sx, double sy, double sz, double px, double py, double pz, double rx, double ry, double rz,
	Uint32 c, int t, double r, double h, int res)
{
	scaleX		= sx;
	scaleY		= sy;
	scaleZ		= sz;

	//posX		= px;
	//posY		= py;
	//posZ		= pz;

	position	= { px, py, pz, 1.0 };

	rotX		= rx;
	rotY		= ry;
	rotZ		= rz;

	colour		= c;
	texture		= t;
	txU			= 1.0;

	radius		= r;
	height		= h;
	resol		= res;
}

SolidCone::~SolidCone()
{
}


int	SolidCone::getTotalVert()
{
	return 2 * resol + 1;
}


int	SolidCone::getTotalPoly()
{
	return 4 * resol - 2;
}


void SolidCone::getVertexData_(vect3* s)
{
	double angleH, stepH;
	stepH = 360.0 / (double(resol) * 2);	//degrees
	int cnt = 0;
	int total;
	total = 2 * resol + 1;
	if (s != nullptr)
	{
		for (int i = 0; i < 2 * resol; i++)
		{
			angleH = PI / resol * i;
			if (cnt < total)
			{
				s[cnt].x = radius * cos(angleH);
				s[cnt].y = radius * sin(angleH);
				s[cnt].z = 0.0;
				s[cnt].w = 1.0;
				cnt++;
			}
		}
		if (cnt < total)
		{
			s[cnt].x = 0.0;
			s[cnt].y = 0.0;
			s[cnt].z = height;
			s[cnt].w = 1.0;
			cnt++;
		}
	}
}


void SolidCone::getTriangleData_(triangle3dV* t)
{
	int totalVert, totalPoly;
	totalVert = 2 * resol + 1;
	totalPoly = 4 * resol - 2;
	vect3* s = new vect3[totalVert];
	this->getVertexData_(s);
	vect3 vectR = { 0.0, 0.0, -(radius * radius) / height, 1.0 };

	int tCount = 0;

	double uvStepHor;
	double perimeter = 2 * radius * PI;
	//uvStepHor = (perimeter / txU) / (2 * double(resol));
	uvStepHor = txU / (2 * double(resol));

	//Adding polygons forming the base

	for (int i = 0; i < 2 * resol - 2; i++)
	{
		if (tCount < totalPoly)
		{
			t[tCount].A = s[0];		t[tCount].B = s[i + 2];	t[tCount].C = s[i + 1];
			vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
			t[tCount].N = unitVector(temp);
			t[tCount].An = t[tCount].Bn = t[tCount].Cn = { 0.0, 0.0, -1.0, 0.0 };
			t[tCount].texture = texture;
			t[tCount].colour = colour;
			tCount++;
		}
	}

	//Adding polygons forming side surface
	double lSide = sqrt(radius * radius + height * height);

	for (int i = 0; i < 2 * resol; i++)
	{
		if (i < resol * 2 - 1)
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[i];		t[tCount].B = s[i + 1];	t[tCount].C = s[2 * resol];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp);
				t[tCount].An = unitVector(subVectors(t[tCount].A, vectR));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, vectR));
				t[tCount].Cn = { (t[tCount].An.x + t[tCount].Bn.x) / 2,
									(t[tCount].An.y + t[tCount].Bn.y) / 2,
									(t[tCount].An.z + t[tCount].Bn.z) / 2,
									0.0 };

				t[tCount].texture = texture;
				t[tCount].colour = colour;

				t[tCount].At.u = uvStepHor * (double(i) + 0.0);		t[tCount].At.v = 0.0;
				t[tCount].Bt.u = uvStepHor * (double(i) + 1.0);		t[tCount].Bt.v = 0.0;
				t[tCount].Ct.u = uvStepHor * (double(i) + 0.5);		t[tCount].Ct.v = txU;

				tCount++;
			}
		}
		else
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[i];		t[tCount].B = s[0];		t[tCount].C = s[2 * resol];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp);
				t[tCount].An = unitVector(subVectors(t[tCount].A, vectR));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, vectR));
				t[tCount].Cn = { (t[tCount].An.x + t[tCount].Bn.x) / 2,
									(t[tCount].An.y + t[tCount].Bn.y) / 2,
									(t[tCount].An.z + t[tCount].Bn.z) / 2,
									0.0 };

				t[tCount].texture = texture;
				t[tCount].colour = colour;

				t[tCount].At.u = uvStepHor * (double(i) + 0.0);		t[tCount].At.v = 0.0;
				t[tCount].Bt.u = txU;								t[tCount].Bt.v = 0.0;
				t[tCount].Ct.u = uvStepHor * (double(i) + 0.5);		t[tCount].Ct.v = txU;

				tCount++;
			}
		}
	}

	delete[] s;

	transformMesh(this->getTotalPoly(), t, scaleX, scaleY, scaleZ, position.x, position.y, position.z, rotX, rotY, rotZ);
}


vect3 SolidCone::getPosition()
{
	return position;
}
