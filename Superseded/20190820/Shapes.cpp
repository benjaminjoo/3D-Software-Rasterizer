#include "Shapes.h"
#include "Definitions.h"
#include "Revolved.h"

#include <iostream>
using namespace std;



Shapes::Shapes()
{
	nEntities			= 0;
	currentEntity		= 0;

	meshTriangles = NULL;
	textures = NULL;
	spotLightPositions = NULL;
	spotLightIntensities = NULL;
}


Shapes::~Shapes()
{
}


int Shapes::getNEntities()
{
	return bodyContainer.size();
}


int Shapes::getNSpotLights()
{
	return lightContainer.size();
}


void Shapes::addSolid(SolidBody* s)
{
	bodyContainer.push_back(s);
}


void Shapes::addTextureName(char* name)
{
	textureNameContainer.push_back(name);
}


void Shapes::addTextureData(txt t)
{
	textureDataContainer.push_back(t);
}


void Shapes::addSpotLight(SpotLight* light)
{
	lightContainer.push_back(light);
}

int Shapes::getTotalPolyCount()
{
	int t = 0;
	int n = bodyContainer.size();

	for (int i = 0; i < n; i++)
	{
		t += bodyContainer[i]->getTotalPoly();
	}

	return t;
}


int Shapes::getPolyCount(int x)
{
	return bodyContainer[x]->getTotalPoly();
}


int* Shapes::getPolyCountEntities()
{
	int n = bodyContainer.size();
	int* polyCountEntities = new int[n];
	for (int i = 0; i < n; i++)
	{
		polyCountEntities[i] = bodyContainer[i]->getTotalPoly();
	}
	return polyCountEntities;
	delete[] polyCountEntities;
}


triangle3dV** Shapes::getPolyData()
{
	int n = bodyContainer.size();
	meshTriangles = new triangle3dV * [n];
	for (int i = 0; i < n; i++)
	{
		int totalPoly = bodyContainer[i]->getTotalPoly();
		meshTriangles[i] = new triangle3dV[totalPoly];
	}
	if (meshTriangles != NULL)
	{
		for (int i = 0; i < n; i++)
		{
			if (i < n)
			{
				meshTriangles[i] = bodyContainer[i]->getTriangleData();
			}	
		}
	}
	return meshTriangles;
	for (int i = 0; i < n; i++)
	{
		delete[] meshTriangles[i];
	}
	delete[] meshTriangles;
}


txt* Shapes::getTextureData()
{
	int n = textureDataContainer.size();
	textures = new txt [n];

	if (textures != NULL)
	{
		for (int i = 0; i < n; i++)
		{
			if (i < n)
			{
				textures[i] = textureDataContainer[i];
			}
		}
	}

	return textures;
	delete[] textures;
}


vect3* Shapes::getSpotLightPositions()
{
	int n = lightContainer.size();
	spotLightPositions = new vect3[n];

	if (spotLightPositions != NULL)
	{
		for (int i = 0; i < n; i++)
		{
			spotLightPositions[i] = lightContainer[i]->getPosition();
		}
	}

	return spotLightPositions;
	delete[] spotLightPositions;
}


double* Shapes::getSpotLightIntensities()
{
	int n = lightContainer.size();
	spotLightIntensities = new double[n];

	if (spotLightIntensities != NULL)
	{
		for (int i = 0; i < n; i++)
		{
			spotLightIntensities[i] = lightContainer[i]->getIntensity();
		}
	}

	return spotLightIntensities;
	delete[] spotLightIntensities;
}


triangle3dV* Shapes::getSupport(int* n)
{
	triangle3dV** meshTriangles = this->getPolyData();
	int* polyCountEntities = this->getPolyCountEntities();
	int nSupport = 0;
	for (unsigned int i = 0; i < bodyContainer.size(); i++)
	{
		int totalPoly = polyCountEntities[i];
		for (int j = 0; j < totalPoly; j++)
		{
			triangle3dV currentNormal = meshTriangles[i][j];
			if (currentNormal.N.z >= 0.866)
			{
				nSupport++;
			}
		}
	}

	triangle3dV* supports = new triangle3dV[nSupport];
	int sCnt = 0;
	for (unsigned int i = 0; i < bodyContainer.size(); i++)
	{
		int totalPoly = polyCountEntities[i];
		for (int j = 0; j < totalPoly; j++)
		{
			triangle3dV currentNormal = meshTriangles[i][j];
			if (currentNormal.N.z >= 0.866)
			{
				if (sCnt < nSupport)
				{
					supports[sCnt] = meshTriangles[i][j];
					sCnt++;
				}

			}
		}
	}

	*n = nSupport;

	return supports;
	delete[] supports;
	delete[] polyCountEntities;
	for (unsigned int i = 0; i < bodyContainer.size(); i++)
	{
		delete[] meshTriangles[i];
	}
	delete[] meshTriangles;
}