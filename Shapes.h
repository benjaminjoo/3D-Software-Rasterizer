#pragma once

#include "Definitions.h"
#include "SolidBody.h"
#include "SpotLight.h"
#include <vector>

#include <SDL.h>

class Shapes
{
	int nEntities;

	std::vector<SolidBody*>		bodyContainer;
	std::vector<boundingBox>	boxContainer;
	std::vector<char*>			textureNameContainer;
	std::vector<txt>			textureDataContainer;
	std::vector<SpotLight*>		lightContainer;

	txt*						textures;
	vect3*						spotLightPositions;
	double*						spotLightIntensities;

public:

	vect3						vertexList[MAXCLIPVERTS];
	textCoord					uvList[MAXCLIPVERTS];
	
public:

	Shapes();
	~Shapes();

	unsigned int getNEntities();
	int* getPolyCountEntities();
	void getPolyCountEntities(int*);

	void addSolid(SolidBody*);
	void addTextureName(char*);
	void addTextureData(txt);

	void setPosition(int, vect3);
	vect3 getPosition(int);
	void setVelocity(int, vect3);
	vect3 getVelocity(int);
	void setAngularVelocity(int, vect3);
	vect3 getAngularVelocity(int);

	int getTotalPolyCount();
	int getPolyCount(int);
	bool assertShadowCasting(int);

	void updateBodyPosition();
	void getPolyData_(int, triangle3dV**);
	txt* getTextureData();
	txt getTextureData(int);

	void textureLoader(int, SDL_Surface**);
	void textureLoaderQ(int, txt*);

	bool getBBState(int);
	void activateBBox(int);
	void updateBBox(int, vect3, vect3);
	boundingBox getBB(int);
};

