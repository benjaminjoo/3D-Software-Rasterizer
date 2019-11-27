#include "Shapes.h"
#include "Definitions.h"
#include <vector>
#include <iostream>
using namespace std;


Shapes::Shapes()
{
	nEntities				= 0;

	textures				= nullptr;
	spotLightPositions		= nullptr;
	spotLightIntensities	= nullptr;


	for (int v = 0; v < MAXCLIPVERTS; v++) { vertexList[v] = { 0.0, 0.0, 0.0, 0.0 }; uvList[v] = { 0.0, 0.0 }; }
}


Shapes::~Shapes()
{
}


unsigned int Shapes::getNEntities()
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


void Shapes::setPosition(unsigned int n, vect3 p)
{
	if (n < bodyContainer.size())
	{
		bodyContainer[n]->setPosition(p);
	}	
}


vect3 Shapes::getPosition(unsigned int n)
{
	vect3 temp = { 0.0, 0.0, 0.0, 1.0 };
	if (n < bodyContainer.size())
	{
		temp = bodyContainer[n]->getPosition();
	}
	return temp;
}


void Shapes::setVelocity(unsigned int n, vect3 v)
{
	if (n < bodyContainer.size())
	{
		bodyContainer[n]->setVelocity(v);
	}
}


vect3 Shapes::getVelocity(unsigned int n)
{
	vect3 temp = { 0.0, 0.0, 0.0, 1.0 };
	if (n < bodyContainer.size())
	{
		temp = bodyContainer[n]->getVelocity();
	}
	return temp;
}


void Shapes::setAngularVelocity(unsigned int n, vect3 v)
{
	if (n < bodyContainer.size())
	{
		bodyContainer[n]->setAngularVelocity(v);
	}
}


vect3 Shapes::getAngularVelocity(unsigned int n)
{
	vect3 temp = { 0.0, 0.0, 0.0, 1.0 };
	if (n < bodyContainer.size())
	{
		temp = bodyContainer[n]->getAngularVelocity();
	}
	return temp;
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


int Shapes::getPolyCount(int n)
{
	return bodyContainer[n]->getTotalPoly();
}


bool Shapes::assertShadowCasting(int n)
{
	return bodyContainer[n]->assertShadowCasting();
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


void Shapes::getPolyCountEntities(std::shared_ptr<int[]> polyCountEntities)
{
	int n = bodyContainer.size();
	for (int i = 0; i < n; i++)
	{
		polyCountEntities[i] = bodyContainer[i]->getTotalPoly();
	}
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


txt Shapes::getTextureData(int n)
{
	return textureDataContainer[n];
}


void Shapes::textureLoader(int nTextures, SDL_Surface** T)
{
	for (int i = 0; i < nTextures; i++)
	{
		if (T[i] == nullptr)
		{
			printf("Image loading failed...\n");
		}
		else
		{
			txt tempTexture;
			SDL_Surface* tempImage = SDL_ConvertSurfaceFormat(T[i], SDL_PIXELFORMAT_ARGB8888, 0);
			tempTexture.pixels = (Uint32*)tempImage->pixels;
			tempTexture.ID = i;
			tempTexture.w = T[i]->w;
			tempTexture.h = T[i]->h;
			this->addTextureData(tempTexture);
		}
	}
}


void Shapes::textureLoaderQ(int n, txt* T)
{
	for (int i = 0; i < n; i++)
	{		
		textureDataContainer.push_back(T[i]);
	}
}


bool Shapes::getBBState(int n)
{
	return bodyContainer[n]->getBBState();
}


void Shapes::activateBBox(int n)
{
	bodyContainer[n]->activateBBox();
}


void Shapes::updateBBox(int n, vect3 min_ext, vect3 max_ext)
{
	bodyContainer[n]->updatebBBox(min_ext, max_ext);
}


boundingBox Shapes::getBB(int n)
{
	return bodyContainer[n]->getBB();
}
