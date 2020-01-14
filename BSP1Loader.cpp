#define _CRT_SECURE_NO_DEPRECATE

#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <SDLImage/SDL_Image.h>
#include "BSP1Loader.h"


BSP1Loader::BSP1Loader(std::string f, double sx, double sy, double sz):
	fileName(f)
{
	std::cout << "BSP1Loader constructor called" << std::endl;

	scale.x = sx;
	scale.y = sy;
	scale.z = sz;

	SDL_Surface* tempPalette = IMG_Load("Palette/quake1palette.jpg");
	SDL_Surface* tempImage = SDL_ConvertSurfaceFormat(tempPalette, SDL_PIXELFORMAT_ARGB8888, 0);
	palette = (Uint32*)tempImage->pixels;
}


BSP1Loader::BSP1Loader(std::string f, vect3 s):
	fileName(f), scale(s)
{
	std::cout << "BSP1Loader constructor called" << std::endl;

	SDL_Surface* tempPalette = IMG_Load("Palette/quake1palette.jpg");
	SDL_Surface* tempImage = SDL_ConvertSurfaceFormat(tempPalette, SDL_PIXELFORMAT_ARGB8888, 0);
	palette = (Uint32*)tempImage->pixels;
}


BSP1Loader::~BSP1Loader()
{
	std::cout << "BSP1Loader destructor called" << std::endl;
}


