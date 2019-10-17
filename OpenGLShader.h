#pragma once

#include <GLEW/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include "OpenGLTransform.h"
#include "OpenGLCamera.h"

class OpenGLShader
{
private:

	static const unsigned int NUM_SHADERS = 2;

	enum
	{
		TRANSFORM_U,

		NUM_UNIFORMS
	};

	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];

public:
	OpenGLShader(const std::string& fileName);

	void bind();
	void update(const OpenGLTransform& transform, const OpenGLCamera& camera);

	~OpenGLShader();
};

