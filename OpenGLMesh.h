#pragma once

#include <GLEW/glew.h>
#include <GLM/glm.hpp>
#include <vector>
#include "Vertex.h"

class OpenGLMesh
{
private:

	enum
	{
		POSITION_VB,
		TXTCOORD_VB,
		NORMAL_VB,

		NUM_BUFFERS
	};

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> txtCoords;
	std::vector<glm::vec3> normals;

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_BUFFERS];
	unsigned int nVert;

public:
	OpenGLMesh(Vertex* vertices, unsigned int numVertices);

	void drawMesh();

	~OpenGLMesh();
};

