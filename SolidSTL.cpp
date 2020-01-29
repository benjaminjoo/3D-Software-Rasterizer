#include <fstream>
#include <iostream>

#include "SolidSTL.h"


SolidSTL::SolidSTL(std::string fn) :
	fileName(fn)
{
	modelFile = std::ifstream(fileName.c_str(), std::ifstream::in | std::ifstream::binary);
	if (modelFile)
	{
		std::cout << "Model file succesfully opened..." << std::endl;
		readSTLfile();
	}
}


SolidSTL::SolidSTL(float px, float py, float pz, Uint32 c, int t, std::string fn)
{
	scale = { 1.0f, 1.0f, 1.0f, 1.0f };
	position = { px, py, pz, 1.0f };
	rotation = { 0.0f, 0.0f, 0.0f, 1.0f };

	colour = c;
	texture = t;
	txU = 1.0f;

	castsShadows = true;

	fileName = fn;

	modelFile = std::ifstream(fileName.c_str(), std::ifstream::in | std::ifstream::binary);
	if (modelFile)
	{
		std::cout << "Model file succesfully opened..." << std::endl;
		readSTLfile();
	}
}


SolidSTL::SolidSTL(float sx, float sy, float sz, float px, float py, float pz, float rx, float ry, float rz, Uint32 c, int t, std::string fn)
{
	scale = { sx, sy, sz, 1.0f };
	position = { px, py, pz, 1.0f };
	rotation = { rx, ry, rz, 1.0f };

	colour = c;
	texture = t;
	txU = 1.0;

	castsShadows = true;

	fileName = fn;

	modelFile = std::ifstream(fileName.c_str(), std::ifstream::in | std::ifstream::binary);
	if (modelFile)
	{
		std::cout << "Model file succesfully opened..." << std::endl;
		readSTLfile();
	}
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


void SolidSTL::getTriangleData(triangle3dV* P)
{
	smoothSurfaces();

	for (unsigned int i = 0; i < polyContainer.size(); i++)
	{
		P[i] = polyContainer[i];
	}

	Projector->transformMesh(nPoly, P, scale.x, scale.y, scale.z,
		position.x, position.y, position.z,
		rotation.x, rotation.y, rotation.z);
}


void SolidSTL::readSTLfile()
{
	if (modelFile)
	{
		char headerInfo[80] = "";
		char nPolygon[4] = "";
		unsigned long nPolyLong = 0;

		modelFile.read(headerInfo, 80);
		std::cout << headerInfo << std::endl;

		modelFile.read(nPolygon, 4);
		nPolyLong = *((unsigned long*)nPolygon);
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

			tempPoly.N = tempPoly.An = tempPoly.Bn = tempPoly.Cn = { (float)xf, (float)yf, (float)zf, 0.0f };



			modelFile.read(vert_a, 12);

			xx[0] = vert_a[0];		xx[1] = vert_a[1];		xx[2] = vert_a[2];		xx[3] = vert_a[3];
			yy[0] = vert_a[4];		yy[1] = vert_a[5];		yy[2] = vert_a[6];		yy[3] = vert_a[7];
			zz[0] = vert_a[8];		zz[1] = vert_a[9];		zz[2] = vert_a[10];		zz[3] = vert_a[11];

			xf = *((float*)xx);
			yf = *((float*)yy);
			zf = *((float*)zz);

			//tempPoly.A = { (float)xf * scale.x, (float)yf * scale.y, (float)zf * scale.z, 1.0f };
			tempPoly.A = { (float)xf, (float)yf, (float)zf, 1.0f };



			modelFile.read(vert_b, 12);

			xx[0] = vert_b[0];		xx[1] = vert_b[1];		xx[2] = vert_b[2];		xx[3] = vert_b[3];
			yy[0] = vert_b[4];		yy[1] = vert_b[5];		yy[2] = vert_b[6];		yy[3] = vert_b[7];
			zz[0] = vert_b[8];		zz[1] = vert_b[9];		zz[2] = vert_b[10];		zz[3] = vert_b[11];

			xf = *((float*)xx);
			yf = *((float*)yy);
			zf = *((float*)zz);

			//tempPoly.B = { (float)xf * scale.x, (float)yf * scale.y, (float)zf * scale.z, 1.0f };
			tempPoly.B = { (float)xf, (float)yf, (float)zf, 1.0f };



			modelFile.read(vert_c, 12);

			xx[0] = vert_c[0];		xx[1] = vert_c[1];		xx[2] = vert_c[2];		xx[3] = vert_c[3];
			yy[0] = vert_c[4];		yy[1] = vert_c[5];		yy[2] = vert_c[6];		yy[3] = vert_c[7];
			zz[0] = vert_c[8];		zz[1] = vert_c[9];		zz[2] = vert_c[10];		zz[3] = vert_c[11];

			xf = *((float*)xx);
			yf = *((float*)yy);
			zf = *((float*)zz);

			//tempPoly.C = { (float)xf * scale.x, (float)yf * scale.y, (float)zf * scale.z, 1.0f };
			tempPoly.C = { (float)xf, (float)yf, (float)zf, 1.0f };



			tempPoly.colour = colour;
			tempPoly.texture = texture;

			modelFile.read(attrib, 2);

			polyContainer.push_back(tempPoly);
		}
		polyContainer.shrink_to_fit();

		if (centered)
			centreXY();

		nPoly = getTotalPoly();
		mesh = new triangle3dV[nPoly];
		getTriangleData(mesh);
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


void SolidSTL::centreXY()
{
	if (polyContainer.size())
	{
		float xMin = polyContainer[0].A.x;
		float xMax = polyContainer[0].A.x;
		float yMin = polyContainer[0].A.y;
		float yMax = polyContainer[0].A.y;
		float zMin = polyContainer[0].A.z;
		float zMax = polyContainer[0].A.z;

		for (auto& p : polyContainer)
		{
			if (p.A.x < xMin)
				xMin = p.A.x;
			if (p.A.x > xMax)
				xMax = p.A.x;

			if (p.A.y < yMin)
				yMin = p.A.y;
			if (p.A.y > yMax)
				yMax = p.A.y;
		}

		float deltaX = (xMin + xMax) * 0.5f;
		float deltaY = (yMin + yMax) * 0.5f;

		float tempX, tempY;

		for (auto& p : polyContainer)
		{
			p.A.x -= deltaX;
			p.A.y -= deltaY;
			tempX = p.A.x * cos(rotZ) - p.A.y * sin(rotZ);
			tempY = p.A.y * cos(rotZ) + p.A.x * sin(rotZ);
			p.A.x = tempX;
			p.A.y = tempY;

			p.B.x -= deltaX;
			p.B.y -= deltaY;
			tempX = p.B.x * cos(rotZ) - p.B.y * sin(rotZ);
			tempY = p.B.y * cos(rotZ) + p.B.x * sin(rotZ);
			p.B.x = tempX;
			p.B.y = tempY;

			p.C.x -= deltaX;
			p.C.y -= deltaY;
			tempX = p.C.x * cos(rotZ) - p.C.y * sin(rotZ);
			tempY = p.C.y * cos(rotZ) + p.C.x * sin(rotZ);
			p.C.x = tempX;
			p.C.y = tempY;

			tempX = p.An.x * cos(rotZ) - p.An.y * sin(rotZ);
			tempY = p.An.y * cos(rotZ) + p.An.x * sin(rotZ);
			p.An.x = tempX;
			p.An.y = tempY;

			tempX = p.Bn.x * cos(rotZ) - p.Bn.y * sin(rotZ);
			tempY = p.Bn.y * cos(rotZ) + p.Bn.x * sin(rotZ);
			p.Bn.x = tempX;
			p.Bn.y = tempY;

			tempX = p.Cn.x * cos(rotZ) - p.Cn.y * sin(rotZ);
			tempY = p.Cn.y * cos(rotZ) + p.Cn.x * sin(rotZ);
			p.Cn.x = tempX;
			p.Cn.y = tempY;

			tempX = p.N.x * cos(rotZ) - p.N.y * sin(rotZ);
			tempY = p.N.y * cos(rotZ) + p.N.x * sin(rotZ);
			p.N.x = tempX;
			p.N.y = tempY;
		}
	}
}