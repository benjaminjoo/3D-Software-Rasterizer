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

		glm::mat4 rotMatrix		= rotZMatrix * rotYMatrix * rotXMatrix;

		glm::mat4 scaleMatrix	= glm::scale(scale);

		//return posMatrix * rotMatrix * scaleMatrix;
		return rotMatrix * posMatrix * scaleMatrix;
	}


	inline glm::vec3* getPos() { return &position;	}
	inline glm::vec3* getRot() { return &rotation;	}
	inline glm::vec3* getScl() { return &scale;		}


	//inline glm::vec3 getPos() { return position; }
	//inline glm::vec3 getRot() { return rotation; }
	//inline glm::vec3 getScl() { return scale;	 }


	inline void setPos(const glm::vec3& pos) { position = pos; }
	inline void setRot(const glm::vec3& rot) { rotation = rot; }
	inline void setScl(const glm::vec3& scl) { scale	= scl; }


	inline void updateAll(EventHandler& Controls)
	{
		//Player->azm = -Controls->turnH;
		//Player->alt = -Controls->turnV;
		//Player->rol = Controls->tiltP;
		//
		//Player->x -= Controls->moveP * cos(Player->azm) - Controls->strafeP * cos(Player->azm + PI * 0.5);
		//Player->y += Controls->moveP * sin(Player->azm) - Controls->strafeP * sin(Player->azm + PI * 0.5);
		//Player->z += Controls->riseP;
		
		rotation.x = (float)Controls.turnV;
		rotation.y = (float)Controls.turnH;
		rotation.z = (float)Controls.tiltP;

		position.z += (float)Controls.moveP * cosf(rotation.y) - (float)Controls.strafeP * cosf(rotation.y + 90.0f);
		position.x += (float)Controls.moveP * sinf(rotation.y) - (float)Controls.strafeP * sinf(rotation.y + 90.0f);
		position.y += (float)Controls.riseP;
	}


	~OpenGLTransform()
	{

	}
};

