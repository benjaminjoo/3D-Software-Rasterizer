#pragma once

#include "Definitions.h"
#include "SolidBody.h"
#include <vector>
#include <SDL/SDL.h>

class Shapes
{
	int nEntities;

	std::vector<SolidBody*>		bodyContainer;
	std::vector<boundingBox>	boxContainer;
	std::vector<char*>			textureNameContainer;
	std::vector<txt>			textureDataContainer;
	
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

	void setPosition(unsigned int, vect3);
	vect3 getPosition(unsigned int);
	void setVelocity(unsigned int, vect3);
	vect3 getVelocity(unsigned int);
	void setAngularVelocity(unsigned int, vect3);
	vect3 getAngularVelocity(unsigned int);

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

