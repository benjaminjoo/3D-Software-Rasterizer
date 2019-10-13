#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>

class OpenGLCamera
{
private:

	glm::mat4 view;
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;

public:

	OpenGLCamera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
	{
		view		= glm::perspective(fov, aspect, zNear, zFar);
		position	= pos;
		forward		= glm::vec3(0.0f, 0.0f, 1.0f);
		up			= glm::vec3(0.0f, 1.0f, 0.0f);
	}


	inline glm::mat4 getView() const
	{
		return view * glm::lookAt(position, position + forward, up);
	}


	~OpenGLCamera()
	{

	}
};

