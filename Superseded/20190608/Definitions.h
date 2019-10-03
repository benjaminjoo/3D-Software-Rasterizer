#pragma once

#include <SDL.h>

#define SCREEN_WIDTH		1200
#define SCREEN_HEIGHT		600
#define SENSITIVITY			0.01f
#define PI					3.141592654


struct camera
{
	double x;
	double y;
	double z;

	double step;

	double azm;
	double alt;
	double rol;
};


struct vect3
{
	double x;
	double y;
	double z;
	double w;
};


struct coord2
{
	int x;
	int y;
};


struct mat4x4
{
	double _00;
	double _01;
	double _02;
	double _03;

	double _10;
	double _11;
	double _12;
	double _13;

	double _20;
	double _21;
	double _22;
	double _23;

	double _30;
	double _31;
	double _32;
	double _33;
};


coord2 world2Plan(double x, double y, double scale);

//vect3 world2view(vect3 vertex, camera eye);

coord2 view2screen(vect3 vertex, int width, int height);

Uint32 getColour(unsigned char a, unsigned char r, unsigned char g, unsigned char b);	

vect3 multiplyMxV(mat4x4 m, vect3 v);					//Multiplies 4x4 matrix and 1*4 column vector

vect3 multiplyVxM(mat4x4 m, vect3 v);					//Multiplies 1*4 column vector and 4x4 matrix

mat4x4 multiplyMxM(mat4x4 p, mat4x4 q);					//Multiplies 4x4 matrix with 4x4 matrix

vect3 rotX(double angle, vect3 v);						//Rotation around X

vect3 rotY(double angle, vect3 v);						//Rotation around Y

vect3 rotZ(double angle, vect3 v);						//Rotation around Z

vect3 scale(double x, double y, double z, vect3 v);		//Scaling

vect3 translate(double x, double y, double z, vect3 v);	//Translation

vect3 world2view(camera p, vect3 v);					//World-space to View space transformation
