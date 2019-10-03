#include "VoxelObject.h"
#include "Cube.h"
#include "Voxel.h"
#include <stdio.h>



VoxelObject::VoxelObject()
{
	sizeX		= 10;
	sizeY		= 10;
	sizeZ		= 10;

	voxelBuffer = new Uint32[1000];
	for (int i = 0; i < 1000; i++) { voxelBuffer[i] = 0; }

	currentX	= 0;
	currentY	= 0;
	currentZ	= 0;

	unit		= 1.0;

	cursorState = 1;
	cursorColour		= 255;
}


VoxelObject::VoxelObject(int sx, int sy, int sz, double a)
{
	sizeX		= sx;
	sizeY		= sy;
	sizeZ		= sz;

	voxelBuffer = new Uint32[sizeX * sizeY * sizeZ];
	for (int i = 0; i < sizeX * sizeY * sizeZ; i++) { voxelBuffer[i] = 0; }

	currentX	= 0;
	currentY	= 0;
	currentZ	= 0;

	unit		= a;

	cursorState = 1;
	cursorColour = 255;
}


VoxelObject::VoxelObject(int sx, int sy, int sz, int cx, int cy, int cz, double a)
{
	sizeX		= sx;
	sizeY		= sy;
	sizeZ		= sz;

	voxelBuffer = new Uint32[sizeX * sizeY * sizeZ];
	for (int i = 0; i < sizeX * sizeY * sizeZ; i++) { voxelBuffer[i] = 0; }

	currentX	= cx;
	currentY	= cy;
	currentZ	= cz;

	unit		= a;

	cursorState = 1;
	cursorColour = 255;
}


VoxelObject::~VoxelObject()
{
	delete[] voxelBuffer;
}


int VoxelObject::getSizeX()
{
	return sizeX;
}


int VoxelObject::getSizeY()
{
	return sizeY;
}


int VoxelObject::getSizeZ()
{
	return sizeZ;
}


int VoxelObject::getTotalSize()
{
	return sizeX * sizeY* sizeZ;
}


int VoxelObject::getCurrentX()
{
	return currentX;
}


int VoxelObject::getCurrentY()
{
	return currentY;
}


int VoxelObject::getCurrentZ()
{
	return currentZ;
}


int VoxelObject::getColour()
{
	return cursorColour;
}


void VoxelObject::stepCurrentX(int step)
{
	currentX += step;
}


void VoxelObject::stepCurrentY(int step)
{
	currentY += step;
}


void VoxelObject::stepCurrentZ(int step)
{
	currentZ += step;
}


void VoxelObject::stepColour(int step)
{
	cursorColour += step;
}


void VoxelObject::setColour(int c)
{
	cursorColour = c;
}


double VoxelObject::getUnitSize()
{
	return unit;
}


void VoxelObject::putVoxel(int x, int y, int z, Uint32 c)
{
	if (!voxelBuffer[z * sizeX * sizeY + y * sizeX + x])
	{
		voxelBuffer[z * sizeX * sizeY + y * sizeX + x] = c;
	}
}


void VoxelObject::delVoxel(int x, int y, int z)
{
	if (voxelBuffer[z * sizeX * sizeY + y * sizeX + x])
	{
		voxelBuffer[z * sizeX * sizeY + y * sizeX + x] = 0;
	}
}


int VoxelObject::countVoxels()
{
	Uint32 currentCube;
	int cubeCounter = 0;
	for (int z = 0; z < sizeZ; z++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			for (int x = 0; x < sizeX; x++)
			{
				currentCube = voxelBuffer[z * sizeX * sizeY + y * sizeX + x];
				if (currentCube)
				{
					cubeCounter++;
				}
			}
		}
	}
	return cubeCounter;
}


