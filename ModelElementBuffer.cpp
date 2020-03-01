#define _CRT_SECURE_NO_DEPRECATE

#include "ModelElementBuffer.h"


ModelElementBuffer::ModelElementBuffer(std::string F)
{
	std::cout << "ModelElementBuffer constructor called" << std::endl;

	fileName = F;
}


ModelElementBuffer::~ModelElementBuffer()
{
	std::cout << "ModelElementBuffer destructor called" << std::endl;
}


void ModelElementBuffer::addVertex3(vertex3 P)
{
	vertex3Buffer.push_back(P);
}


void ModelElementBuffer::addPointCloud(std::shared_ptr<PointCloud> P)
{
	P->getPoints(vertex3Buffer);
}


void ModelElementBuffer::addLine3(line3 L)
{
	line3Buffer.push_back(L);
}


void ModelElementBuffer::addTriangle3(triangle3 TR)
{
	triangle3Buffer.push_back(TR);
}


void ModelElementBuffer::addTriangle3Vert(int ID, int id, vertex3 V)
{
	for (auto& tr : triangle3Buffer)
		if (tr.id == ID && id < 3)
		{
			tr.vert[id] = V;
			tr.vertDrawn[id] = true;
		}		
}


void ModelElementBuffer::addPolyline3(polyline3 PL)
{
	polyline3Buffer.push_back(PL);
}


void ModelElementBuffer::addPolyline3ControlPoint(int ID, vertex3 C)
{
	for (auto& pl : polyline3Buffer)
		if (pl.id == ID)
			pl.ctrlPoints.push_back(C);
}


void ModelElementBuffer::addSpline3(spline3 S)
{
	spline3Buffer.push_back(S);
}


void ModelElementBuffer::addSpline3ControlPoint(int ID, vertex3 C)
{
	for (auto& sp : spline3Buffer)
		if (sp.id == ID)
			sp.ctrlPoints.push_back(C);
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

	newTemp.id = newID;
	newTemp.pos = vertex3Buffer[i].pos;
	newTemp.selected = true;
	newTemp.deleted = false;

	newTemp.pos.x += move.x;
	newTemp.pos.y += move.y;
	newTemp.pos.z += move.z;

	vertex3Buffer.push_back(newTemp);
}


void ModelElementBuffer::rotVertex3byIndex(int i, Side currentView, worldCoord origin, float angle)
{
	vertex3Buffer[i].pos = rotate2(vertex3Buffer[i].pos, currentView, origin, angle);
}


void ModelElementBuffer::selectTriangleVertex3byIndex(int tr, int v)
{
	triangle3Buffer[tr].vert[v].selected = triangle3Buffer[tr].vert[v].selected ? false : true;
}


void ModelElementBuffer::deselectTriangleVertex3byIndex(int tr, int v)
{
	triangle3Buffer[tr].vert[v].selected = false;
}


void ModelElementBuffer::moveTriangleVertex3byIndex(int tr, int v, worldCoord move)
{
	triangle3Buffer[tr].vert[v].pos.x += move.x;
	triangle3Buffer[tr].vert[v].pos.y += move.y;
	triangle3Buffer[tr].vert[v].pos.z += move.z;
}


void ModelElementBuffer::selectPolylineControlVertex3byIndex(int pline, int ctrl)
{
	polyline3Buffer[pline].ctrlPoints[ctrl].selected = polyline3Buffer[pline].ctrlPoints[ctrl].selected ? false : true;
}


void ModelElementBuffer::deselectPolylineControlVertex3byIndex(int pline, int ctrl)
{
	polyline3Buffer[pline].ctrlPoints[ctrl].selected = false;
}


void ModelElementBuffer::deletePolylineControlVertex3byIndex(int pline, int ctrl)
{
	polyline3Buffer[pline].ctrlPoints[ctrl].deleted = true;
	polyline3Buffer[pline].ctrlPoints[ctrl].selected = false;
}


