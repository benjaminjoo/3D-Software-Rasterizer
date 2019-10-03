#pragma once

#include "Definitions.h"
//#include "Sphere.h"
//#include "Cube.h"
//#include "Prism.h"
//#include "Cylinder.h"
//#include "Cone.h"
//#include "Revolved.h"

#include "SolidBody.h"

class Shapes
{
	int nEntities;
	int currentEntity;

	std::vector<SolidBody*> bodyContainer;
	std::vector<char*> textureNameContainer;
	std::vector<txt> textureDataContainer;

	triangle3dV** meshTriangles;
	txt* textures;

public:

	Shapes();
	~Shapes();

	int getNEntities();
	int* getPolyCountEntities();
	Uint32* getColourEntities();
	int* getTextureEntities();
	bool* getGouraudEntities();

	void addSolid(SolidBody*);
	void addTextureName(char*);
	void addTextureData(txt);

	int getTotalPolyCount();
	int getTextureID(int);
	int getPolyCount(int);

	triangle3dV** getPolyData();
	txt* getTextureData();
	triangle3dV* getSupport(int* n);
};

