#include "Shapes.h"
#include "Definitions.h"
#include "Revolved.h"

#include <iostream>
using namespace std;


Shapes::Shapes()
{
	nEntities				= 0;

	textures				= nullptr;
	spotLightPositions		= nullptr;
	spotLightIntensities	= nullptr;
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
	//bodyContainer.shrink_to_fit();
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


void Shapes::setPosition(int n, vect3 p)
{
	if (n < bodyContainer.size())
	{
		bodyContainer[n]->setPosition(p);
	}	
}


vect3 Shapes::getPosition(int n)
{
	vect3 temp = { 0.0, 0.0, 0.0, 1.0 };
	if (n < bodyContainer.size())
	{
		temp = bodyContainer[n]->getPosition();
	}
	return temp;
}


void Shapes::setVelocity(int n, vect3 v)
{
	if (n < bodyContainer.size())
	{
		bodyContainer[n]->setVelocity(v);
	}
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


void Shapes::updateBodyPosition()
{
	int nBodies = this->getNEntities();
	for (int i = 0; i < nBodies; i++)
	{
		bodyContainer[i]->updatePosition();
	}
}


void Shapes::getPolyData_(int n, triangle3dV** meshTriangles)
{
	for (int i = 0; i < n; i++)
	{
		bodyContainer[i]->getTriangleData_(meshTriangles[i]);
	}
}


void Shapes::getBoundingBoxData(int, boundingBox*)
{
	

}


txt* Shapes::getTextureData()
{
	int n = textureDataContainer.size();
	textures = new txt [n];

	if (textures != nullptr)
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

	if (spotLightPositions != nullptr)
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

	if (spotLightIntensities != nullptr)
	{
		for (int i = 0; i < n; i++)
		{
			spotLightIntensities[i] = lightContainer[i]->getIntensity();
		}
	}

	return spotLightIntensities;
	delete[] spotLightIntensities;
}
