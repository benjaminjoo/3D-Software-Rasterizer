#pragma once

#include <SDL.h>
#include <vector>


#define GRAVITY				0.5

#define SCREEN_WIDTH		900
#define SCREEN_HEIGHT		450
#define MAX_MARGIN			200
#define SENSITIVITY			0.01f
#define PI					3.141592654
#define MAXCLIPVERTS		10

#define MIN_ILLUMINATION	0.1
#define MAX_ILLUMINATION	1.0

#define MAXENTITIES			1024

#define MAXLIGHTSOURCES		64
#define MAXSPHERES			64
#define MAXCUBES			64
#define MAXPRISMS			64
#define MAXCYLINDERS		64
#define MAXCONES			64


enum Side { Bottom, Top, Back, Front, Left, Right };


enum handedness { left, right };


typedef unsigned char byte;


struct light
{
	double azm;
	double alt;
	double intensity;
};


struct textCoord
{
	double u;
	double v;
};


struct coord2
{
	int x = 0;
	int y = 0;
	double z = 0.0;
};


struct rect
{
	double xStart;
	double yStart;
	double xEnd;
	double yEnd;
};


struct vect2
{
	double x;
	double y;
	double		u;
	double		v;
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

	textCoord At;
	textCoord Bt;
	textCoord Ct;

	Uint32 colour;
	int texture;
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


struct polyNode
{
	double	x;
	double 	y;
	int 	s;
};


struct polygon
{
	polyNode points[10];
	polygon* childLeft;
	polygon* childRight;
};


struct polygon4uv
{
	vect2 points[4];
	polygon4uv* leftChild;
	polygon4uv* rightChild;
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
	int texture;

	double illumA;
	double illumB;
	double illumC;

	textCoord At;
	textCoord Bt;
	textCoord Ct;
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


struct txt
{
	int ID;

	int w;
	int h;

	Uint32* pixels;
};


float quickSqrt(float number);

void clampValue(double* value, double lower, double upper);

double distancePN(polyNode a, polyNode b);

polyNode subNodes(polyNode a, polyNode b);

polyNode addNodes(polyNode a, polyNode b);

polyNode unitNode(polyNode a);

double dotNodes(polyNode a, polyNode b);

polyNode getNormal(polyNode a, polyNode b);

//bool checkPolygonForSplitting(int n, polyNode* P, polyNode a, polyNode b);
bool checkPolygonForSplitting(int n, vect2* P, polyNode a, polyNode b);
bool checkPolygonForSplitting(polygon4uv P, polyNode a, polyNode b);

//void splitPolygon(polygon4uv V, polyNode p, polyNode q);

//bool iSect2dLine(polyNode a, polyNode b, polyNode p, polyNode q, polyNode* r);
bool iSect2dLine(vect2 a, vect2 b, polyNode p, polyNode q, vect2* r);

vect3 addVectors(vect3 a, vect3 b);

vect3 subVectors(vect3 a, vect3 b);

double distanceSquared(vect3 a, vect3 b);

vect3 unitVector(vect3 v);

vect3 dirVector(double azm, double alt);

double dotProduct(vect3 a, vect3 b);

vect3 crossProduct(vect3 a, vect3 b);

coord2 view2screen(vect3 vertex, int width, int height, double hR, double vR);

Uint32 getColour(unsigned char a, unsigned char r, unsigned char g, unsigned char b);

 Uint32 modifyColour(Uint32, double, bool, bool, bool);

vect3 multiplyMxV(mat4x4 m, vect3 v);					//Multiplies 4x4 matrix and 1*4 column vector

vect3 multiplyVxM(mat4x4 m, vect3 v);					//Multiplies 1*4 column vector and 4x4 matrix

vect3 rotXrad(double sinA, double cosA, vect3 v);		//Rotation around X - pre-calculated sin & cos values

vect3 rotYrad(double sinA, double cosA, vect3 v);		//Rotation around Y - pre-calculated sin & cos values

vect3 rotZrad(double sinA, double cosA, vect3 v);		//Rotation around Z - pre-calculated sin & cos values

vect3 rotX(double angle, vect3 v);						//Rotation around X

vect3 rotY(double angle, vect3 v);						//Rotation around Y

vect3 rotZ(double angle, vect3 v);						//Rotation around Z

vect3 scale(double x, double y, double z, vect3 v);		//Scaling

vect3 translate(double x, double y, double z, vect3 v);	//Translation

triangle3dV rotXT(double angle, triangle3dV T);							//Rotation around X

triangle3dV rotYT(double angle, triangle3dV T);							//Rotation around Y

triangle3dV rotZT(double angle, triangle3dV T);							//Rotation around Z

triangle3dV scaleT(double x, double y, double z, triangle3dV T);		//Scaling

triangle3dV translateT(double x, double y, double z, triangle3dV T);	//Translation

triangle3dV* transformObject(int n, triangle3dV* object, double scX, double scY, double scZ, double mvX, double mvY, double mvZ,
						double rX, double rY, double rZ);

void transformMesh(int n, triangle3dV* object, double scX, double scY, double scZ, double mvX, double mvY, double mvZ,
	double rX, double rY, double rZ);


vect3 sun2view(double sinAzm, double cosAzm, double sinAlt, double cosAlt,
				double sinRol, double cosRol, vect3 v);	//World-space to View space transformation of lightsources

bool onScreen(coord2 test, int w, int h);

int sign(double a);

int roundInt(double a);

int GetYMin3(coord2* p);

int GetYMax3(coord2* p);

double pow10(int n);

int pow2(int n);

int* getFractionals(double number, int nDecimals);

int* getIntegers(double number, int* n);
