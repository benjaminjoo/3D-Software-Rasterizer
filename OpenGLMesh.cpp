#include <GLM/glm.hpp>
#include <vector>
#include "OpenGLMesh.h"

#include <stdio.h>


OpenGLMesh::OpenGLMesh(Vertex* vertices, unsigned int numVertices)
{
	nVert = numVertices;

	for (auto i = 0; i < numVertices; i++)
	{
		positions.push_back(vertices[i].getPosition());
		txtCoords.push_back(vertices[i].getTxtCoord());
		normals.push_back(vertices[i].getNormal());

		//printf("Vertex No. %d\tPosition %.2f\t%.2f\t%.2f\n", i, vertices[i].getPosition().x, vertices[i].getPosition().y, vertices[i].getPosition().z);
	}

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TXTCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(txtCoords[0]), &txtCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}


void OpenGLMesh::drawMesh()
{
	glBindVertexArray(vertexArrayObject);

	glDrawArrays(GL_TRIANGLES, 0, nVert);

	glBindVertexArray(0);
}


OpenGLMesh::~OpenGLMesh()
{

}
