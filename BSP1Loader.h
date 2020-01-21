#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "Definitions.h"
#include "SolidBody.h"


class BSP1Loader : public SolidBody
{

	std::string					fileName;
	std::vector<point3>			vertexContainer;
	std::vector<edge3d>			edgeContainer;
	std::vector<long>			edgeListContainer;
	std::vector<face_t>			faceContainer;
	std::vector<surface_t>		surfaceContainer;
	std::vector<miptex_t>		miptexContainer;
	std::vector<txt>			textureDataContainer;
	std::vector<vert>			idVertices;
	std::vector<int>			idMeshVerts;
	std::vector<face>			idFaces;
	std::vector<triangle3dV>	polyContainer;

	vect3						scale = { 1.0, 1.0, 1.0, 1.0 };

	Uint32*						palette;

public:
	BSP1Loader(std::string, float, float, float);
	BSP1Loader(std::string, vect3);
	~BSP1Loader();

	void readData();
	int	getTotalText();
	int	getTotalVert();
	int	getTotalEdge();
	int	getTotalPoly();
	void getVertexData(point3*);
	void calculateTriangles();
	void getTriangleData(triangle3dV*);
	txt	getTextureData(unsigned int);
	line3d	getLine(unsigned int);
};