void ModelElementBuffer::movePolylineControlVertex3byIndex(int pline, int ctrl, worldCoord move)
{
	polyline3Buffer[pline].ctrlPoints[ctrl].pos.x += move.x;
	polyline3Buffer[pline].ctrlPoints[ctrl].pos.y += move.y;
	polyline3Buffer[pline].ctrlPoints[ctrl].pos.z += move.z;
}


void ModelElementBuffer::selectSplineControlVertex3byIndex(int spline, int ctrl)
{
	spline3Buffer[spline].ctrlPoints[ctrl].selected = spline3Buffer[spline].ctrlPoints[ctrl].selected ? false : true;
}


void ModelElementBuffer::deselectSplineControlVertex3byIndex(int spline, int ctrl)
{
	spline3Buffer[spline].ctrlPoints[ctrl].selected = false;
}


void ModelElementBuffer::deleteSplineControlVertex3byIndex(int spline, int ctrl)
{
	spline3Buffer[spline].ctrlPoints[ctrl].deleted = true;
	spline3Buffer[spline].ctrlPoints[ctrl].selected = false;
}


void ModelElementBuffer::moveSplineControlVertex3byIndex(int spline, int ctrl, worldCoord move)
{
	spline3Buffer[spline].ctrlPoints[ctrl].pos.x += move.x;
	spline3Buffer[spline].ctrlPoints[ctrl].pos.y += move.y;
	spline3Buffer[spline].ctrlPoints[ctrl].pos.z += move.z;
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
	case Side::Top:
		line3Buffer[lineIndex].vert[vertIndex].x += move.x;
		line3Buffer[lineIndex].vert[vertIndex].y += move.y;
		break;
	case Side::Front:
		line3Buffer[lineIndex].vert[vertIndex].x += move.x;
		line3Buffer[lineIndex].vert[vertIndex].z += move.z;
		break;
	case Side::Right:
		line3Buffer[lineIndex].vert[vertIndex].y += move.y;
		line3Buffer[lineIndex].vert[vertIndex].z += move.z;
		break;
	}
}


void ModelElementBuffer::rotLine3byIndex(int i, Side currentView, worldCoord origin, float angle)
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


bool ModelElementBuffer::isTriangle3Selected(int n)
{
	return triangle3Buffer[n].selected;
}


bool ModelElementBuffer::isTriangle3Deleted(int n)
{
	return triangle3Buffer[n].deleted;
}


int ModelElementBuffer::getTriangle3BufferSize()
{
	return triangle3Buffer.size();
}


triangle3 ModelElementBuffer::getTriangle3(int n)
{
	return triangle3Buffer[n];
}


bool ModelElementBuffer::isPolyline3Selected(int n)
{
	return polyline3Buffer[n].selected;
}


bool ModelElementBuffer::isPolyline3Deleted(int n)
{
	return polyline3Buffer[n].deleted;
}


int ModelElementBuffer::getPolyline3BufferSize()
{
	return polyline3Buffer.size();
}


polyline3 ModelElementBuffer::getPolyline3(int n)
{
	return polyline3Buffer[n];
}


bool ModelElementBuffer::isSpline3Selected(int n)
{
	return spline3Buffer[n].selected;
}


bool ModelElementBuffer::isSpline3Deleted(int n)
{
	return spline3Buffer[n].deleted;
}


int ModelElementBuffer::getSpline3BufferSize()
{
	return spline3Buffer.size();
}


spline3 ModelElementBuffer::getSpline3(int n)
{
	return spline3Buffer[n];
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
		for (auto& i : line3Buffer)
		{
			float x1, x2, y1, y2;
			x1 = i.vert[0].x;
			y1 = i.vert[0].y;
			x2 = i.vert[1].x;
			y2 = i.vert[1].y;

			fprintf(f, "Space.addEdge(edge(vect2(%.1f, %.1f), vect2(%.1f, %.1f)));\n", x1, y1, x2, y2);
			fprintf(f, "Space.addEdge(edge(vect2(%.1f, %.1f), vect2(%.1f, %.1f)));\n", x2, y2, x1, y1);
		}
		fclose(f);
	}
}
