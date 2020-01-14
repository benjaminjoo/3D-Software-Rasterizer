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

	vect3	scale;
	int		patchLod;

public:
	BSP3Loader(std::string, double, double, double);
	BSP3Loader(std::string, vect3);
	~BSP3Loader();

	void			readData();
	int				getTotalVert();
	int				getTotalPoly();
	void			getVertexData(point3*);
	void			getTriangleData_(triangle3dV*);
	void			constructShadowVolume(vect3);
};

