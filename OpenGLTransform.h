#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>

class OpenGLTransform
{
private:

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	
public:
	OpenGLTransform(const glm::vec3& pos = glm::vec3(),
					const glm::vec3& rot = glm::vec3(),
					const glm::vec3& scl = glm::vec3(1.0f, 1.0f, 1.0f))
	{
		position	= pos;
		rotation	= rot;
		scale		= scl;
	}


	inline glm::mat4 getModel()
	{
		glm::mat4 posMatrix		= glm::translate(position);

		glm::mat4 rotXMatrix	= glm::rotate(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotYMatrix	= glm::rotate(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotZMatrix	= glm::rotate(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 rotMatrix		= rotZMatrix * rotYMatrix * rotXMatrix;

		glm::mat4 scaleMatrix	= glm::scale(scale);

		return posMatrix * rotMatrix * scaleMatrix;
	}


	inline glm::vec3* getPos() { &position; }
	inline glm::vec3* getRot() { &rotation; }
	inline glm::vec3* getScl() { &scale;	}


	inline void setPos(const glm::vec3& pos) { position = pos; }
	inline void setRot(const glm::vec3& rot) { rotation = rot; }
	inline void setScl(const glm::vec3& scl) { scale	= scl; }


	~OpenGLTransform()
	{

	}
};

