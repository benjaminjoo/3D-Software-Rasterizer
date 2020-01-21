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

public:
	ViewVolume();
	~ViewVolume();

	void initFrustum(float, float, float, float);

	plane getNearPlane();
	plane getFarPlane();
	plane getLeftPlane();
	plane getRightPlane();
	plane getTopPlane();
	plane getBottomPlane();
};

