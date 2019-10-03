#pragma once

#include "Definitions.h"
#include "SolidBody.h"

class Room: public SolidBody
{

	double			sizeX;
	double			sizeY;
	double			sizeZ;

	Uint32			colBottom;
	Uint32			colTop;
	Uint32			colBack;
	Uint32			colFront;
	Uint32			colLeft;
	Uint32			colRight;

	int				txtBottom;
	int				txtTop;
	int				txtBack;
	int				txtFront;
	int				txtLeft;
	int				txtRight;

	double			txUBottom;
	double			txUTop;
	double			txUBack;
	double			txUFront;
	double			txULeft;
	double			txURight;

	bool			isBottomOn;
	bool			isTopOn;
	bool			isBackOn;
	bool			isFrontOn;
	bool			isLeftOn;
	bool			isRightOn;

	bool			openingOnBottom;
	bool			openingOnTop;
	bool			openingOnBack;
	bool			openingOnFront;
	bool			openingOnLeft;
	bool			openingOnRight;

	vect3*			openingBottom;
	vect3*			openingTop;
	vect3*			openingBack;
	vect3*			openingFront;
	vect3*			openingLeft;
	vect3*			openingRight;

	std::vector<triangle3dV> bottomPolyContainer;
	std::vector<triangle3dV> topPolyContainer;
	std::vector<triangle3dV> backPolyContainer;
	std::vector<triangle3dV> frontPolyContainer;
	std::vector<triangle3dV> leftPolyContainer;
	std::vector<triangle3dV> rightPolyContainer;

public:

	Room();
	Room(double, double, double, double, double, double, double, double, double, Uint32*, int*);
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
	//triangle3dV* getTriangleData();
	void getTriangleData_(triangle3dV*);	
	vect3 getPosition();
};

