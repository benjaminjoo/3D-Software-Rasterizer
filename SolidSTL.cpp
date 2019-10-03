#include <fstream>
#include <iostream>

#include "SolidSTL.h"


SolidSTL::SolidSTL(std::string fn)
{
	scale			= { 1.0, 1.0, 1.0, 1.0 };
	position		= { 0.0, 0.0, 0.0, 1.0 };
	rotation		= { 0.0, 0.0, 0.0, 1.0 };

	colour			= 255;
	texture			= 0;
	txU				= 1.0;

	castsShadows	= true;

	fileName		= fn;
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
}


SolidSTL::~SolidSTL()
{
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
	for (int i = 0; i < polyContainer.size(); i++)
	{
		P[i] = polyContainer[i];
	}
}


vect3 SolidSTL::getPosition()
{
	return position;
}


void SolidSTL::constructShadowVolume(vect3)
{

}


void SolidSTL::readSTLfile()
{
	std::ifstream modelFile(fileName.c_str(), std::ifstream::in | std::ifstream::binary);

	char headerInfo[80] = "";
	char nPoly[4] = "";
	unsigned long nPolyLong = 0;

	if (modelFile)
	{
		modelFile.read(headerInfo, 80);
		std::cout << headerInfo << std::endl;
	}
	else
	{
		std::cout << "Error reading.stl file..." << std::endl;
	}

	if (modelFile)
	{
		modelFile.read(nPoly, 4);
		nPolyLong = *((unsigned long*)nPoly);
		std::cout << "Number of polygons: " << nPolyLong << std::endl;
	}
	else
	{
		std::cout << "Error reading.stl file..." << std::endl;
	}

	char normal[12] = "";
	char vert_a[12] = "";
	char vert_b[12] = "";
	char vert_c[12] = "";
	char attrib[2]	= "";

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
		//std::cout << "\tnx: " << xf << "\tny: " << yf << "\tnz: " << zf << std::endl;
		tempPoly.N = tempPoly.An = tempPoly.Bn = tempPoly.Cn = { (double)xf, (double)yf, (double)zf, 0.0f };

		modelFile.read(vert_a, 12);
		xx[0] = vert_a[0];		xx[1] = vert_a[1];		xx[2] = vert_a[2];		xx[3] = vert_a[3];
		yy[0] = vert_a[4];		yy[1] = vert_a[5];		yy[2] = vert_a[6];		yy[3] = vert_a[7];
		zz[0] = vert_a[8];		zz[1] = vert_a[9];		zz[2] = vert_a[10];		zz[3] = vert_a[11];

		xf = *((float*)xx);
		yf = *((float*)yy);
		zf = *((float*)zz);
		//std::cout << "\tax: " << xf << "\tay: " << yf << "\taz: " << zf << std::endl;
		tempPoly.A = { (double)xf, (double)yf, (double)zf, 1.0f };

		modelFile.read(vert_b, 12);
		xx[0] = vert_b[0];		xx[1] = vert_b[1];		xx[2] = vert_b[2];		xx[3] = vert_b[3];
		yy[0] = vert_b[4];		yy[1] = vert_b[5];		yy[2] = vert_b[6];		yy[3] = vert_b[7];
		zz[0] = vert_b[8];		zz[1] = vert_b[9];		zz[2] = vert_b[10];		zz[3] = vert_b[11];

		xf = *((float*)xx);
		yf = *((float*)yy);
		zf = *((float*)zz);
		//std::cout << "\tbx: " << xf << "\tby: " << yf << "\tbz: " << zf << std::endl;
		tempPoly.B = { (double)xf, (double)yf, (double)zf, 1.0f };

		modelFile.read(vert_c, 12);
		xx[0] = vert_c[0];		xx[1] = vert_c[1];		xx[2] = vert_c[2];		xx[3] = vert_c[3];
		yy[0] = vert_c[4];		yy[1] = vert_c[5];		yy[2] = vert_c[6];		yy[3] = vert_c[7];
		zz[0] = vert_c[8];		zz[1] = vert_c[9];		zz[2] = vert_c[10];		zz[3] = vert_c[11];

		xf = *((float*)xx);
		yf = *((float*)yy);
		zf = *((float*)zz);
		//std::cout << "\tcx: " << xf << "\tcy: " << yf << "\tcz: " << zf << std::endl;
		tempPoly.C = { (double)xf, (double)yf, (double)zf, 1.0f };
		tempPoly.colour		= colour;
		tempPoly.texture	= texture;
		//std::cout << "\v" << std::endl;
		modelFile.read(attrib, 2);

		polyContainer.push_back(tempPoly);
	}
	polyContainer.shrink_to_fit();
}
