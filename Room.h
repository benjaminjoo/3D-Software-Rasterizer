#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class Room: public SolidBody
{

	double			sizeX				= 2.0f;
	double			sizeY				= 2.0f;
	double			sizeZ				= 2.0f;

	Uint32			colBottom			= 0x000000ff;
	Uint32			colTop				= 0x000000ff;
	Uint32			colBack				= 0x000000ff;
	Uint32			colFront			= 0x000000ff;
	Uint32			colLeft				= 0x000000ff;
	Uint32			colRight			= 0x000000ff;

	int				txtBottom			= 1;
	int				txtTop				= 1;
	int				txtBack				= 1;
	int				txtFront			= 1;
	int				txtLeft				= 1;
	int				txtRight			= 1;

	double			txUBottom			= 1.0f;
	double			txUTop				= 1.0f;
	double			txUBack				= 1.0f;
	double			txUFront			= 1.0f;
	double			txULeft				= 1.0f;
	double			txURight			= 1.0f;

	bool			isBottomOn			= true;
	bool			isTopOn				= true;
	bool			isBackOn			= true;
	bool			isFrontOn			= true;
	bool			isLeftOn			= true;
	bool			isRightOn			= true;

	bool			openingOnBottom		= false;
	bool			openingOnTop		= false;
	bool			openingOnBack		= false;
	bool			openingOnFront		= false;
	bool			openingOnLeft		= false;
	bool			openingOnRight		= false;

	vect3*			openingBottom		= nullptr;
	vect3*			openingTop			= nullptr;
	vect3*			openingBack			= nullptr;
	vect3*			openingFront		= nullptr;
	vect3*			openingLeft			= nullptr;
	vect3*			openingRight		= nullptr;

	std::vector<triangle3dV> bottomPolyContainer;
	std::vector<triangle3dV> topPolyContainer;
	std::vector<triangle3dV> backPolyContainer;
	std::vector<triangle3dV> frontPolyContainer;
	std::vector<triangle3dV> leftPolyContainer;
	std::vector<triangle3dV> rightPolyContainer;

public:

	Room();
	Room(double, double, double, double, double, double);
	~Room();

	void setPosition(double, double, double);
	void setDimension(double, double, double);
	void setRotation(double, double, double);
	void setColour(Uint32 bottom, Uint32 top, Uint32 back, Uint32 front, Uint32 left, Uint32 right);
	void setTexture(int bottom, int top, int back, int front, int left, int right);
	void setTextureScale(double bottom, double top, double back, double front, double left, double right);
	void setSideOn(bool bottom, bool top, bool back, bool front, bool left, bool right);

	void addOpening(Side, double, double, double, double);

	void addBottomPoly(triangle3dV);
	void addTopPoly(triangle3dV);
	void addBackPoly(triangle3dV);
	void addFrontPoly(triangle3dV);
	void addLeftPoly(triangle3dV);
	void addRightPoly(triangle3dV);

	int getTotalVert();
	int getTotalPoly();

	void breakUpFace(Side, polygon4uv*, polyNode*, vect3, Uint32, int);
	void splitPolygon(Side, polygon4uv* V, polyNode p, polyNode q);
	bool checkForHole(Side, polygon4uv*, vect3*);
	void traversePolygonTree(Side, polygon4uv*, vect3, Uint32, int);
	void calculateMesh();
	void getTriangleData(triangle3dV*);	
};

