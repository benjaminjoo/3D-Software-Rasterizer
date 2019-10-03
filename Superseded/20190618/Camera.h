#pragma once

class Camera
{
public:
	double x;
	double y;
	double z;

	double step;

	double azm;
	double alt;
	double rol;

	double fovH;

	double zNear;
	double zFar;

	int w;
	int h;

	Camera();
	Camera(double, double, double, int, int);
	Camera(double, double, double, double, double, double, double, double, double, double, int, int);
	~Camera();

	double getFovH();
	double getFovV();

	double getHRatio();
	double getVRatio();
};

