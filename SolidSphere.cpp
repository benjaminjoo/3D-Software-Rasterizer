#include "SolidSphere.h"


SolidSphere::SolidSphere()
{
	scale			= { 1.0, 1.0, 1.0, 1.0 };
	position		= { 0.0, 0.0, 0.0, 1.0 };
	rotation		= { 0.0, 0.0, 0.0, 1.0 };

	colour			= 255;
	texture			= 0;
	castsShadows	= false;

	radius			= 1.0;
	resol			= 24;
}


SolidSphere::SolidSphere(double px, double py, double pz, Uint32 c)
{
	scale			= { 1.0, 1.0, 1.0, 1.0 };
	position		= { px, py, pz, 1.0 };
	rotation		= { 0.0, 0.0, 0.0, 1.0 };

	colour			= c;
	texture			= 0;
	castsShadows	= false;

	radius			= 1.0;
	resol			= 24;
}


SolidSphere::SolidSphere(double sx, double sy, double sz, double px, double py, double pz, double rx, double ry, double rz,
							Uint32 c, int t, double r, int res)
{
	scale			= { sx, sy, sz, 1.0 };
	position		= { px, py, pz, 1.0 };
	rotation		= { rx, ry, rz, 1.0 };

	colour			= c;
	texture			= t;
	castsShadows	= false;

	radius			= r;
	resol			= res;
}


SolidSphere::~SolidSphere()
{
}


void SolidSphere::setRadius(double r)
{
	radius = r;
}


double SolidSphere::getRadius()
{
	return radius;
}


int	SolidSphere::getTotalVert()
{
	return 2 * resol* (resol - 1) + 2;
}


int	SolidSphere::getTotalPoly()
{
	return 4 * resol* (resol - 1);
}


void SolidSphere::getVertexData_(vect3* s)
{
	double angleV, angleH, stepV, stepH;
	stepV = 180.0 / resol;
	stepH = 360.0 / (double(resol) * 2);
	int cnt = 0;
	int total;
	total = 2 * resol * (resol - 1) + 2;
	if (s != nullptr)
	{
		if (cnt < total)
		{
			s[cnt].x = 0.0;				//South Pole
			s[cnt].y = 0.0;
			s[cnt].z = -radius;
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
						s[cnt].x = radius * cos(angleV) * cos(angleH);
						s[cnt].y = radius * cos(angleV) * sin(angleH);
						s[cnt].z = radius * sin(angleV);
						s[cnt].w = 1.0;
						cnt++;
					}
				}
			}
		}
		if (cnt < total)
		{
			s[cnt].x = 0.0;				//North Pole
			s[cnt].y = 0.0;
			s[cnt].z = radius;
			s[cnt].w = 1.0;
			cnt++;
		}
	}
}


