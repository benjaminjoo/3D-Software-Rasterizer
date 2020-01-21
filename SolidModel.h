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
	SolidModel(float, float, float, Uint32, int);
	SolidModel(float, float, float, float, float, float, float, float, float, Uint32, int);
	~SolidModel();

	virtual int	getTotalVert();
	virtual int	getTotalPoly();
	virtual void getTriangleData(triangle3dV*);

	void readModelFile(const std::string& fileName);

	void logPolyContainerSize();
};

