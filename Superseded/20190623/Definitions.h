#pragma once

#include <SDL.h>

#define SCREEN_WIDTH		1200
#define SCREEN_HEIGHT		600
#define SENSITIVITY			0.01f
#define PI					3.141592654
#define MAXCLIPVERTS		10

#define MAXENTITIES			1024

#define MAXLIGHTSOURCES		64
#define MAXSPHERES			64
#define MAXCUBES			64
#define MAXPRISMS			64
#define MAXCYLINDERS		64
#define MAXCONES			64
#define MAXMODELS			64


typedef unsigned char byte;


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


struct triangle3dV	//3D Triangle with vertex normals for Gouraud shading
{
	vect3 A;
	vect3 B;
	vect3 C;

	vect3 An;
	vect3 Bn;
	vect3 Cn;

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


struct triangle2dG	//2D Triangle for Gouraud shading
{
	coord2 a;
	coord2 b;
	coord2 c;

	Uint32 h;

	double illumA;
	double illumB;
	double illumC;
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


struct transform3d
{
	double x;
	double y;
	double z;

	double sinAzm;
	double cosAzm;
	double sinAlt;
	double cosAlt;
	double sinRol;
	double cosRol;
};


void clampValue(double* value, double lower, double upper);

vect3 addVectors(vect3 a, vect3 b);

vect3 subVectors(vect3 a, vect3 b);

vect3 unitVector(vect3 v);

vect3 dirVector(double azm, double alt);

double dotProduct(vect3 a, vect3 b);

vect3 crossProduct(vect3 a, vect3 b);

coord2 view2screen(vect3 vertex, int width, int height, double hR, double vR);

Uint32 getColour(unsigned char a, unsigned char r, unsigned char g, unsigned char b);	

vect3 multiplyMxV(mat4x4 m, vect3 v);					//Multiplies 4x4 matrix and 1*4 column vector

vect3 multiplyVxM(mat4x4 m, vect3 v);					//Multiplies 1*4 column vector and 4x4 matrix

vect3 rotXrad(double sinA, double cosA, vect3 v);		//Rotation around X - pre-calculated sin & cos values

vect3 rotYrad(double sinA, double cosA, vect3 v);		//Rotation around Y - pre-calculated sin & cos values

vect3 rotZrad(double sinA, double cosA, vect3 v);		//Rotation around Z - pre-calculated sin & cos values

vect3 translate(double x, double y, double z, vect3 v);	//Translation

//void initFrustum(double fovH, double fovV, double zNear, double zFar, box* f);

vect3 sun2view(double sinAzm, double cosAzm, double sinAlt, double cosAlt,
	double sinRol, double cosRol, vect3 v);	//World-space to View space transformation of lightsources

bool onScreen(coord2 test, int w, int h);

//void drawCrosshair(Uint32* pixels, int w, int h, int hole, int size, Uint32 colour);

int sign(double a);

int GetYMin3(coord2* p);

int GetYMax3(coord2* p);
