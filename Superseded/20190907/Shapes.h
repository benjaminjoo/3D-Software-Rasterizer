#pragma once

#include "Definitions.h"

#include "SolidBody.h"
#include "SpotLight.h"

class Shapes
{
	int nEntities;

	std::vector<SolidBody*>		bodyContainer;
	std::vector<boundingBox>	boxContainer;
	std::vector<char*>			textureNameContainer;
	std::vector<txt>			textureDataContainer;
	std::vector<SpotLight*>		lightContainer;

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

	void setPosition(int, vect3);
	vect3 getPosition(int);
	void setVelocity(int, vect3);

	int getTotalPolyCount();
	int getPolyCount(int);

	void updateBodyPosition();
	void getPolyData_(int, triangle3dV**);
	void getBoundingBoxData(int, boundingBox*);
	txt* getTextureData();
	vect3* getSpotLightPositions();
	double* getSpotLightIntensities();
};