void SolidSphere::getTriangleData_(triangle3dV* t)
{
	int totalVert, totalPoly;
	totalVert = 2 * resol * (resol - 1) + 2;
	totalPoly = 4 * resol * (resol - 1);
	vect3 centreV = { 0.0, 0.0, 0.0, 1.0 };
	vect3 * s = new vect3[totalVert];
	this->getVertexData_(s);

	int tCount = 0;

	double uvStepHor, uvStepVer;
	uvStepHor = 1.0 / (2 * double(resol));
	uvStepVer = 1.0 / double(resol);

	//Adding triangles forming caps, South Pole first

	for (int i = 0; i < resol * 2; i++)
	{
		if (i < resol * 2 - 1)
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[0];		t[tCount].B = s[i + 2];	t[tCount].C = s[i + 1];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
				t[tCount].N = unitVector(temp);
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));

				t[tCount].texture = texture;
				t[tCount].colour = colour;

				t[tCount].At.u = uvStepHor * (double(i) + 0.5);		t[tCount].At.v = 0.0;
				t[tCount].Bt.u = uvStepHor * (double(i) + 1.0);		t[tCount].Bt.v = uvStepVer;
				t[tCount].Ct.u = uvStepHor * (double(i) + 0.0);		t[tCount].Ct.v = uvStepVer;

				tCount++;
			}
		}
		else
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[0];		t[tCount].B = s[1];		t[tCount].C = s[i];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
				t[tCount].N = unitVector(temp);
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));

				t[tCount].texture = texture;
				t[tCount].colour = colour;

				t[tCount].At.u = 1.0 - 0.5 * uvStepHor;				t[tCount].At.v = 0.0;
				t[tCount].Bt.u = 1.0 - 0.0 * uvStepHor;				t[tCount].Bt.v = uvStepVer;
				t[tCount].Ct.u = 1.0 - 1.0 * uvStepHor;				t[tCount].Ct.v = uvStepVer;

				tCount++;
			}
		}
	}

	//Adding triangles forming bands, going from south to north

	for (int j = 0; j < resol - 2; j++)			//incrementing 'j' represents vertical rising
	{
		for (int i = 0; i < resol * 2; i++)		//incrementing 'i' represents horizontal rotation
		{
			if (i < resol * 2 - 1)
			{
				if (tCount < totalPoly)
				{
					t[tCount].A = s[1 + (j * resol * 2) + i];
					t[tCount].B = s[1 + (j * resol * 2) + i + 1];
					t[tCount].C = s[1 + ((j + 1) * resol * 2) + i];
					//vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
					//vect3 temp = crossProduct(subVectors(t[tCount].A, t[tCount].B), subVectors(t[tCount].A, t[tCount].C));
					//t[tCount].N = unitVector(temp);
					t[tCount].N = unitVector(midPoint(t[tCount].B, t[tCount].C));
					t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
					t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
					t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));

					t[tCount].texture = texture;
					t[tCount].colour = colour;

					t[tCount].At.u = uvStepHor * (double(i));			t[tCount].At.v = uvStepVer * (double(j) + 1);
					t[tCount].Bt.u = uvStepHor * (double(i) + 1.0);		t[tCount].Bt.v = uvStepVer * (double(j) + 1);
					t[tCount].Ct.u = uvStepHor * (double(i));			t[tCount].Ct.v = uvStepVer * (double(j) + 2);

					tCount++;
				}
				if (tCount < totalPoly)
				{
					t[tCount].A = s[1 + (j * resol * 2) + i + 1];
					t[tCount].B = s[1 + ((j + 1) * resol * 2) + i + 1];
					t[tCount].C = s[1 + ((j + 1) * resol * 2) + i];
					//vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
					//vect3 temp = crossProduct(subVectors(t[tCount].A, t[tCount].B), subVectors(t[tCount].A, t[tCount].C));
					//t[tCount].N = unitVector(temp);
					t[tCount].N = unitVector(midPoint(t[tCount].A, t[tCount].C));
					t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
					t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
					t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));

					t[tCount].texture = texture;
					t[tCount].colour = colour;

					t[tCount].At.u = uvStepHor * (double(i) + 1.0);		t[tCount].At.v = uvStepVer * (double(j) + 1);
					t[tCount].Bt.u = uvStepHor * (double(i) + 1.0);		t[tCount].Bt.v = uvStepVer * (double(j) + 2);
					t[tCount].Ct.u = uvStepHor * (double(i));			t[tCount].Ct.v = uvStepVer * (double(j) + 2);

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
					vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));	//BACB
					t[tCount].N = unitVector(temp);
					t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
					t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
					t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));

					t[tCount].texture = texture;
					t[tCount].colour = colour;

					t[tCount].At.u = 1.0 - uvStepHor;					t[tCount].At.v = uvStepVer * (double(j) + 1);
					t[tCount].Bt.u = 1.0;								t[tCount].Bt.v = uvStepVer * (double(j) + 1);
					t[tCount].Ct.u = 1.0 - uvStepHor;					t[tCount].Ct.v = uvStepVer * (double(j) + 2);

					tCount++;
				}
				if (tCount < totalPoly)
				{
					t[tCount].A = s[1 + (j * resol * 2)];
					t[tCount].B = s[1 + (j * resol * 2) + i + 1];
					t[tCount].C = s[1 + ((j + 1) * resol * 2) + i];
					//vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
					vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
					t[tCount].N = unitVector(temp);
					t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
					t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
					t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));

					t[tCount].texture = texture;
					t[tCount].colour = colour;

					t[tCount].At.u = 1.0;								t[tCount].At.v = uvStepVer * (double(j) + 1);
					t[tCount].Bt.u = 1.0;								t[tCount].Bt.v = uvStepVer * (double(j) + 2);
					t[tCount].Ct.u = 1.0 - uvStepHor;					t[tCount].Ct.v = uvStepVer * (double(j) + 2);

					tCount++;
				}
			}
		}
	}

	//Adding triangles forming caps, North Pole

	for (int i = 0; i < resol * 2; i++)
	{
		if (i < resol * 2 - 1)
		{
			if (tCount < totalPoly)
			{
				t[tCount].A = s[(resol - 2) * 2 * resol + 1 + i];
				t[tCount].B = s[(resol - 2) * 2 * resol + 1 + i + 1];
				t[tCount].C = s[(resol - 1) * 2 * resol + 1];
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
				t[tCount].N = unitVector(temp);
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));

				t[tCount].texture = texture;
				t[tCount].colour = colour;

				t[tCount].At.u = uvStepHor * (double(i) + 0.0);		t[tCount].At.v = 1.0 - uvStepVer;
				t[tCount].Bt.u = uvStepHor * (double(i) + 1.0);		t[tCount].Bt.v = 1.0 - uvStepVer;
				t[tCount].Ct.u = uvStepHor * (double(i) + 0.5);		t[tCount].Ct.v = 1.0;

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
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
				t[tCount].N = unitVector(temp);
				t[tCount].An = unitVector(subVectors(t[tCount].A, centreV));
				t[tCount].Bn = unitVector(subVectors(t[tCount].B, centreV));
				t[tCount].Cn = unitVector(subVectors(t[tCount].C, centreV));

				t[tCount].texture = texture;
				t[tCount].colour = colour;

				t[tCount].At.u = 1.0 - 1.0 * uvStepHor;				t[tCount].At.v = 1.0 - uvStepVer;
				t[tCount].Bt.u = 1.0 - 0.0 * uvStepHor;				t[tCount].Bt.v = 1.0 - uvStepVer;
				t[tCount].Ct.u = 1.0 - 0.5 * uvStepHor;				t[tCount].Ct.v = 1.0;

				tCount++;
			}
		}
	}

	delete[] s;

	transformMesh(this->getTotalPoly(), t, scale.x, scale.y, scale.z, position.x, position.y, position.z, rotation.x, rotation.y, rotation.z);
}


vect3 SolidSphere::getPosition()
{
	return position;
}


void SolidSphere::constructShadowVolume(vect3)
{

}

