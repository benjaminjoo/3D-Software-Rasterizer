#pragma once

#include "Definitions.h"

class Voxel
{
	double x;
	double y;
	double z;

	double s;

	Uint32 c;

	bool front;
	bool back;
	bool left;
	bool right;
	bool top;
	bool bottom;

public:

	int posX;
	int posY;
	int posZ;

	Voxel();
	Voxel(double, double, double, int, int, int, Uint32);
	Voxel(double, double, double, int, int, int, double, Uint32);
	~Voxel();

	int				getTotalVert();
	int				getTotalPoly();
	triangle3dV*	getTriangleData();
	Uint32			getColour();
	unsigned char	getRedValue();
	unsigned char	getGreenValue();
	unsigned char	getBlueValue();
	int				countFaces();

	void			setFront(bool);
	void			setBack(bool);
	void			setLeft(bool);
	void			setRight(bool);
	void			setTop(bool);
	void			setBottom(bool);

	void updateX(double);
	void updateY(double);
	void updateZ(double);
	void updateC(Uint32);
	void updateS(double);
};

