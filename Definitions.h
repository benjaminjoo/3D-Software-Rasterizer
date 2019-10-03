#pragma once

#include <SDL.h>
#include <vector>
#include <string>


#define GRAVITY				0.5

#define SCREEN_WIDTH		900
#define SCREEN_HEIGHT		450
#define EDITOR_WIDTH		1200
#define EDITOR_HEIGHT		600
#define MAX_MARGIN			200
#define SENSITIVITY			0.01f
#define PI					3.141592654
#define MAXCLIPVERTS		10

#define MIN_ILLUMINATION	0.1
#define MAX_ILLUMINATION	1.25

#define MAXENTITIES			1024

#define MAXLIGHTSOURCES		64
#define MAXSPHERES			64
#define MAXCUBES			64
#define MAXPRISMS			64
#define MAXCYLINDERS		64
#define MAXCONES			64

#define RED			getColour(0, 255,   0,   0)
#define GREEN		getColour(0,   0, 255,   0)
#define BLUE		getColour(0,   0,   0, 255)
#define YELLOW		getColour(0, 255, 255,   0)
#define ORANGE		getColour(0, 255, 127,	 0)
#define LIGHT_BLUE	getColour(0, 127, 127, 255)
#define WHITE		getColour(0, 255, 255, 255)
#define LIGHT_GRAY	getColour(0, 191, 191, 191)
#define DARK_GRAY	getColour(0, 127, 127, 127)



enum Side { Bottom, Top, Back, Front, Left, Right };


enum editingMode { Selection, Placement, Relocation, Rotation };


enum tool { none, arrow, cross, move, rotate, view_top, view_front, view_side };


enum toolStatus { active, inactive };


enum handedness { left, right };


enum projectionStyle { wireframe, solid_colour, checkerboard, flat_shaded, gouraud_shaded, depth_visualised, sunlight,
						torchlight, torchlight_solid, test };


enum model { solid, actor };


typedef unsigned char byte;


struct zColour
{
	double z;
	Uint32 colour;
};


struct light
{
	double azm;
	double alt;
	double intensity;
};


struct screenCoord
{
	int x;
	int y;
};


struct worldCoord
{
	double x;
	double y;
	double z;
};


struct vertex3
{
	int			id;
	worldCoord	pos;
	bool		selected;
	bool		deleted;
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


struct point3
{
	vect3 P;
	Uint32 colour;
};


struct lump
{
	unsigned long offset;
	unsigned long length;
};


struct edge3d
{
	unsigned short startPoint;
	unsigned short endPoint;
};


struct vert
{
	float 			x;
	float 			y;
	float 			z;

	float 			u;
	float 			v;

	float 			m;
	float 			n;

	float 			nx;
	float 			ny;
	float 			nz;

	unsigned int 	r;
	unsigned int 	g;
	unsigned int 	b;
	unsigned int 	a;
};


struct face_t
{
	unsigned short 	plane_id;
	unsigned short 	side;
	long			ledge_id;
	unsigned short	ledge_num;
	unsigned short	texinfo_id;
	unsigned char	typelight;
	unsigned char	baselight;
	unsigned char	light[2];
	long			lightmap;
};


struct surface_t
{
	vert			S;
	float			distS;
	vert			T;
	float			distT;
	unsigned long	texture_id;
	unsigned long	animated;
};


struct miptex_t
{
	char 			name[16];
	unsigned long 	width;
	unsigned long 	height;
	unsigned long 	offset_1;
	unsigned long 	offset_2;
	unsigned long 	offset_4;
	unsigned long 	offset_8;
};


struct face
{
	int			texture;
	int			effect;
	int			type;
	int			firstVert;
	int			nVert;
	int			meshVert;
	int			nMeshVert;
	int			lightMap;
	coord2		lmStart;
	coord2		lmSize;
	vect3		lmOrigin;
	vect3		lmSvect;
	vect3		lmTvect;
	vect3		N;
	coord2		patchSize;
};


struct line3d
{
	vect3 A;
	vect3 B;

