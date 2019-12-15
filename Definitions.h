#pragma once

#include <SDL/SDL.h>
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


enum editingMode { Selection, Placement, LineDrawing, Relocation, Rotation, CopyRelocation, CopyRotation };


enum tool { none, arrow, cross, line, move, rotate, copy_move, copy_rotate, view_top, view_front, view_side, obj_snap, grid_snap};


enum toolStatus { active, inactive };


enum handedness { left, right };


enum projectionStyle { wireframe, solid_colour, checkerboard, flat_shaded, gouraud_shaded, depth_visualised, sunlight,
						torchlight, torchlight_solid, test };


enum model { solid, actor };


enum hit_response { penetrate, stick, bounce, slide };


enum aiGoal { be_idle, follow_player, kill_player, follow_others, kill_others };


typedef unsigned char byte;


struct soundBite
{
	SDL_AudioSpec	wavSpec;
	Uint32			wavLength;
	Uint8*			wavBuffer;
};


struct zColour
{
	double z		= 0.0f;
	Uint32 colour	= 0;
};


struct light
{
	double azm			= 0.0f;
	double alt			= 0.0f;
	double intensity	= 0.0f;
};


struct screenCoord
{
	int x = 0;
	int y = 0;
};


struct worldCoord
{
	double x = 0.0f;
	double y = 0.0f;
	double z = 0.0f;


	worldCoord operator + (const worldCoord& p)
	{
		return { x + p.x, y + p.y, z + p.z };
	}


	worldCoord operator - (const worldCoord& p)
	{
		return { x - p.x, y - p.y, z - p.z };
	}


	worldCoord operator += (const worldCoord& p)
	{
		return { x += p.x, y += p.y, z += p.z };
	}


	worldCoord operator -= (const worldCoord& p)
	{
		return { x -= p.x, y -= p.y, z -= p.z };
	}


	worldCoord operator ^ (const worldCoord& p)	//Cross Product
	{
		return { y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x };
	}


	double operator * (const worldCoord& p)		//Dot Product
	{
		return x * p.x + y * p.y + z * p.z;
	}
};


struct vertex3
{
	int			id			= 0;
	worldCoord	pos;
	bool		selected	= false;
	bool		deleted		= false;
};


struct line3
{
	int			id			= 0;
	worldCoord	vert[2];
	bool		selected	= false;
	bool		deleted		= false;
};


struct textCoord
{
	double u = 0.0f;
	double v = 0.0f;
};


struct coord2
{
	int x		= 0;
	int y		= 0;
	double z	= 0.0;
};


struct rect
{
	double xStart	= 0.0f;
	double yStart	= 0.0f;
	double xEnd		= 0.0f;
	double yEnd		= 0.0f;
};


struct vect2
{
	double x = 0.0f;
	double y = 0.0f;
	double u = 0.0f;
	double v = 0.0f;
};


struct vect3
{
	double x = 0.0f;
	double y = 0.0f;
	double z = 0.0f;
	double w = 0.0f;

	vect3	operator + 	(const vect3& p)
	{
		return { x + p.x, y + p.y, z + p.z, 1.0f };
	}

	vect3	operator - 	(const vect3& p)
	{
		return { x - p.x, y - p.y, z - p.z, 1.0f };
	}

	vect3	operator += (const vect3& p)
	{
		return { x += p.x, y += p.y, z += p.z, 1.0f };
	}

	vect3	operator -= (const vect3& p)
	{
		return { x -= p.x, y -= p.y, z -= p.z, 1.0f };
	}

	vect3	operator ^ (const vect3& p)				//Cross Product
	{
		return { y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x, 1.0f };
	}

	double	operator * (const vect3& p)				//Dot Product
	{
		return x * p.x + y * p.y + z * p.z;
	}
	vect3	operator *	(const double& s)			//Scale
	{
		return { x * s, y * s, z * s, 1.0f };
	}
};


bool match(const vect3& a, const vect3& b);


struct point3
{
	vect3 P;
	Uint32 colour = 0;
};


struct lump
{
	unsigned long offset = 0;
	unsigned long length = 0;
};


struct edge3d
{
	unsigned short startPoint	= 0;
	unsigned short endPoint		= 0;
};


struct vert
{
	float 			x = 0.0f;
	float 			y = 0.0f;
	float 			z = 0.0f;

	float 			u = 0.0f;
	float 			v = 0.0f;

	float 			m = 0.0f;
	float 			n = 0.0f;

	float 			nx = 0.0f;
	float 			ny = 0.0f;
	float 			nz = 0.0f;

	unsigned int 	r = 0;
	unsigned int 	g = 0;
	unsigned int 	b = 0;
	unsigned int 	a = 0;
};


struct face_t
{
	unsigned short 	plane_id	= 0;
	unsigned short 	side		= 0;
	long			ledge_id	= 0;
	unsigned short	ledge_num	= 0;
	unsigned short	texinfo_id	= 0;
	unsigned char	typelight	= 0;
	unsigned char	baselight	= 0;
	unsigned char	light[2]	= { 0, 0 };
	long			lightmap	= 0;
};


struct surface_t
{
	vert			S;
	float			distS		= 0.0f;
	vert			T;
	float			distT		= 0.0f;
	unsigned long	texture_id	= 0;
	unsigned long	animated	= 0;
};


