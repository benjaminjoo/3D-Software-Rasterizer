#pragma once

#include "Definitions.h"

#include "SolidBody.h"
#include "SpotLight.h"

class Shapes
{
	int nEntities;

	std::vector<SolidBody*>		bodyContainer;
	std::vector<char*>			textureNameContainer;
	std::vector<txt>			textureDataContainer;
	std::vector<SpotLight*>		lightContainer;

	//triangle3dV**	meshTriangles;
	txt*			textures;
	vect3*			spotLightPositions;
	double*			spotLightIntensities;
	
public:

	Shapes();
	~Shapes();

	int getNEntities();
	int getNSpotLights();
	int* getPolyCountEntities();

	void addSolid(SolidBody*);
	void addTextureName(char*);
	void addTextureData(txt);
	void addSpotLight(SpotLight*);

	int getTotalPolyCount();
	int getPolyCount(int);

	triangle3dV** getPolyData();
	void getPolyData_(int, triangle3dV**);
	triangle3dV* getPolyDataOf(int);
	txt* getTextureData();
	vect3* getSpotLightPositions();
	double* getSpotLightIntensities();
	triangle3dV* getSupport(int* n);
};