	Uint32 colour;
};


struct triangle3d
{
	vect3 A;
	vect3 B;
	vect3 C;
	vect3 N;
};


struct txt
{
	int ID;

	int w;
	int h;

	Uint32* pixels;
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


struct boundingBox
{
	vect3 minExt;
	vect3 maxExt;
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

	double illumFlat;

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


float quickSqrt(float number);

void clampValue(double* value, double lower, double upper);

double distancePN(polyNode a, polyNode b);

polyNode subNodes(polyNode a, polyNode b);

polyNode addNodes(polyNode a, polyNode b);

polyNode unitNode(polyNode a);

double dotNodes(polyNode a, polyNode b);

polyNode getNormal(polyNode a, polyNode b);

bool checkPolygonForSplitting(int n, vect2* P, polyNode a, polyNode b);

bool checkPolygonForSplitting(polygon4uv P, polyNode a, polyNode b);

bool iSect2dLine(vect2 a, vect2 b, polyNode p, polyNode q, vect2* r);

void printCoord3(vect3);

vect3 invertVector(vect3 a);

vect3 addVectors(vect3 a, vect3 b);

worldCoord addVectors2(worldCoord a, worldCoord b);

inline vect3 subVectors(vect3 a, vect3 b);

worldCoord subVectors2(worldCoord a, worldCoord b);

vect3 halfwayPoint(vect3 a, vect3 b);

double distanceSquared(vect3 a, vect3 b);

vect3 unitVector(vect3 v);

worldCoord unitVector2(worldCoord v);

vect3 dirVector(double azm, double alt);

inline double dotProduct(vect3 a, vect3 b);

double dotProduct2(worldCoord a, worldCoord b);

worldCoord rotate2(worldCoord target, Side view, worldCoord origin, double angle);

double dotProductSquared(vect3 a, vect3 b);

double distPoint2Plane(vect3, triangle3dV);

vect3 crossProduct(vect3 a, vect3 b);

worldCoord crossProduct(worldCoord a, worldCoord b);

vect3 midPoint(vect3 a, vect3 b);

coord2 view2screen(vect3 vertex, int width, int height, double hR, double vR);

Uint32 getColour(unsigned char a, unsigned char r, unsigned char g, unsigned char b);

Uint32 modifyColour(Uint32, double, bool, bool, bool);

vect3 multiplyMxV(mat4x4 m, vect3 v);									//Multiplies 4x4 matrix and 1*4 column vector

vect3 multiplyVxM(mat4x4 m, vect3 v);									//Multiplies 1*4 column vector and 4x4 matrix

vect3 rotXrad(double sinA, double cosA, vect3 v);						//Rotation around X - pre-calculated sin & cos values

vect3 rotYrad(double sinA, double cosA, vect3 v);						//Rotation around Y - pre-calculated sin & cos values

vect3 rotZrad(double sinA, double cosA, vect3 v);						//Rotation around Z - pre-calculated sin & cos values

vect3 rotX(double angle, vect3 v);										//Rotation around X

vect3 rotY(double angle, vect3 v);										//Rotation around Y

vect3 rotZ(double angle, vect3 v);										//Rotation around Z

vect3 scale(double x, double y, double z, vect3 v);						//Scaling

vect3 scaleVector(double s, vect3 v);

vect3 translate(double x, double y, double z, vect3 v);					//Translation

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
				double sinRol, double cosRol, vect3 v);					//World-space to View space transformation of lightsources

bool onScreen(coord2 test, int w, int h);

int sign(double a);

int roundInt(double a);

int GetYMin3(coord2* p);

int GetYMax3(coord2* p);

template <class T> T getMinN(int n, T* list);

vect3 getXYZmin(int n, triangle3dV* T, double padding);

vect3 getXYZmax(int n, triangle3dV* T, double padding);

bool checkBoundingBoxForCollision(vect3 p, boundingBox B);

double pow10(int n);

int pow2(int n);

int* getFractionals(double number, int nDecimals);

int* getIntegers(double number, int* n);

void freePointerToPolyArray(triangle3dV** ptr, int nDimensions);
