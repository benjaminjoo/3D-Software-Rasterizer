#include "Camera.h"
#include "math.h"

#define PI					3.141592654

Camera::Camera()
{
	x		= 0.0;
	y		= 0.0;
	z		= 0.0;

	step	= 1.0;

	azm		= 0.0;
	alt		= 0.0;
	rol		= 0.0;

	fovH	= PI * 0.5 / 90 * 85;
	zNear	= 1.0;
	zFar	= 99.9;

	w		= 320;
	h		= 200;
}


Camera::Camera(double cx, double cy, double cz, int width, int height)
{
	x		= cx;
	y		= cy;
	z		= cz;

	step	= 1.0;

	azm		= 0.0;
	alt		= 0.0;
	rol		= 0.0;

	fovH	= 90.0;
	zNear	= 1.0;
	zFar	= 999.0;

	w		= width;
	h		= height;
}


Camera::Camera(double cx, double cy, double cz, double stp, double az, double al, double rl,
				double fov, double nr, double fr, int width, int height)
{
	x		= cx;
	y		= cy;
	z		= cz;

	step	= stp;

	azm		= az;
	alt		= al;
	rol		= rl;

	fovH	= fov;

	zNear	= nr;
	zFar	= fr;

	w		= width;
	h		= height;
}


Camera::~Camera()
{
}


double Camera::getFovH()
{
	return fovH;
}


double Camera::getFovV()
{
	return (atan(tan(fovH * 0.5) / (w / h))) * 2;
}


double Camera::getHRatio()
{
	return 1 / (tan(fovH / 2));
}


double Camera::getVRatio()
{
	return 1 / (tan(atan(tan(fovH * 0.5) / (w / h) * 2)));
}
