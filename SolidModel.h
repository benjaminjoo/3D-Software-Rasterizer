#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "Definitions.h"
#include "SolidBody.h"

class SolidModel: public SolidBody
{
	std::string					fileName;
	std::vector<triangle3dV>	polyContainer;

	std::ifstream				importFile;

public:
	SolidModel();
	SolidModel(double, double, double, Uint32, int);
	SolidModel(double, double, double, double, double, double, double, double, double, Uint32, int);
	~SolidModel();

	virtual int				getTotalVert();
	virtual int				getTotalPoly();
	virtual void			getTriangleData_(triangle3dV*);
	virtual vect3			getPosition();
	virtual void			constructShadowVolume(vect3);

	void			readModelFile(const std::string& fileName);

	void logPolyContainerSize();
};

