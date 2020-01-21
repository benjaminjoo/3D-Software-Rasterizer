#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class Room: public SolidBody
{

	float			sizeX				= 2.0f;
	float			sizeY				= 2.0f;
	float			sizeZ				= 2.0f;

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

	float			txUBottom			= 1.0f;
	float			txUTop				= 1.0f;
	float			txUBack				= 1.0f;
	float			txUFront			= 1.0f;
	float			txULeft				= 1.0f;
	float			txURight			= 1.0f;

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
	Room(float, float, float, float, float, float);
	~Room();

	void setPosition(float, float, float);
	void setDimension(float, float, float);
	void setRotation(float, float, float);
	void setColour(Uint32 bottom, Uint32 top, Uint32 back, Uint32 front, Uint32 left, Uint32 right);
	void setTexture(int bottom, int top, int back, int front, int left, int right);
	void setTextureScale(float bottom, float top, float back, float front, float left, float right);
	void setSideOn(bool bottom, bool top, bool back, bool front, bool left, bool right);

	void addOpening(Side, float, float, float, float);

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

