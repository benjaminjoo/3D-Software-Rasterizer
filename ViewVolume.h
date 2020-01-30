#pragma once

#include "Definitions.h"

class ViewVolume
{

	plane nearPlane;
	plane farPlane;
	plane leftPlane;
	plane rightPlane;
	plane topPlane;
	plane bottomPlane;

	plane nearPlaneT;
	plane farPlaneT;
	plane leftPlaneT;
	plane rightPlaneT;
	plane topPlaneT;
	plane bottomPlaneT;

public:
	ViewVolume();
	~ViewVolume();

	void initFrustum(float, float, float, float);
	void transformFrustum(mat4x4& RM, mat4x4& R);

	plane getNearPlane();
	plane getFarPlane();
	plane getLeftPlane();
	plane getRightPlane();
	plane getTopPlane();
	plane getBottomPlane();

	plane getNearPlaneT();
	plane getFarPlaneT();
	plane getLeftPlaneT();
	plane getRightPlaneT();
	plane getTopPlaneT();
	plane getBottomPlaneT();
};