void BSP1Loader::readData()
{
	std::ifstream modelFile(fileName.c_str(), std::ifstream::in | std::ifstream::binary);

	char version[4] = "";

	char offset[4] = "";
	char length[4] = "";

	lump direntry[15] = { 0, 0 };

	if (modelFile)
	{
		modelFile.read(version, 4);
		int version_no = *((int*)version);
		std::cout << "Version: " << version_no << std::endl;
		for (int i = 0; i < 15; i++)
		{
			modelFile.read(offset, 4);
			modelFile.read(length, 4);
			direntry[i].offset = *((int*)offset);
			direntry[i].length = *((int*)length);
		}

		/*
		*	EXTRACTING VERTICES FROM BSP FILE
		*/
		
		modelFile.seekg(direntry[3].offset, modelFile.beg);
		for (unsigned int i = 0; i < (direntry[3].length / 12); i++)
		{
			char temp_4[4] = "";

			modelFile.read(temp_4, 4);
			float vert_x = *((float*)temp_4);

			modelFile.read(temp_4, 4);
			float vert_y = *((float*)temp_4);

			modelFile.read(temp_4, 4);
			float vert_z = *((float*)temp_4);

			point3 tempPoint;
			tempPoint.P.x = (double)vert_x * scale.x;
			tempPoint.P.y = (double)vert_y * scale.y;
			tempPoint.P.z = (double)vert_z * scale.z;
			tempPoint.P.w = 1.0;
			tempPoint.colour = getColour(0, 127, 127, 255);

			vertexContainer.push_back(tempPoint);
		}
		vertexContainer.shrink_to_fit();

		/*
		*	EXTRACTING EDGES FROM BSP FILE
		*/

		modelFile.seekg(direntry[12].offset, modelFile.beg);
		for (unsigned int i = 0; i < (direntry[12].length / 4); i++)
		{
			char temp_2[2] = "";

			modelFile.read(temp_2, 2);
			unsigned short a = *((unsigned short*)temp_2);

			modelFile.read(temp_2, 2);
			unsigned short b = *((unsigned short*)temp_2);

			edge3d tempEdge;

			tempEdge.startPoint		= a;
			tempEdge.endPoint		= b;

			edgeContainer.push_back(tempEdge);
		}
		edgeContainer.shrink_to_fit();

		/*
		*	EXTRACTING LIST OF EDGES FROM BSP FILE
		*/

		modelFile.seekg(direntry[13].offset, modelFile.beg);
		for (unsigned int i = 0; i < (direntry[13].length / 4); i++)
		{
			char temp_4[4] = "";

			long tempLEdge;

			modelFile.read(temp_4, 4);
			tempLEdge = *((long*)temp_4);

			edgeListContainer.push_back(tempLEdge);
		}
		edgeListContainer.shrink_to_fit();

		/*
		*	EXTRACTING TEXTURE INFORMATION FROM BSP FILE
		*/

		modelFile.seekg(direntry[6].offset, modelFile.beg);
		for (unsigned int i = 0; i < (direntry[6].length / 40); i++)
		{
			char temp_2[2] = "";
			char temp_4[4] = "";

			surface_t tempSurface;

			modelFile.read(temp_4, 4);
			tempSurface.S.x = *((float*)temp_4);
			modelFile.read(temp_4, 4);
			tempSurface.S.y = *((float*)temp_4);
			modelFile.read(temp_4, 4);
			tempSurface.S.z = *((float*)temp_4);
			modelFile.read(temp_4, 4);
			tempSurface.distS = *((float*)temp_4);

			modelFile.read(temp_4, 4);
			tempSurface.T.x = *((float*)temp_4);
			modelFile.read(temp_4, 4);
			tempSurface.T.y = *((float*)temp_4);
			modelFile.read(temp_4, 4);
			tempSurface.T.z = *((float*)temp_4);
			modelFile.read(temp_4, 4);
			tempSurface.distT = *((float*)temp_4);

			modelFile.read(temp_4, 4);
			tempSurface.texture_id = *((unsigned long*)temp_4);

			modelFile.read(temp_4, 4);
			tempSurface.animated = *((unsigned long*)temp_4);

			surfaceContainer.push_back(tempSurface);
		}
		surfaceContainer.shrink_to_fit();

		/*
		*	EXTRACTING MIPTEX INFORMATION FROM BSP FILE
		*/

		modelFile.seekg(direntry[2].offset, modelFile.beg);

		char temp_4[4] = "";
		modelFile.read(temp_4, 4);
		unsigned long	numtex = *((unsigned long*)temp_4);
		unsigned long* offset = new unsigned long[numtex];

		for (unsigned long i = 0; i < numtex; i++)
		{
			modelFile.read(temp_4, 4);
			offset[i] = *((int*)temp_4);
		}
		for (unsigned long i = 0; i < numtex; i++)
		{
			modelFile.seekg(unsigned long(direntry[2].offset) + unsigned long(offset[i]), modelFile.beg);

			miptex_t temp;

			modelFile.read(temp.name, 16);
			//std::cout << temp.name << std::endl;

			modelFile.read(temp_4, 4);
			temp.width = *((unsigned long*)temp_4);

			modelFile.read(temp_4, 4);
			temp.height = *((unsigned long*)temp_4);

			modelFile.read(temp_4, 4);
			temp.offset_1 = *((unsigned long*)temp_4);

			modelFile.read(temp_4, 4);
			temp.offset_2 = *((unsigned long*)temp_4);

			modelFile.read(temp_4, 4);
			temp.offset_4 = *((unsigned long*)temp_4);

			modelFile.read(temp_4, 4);
			temp.offset_8 = *((unsigned long*)temp_4);

			miptexContainer.push_back(temp);

			txt tempTexture;

			tempTexture.ID = i;
			tempTexture.w = temp.width;
			tempTexture.h = temp.height;
			tempTexture.pixels = nullptr;

			int nPixel = temp.width * temp.height;

			tempTexture.pixels = new Uint32[nPixel];

			modelFile.seekg(unsigned long(direntry[2].offset) + unsigned long(offset[i]) + temp.offset_1, modelFile.beg);
			for (int p = 0; p < nPixel; p++)
			{
				char colour;
				modelFile.read(&colour, 1);
				unsigned char currentPixel = (unsigned char)colour;
				tempTexture.pixels[p] = palette[currentPixel];
			}

			textureDataContainer.push_back(tempTexture);
			miptexContainer.push_back(temp);

		}
		miptexContainer.shrink_to_fit();
		textureDataContainer.shrink_to_fit();

		/*
		*	EXTRACTING FACES FROM BSP FILE
		*/
		
		modelFile.seekg(direntry[7].offset, modelFile.beg);
		for (unsigned int i = 0; i < (direntry[7].length / 20); i++)
		{
			char temp_2[2] = "";
			char temp_4[4] = "";

			face_t tempFace;

			modelFile.read(temp_2, 2);
			tempFace.plane_id = *((unsigned short*)temp_2);

			modelFile.read(temp_2, 2);
			tempFace.side = *((unsigned short*)temp_2);

			modelFile.read(temp_4, 4);
			tempFace.ledge_id = *((long*)temp_4);

			modelFile.read(temp_2, 2);
			tempFace.ledge_num = *((unsigned short*)temp_2);

			modelFile.read(temp_2, 2);
			tempFace.texinfo_id = *((unsigned short*)temp_2);

			modelFile.read(temp_4, 4);
			tempFace.typelight = temp_4[0];
			tempFace.baselight = temp_4[1];
			tempFace.light[0] = temp_4[2];
			tempFace.light[1] = temp_4[3];

			modelFile.read(temp_4, 4);
			tempFace.lightmap = *((long*)temp_4);
		
			faceContainer.push_back(tempFace);
		}
		faceContainer.shrink_to_fit();

		delete[] offset;
	}
	else
	{
		std::cout << "Could not read .bsp file" << std::endl;
	}
	modelFile.close();

	this->calculateTriangles();
}


int BSP1Loader::getTotalText()
{
	return textureDataContainer.size();
}


txt BSP1Loader::getTextureData(unsigned int n)
{
	if (n < textureDataContainer.size())
	{
		return textureDataContainer[n];
	}
	return { 0, 0, 0, nullptr };
}


int	BSP1Loader::getTotalVert()
{
	return vertexContainer.size();
}