//Cube* VoxelObject::getCubeData()
Voxel* VoxelObject::getCubeData()
{
	int n = this->countVoxels();
	//Cube* cubes = new Cube[n];
	Voxel* cubes = new Voxel[n];
	Uint32 currentCube;
	int cubeCounter = 0;
	for (int cz = 0; cz < sizeZ; cz++)
	{
		for (int cy = 0; cy < sizeY; cy++)
		{
			for (int cx = 0; cx < sizeX; cx++)
			{
				currentCube = voxelBuffer[cz * sizeX * sizeY + cy * sizeX + cx];
				if (currentCube)
				{
					cubes[cubeCounter].posX = cx;
					cubes[cubeCounter].posY = cy;
					cubes[cubeCounter].posZ = cz;
					cubes[cubeCounter].updateX(cx * unit);
					cubes[cubeCounter].updateY(cy * unit);
					cubes[cubeCounter].updateZ(cz * unit);
					cubes[cubeCounter].updateC(currentCube);
					cubes[cubeCounter].updateS(unit);
					cubeCounter++;
				}
			}
		}
	}
	//this->faceCulling(cubes);

	return cubes;
	delete[] cubes;
}


int VoxelObject::reportNSupport()
{
	int nSupport = 0;
	int n = this->countVoxels();
	//Cube* cubes = new Cube[n];
	Voxel* cubes = new Voxel[n];
	cubes = this->getCubeData();
	for (int i = 0; i < n; i++)
	{
		triangle3dV* temp = new triangle3dV[12];
		temp = cubes[i].getTriangleData();
		for (int j = 0; j < cubes[i].countFaces(); j++)
		{
			if (temp[j].N.z >= 0.866) { nSupport++; }
		}
		delete[] temp;
	}

	printf("Number of supporting triangles: %d\n", nSupport);
	return nSupport;
	delete[] cubes;
}


triangle3dV* VoxelObject::provideSupport()
{
	int nSupport = this->reportNSupport();
	int n = this->countVoxels();
	//Cube* cubes = new Cube[n];
	Voxel* cubes = new Voxel[n];
	cubes = this->getCubeData();
	//this->faceCulling(cubes);

	triangle3dV* supports = new triangle3dV[nSupport];
	int sCnt = 0;
	for (int i = 0; i < n; i++)
	{
		triangle3dV* temp = new triangle3dV[12];
		temp = cubes[i].getTriangleData();
		for (int j = 0; j < 12; j++)
		{
			if (temp[j].N.z >= 0.866)
			{
				if (j < nSupport)
				{
					supports[sCnt] = temp[j];
					sCnt++;
				}
			}
		}
		delete[] temp;
	}

	return supports;
	delete[] cubes;
	delete[] supports;
}


void VoxelObject::faceCulling(Voxel* v)
{
	int n = this->countVoxels();
	//Voxel* v = new Voxel[n];
	//v = this->getCubeData();
	//v = voxelData;
	for (int i = 0; i < n; i++)
	{
		int x, y, z;
		x = v[i].posX;
		y = v[i].posY;
		z = v[i].posZ;
		if (x > 0)
		{
			if (voxelBuffer[z * sizeX * sizeY + y * sizeX + x - 1]) { v[i].setLeft(false); }
		}
		if (x < sizeX - 1)
		{
			if (voxelBuffer[z * sizeX * sizeY + y * sizeX + x + 1]) { v[i].setRight(false); }
		}
		if (y > 0)
		{
			if (voxelBuffer[z * sizeX * sizeY + (y - 1) * sizeX + x]) { v[i].setFront(false); }
		}
		if (y < sizeY - 1)
		{
			if (voxelBuffer[z * sizeX * sizeY + (y + 1) * sizeX + x]) { v[i].setBack(false); }
		}
		if (z > 0)
		{
			if (voxelBuffer[(z - 1) * sizeX * sizeY + y * sizeX + x]) { v[i].setBottom(false); }
		}
		if (z < sizeZ - 1)
		{
			if (voxelBuffer[(z + 1) * sizeX * sizeY + y * sizeX + x]) { v[i].setTop(false); }
		}
	}
	//delete[] v;
}
