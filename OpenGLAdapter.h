#pragma once

#include <GLM/glm.hpp>
#include <vector>
#include <memory>
#include "Definitions.h"
#include "Shapes.h"
#include "Vertex.h"

class OpenGLAdapter
{
private:

	Shapes* Solids;
	Shapes* Actors;
	Shapes* Entities;

	unsigned int solidN;
	unsigned int actorN;

	triangle3dV** solidMesh;
	triangle3dV** actorMesh;
	triangle3dV** triangleMesh;

public:

	OpenGLAdapter(Shapes* B, Shapes* A);

	unsigned int getNVert(model M, unsigned int n);
	void getVertices(model M, unsigned int n, Vertex* V);

	~OpenGLAdapter();
};

