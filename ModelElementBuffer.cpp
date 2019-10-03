#define _CRT_SECURE_NO_DEPRECATE

#include "ModelElementBuffer.h"


ModelElementBuffer::ModelElementBuffer(std::string F)
{
	fileName	= F;
}


ModelElementBuffer::~ModelElementBuffer()
{
}


void ModelElementBuffer::addVertex3(vertex3 P)
{
	vertex3Buffer.push_back(P);
}


void ModelElementBuffer::addLine3(line3 L)
{
	line3Buffer.push_back(L);
}


void ModelElementBuffer::selectVertex3byID(int ID)
{
	for (auto i = 0; i < vertex3Buffer.size(); i++)
	{
		if (vertex3Buffer[i].id == ID) { vertex3Buffer[i].selected = vertex3Buffer[i].selected ? false : true; }
	}
}


void ModelElementBuffer::deselectVertex3byID(int ID)
{
	for (auto i = 0; i < vertex3Buffer.size(); i++)
	{
		if (vertex3Buffer[i].id == ID) { vertex3Buffer[i].selected = false; }
	}
}


void ModelElementBuffer::deleteVertex3byID(int ID)
{
	for (auto i = 0; i < vertex3Buffer.size(); i++)
	{
		if (vertex3Buffer[i].id == ID)
		{
			vertex3Buffer[i].deleted = true;
			vertex3Buffer[i].selected = false;
		}
	}
}


void ModelElementBuffer::moveVertex3byID(int ID, worldCoord move)
{
	for (auto i = 0; i < vertex3Buffer.size(); i++)
	{
		if (vertex3Buffer[i].id == ID)
		{
			vertex3Buffer[i].pos.x += move.x;
			vertex3Buffer[i].pos.y += move.y;
			vertex3Buffer[i].pos.z += move.z;
		}
	}
}


void ModelElementBuffer::rotVertex3byID(int ID, Side currentView, worldCoord origin, double angle)
{
	for (auto i = 0; i < vertex3Buffer.size(); i++)
	{
		if (vertex3Buffer[i].id == ID)
		{
			vertex3Buffer[i].pos = rotate2(vertex3Buffer[i].pos, currentView, origin, angle);
		}
	}
}


void ModelElementBuffer::selectVertex3byIndex(int i)
{
	vertex3Buffer[i].selected = vertex3Buffer[i].selected ? false : true;
}


void ModelElementBuffer::deselectVertex3byIndex(int i)
{
	vertex3Buffer[i].selected = false;
}


void ModelElementBuffer::deleteVertex3byIndex(int i)
{
	vertex3Buffer[i].deleted = true;
	vertex3Buffer[i].selected = false;
}


void ModelElementBuffer::moveVertex3byIndex(int i, worldCoord move)
{
	vertex3Buffer[i].pos.x += move.x;
	vertex3Buffer[i].pos.y += move.y;
	vertex3Buffer[i].pos.z += move.z;
}


void ModelElementBuffer::rotVertex3byIndex(int i, Side currentView, worldCoord origin, double angle)
{
	vertex3Buffer[i].pos = rotate2(vertex3Buffer[i].pos, currentView, origin, angle);
}


void ModelElementBuffer::selectLine3byIndex(int i)
{
	line3Buffer[i].selected = line3Buffer[i].selected ? false : true;
}


void ModelElementBuffer::deselectLine3byIndex(int i)
{
	line3Buffer[i].selected = false;
}


void ModelElementBuffer::deleteLine3byIndex(int i)
{
	line3Buffer[i].deleted = true;
	line3Buffer[i].selected = false;
}


bool ModelElementBuffer::isVertex3Selected(int n)
{
	return vertex3Buffer[n].selected;
}


bool ModelElementBuffer::isVertex3Deleted(int n)
{
	return vertex3Buffer[n].deleted;
}


int ModelElementBuffer::getVertex3BufferSize()
{
	return vertex3Buffer.size();
}


vertex3 ModelElementBuffer::getVertex3(int n)
{
	return vertex3Buffer[n];
}


bool ModelElementBuffer::isLine3Selected(int n)
{
	return line3Buffer[n].selected;
}


bool ModelElementBuffer::isLine3Deleted(int n)
{
	return line3Buffer[n].deleted;
}


int ModelElementBuffer::getLine3BufferSize()
{
	return line3Buffer.size();
}


line3 ModelElementBuffer::getLine3(int n)
{
	return line3Buffer[n];
}


void ModelElementBuffer::exportTextFile()
{
	FILE* f = fopen(fileName.c_str(), "w");
	if (f == NULL)
	{
		printf("Could not write to file...\n");
	}
	else
	{
		int nVert = vertex3Buffer.size();
		printf("Number of vertices: %d\n", nVert);
		for (int i = 0; i < nVert; i++)
		{
			vertex3 temp = vertex3Buffer[i];
			fprintf(f, "%4d\t%8.8f\t%8.8f\t%8.8f\t%d\n", temp.id, temp.pos.x, temp.pos.y, temp.pos.z, temp.deleted);
		}
		fclose(f);
	}
}
