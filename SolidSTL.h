#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "Definitions.h"
#include "SolidBody.h"		//Base class

class SolidSTL: public SolidBody
{
	std::string					fileName;
	std::vector<triangle3dV>	polyContainer;

public:
	SolidSTL(std::string);
	SolidSTL(double, double, double, Uint32, int, std::string);
	SolidSTL(double, double, double, double, double, double, double, double, double, Uint32, int, std::string);
	~SolidSTL();

	int				getTotalVert();
	int				getTotalPoly();
	void			getTriangleData_(triangle3dV*);
	vect3			getPosition();
	void			constructShadowVolume(vect3);

	void			readSTLfile();
};

