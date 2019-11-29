#define _CRT_SECURE_NO_DEPRECATE

#include "ModelElementBuffer.h"


ModelElementBuffer::ModelElementBuffer(std::string F)
{
	std::cout << "ModelElementBuffer constructor called" << std::endl;

	fileName	= F;
}


ModelElementBuffer::~ModelElementBuffer()
{
	std::cout << "ModelElementBuffer destructor called" << std::endl;
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
	for (auto& i : vertex3Buffer)
		if (i.id == ID)
			i.selected = i.selected ? false : true;
}


void ModelElementBuffer::deselectVertex3byID(int ID)
{
	for (auto& i : vertex3Buffer)
		if (i.id == ID)
			i.selected = false;
}


void ModelElementBuffer::deleteVertex3byID(int ID)
{
	for(auto& i : vertex3Buffer)
		if (i.id == ID)
		{
			i.deleted	= true;
			i.selected	= false;
		}
}


void ModelElementBuffer::moveVertex3byID(int ID, worldCoord move)
{
	for(auto& i : vertex3Buffer)
		if (i.id == ID)
		{
			i.pos.x += move.x;
			i.pos.y += move.y;
			i.pos.z += move.z;
		}
}


void ModelElementBuffer::copyVertex3byID(int ID, int newID, worldCoord move)
{
	vertex3 newTemp;

	for(auto i : vertex3Buffer)
		if (i.id == ID)
		{
			newTemp.id = newID;
			newTemp.pos = i.pos;
			newTemp.selected = true;
			newTemp.deleted = false;

			newTemp.pos.x += move.x;
			newTemp.pos.y += move.y;
			newTemp.pos.z += move.z;

			vertex3Buffer.push_back(newTemp);
		}
}


void ModelElementBuffer::rotVertex3byID(int ID, Side currentView, worldCoord origin, double angle)
{
	for (auto& i : vertex3Buffer)
		if (i.id == ID)
			i.pos = rotate2(i.pos, currentView, origin, angle);
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


void ModelElementBuffer::copyVertex3byIndex(int i, int newID, worldCoord move)
{
	vertex3 newTemp;

	newTemp.id			= newID;
	newTemp.pos			= vertex3Buffer[i].pos;
	newTemp.selected	= true;
	newTemp.deleted		= false;

	newTemp.pos.x += move.x;
	newTemp.pos.y += move.y;
	newTemp.pos.z += move.z;

	vertex3Buffer.push_back(newTemp);
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


void ModelElementBuffer::moveLine3byIndex(int i, worldCoord move)
{
	line3Buffer[i].vert[0].x += move.x;
	line3Buffer[i].vert[0].y += move.y;
	line3Buffer[i].vert[0].z += move.z;

	line3Buffer[i].vert[1].x += move.x;
	line3Buffer[i].vert[1].y += move.y;
	line3Buffer[i].vert[1].z += move.z;
}


void ModelElementBuffer::moveLine3EndPoint(int lineIndex, int vertIndex, Side view, worldCoord move)
{
	switch (view)
	{
	case Top:
		line3Buffer[lineIndex].vert[vertIndex].x += move.x;
		line3Buffer[lineIndex].vert[vertIndex].y += move.y;
		break;
	case Front:
		line3Buffer[lineIndex].vert[vertIndex].x += move.x;
		line3Buffer[lineIndex].vert[vertIndex].z += move.z;
		break;
	case Right:
		line3Buffer[lineIndex].vert[vertIndex].y += move.y;
		line3Buffer[lineIndex].vert[vertIndex].z += move.z;
		break;
	}
}


void ModelElementBuffer::rotLine3byIndex(int i, Side currentView, worldCoord origin, double angle)
{
	line3Buffer[i].vert[0] = rotate2(line3Buffer[i].vert[0], currentView, origin, angle);
	line3Buffer[i].vert[1] = rotate2(line3Buffer[i].vert[1], currentView, origin, angle);
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
		for (auto i = line3Buffer.begin(); i != line3Buffer.end(); ++i)
		{
			double x1, x2, y1, y2;
			x1 = i->vert[0].x;
			y1 = i->vert[0].y;
			x2 = i->vert[1].x;
			y2 = i->vert[1].y;

			fprintf(f, "Space.addEdge(edge(vect2(%.1f, %.1f), vect2(%.1f, %.1f)));\n", x1, y1, x2, y2);
			fprintf(f, "Space.addEdge(edge(vect2(%.1f, %.1f), vect2(%.1f, %.1f)));\n", x2, y2, x1, y1);
		}
		fclose(f);
	}
}
