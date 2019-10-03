#pragma once

#include "Definitions.h"
#include "Cube.h"
#include "Voxel.h"

class VoxelObject
{
	int		sizeX;
	int		sizeY;
	int		sizeZ;

	int		currentX;
	int		currentY;
	int		currentZ;

	double	unit;
	int		cursorColour;

	Uint32* voxelBuffer;

public:
	
	char	cursorState;

	VoxelObject();
	VoxelObject(int, int, int, double);
	VoxelObject(int, int, int, int, int, int, double);
	~VoxelObject();

	int getSizeX();
	int getSizeY();
	int getSizeZ();

	int getTotalSize();

	int getCurrentX();
	int getCurrentY();
	int getCurrentZ();

	int getColour();

	void stepCurrentX(int);
	void stepCurrentY(int);
	void stepCurrentZ(int);

	void stepColour(int);
	void setColour(int);

	double getUnitSize();

	void putVoxel(int, int, int, Uint32);
	void delVoxel(int, int, int);

	int countVoxels();

	//Cube* getCubeData();
	Voxel* getCubeData();

	int reportNSupport();
	triangle3dV* provideSupport();

	void faceCulling(Voxel*);

};

