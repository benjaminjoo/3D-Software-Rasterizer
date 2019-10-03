#pragma once

#include "Definitions.h"

class Cube
{
	double x;
	double y;
	double z;

	double s;

	Uint32 c;

	double		scaleX;
	double		scaleY;
	double		scaleZ;

	double		moveX;
	double		moveY;
	double		moveZ;

	double		rotX;
	double		rotY;
	double		rotZ;

public:
	Cube();
	Cube(double, double, double, Uint32);
	Cube(double, double, double, double, Uint32);
	~Cube();

	int				getTotalVert();
	int				getTotalPoly();
	triangle3dV*	getTriangleData();
	Uint32			getColour();

	//void updateX(double);
	//void updateY(double);
	//void updateZ(double);
	//void updateC(Uint32);
	//void updateS(double);

	//void setScaleX(double);
	//void setScaleY(double);
	//void setScaleZ(double);

	//void setMoveX(double);
	//void setMoveY(double);
	//void setMoveZ(double);

	//void setRotX(double);
	//void setRotY(double);
	//void setRotZ(double);

	void setAllTransforms(double, double, double, double, double, double, double, double, double);
};

