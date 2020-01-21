#pragma once
#include "Definitions.h"
#include "SolidBody.h"
#include "SolidCube.h"
#include "SolidPrism.h"
#include "SolidCylinder.h"
#include "SolidCone.h"
#include "SolidSphere.h"
#include "Room.h"
#include "SolidSTL.h"
#include <iostream>
#include <vector>
#include <string>

class ModelElementBuffer
{
	std::string			fileName;

	std::vector<vertex3>		vertex3Buffer;
	std::vector<line3>			line3Buffer;
	std::vector<triangle3dV>	polyBuffer;

public:
	ModelElementBuffer(std::string);
	~ModelElementBuffer();

	void addVertex3(vertex3);
	void addLine3(line3);

	void selectVertex3byID(int);
	void deselectVertex3byID(int);
	void deleteVertex3byID(int);
	void moveVertex3byID(int, worldCoord);
	void copyVertex3byID(int, int, worldCoord);
	void rotVertex3byID(int, Side, worldCoord, float);

	void selectVertex3byIndex(int);
	void deselectVertex3byIndex(int);
	void deleteVertex3byIndex(int);
	void moveVertex3byIndex(int, worldCoord);
	void copyVertex3byIndex(int, int, worldCoord);
	void rotVertex3byIndex(int, Side, worldCoord, float);

	void selectLine3byIndex(int);
	void deselectLine3byIndex(int);
	void deleteLine3byIndex(int);
	void moveLine3byIndex(int, worldCoord);
	void moveLine3EndPoint(int, int, Side, worldCoord);
	void rotLine3byIndex(int, Side, worldCoord, float);

	bool isVertex3Selected(int);
	bool isVertex3Deleted(int);
	int getVertex3BufferSize();
	vertex3 getVertex3(int);

	bool isLine3Selected(int);
	bool isLine3Deleted(int);
	int getLine3BufferSize();
	line3 getLine3(int);

	void exportTextFile();
};