struct miptex_t
{
	char 			name[16]	= { 0 };
	unsigned long 	width		= 0;
	unsigned long 	height		= 0;
	unsigned long 	offset_1	= 0;
	unsigned long 	offset_2	= 0;
	unsigned long 	offset_4	= 0;
	unsigned long 	offset_8	= 0;
};


struct face
{
	int			texture		= 0;
	int			effect		= 0;
	int			type		= 0;
	int			firstVert	= 0;
	int			nVert		= 0;
	int			meshVert	= 0;
	int			nMeshVert	= 0;
	int			lightMap	= 0;
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

	Uint32 colour = 0;
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
	int ID			= 0;

	int w			= 0;
	int h			= 0;

	Uint32* pixels	= nullptr;
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

	Uint32 colour	= 0;
	int texture		= 0;
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
	double	x = 0.0f;
	double 	y = 0.0f;
	int 	s = 0;
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

	Uint32 h		= 0;
	int texture		= 0;

	double illumA	= 0.0f;
	double illumB	= 0.0f;
	double illumC	= 0.0f;

	double illumFlat = 0.0f;

	textCoord At;
	textCoord Bt;
	textCoord Ct;
};


struct mat4x4
{
	double _00 = 0.0f;
	double _01 = 0.0f;
	double _02 = 0.0f;
	double _03 = 0.0f;

	double _10 = 0.0f;
	double _11 = 0.0f;
	double _12 = 0.0f;
	double _13 = 0.0f;

	double _20 = 0.0f;
	double _21 = 0.0f;
	double _22 = 0.0f;
	double _23 = 0.0f;

	double _30 = 0.0f;
	double _31 = 0.0f;
	double _32 = 0.0f;
	double _33 = 0.0f;
};


struct transform3d
{
	double x = 0.0f;
	double y = 0.0f;
	double z = 0.0f;

	double sinAzm = 0.0f;
	double cosAzm = 0.0f;
	double sinAlt = 0.0f;
	double cosAlt = 0.0f;
	double sinRol = 0.0f;
	double cosRol = 0.0f;
};


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

vect3 addVectors(const vect3& a, const vect3& b);

vect3 subVectors(const vect3& a, const vect3& b);

vect3 halfwayPoint(vect3 a, vect3 b);

double distanceSquared(vect3 a, vect3 b);

double lengthSquared(vect3 v);

vect3 unitVector(vect3 v);

worldCoord unitVector2(worldCoord v);

vect3 dirVector(double azm, double alt);

double dotProduct(const vect3& a, const vect3& b);

worldCoord rotate2(worldCoord target, Side view, worldCoord origin, double angle);

double dotProductSquared(const vect3& a, const vect3& b);

double distPoint2Plane(vect3 P, triangle3dV T);

double distPoint2Line(worldCoord P, Side view, line3 L);

double distPoint2LineSquared(vect3 p, vect3 a, vect3 b);

bool pointIsAroundLine(worldCoord P, Side view, line3 L);

vect3 crossProduct(vect3 a, vect3 b);

vect3 midPoint(vect3 a, vect3 b);

coord2 view2screen(vect3 vertex, int width, int height, double hR, double vR);

Uint32 getColour(const unsigned char& a, const unsigned char& r, const unsigned char& g, const unsigned char& b);

vect3 multiplyMxV(const mat4x4& m, const vect3& v);						//Multiplies 4x4 matrix and 1*4 column vector

vect3 multiplyVxM(const mat4x4& m, const vect3& v);						//Multiplies 1*4 column vector and 4x4 matrix

vect3 rotXrad(const double& sinA, const double& cosA, const vect3& v);	//Rotation around X - pre-calculated sin & cos values

vect3 rotYrad(const double& sinA, const double& cosA, const vect3& v);	//Rotation around Y - pre-calculated sin & cos values

vect3 rotZrad(const double& sinA, const double& cosA, const vect3& v);	//Rotation around Z - pre-calculated sin & cos values

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

void movePoly(vect3 m, triangle3dV& poly);

void transformMesh(int n, triangle3dV* object, vect3 m);

void rotateMesh(int n, triangle3dV* object, double rx, double ry, double rz);

void transformMesh(int n, triangle3dV* object, double mvX, double mvY, double mvZ);

void transformMesh(int n, triangle3dV* object, double scX, double scY, double scZ, double mvX, double mvY, double mvZ,
	double rX, double rY, double rZ);


vect3 sun2view(double sinAzm, double cosAzm, double sinAlt, double cosAlt,
				double sinRol, double cosRol, vect3 v);					//World-space to View space transformation of lightsources

bool onScreen(coord2 test, int w, int h);

int sign(const double& a);

int roundInt(double a);

//int GetYMin3(coord2* p);

//int GetYMax3(coord2* p);

template <class T> T getMinN(int n, T* list);

vect3 getXYZmin(int n, triangle3dV* T, double padding);

vect3 getXYZmax(int n, triangle3dV* T, double padding);

bool checkBoundingBoxForCollision(vect3 p, boundingBox B);

double pow10(int n);

int pow2(int n);

int* getFractionals(double number, int nDecimals);

int* getIntegers(double number, int* n);

void freePointerToPolyArray(triangle3dV** ptr, int nDimensions);
