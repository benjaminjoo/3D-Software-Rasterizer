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
#include "PointCloud.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

class ModelElementBuffer
{
	std::string			fileName;

	std::vector<vertex3>		vertex3Buffer;
	std::vector<line3>			line3Buffer;
	std::vector<triangle3>		triangle3Buffer;
	std::vector<polyline3>		polyline3Buffer;
	std::vector<spline3>		spline3Buffer;
	std::vector<triangle3dV>	polyBuffer;

public:
	ModelElementBuffer(std::string);
	~ModelElementBuffer();

	void addVertex3(vertex3);
	void addPointCloud(std::shared_ptr<PointCloud>);
	void addLine3(line3);
	void addTriangle3(triangle3);
	void addTriangle3Vert(int, int, vertex3);
	void addPolyline3(polyline3);
	void addPolyline3ControlPoint(int, vertex3);
	void addSpline3(spline3);
	void addSpline3ControlPoint(int, vertex3);

	void selectVertex3byIndex(int);
	void deselectVertex3byIndex(int);
	void deleteVertex3byIndex(int);
	void moveVertex3byIndex(int, worldCoord);
	void copyVertex3byIndex(int, int, worldCoord);
	void rotVertex3byIndex(int, Side, worldCoord, float);

	void selectTriangleVertex3byIndex(int, int);
	void deselectTriangleVertex3byIndex(int, int);
	void moveTriangleVertex3byIndex(int, int, worldCoord);

	void selectPolylineControlVertex3byIndex(int, int);
	void deselectPolylineControlVertex3byIndex(int, int);
	void deletePolylineControlVertex3byIndex(int, int);
	void movePolylineControlVertex3byIndex(int, int, worldCoord);

	void selectSplineControlVertex3byIndex(int, int);
	void deselectSplineControlVertex3byIndex(int, int);
	void deleteSplineControlVertex3byIndex(int, int);
	void moveSplineControlVertex3byIndex(int, int, worldCoord);

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

	bool isTriangle3Selected(int);
	bool isTriangle3Deleted(int);
	int getTriangle3BufferSize();
	triangle3 getTriangle3(int);

	bool isPolyline3Selected(int);
	bool isPolyline3Deleted(int);
	int getPolyline3BufferSize();
	polyline3 getPolyline3(int);

	bool isSpline3Selected(int);
	bool isSpline3Deleted(int);
	int getSpline3BufferSize();
	spline3 getSpline3(int);

	void exportTextFile();
};

