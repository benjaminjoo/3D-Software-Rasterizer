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

	nearPlaneT		= { (0.0f, 0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f, 0.0f) };
	farPlaneT		= { (0.0f, 0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f, 0.0f) };
	leftPlaneT		= { (0.0f, 0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f, 0.0f) };
	rightPlaneT		= { (0.0f, 0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f, 0.0f) };
	topPlaneT		= { (0.0f, 0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f, 0.0f) };
	bottomPlaneT	= { (0.0f, 0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f, 0.0f) };
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


void ViewVolume::transformFrustum(mat4x4& MR, mat4x4& R)
{
	nearPlaneT.N	= R * nearPlane.N;
	nearPlaneT.P	= MR * nearPlane.P;
	farPlaneT.N		= R * farPlane.N;
	farPlaneT.P		= MR * farPlane.P;
	leftPlaneT.N	= R * leftPlane.N;
	leftPlaneT.P	= MR * leftPlane.P;
	rightPlaneT.N	= R * rightPlane.N;
	rightPlaneT.P	= MR * rightPlane.P;
	topPlaneT.N		= R * topPlane.N;
	topPlaneT.P		= MR * topPlane.P;
	bottomPlaneT.N	= R * bottomPlane.N;
	bottomPlaneT.P	= MR * bottomPlane.P;
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


plane ViewVolume::getNearPlaneT()
{
	return nearPlaneT;
}


plane ViewVolume::getFarPlaneT()
{
	return farPlaneT;
}


plane ViewVolume::getLeftPlaneT()
{
	return leftPlaneT;
}


plane ViewVolume::getRightPlaneT()
{
	return rightPlaneT;
}


plane ViewVolume::getTopPlaneT()
{
	return topPlaneT;
}


plane ViewVolume::getBottomPlaneT()
{
	return bottomPlaneT;
}
