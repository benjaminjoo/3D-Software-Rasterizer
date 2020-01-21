#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>
#include "EventHandler.h"

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


	inline glm::mat4 getModel() const
	{
		glm::mat4 posMatrix		= glm::translate(position);

		glm::mat4 rotXMatrix	= glm::rotate(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotYMatrix	= glm::rotate(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotZMatrix	= glm::rotate(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		return  rotYMatrix * rotXMatrix * rotZMatrix * posMatrix;
	}


	inline glm::vec3* getPos() { return &position;	}
	inline glm::vec3* getRot() { return &rotation;	}
	inline glm::vec3* getScl() { return &scale;		}

	inline void setPos(const glm::vec3& pos) { position = pos; }
	inline void setRot(const glm::vec3& rot) { rotation = rot; }
	inline void setScl(const glm::vec3& scl) { scale	= scl; }


	inline void updateAll(EventHandler& Controls)
	{
		rotation.x = Controls.turnV;
		rotation.y = Controls.tiltP;
		rotation.z = Controls.turnH;

		position.y += Controls.moveP * (float)cos(rotation.z) + Controls.strafeP * (float)cos(rotation.z + PI * 0.5f);
		position.x += Controls.moveP * (float)sin(rotation.z) + Controls.strafeP * (float)sin(rotation.z + PI * 0.5f);
		position.z -= Controls.riseP;
	}


	inline void updateRotX(const float& rx)
	{
		rotation.x += rx;
	}


	inline void updateRotY(const float& ry)
	{
		rotation.y += ry;
	}


	inline void updateRotZ(const float& rz)
	{
		rotation.z += rz;
	}


	~OpenGLTransform()
	{

	}
};

