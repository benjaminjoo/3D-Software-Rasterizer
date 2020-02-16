#include "DynamicMesh.h"


DynamicMesh::DynamicMesh(int x, int y, float f, float s, float u, float a, Uint32 c, bool side) :
	width(x), height(y), frequency(f), speed(s), unit(u), amplitude(a), colour(c), sidesOn(side)
{
	size = width * height;
	pGrid = new point3[size];
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			point3 temp;
			temp.colour = colour;
			temp.P = { i * unit, j * unit, 0.0f, 1.0f };
			pGrid[j * width + i] = temp;
		}
	}

	polyCount = (width - 1) * (height - 1) * 2;
	mesh = new triangle3dV[polyCount];
	for (int i = 0; i < polyCount; i++)
	{
		mesh[i].colour = colour;
		mesh[i].texture = 0;
		if (i < polyCount / 2)
		{
			mesh[i].At = { 0.0f, 0.0f };
			mesh[i].Bt = { 0.0f, 1.0f };
			mesh[i].Ct = { 1.0f, 1.0f };
		}
		else
		{
			mesh[i].At = { 0.0f, 0.0f };
			mesh[i].Bt = { 1.0f, 1.0f };
			mesh[i].Ct = { 1.0f, 0.0f };
		}
	}

	polyCountS = (width - 1) * 4 + (height - 1) * 4 + 2;
	sideMesh = new triangle3dV[polyCountS];
	for (int i = 0; i < polyCountS; i++)
	{
		sideMesh[i].colour = colour;
	}
}


DynamicMesh::~DynamicMesh()
{
	delete[] pGrid;
	delete[] mesh;
	delete[] sideMesh;
}


