#pragma once

#include <GLM/glm.hpp>

class Vertex
{
private:

	glm::vec3 position;
	glm::vec2 txtCoord;
	glm::vec3 normal;

public:

	Vertex()
	{
		position	= glm::vec3(0.0f, 0.0f, 0.0f);
		txtCoord	= glm::vec2(0.0f, 0.0f);
		normal		= glm::vec3(0.0f, 0.0f, 0.0f);
	}

	Vertex(const glm::vec3& pos, const glm::vec2& txt, const glm::vec3& norm = glm::vec3(0.0f, 0.0f, 0.0f))
	{
		position	= pos;
		txtCoord	= txt;
		normal		= norm;
	}

	inline glm::vec3 getPosition()	{ return position;	}
	inline glm::vec2 getTxtCoord()	{ return txtCoord;	}
	inline glm::vec3 getNormal()	{ return normal;	}

	inline void setPosition(const glm::vec3& pos) { position = pos; }
	inline void setTxtCoord(const glm::vec2& txt) { txtCoord = txt; }
	inline void setNormal(const glm::vec3& norm) { normal = norm; }
};

