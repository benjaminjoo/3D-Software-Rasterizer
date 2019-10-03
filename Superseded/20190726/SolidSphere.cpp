#include "SolidSphere.h"



SolidSphere::SolidSphere()
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

	radius		= 1.0;
	resol		= 24;
}


SolidSphere::SolidSphere(double px, double py, double pz, Uint32 c)
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

	colour		= c;
	texture		= 0;

	radius		= 1.0;
	resol		= 24;
}


SolidSphere::SolidSphere(double sx, double sy, double sz, double px, double py, double pz, double rx, double ry, double rz,
							Uint32 c, int t, double r, int res)
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

	radius		= r;
	resol		= res;
}


SolidSphere::~SolidSphere()
{
}


int	SolidSphere::getTotalVert()
{
	return 2 * resol* (resol - 1) + 2;
}


int SolidSphere::getTextureID()
{
	return texture;
}


int	SolidSphere::getTotalPoly()
{
	return 4 * resol* (resol - 1);
}


vect3* SolidSphere::getVertexData()
{
	double angleV, angleH, stepV, stepH;
	stepV = 180.0 / resol;
	stepH = 360.0 / (double(resol) * 2);
	int cnt = 0;
	int total;
	total = 2 * resol * (resol - 1) + 2;
	vect3 * s = new vect3[total];
	if (s != NULL)
	{
		if (cnt < total)
		{
			s[cnt].x = posX;				//South Pole
			s[cnt].y = posY;
			s[cnt].z = posZ - radius;
			s[cnt].w = 1.0;
			cnt++;
		}
		for (int i = 1; i < resol; i++)
		{
			for (int j = 0; j < resol * 2; j++)
			{
				angleV = (-90.0 + i * stepV) * PI / 180.0;
				angleH = (j * stepH) * PI / 180.0;
				if (cnt < total)
				{
					if (cnt < total)
					{
						s[cnt].x = posX + radius * cos(angleV) * cos(angleH);
						s[cnt].y = posY + radius * cos(angleV) * sin(angleH);
						s[cnt].z = posZ + radius * sin(angleV);
						s[cnt].w = 1.0;
						cnt++;
					}
				}
			}
		}
		if (cnt < total)
		{
			s[cnt].x = posX;				//North Pole
			s[cnt].y = posY;
			s[cnt].z = posZ + radius;
			s[cnt].w = 1.0;
			cnt++;
		}
	}

	return s;

	delete[] s;
}


triangle3dV* SolidSphere::getTriangleData()
{
	int totalVert, totalPoly;
	totalVert = 2 * resol * (resol - 1) + 2;
	totalPoly = 4 * resol * (resol - 1);
	vect3 centreV = { posX, posY, posZ, 1.0 };
	vect3 * s = new vect3[totalVert];
	triangle3dV * t = new triangle3dV[totalPoly];
	if (s != NULL) { s = this->getVertexData(); }

	int tCount = 0;

	//Adding triangles forming caps, North Pole first

	for (int i = 1; i < resol * 2 + 1; i++)
	{
		if (i < resol * 2)
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[0];		t[tCount].B = s[i + 1];	t[tCount].C = s[i];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp); t[tCount].N.w = 0.0;
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));
				t[tCount].texture = texture;
				tCount++;
			}
		}
		else
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[0];		t[tCount].B = s[1];		t[tCount].C = s[i];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp); t[tCount].N.w = 0.0;
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));
				t[tCount].texture = texture;
				tCount++;
			}
		}
	}

	//Adding triangles forming bands, going from south to north

	for (int j = 0; j < resol - 2; j++)				//incrementing 'j' represents vertical rising
	{
		for (int i = 0; i < resol * 2; i++)		//incrementing 'j' represents horizontal rotation
		{
			if (i < resol * 2 - 1)
			{
				if (tCount < totalPoly)
				{
					t[tCount].A = s[1 + (j * resol * 2) + i];
					t[tCount].B = s[1 + (j * resol * 2) + i + 1];
					t[tCount].C = s[1 + ((j + 1) * resol * 2) + i];
					//vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
					vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
					t[tCount].N = unitVector(temp); t[tCount].N.w = 0.0;
					t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
					t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
					t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));
					t[tCount].texture = texture;
					tCount++;
				}
				if (tCount < totalPoly)
				{
					t[tCount].A = s[1 + (j * resol * 2) + i + 1];
					t[tCount].B = s[1 + ((j + 1) * resol * 2) + i + 1];
					t[tCount].C = s[1 + ((j + 1) * resol * 2) + i];
					//vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
					vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
					t[tCount].N = unitVector(temp); t[tCount].N.w = 0.0;
					t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
					t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
					t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));
					t[tCount].texture = texture;
					tCount++;
				}
			}
			else
			{
				if (tCount < totalPoly)
				{
					t[tCount].A = s[1 + (j * resol * 2) + i];
					t[tCount].B = s[1 + (j * resol * 2)];
					t[tCount].C = s[1 + ((j + 1) * resol * 2) + i];
					//vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
					vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
					t[tCount].N = unitVector(temp); t[tCount].N.w = 0.0;
					t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
					t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
					t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));
					t[tCount].texture = texture;
					tCount++;
				}
				if (tCount < totalPoly)
				{
					t[tCount].A = s[1 + (j * resol * 2)];
					t[tCount].B = s[1 + (j * resol * 2) + i + 1];
					t[tCount].C = s[1 + ((j + 1) * resol * 2) + i];
					//vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
					vect3 temp = crossProduct(subVectors(t[tCount].A, t[tCount].B), subVectors(t[tCount].A, t[tCount].C));
					t[tCount].N = unitVector(temp); t[tCount].N.w = 0.0;
					t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
					t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
					t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));
					t[tCount].texture = texture;
					tCount++;
				}
			}
		}
	}

	//Adding triangles forming caps, South Pole

	for (int i = 0; i < resol * 2; i++)
	{
		if (i < resol * 2 - 1)
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[(resol - 2) * 2 * resol + 1 + i];
				t[tCount].B = s[(resol - 2) * 2 * resol + 1 + i + 1];
				t[tCount].C = s[(resol - 1) * 2 * resol + 1];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp); t[tCount].N.w = 0.0;
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));
				t[tCount].texture = texture;
				tCount++;
			}
		}
		else
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[(resol - 2) * 2 * resol + 1 + i];
				t[tCount].B = s[(resol - 2) * 2 * resol + 1];
				t[tCount].C = s[(resol - 1) * 2 * resol + 1];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
				t[tCount].N = unitVector(temp); t[tCount].N.w = 0.0;
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));
				t[tCount].texture = texture;
				tCount++;
			}
		}
	}

	return transformObject(this->getTotalPoly(), t, scaleX, scaleY, scaleZ, 0, 0, 0, rotX, rotY, rotZ);

	delete[] s;
	delete[] t;
}


Uint32 SolidSphere::getColour()
{
	return colour;
}