void DynamicMesh::updateMesh()
{
	//Surface - A

	for (int j = 0; j < height - 1; j++)
	{
		for (int i = 0; i < width - 1; i++)
		{
			vect3 A = pGrid[j * width + i].P;
			vect3 B = pGrid[(j + 1) * width + i].P;
			vect3 C = pGrid[(j + 1) * width + i + 1].P;
			vect3 N = ((A - B) ^ (C - B)).norm();

			mesh[j * (width - 1) + i].A		= A;
			mesh[j * (width - 1) + i].B		= B;
			mesh[j * (width - 1) + i].C		= C;
			mesh[j * (width - 1) + i].An	= N;
			mesh[j * (width - 1) + i].Bn	= N;
			mesh[j * (width - 1) + i].Cn	= N;
			mesh[j * (width - 1) + i].N		= N;
		}
	}
	
	//Surface - B

	for (int j = 0; j < height - 1; j++)
	{
		for (int i = 0; i < width - 1; i++)
		{
			vect3 A = pGrid[j * width + i].P;
			vect3 B = pGrid[(j + 1) * width + i + 1].P;
			vect3 C = pGrid[j * width + i + 1].P;
			vect3 N = ((B - C) ^ (A - C)).norm();

			mesh[polyCount / 2 + j * (width - 1) + i].A		= A;
			mesh[polyCount / 2 + j * (width - 1) + i].B		= B;
			mesh[polyCount / 2 + j * (width - 1) + i].C		= C;
			mesh[polyCount / 2 + j * (width - 1) + i].An	= N;
			mesh[polyCount / 2 + j * (width - 1) + i].Bn	= N;
			mesh[polyCount / 2 + j * (width - 1) + i].Cn	= N;
			mesh[polyCount / 2 + j * (width - 1) + i].N		= N;
		}
	}

	//Sides

	for (int i = 0; i < (width - 1); i++)
	{
		vect3 A = pGrid[i].P;
		vect3 B = { i * unit, 0.0f, depth, 1.0f };
		vect3 C = pGrid[i + 1].P;
		vect3 N = { 0.0f, -1.0f, 0.0f, 1.0f };

		sideMesh[i].A	= A;
		sideMesh[i].B	= B;
		sideMesh[i].C	= C;
		sideMesh[i].An	= N;
		sideMesh[i].Bn	= N;
		sideMesh[i].Cn	= N;
		sideMesh[i].N	= N;
	}

	for (int i = 0; i < (width - 1); i++)
	{
		vect3 A = pGrid[i + 1].P;
		vect3 B = { i * unit, 0.0f, depth, 1.0f };
		vect3 C = { (i + 1) * unit, 0.0f, depth, 1.0f };
		vect3 N = { 0.0f, -1.0f, 0.0f, 0.0f };
	
		sideMesh[width - 1 + i].A	= A;
		sideMesh[width - 1 + i].B	= B;
		sideMesh[width - 1 + i].C	= C;
		sideMesh[width - 1 + i].An	= N;
		sideMesh[width - 1 + i].Bn	= N;
		sideMesh[width - 1 + i].Cn	= N;
		sideMesh[width - 1 + i].N	= N;
	}

	for (int j = 0; j < (height - 1); j++)
	{
		vect3 A = pGrid[(width - 1) * (j + 1) + j].P;
		vect3 B = { (width - 1) * unit, j * unit, depth, 1.0f };
		vect3 C = pGrid[(width - 1) * (j + 2) + j + 1].P;
		vect3 N = { 1.0f, 0.0f, 0.0f, 0.0f };

		sideMesh[(width - 1) * 2 + j].A		= A;
		sideMesh[(width - 1) * 2 + j].B		= B;
		sideMesh[(width - 1) * 2 + j].C		= C;
		sideMesh[(width - 1) * 2 + j].An	= N;
		sideMesh[(width - 1) * 2 + j].Bn	= N;
		sideMesh[(width - 1) * 2 + j].Cn	= N;
		sideMesh[(width - 1) * 2 + j].N		= N;
	}

	for (int j = 0; j < (height - 1); j++)
	{
		vect3 A = pGrid[(width - 1) * (j + 2) + j + 1].P;
		vect3 B = { (width - 1) * unit, j * unit, depth, 1.0f };
		vect3 C = { (width - 1) * unit, (j + 1) * unit, depth, 1.0f };
		vect3 N = { 1.0f, 0.0f, 0.0f, 0.0f };

		sideMesh[(width - 1) * 2 + (height - 1) + j].A	= A;
		sideMesh[(width - 1) * 2 + (height - 1) + j].B	= B;
		sideMesh[(width - 1) * 2 + (height - 1) + j].C	= C;
		sideMesh[(width - 1) * 2 + (height - 1) + j].An = N;
		sideMesh[(width - 1) * 2 + (height - 1) + j].Bn = N;
		sideMesh[(width - 1) * 2 + (height - 1) + j].Cn = N;
		sideMesh[(width - 1) * 2 + (height - 1) + j].N	= N;
	}

	for (int i = 0; i < (width - 1); i++)
	{
		vect3 A = pGrid[width * height - 1 - i].P;
		vect3 B = { (width - 1 - i) * unit, (height - 1) * unit, depth, 1.0f };
		vect3 C = pGrid[width * height - 2 - i].P;
		vect3 N = { 0.0f, 1.0f, 0.0f, 1.0f };

		sideMesh[(width - 1) * 2 + (height - 1) * 2 + i].A	= A;
		sideMesh[(width - 1) * 2 + (height - 1) * 2 + i].B	= B;
		sideMesh[(width - 1) * 2 + (height - 1) * 2 + i].C	= C;
		sideMesh[(width - 1) * 2 + (height - 1) * 2 + i].An = N;
		sideMesh[(width - 1) * 2 + (height - 1) * 2 + i].Bn = N;
		sideMesh[(width - 1) * 2 + (height - 1) * 2 + i].Cn = N;
		sideMesh[(width - 1) * 2 + (height - 1) * 2 + i].N	= N;
	}

	for (int i = 0; i < (width - 1); i++)
	{
		vect3 A = pGrid[width * height - 2 - i].P;
		vect3 B = { (width - 1 - i) * unit, (height - 1) * unit, depth, 1.0f };
		vect3 C = { (width - 2 - i) * unit, (height - 1) * unit, depth, 1.0f };
		vect3 N = { 0.0f, 1.0f, 0.0f, 1.0f };

		sideMesh[(width - 1) * 3 + (height - 1) * 2 + i].A	= A;
		sideMesh[(width - 1) * 3 + (height - 1) * 2 + i].B	= B;
		sideMesh[(width - 1) * 3 + (height - 1) * 2 + i].C	= C;
		sideMesh[(width - 1) * 3 + (height - 1) * 2 + i].An = N;
		sideMesh[(width - 1) * 3 + (height - 1) * 2 + i].Bn = N;
		sideMesh[(width - 1) * 3 + (height - 1) * 2 + i].Cn = N;
		sideMesh[(width - 1) * 3 + (height - 1) * 2 + i].N	= N;
	}

	for (int j = 0; j < (height - 1); j++)
	{
		vect3 A = pGrid[width * (height - 1 - j)].P;
		vect3 B = { 0.0f, (height - 1 - j) * unit, depth, 1.0f };
		vect3 C = pGrid[width * (height - 2 - j)].P;
		vect3 N = { -1.0f, 0.0f, 0.0f, 1.0f };

		sideMesh[(width - 1) * 4 + (height - 1) * 2 + j].A	= A;
		sideMesh[(width - 1) * 4 + (height - 1) * 2 + j].B	= B;
		sideMesh[(width - 1) * 4 + (height - 1) * 2 + j].C	= C;
		sideMesh[(width - 1) * 4 + (height - 1) * 2 + j].An = N;
		sideMesh[(width - 1) * 4 + (height - 1) * 2 + j].Bn = N;
		sideMesh[(width - 1) * 4 + (height - 1) * 2 + j].Cn = N;
		sideMesh[(width - 1) * 4 + (height - 1) * 2 + j].N	= N;
	}

	for (int j = 0; j < (height - 1); j++)
	{
		vect3 A = pGrid[width * (height - 2 - j)].P;
		vect3 B = { 0.0f, (height - 1 - j) * unit, depth, 1.0f };
		vect3 C = { 0.0f, (height - 2 - j) * unit, depth, 1.0f };
		vect3 N = { -1.0f, 0.0f, 0.0f, 1.0f };

		sideMesh[(width - 1) * 4 + (height - 1) * 3 + j].A	= A;
		sideMesh[(width - 1) * 4 + (height - 1) * 3 + j].B	= B;
		sideMesh[(width - 1) * 4 + (height - 1) * 3 + j].C	= C;
		sideMesh[(width - 1) * 4 + (height - 1) * 3 + j].An = N;
		sideMesh[(width - 1) * 4 + (height - 1) * 3 + j].Bn = N;
		sideMesh[(width - 1) * 4 + (height - 1) * 3 + j].Cn = N;
		sideMesh[(width - 1) * 4 + (height - 1) * 3 + j].N	= N;
	}

	//Bottom

	vect3 A = { 0.0f, 0.0f, depth, 1.0f };
	vect3 B = { 0.0f, (height - 1) * unit, depth, 1.0f };
	vect3 C = { (width - 1) * unit, (height - 1) * unit, depth, 1.0f };
	vect3 N = { 0.0f, 0.0f, -1.0f, 1.0f };

	sideMesh[(width - 1) * 4 + (height - 1) * 4 + 0].A	= A;
	sideMesh[(width - 1) * 4 + (height - 1) * 4 + 0].B	= B;
	sideMesh[(width - 1) * 4 + (height - 1) * 4 + 0].C	= C;
	sideMesh[(width - 1) * 4 + (height - 1) * 4 + 0].An = N;
	sideMesh[(width - 1) * 4 + (height - 1) * 4 + 0].Bn = N;
	sideMesh[(width - 1) * 4 + (height - 1) * 4 + 0].Cn = N;
	sideMesh[(width - 1) * 4 + (height - 1) * 4 + 0].N	= N;


	A = { 0.0f, 0.0f, depth, 1.0f };
	B = { (width - 1) * unit, (height - 1) * unit, depth, 1.0f };
	C = { (width - 1) * unit, 0.0f, depth, 1.0f };
	N = { 0.0f, 0.0f, -1.0f, 1.0f };

	sideMesh[(width - 1) * 4 + (height - 1) * 4 + 1].A	= A;
	sideMesh[(width - 1) * 4 + (height - 1) * 4 + 1].B	= B;
	sideMesh[(width - 1) * 4 + (height - 1) * 4 + 1].C	= C;
	sideMesh[(width - 1) * 4 + (height - 1) * 4 + 1].An = N;
	sideMesh[(width - 1) * 4 + (height - 1) * 4 + 1].Bn = N;
	sideMesh[(width - 1) * 4 + (height - 1) * 4 + 1].Cn = N;
	sideMesh[(width - 1) * 4 + (height - 1) * 4 + 1].N	= N;
}


void DynamicMesh::update()
{
	tick += speed;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			float index = tick + static_cast<float>(i + j);
			phase = index * frequency;
			pGrid[j * width + i].P.z = sin(phase * PI / 180.0f) * amplitude;
		}
	}
	updateMesh();
}


void DynamicMesh::renderGrid(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen)
{
	for (int i = 0; i < size; i++)
		eye->renderPoint(1, pGrid[i], screen->pixelBuffer, screen->depthBuffer);
}


void DynamicMesh::renderMesh(std::shared_ptr<Camera> eye, LightSource sun,
				const projectionStyle& visualStyle, float torchIntensity, float maxIllumination)
{
	eye->renderMesh(polyCount, mesh, nullptr, sun, visualStyle, torchIntensity, maxIllumination);
	eye->renderMesh(polyCountS, sideMesh, nullptr, sun, visualStyle, torchIntensity, maxIllumination);
}
