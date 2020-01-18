#include <fstream>
#include <iostream>

#include "SolidSTL.h"


SolidSTL::SolidSTL(std::string fn):
	fileName(fn)
{
	modelFile = std::ifstream(fileName.c_str(), std::ifstream::in | std::ifstream::binary);
	if (modelFile) { std::cout << "Model file succesfully opened..." << std::endl; }
}


SolidSTL::SolidSTL(double px, double py, double pz, Uint32 c, int t, std::string fn)
{
	scale			= { 1.0, 1.0, 1.0, 1.0 };
	position		= { px, py, pz, 1.0 };
	rotation		= { 0.0, 0.0, 0.0, 1.0 };

	colour			= c;
	texture			= t;
	txU				= 1.0;

	castsShadows	= true;

	fileName		= fn;

	modelFile = std::ifstream(fileName.c_str(), std::ifstream::in | std::ifstream::binary);
	if (modelFile) { std::cout << "Model file succesfully opened..." << std::endl; }
}


SolidSTL::SolidSTL(double sx, double sy, double sz, double px, double py, double pz, double rx, double ry, double rz, Uint32 c, int t, std::string fn)
{
	scale			= { sx, sy, sz, 1.0 };
	position		= { px, py, pz, 1.0 };
	rotation		= { rx, ry, rz, 1.0 };

	colour			= c;
	texture			= t;
	txU				= 1.0;

	castsShadows	= true;

	fileName		= fn;

	modelFile = std::ifstream(fileName.c_str(), std::ifstream::in | std::ifstream::binary);
	if (modelFile) { std::cout << "Model file succesfully opened..." << std::endl; }
}


SolidSTL::~SolidSTL()
{
	if (modelFile)
	{
		modelFile.close();
		std::cout << "Model file closed..." << std::endl;
	}
}


int	SolidSTL::getTotalVert()
{
	return 0;
}


int	SolidSTL::getTotalPoly()
{
	return polyContainer.size();
}


void SolidSTL::getTriangleData_(triangle3dV* P)
{
	for (unsigned int i = 0; i < polyContainer.size(); i++)
	{
		P[i] = polyContainer[i];
	}

	//transformMesh(polyContainer.size(), P, scale.x, scale.y, scale.z, position.x, position.y, position.z, rotation.x, rotation.y, rotation.z);
}


void SolidSTL::constructShadowVolume(vect3)
{

}


