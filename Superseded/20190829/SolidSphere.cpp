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


void SolidSphere::getVertexData_(vect3* s)
{
	double angleV, angleH, stepV, stepH;
	stepV = 180.0 / resol;
	stepH = 360.0 / (double(resol) * 2);
	int cnt = 0;
	int total;
	total = 2 * resol * (resol - 1) + 2;
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
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
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
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
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

	for (int j = 0; j < resol - 2; j++)				//incrementing 'j' represents vertical rising
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
					vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
					t[tCount].N = unitVector(temp);
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
					vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
					t[tCount].N = unitVector(temp);
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
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
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
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
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

	return transformObject(this->getTotalPoly(), t, scaleX, scaleY, scaleZ, 0, 0, 0, rotX, rotY, rotZ);

	delete[] s;
	delete[] t;
}


void SolidSphere::getTriangleData_(triangle3dV* t)
{
	int totalVert, totalPoly;
	totalVert = 2 * resol * (resol - 1) + 2;
	totalPoly = 4 * resol * (resol - 1);
	vect3 centreV = { posX, posY, posZ, 1.0 };
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
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
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
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
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

	for (int j = 0; j < resol - 2; j++)				//incrementing 'j' represents vertical rising
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
					vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
					t[tCount].N = unitVector(temp);
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
					vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].A));
					t[tCount].N = unitVector(temp);
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
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
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
				vect3 temp = crossProduct(subVectors(t[tCount].B, t[tCount].A), subVectors(t[tCount].C, t[tCount].B));
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
}
