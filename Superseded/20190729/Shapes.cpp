#include "Shapes.h"
#include "Definitions.h"
#include "Sphere.h"
#include "Cube.h"
#include "Prism.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Revolved.h"

#include <iostream>
using namespace std;



Shapes::Shapes()
{
	nEntities			= 0;
	currentEntity		= 0;
	//polyCountEntities	= new int[MAXENTITIES];
	//colourEntities		= new Uint32[MAXENTITIES];
	//gouraudEntities		= new bool[MAXENTITIES];

	//nSphere		= 0;
	//nCube		= 0;
	//nPrism		= 0;
	//nCylinder	= 0;
	//nCone		= 0;
	//nRevolved	= 0;

	meshTriangles = NULL;
	textures = NULL;
}


Shapes::~Shapes()
{
}


int Shapes::getNEntities()
{
	return bodyContainer.size();
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


int Shapes::getTextureID(int x)
{
	return bodyContainer[x]->getTextureID();
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


Uint32* Shapes::getColourEntities()
{
	int n = bodyContainer.size();
	Uint32* colourEntities = new Uint32[n];
	for (int i = 0; i < n; i++)
	{
		colourEntities[i] = bodyContainer[i]->getColour();
	}
	return colourEntities;
	delete[] colourEntities;
}


int* Shapes::getTextureEntities()
{
	int n = bodyContainer.size();
	int* textureEntities = new int[n];
	for (int i = 0; i < n; i++)
	{
		textureEntities[i] = bodyContainer[i]->getTextureID();
	}
	return textureEntities;
	delete[] textureEntities;
}


bool* Shapes::getGouraudEntities()
{
	int n = bodyContainer.size();
	bool* gouraudEntities = new bool[n];
	for (int i = 0; i < n; i++)
	{
		gouraudEntities[i] = true;
	}
	return gouraudEntities;
	delete[] gouraudEntities;
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