void SolidSTL::readSTLfile()
{
	if (modelFile)
	{
		char headerInfo[80] = "";
		char nPoly[4] = "";
		unsigned long nPolyLong = 0;

		modelFile.read(headerInfo, 80);
		std::cout << headerInfo << std::endl;

		modelFile.read(nPoly, 4);
		nPolyLong = *((unsigned long*)nPoly);
		std::cout << "Number of polygons: " << nPolyLong << std::endl;

		char normal[12] = "";
		char vert_a[12] = "";
		char vert_b[12] = "";
		char vert_c[12] = "";
		char attrib[2] = "";

		char xx[4] = "";
		char yy[4] = "";
		char zz[4] = "";

		float xf, yf, zf;

		triangle3dV tempPoly;

		for (unsigned long i = 0; i < nPolyLong; i++)
		{
			modelFile.read(normal, 12);

			xx[0] = normal[0];		xx[1] = normal[1];		xx[2] = normal[2];		xx[3] = normal[3];
			yy[0] = normal[4];		yy[1] = normal[5];		yy[2] = normal[6];		yy[3] = normal[7];
			zz[0] = normal[8];		zz[1] = normal[9];		zz[2] = normal[10];		zz[3] = normal[11];

			xf = *((float*)xx);
			yf = *((float*)yy);
			zf = *((float*)zz);

			tempPoly.N = tempPoly.An = tempPoly.Bn = tempPoly.Cn = { (double)xf, (double)yf, (double)zf, 0.0f };



			modelFile.read(vert_a, 12);

			xx[0] = vert_a[0];		xx[1] = vert_a[1];		xx[2] = vert_a[2];		xx[3] = vert_a[3];
			yy[0] = vert_a[4];		yy[1] = vert_a[5];		yy[2] = vert_a[6];		yy[3] = vert_a[7];
			zz[0] = vert_a[8];		zz[1] = vert_a[9];		zz[2] = vert_a[10];		zz[3] = vert_a[11];

			xf = *((float*)xx);
			yf = *((float*)yy);
			zf = *((float*)zz);

			tempPoly.A = { (double)xf * scale.x, (double)yf * scale.y, (double)zf * scale.z, 1.0f };



			modelFile.read(vert_b, 12);

			xx[0] = vert_b[0];		xx[1] = vert_b[1];		xx[2] = vert_b[2];		xx[3] = vert_b[3];
			yy[0] = vert_b[4];		yy[1] = vert_b[5];		yy[2] = vert_b[6];		yy[3] = vert_b[7];
			zz[0] = vert_b[8];		zz[1] = vert_b[9];		zz[2] = vert_b[10];		zz[3] = vert_b[11];

			xf = *((float*)xx);
			yf = *((float*)yy);
			zf = *((float*)zz);

			tempPoly.B = { (double)xf * scale.x, (double)yf * scale.y, (double)zf * scale.z, 1.0f };



			modelFile.read(vert_c, 12);

			xx[0] = vert_c[0];		xx[1] = vert_c[1];		xx[2] = vert_c[2];		xx[3] = vert_c[3];
			yy[0] = vert_c[4];		yy[1] = vert_c[5];		yy[2] = vert_c[6];		yy[3] = vert_c[7];
			zz[0] = vert_c[8];		zz[1] = vert_c[9];		zz[2] = vert_c[10];		zz[3] = vert_c[11];

			xf = *((float*)xx);
			yf = *((float*)yy);
			zf = *((float*)zz);

			tempPoly.C = { (double)xf * scale.x, (double)yf * scale.y, (double)zf * scale.z, 1.0f };



			tempPoly.colour = colour;
			tempPoly.texture = texture;

			modelFile.read(attrib, 2);

			polyContainer.push_back(tempPoly);
		}
		polyContainer.shrink_to_fit();

	}
	else
	{
		std::cout << "Error reading.stl file..." << std::endl;
	}
}


void SolidSTL::smoothSurfaces()
{
	for (auto& p : polyContainer)
	{
		vect3 accA, accB, accC;

		accA = accB = accC = { 0.0f, 0.0f, 0.0f, 0.0f };

		int neighbourCountA = 0, neighbourCountB = 0, neighbourCountC = 0;

		//Vertex A

		for (auto& a : polyContainer)
		{
			if ((match(p.A, a.A) || match(p.A, a.B) || match(p.A, a.C)) && dotProduct(p.N, a.N) > 0.5f)
			{
				accA = addVectors(accA, a.N);
				neighbourCountA++;
				if (neighbourCountA >= 5)
					break;
			}
		}

		//Vertex B

		for (auto& b : polyContainer)
		{
			if ((match(p.B, b.A) || match(p.B, b.B) || match(p.B, b.C)) && dotProduct(p.N, b.N) > 0.5f)
			{
				accB = addVectors(accB, b.N);
				neighbourCountB++;
				if (neighbourCountB >= 5)
					break;
			}
		}

		//Vertex C

		for (auto& c : polyContainer)
		{
			if ((match(p.C, c.A) || match(p.C, c.B) || match(p.C, c.C)) && dotProduct(p.N, c.N) > 0.5f)
			{
				accC = addVectors(accC, c.N);
				neighbourCountC++;
				if (neighbourCountC >= 5)
					break;
			}
		}

		p.An = unitVector(accA);
		p.Bn = unitVector(accB);
		p.Cn = unitVector(accC);
	}
}
