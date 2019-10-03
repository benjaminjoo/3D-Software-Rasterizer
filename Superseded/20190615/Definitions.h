#pragma once

#include <SDL.h>

#define SCREEN_WIDTH		1200
#define SCREEN_HEIGHT		600
#define SENSITIVITY			0.01f
#define PI					3.141592654
#define MAXCLIPVERTS		10

const double zNear	=	1.0;
const double zFar	=	99.0;
const double zVis	=	99.9;


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


struct light
{
	double azm;
	double alt;
	double intensity;
};


struct vect3
{
	double x;
	double y;
	double z;
	double w;
};


struct triangle3d
{
	vect3 A;
	vect3 B;
	vect3 C;
	vect3 N;
};


struct plane
{
	vect3 N;
	vect3 P;
};


struct box
{
	plane nearPlane;
	plane farPlane;
	plane leftPlane;
	plane rightPlane;
	plane topPlane;
	plane bottomPlane;
};


struct coord2
{
	int x = 0;
	int y = 0;
	double z = 0.0;
};


struct triangle2d
{
	coord2 a;
	coord2 b;
	coord2 c;

	Uint32 h;
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


vect3 addVectors(vect3 a, vect3 b);

vect3 subVectors(vect3 a, vect3 b);

vect3 unitVector(vect3 v);

vect3 dirVector(double azm, double alt);

double dotProduct(vect3 a, vect3 b);

coord2 world2Plan(double x, double y, double scale);

coord2 view2screen(vect3 vertex, int width, int height);

Uint32 getColour(unsigned char a, unsigned char r, unsigned char g, unsigned char b);	

vect3 multiplyMxV(mat4x4 m, vect3 v);					//Multiplies 4x4 matrix and 1*4 column vector

vect3 multiplyVxM(mat4x4 m, vect3 v);					//Multiplies 1*4 column vector and 4x4 matrix

mat4x4 multiplyMxM(mat4x4 p, mat4x4 q);					//Multiplies 4x4 matrix with 4x4 matrix

vect3 rotX(double angle, vect3 v);						//Rotation around X

vect3 rotY(double angle, vect3 v);						//Rotation around Y

vect3 rotZ(double angle, vect3 v);						//Rotation around Z

vect3 rotXrad(double sinA, double cosA, vect3 v);		//Rotation around X - pre-calculated sin & cos values

vect3 rotYrad(double sinA, double cosA, vect3 v);		//Rotation around Y - pre-calculated sin & cos values

vect3 rotZrad(double sinA, double cosA, vect3 v);		//Rotation around Z - pre-calculated sin & cos values

vect3 scale(double x, double y, double z, vect3 v);		//Scaling

vect3 translate(double x, double y, double z, vect3 v);	//Translation

void initFrustum(double fovH, double fovV, double zNear, double zFar, box* f);

vect3 world2view(camera p, vect3 v);					//World-space to View space transformation

vect3 world2viewT(double sinAzm, double cosAzm, double sinAlt, double cosAlt,
	double sinRol, double cosRol, camera p, vect3 v);	//World-space to View space transformation - pre-calculated values

vect3 sun2view(double sinAzm, double cosAzm, double sinAlt, double cosAlt,
	double sinRol, double cosRol, vect3 v);	//World-space to View space transformation of lightsources

void renderVertices(Uint32* canvas, Uint32 c, int n, vect3 vert, camera p, int w, int h);		//Not in use

Uint32 fillTriangle(triangle2d t, Uint32* pixels, int width, int height, Uint32 colour);

void fillTriangleZ(triangle2d t, Uint32* pixels, double* z, int width, int height, Uint32 colour);

/*Uint32 fillTriangleZLit(triangle2d t, Uint32* pixels, double* z, int width, int height,
	double illum, unsigned char r, unsigned char g, unsigned char b);*/


//bool inView(vect3 test, box* f);


bool onScreen(coord2 test, int w, int h);


void drawCrosshair(Uint32* pixels, int w, int h, int hole, int size, Uint32 colour);


int clipToFrustum(vect3* vertList, box f);


void clipPoly(int* nVert, vect3* vList, plane clippingPlane);


void clipEdge(plane p, vect3 startV, vect3 endV, int* nResult, vect3* temp);


int sign(double a);
