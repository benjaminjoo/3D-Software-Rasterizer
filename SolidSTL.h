#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "Definitions.h"
#include "SolidBody.h"

class SolidSTL: public SolidBody
{
	std::string					fileName;
	std::vector<triangle3dV>	polyContainer;

	std::ifstream				modelFile;

	bool						centered	= true;
	float						rotZ		= -PI * 0.5f;

public:
	SolidSTL(std::string);
	SolidSTL(float, float, float, Uint32, int, std::string);
	SolidSTL(float, float, float, float, float, float, float, float, float, Uint32, int, std::string);
	~SolidSTL();

	virtual int	getTotalVert();
	virtual int	getTotalPoly();
	virtual void getTriangleData(triangle3dV*);

	void readSTLfile();
	void smoothSurfaces();
	void centreXY();

	bool match(const vect3& a, const vect3& b)
	{
		if (a.x - b.x == 0.0f &&
			a.y - b.y == 0.0f &&
			a.z - b.z == 0.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

