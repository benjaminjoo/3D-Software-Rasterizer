#include "ViewVolume.h"
#include "Definitions.h"



ViewVolume::ViewVolume()
{
	nearPlane		= { (0.0, 0.0, 0.0, 0.0), (0.0, 0.0, 0.0, 0.0) };
	farPlane		= { (0.0, 0.0, 0.0, 0.0), (0.0, 0.0, 0.0, 0.0) };
	leftPlane		= { (0.0, 0.0, 0.0, 0.0), (0.0, 0.0, 0.0, 0.0) };
	rightPlane		= { (0.0, 0.0, 0.0, 0.0), (0.0, 0.0, 0.0, 0.0) };
	topPlane		= { (0.0, 0.0, 0.0, 0.0), (0.0, 0.0, 0.0, 0.0) };
	bottomPlane		= { (0.0, 0.0, 0.0, 0.0), (0.0, 0.0, 0.0, 0.0) };
}


ViewVolume::~ViewVolume()
{
}


void ViewVolume::initFrustum(double fovH, double fovV, double zNear, double zFar)
{
	nearPlane.N		= { 0.0,            0.0,            1.0,            0.0 };
	nearPlane.P		= { 0.0,            0.0,          zNear,            1.0 };
	farPlane.N		= { 0.0,            0.0,           -1.0,            0.0 };
	farPlane.P		= { 0.0,            0.0,           zFar,            1.0 };
	leftPlane.N		= { cos(fovH / 2),  0.0,  sin(fovH / 2), 	        0.0 };
	leftPlane.P		= { 0.0,            0.0,            0.0001,            1.0 };
	rightPlane.N	= { -cos(fovH / 2), 0.0,  sin(fovH / 2),            0.0 };
	rightPlane.P	= { 0.0,            0.0,            0.0001,            1.0 };
	topPlane.N		= { 0.0, cos(fovV / 2),  sin(fovV / 2),            0.0 };
	topPlane.P		= { 0.0,            0.0,            0.0001,            1.0 };
	bottomPlane.N	= { 0.0,  -cos(fovV / 2),  sin(fovV / 2),            0.0 };
	bottomPlane.P	= { 0.0,            0.0,            0.0001,            1.0 };
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