int BSP1Loader::getTotalEdge()
{
	return edgeContainer.size();
}


int BSP1Loader::getTotalPoly()
{
	return polyContainer.size();
}


void BSP1Loader::getVertexData(point3 * v)
{
	int nVert = this->getTotalVert();
	for (int i = 0; i < nVert; i++)
	{
		v[i] = vertexContainer[i];
	}
}


void BSP1Loader::calculateTriangles()
{
	int nFace = faceContainer.size();
	for (int i = 0; i < nFace; i++)
	{
		char first	= miptexContainer[faceContainer[i].texinfo_id].name[0];
		char second = miptexContainer[faceContainer[i].texinfo_id].name[1];
		std::string texture_name;
		texture_name += miptexContainer[faceContainer[i].texinfo_id].name;

		std::cout << texture_name.c_str() << std::endl;

		int nEdge = faceContainer[i].ledge_num;

		unsigned short textureId = faceContainer[i].texinfo_id;
		vect3 S, T;
		S.x = surfaceContainer[textureId].S.x;		T.x = surfaceContainer[textureId].T.x;
		S.y = surfaceContainer[textureId].S.y;		T.y = surfaceContainer[textureId].T.y;
		S.z = surfaceContainer[textureId].S.z;		T.z = surfaceContainer[textureId].T.z;
		double distS, distT;
		distS = (double)surfaceContainer[textureId].distS;
		distT = (double)surfaceContainer[textureId].distT;

		unsigned long txt_id = surfaceContainer[textureId].texture_id;

		unsigned short* vertIndices = new unsigned short[nEdge];
		for (int j = 0; j < nEdge; j++)
		{
			long currentEdge = edgeListContainer[faceContainer[i].ledge_id + j];
			if (currentEdge >= 0)
			{
				vertIndices[j] = edgeContainer[currentEdge].startPoint;
			}
			else
			{
				currentEdge = -currentEdge;
				vertIndices[j] = edgeContainer[currentEdge].endPoint;
			}
		}
		for (int k = 0; k < (nEdge - 2); k++)
		{
			triangle3dV temp;
			int aIndex = vertIndices[0];
			int bIndex = vertIndices[k + 1];
			int cIndex = vertIndices[k + 2];

			temp.A = vertexContainer[aIndex].P;
			temp.B = vertexContainer[bIndex].P;
			temp.C = vertexContainer[cIndex].P;

			//temp.N = unitVector((temp.A - temp.B) ^ (temp.C - temp.B));
			temp.N = unitVector(crossProduct(subVectors(temp.A, temp.B), subVectors(temp.C, temp.B)));

			temp.An = temp.Bn = temp.Cn = temp.N;

			//temp.At.u = ((temp.A * S) + distS) * 1.6;
			temp.At.u = (dotProduct(temp.A, S) + distS) * 1.6f;
			//temp.At.v = ((temp.A * T) + distT) * 1.6;
			temp.At.v = (dotProduct(temp.A, T) + distT) * 1.6f;
			//temp.Bt.u = ((temp.B * S) + distS) * 1.6;
			temp.Bt.u = (dotProduct(temp.B, S) + distS) * 1.6f;
			//temp.Bt.v = ((temp.B * T) + distT) * 1.6;
			temp.Bt.v = (dotProduct(temp.B, T) + distT) * 1.6f;
			//temp.Ct.u = ((temp.C * S) + distS) * 1.6;
			temp.Ct.u = (dotProduct(temp.C, S) + distS) * 1.6f;
			//temp.Ct.v = ((temp.C * T) + distT) * 1.6;
			temp.Ct.v = (dotProduct(temp.C, T) + distT) * 1.6f;

			temp.colour = getColour(0, 255, 255, 255);

			temp.texture = txt_id;

			polyContainer.push_back(temp);
		}
	}
	polyContainer.shrink_to_fit();
}


void BSP1Loader::getTriangleData_(triangle3dV* T)
{
	for (int i = 0; i < this->getTotalPoly(); i++)
	{
		T[i] = polyContainer[i];
	}
}


void BSP1Loader::constructShadowVolume(vect3)
{

}


line3d BSP1Loader::getLine(unsigned int n)
{
	unsigned int nLine = this->getTotalEdge();
	if (n <= nLine)
	{
		vect3 start_point, end_point;

		unsigned short startIndex, endIndex;

		startIndex	= edgeContainer[n].startPoint;
		endIndex	= edgeContainer[n].endPoint;

		start_point = vertexContainer[startIndex].P;
		end_point	= vertexContainer[endIndex].P;

		line3d temp;

		temp.A = start_point;
		temp.B = end_point;

		temp.colour = getColour(0, 127, 127, 255);

		return temp;
	}
	else
	{
		return { (0.0, 0.0, 0.0, 1.0), (0.0, 0.0, 0.0, 1.0), 255 };
	}
}
