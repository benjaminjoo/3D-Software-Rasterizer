#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "Definitions.h"
#include "SolidBody.h"


class BSP3Loader: public SolidBody
{

	std::string					fileName;
	std::vector<point3>			vertexContainer;
	std::vector<vert>			idVertices;
	std::vector<int>			idMeshVerts;
	std::vector<face>			idFaces;
	std::vector<triangle3dV>	polyContainer;

	vect3					scale;

public:
	BSP3Loader(std::string);
	BSP3Loader(std::string, vect3);
	~BSP3Loader();

	void			readData();
	int				getTotalVert();
	int				getTotalPoly();
	void			getVertexData(point3*);
	void			getTriangleData_(triangle3dV*);
	vect3			getPosition();
	void			constructShadowVolume(vect3);
};

