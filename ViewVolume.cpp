#include "ViewVolume.h"
#include "Definitions.h"



ViewVolume::ViewVolume()
{
	nearPlane		= { (0.0f, 0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f, 0.0f) };
	farPlane		= { (0.0f, 0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f, 0.0f) };
	leftPlane		= { (0.0f, 0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f, 0.0f) };
	rightPlane		= { (0.0f, 0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f, 0.0f) };
	topPlane		= { (0.0f, 0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f, 0.0f) };
	bottomPlane		= { (0.0f, 0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f, 0.0f) };
}


ViewVolume::~ViewVolume()
{
}


void ViewVolume::initFrustum(float fovH, float fovV, float zNear, float zFar)
{
	float cosFovH2 = static_cast<float>(cos(fovH / 2));
	float cosFovV2 = static_cast<float>(cos(fovV / 2));
	float sinFovH2 = static_cast<float>(sin(fovH / 2));
	float sinFovV2 = static_cast<float>(sin(fovV / 2));

	nearPlane.N		= { 0.0f,            0.0f,          1.0f,            0.0f };
	nearPlane.P		= { 0.0f,            0.0f,         zNear,            1.0f };
	farPlane.N		= { 0.0f,            0.0f,         -1.0f,            0.0f };
	farPlane.P		= { 0.0f,            0.0f,          zFar,            1.0f };
	leftPlane.N		= { cosFovH2,		 0.0f,		sinFovH2, 	         0.0f };
	leftPlane.P		= { 0.0f,            0.0f,       0.0001f,            1.0f };
	rightPlane.N	= { -cosFovH2,		 0.0f,		sinFovH2,            0.0f };
	rightPlane.P	= { 0.0f,            0.0f,       0.0001f,            1.0f };
	topPlane.N		= { 0.0f,		 cosFovV2,		sinFovV2,            0.0f };
	topPlane.P		= { 0.0f,            0.0f,       0.0001f,            1.0f };
	bottomPlane.N	= { 0.0f,		-cosFovV2,		sinFovV2,            0.0f };
	bottomPlane.P	= { 0.0f,            0.0f,       0.0001f,            1.0f };
}


plane ViewVolume::getNearPlane()
{
	return nearPlane;
}


plane ViewVolume::getFarPlane()
{
	return farPlane;
}


plane ViewVolume::getLeftPlane()
{
	return leftPlane;
}


plane ViewVolume::getRightPlane()
{
	return rightPlane;
}


plane ViewVolume::getTopPlane()
{
	return topPlane;
}


plane ViewVolume::getBottomPlane()
{
	return bottomPlane;
}
