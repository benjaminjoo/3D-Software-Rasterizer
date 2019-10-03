#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "Definitions.h"

using namespace std;

class Model
{
	string filename;

	double x;
	double y;
	double z;

	double scale;

	Uint32 colour;


public:
	Model();
	Model(string, double, double, double);
	Model(string, double, double, double, double, Uint32);
	~Model();

	int				getTotalPoly();
	triangle3dV*	getTriangleData();
	Uint32			getColour();
	unsigned char	getRedValue();
	unsigned char	getGreenValue();
	unsigned char	getBlueValue();
};

