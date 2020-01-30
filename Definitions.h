#pragma once

#include <SDL/SDL.h>
#include <vector>
#include <string>
#include <algorithm>
#include "c64Fonts.h"


#define GRAVITY				0.5f

#define SCREEN_WIDTH		1200
#define SCREEN_HEIGHT		600
#define EDITOR_WIDTH		1200
#define EDITOR_HEIGHT		600
#define MAX_MARGIN			200
#define SENSITIVITY			0.01f
#define PI					3.14159f
#define MAXCLIPVERTS		10

#define MIN_ILLUMINATION	0.1f
#define MAX_ILLUMINATION	1.25f

#define RED			getColour(0, 255,   0,   0)
#define GREEN		getColour(0,   0, 255,   0)
#define BLUE		getColour(0,   0,   0, 255)
#define YELLOW		getColour(0, 255, 255,   0)
#define ORANGE		getColour(0, 255, 127,	 0)
#define LIGHT_BLUE	getColour(0, 127, 127, 255)
#define WHITE		getColour(0, 255, 255, 255)
#define LIGHT_GRAY	getColour(0, 191, 191, 191)
#define DARK_GRAY	getColour(0, 127, 127, 127)


enum class axis { x, y, z };


enum class Side
{
	Bottom,
	Top,
	Back,
	Front,
	Left,
	Right
};


enum class editingMode
{
	Selection,
	Placement,
	LineDrawing,
	Relocation,
	Rotation,
	CopyRelocation,
	CopyRotation
};


enum class tool
{
	none,
	arrow,
	cross,
	line,
	move,
	rotate,
	copy_move,
	copy_rotate,
	view_top,
	view_front,
	view_side,
	obj_snap,
	grid_snap
};


enum class toolStatus
{
	active,
	inactive
};


enum class handedness
{
	left,
	right
};


enum class projectionStyle
{
	wireframe,
	solid_colour,
	checkerboard,
	flat_shaded,
	gouraud_shaded,
	blinn_phong,
	depth_visualised,
	sunlight,
	torchlight,
	torchlight_solid,
	fill_depth_buffer,
	shadow_test,
	test
};


enum class model
{
	solid,
	actor
};


enum class hit_response
{
	penetrate,
	stick,
	bounce,
	slide
};


enum class aiGoal
{
	be_idle,
	follow_player,
	kill_player,
	follow_others,
	kill_others,
	follow_each_other,
	kill_each_other
};


typedef unsigned char byte;


union colour32
{
	unsigned argb = 0;
	unsigned char c[4];
};


struct soundBite
{
	SDL_AudioSpec	wavSpec;
	Uint32			wavLength;
	Uint8* wavBuffer;
};


struct zColour
{
	float z = 0.0f;
	Uint32 colour = 0;
};


struct light
{
	float azm = 0.0f;
	float alt = 0.0f;
	float intensity = 0.0f;
};


struct screenCoord
{
	int x = 0;
	int y = 0;
};


struct worldCoord
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;


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


	float operator * (const worldCoord& p)		//Dot Product
	{
		return x * p.x + y * p.y + z * p.z;
	}
};


struct vertex3
{
	int			id = 0;
	worldCoord	pos;
	bool		selected = false;
	bool		deleted = false;
};


struct line3
{
	int			id = 0;
	worldCoord	vert[2];
	bool		selected = false;
	bool		deleted = false;
};


struct textCoord
{
	float u = 0.0f;
	float v = 0.0f;
};


struct coord2
{
	int x = 0;
	int y = 0;
	float z = 0.0f;
};


struct rect
{
	float xStart = 0.0f;
	float yStart = 0.0f;
	float xEnd = 0.0f;
	float yEnd = 0.0f;
};


struct vect2
{
	float x = 0.0f;
	float y = 0.0f;
	float u = 0.0f;
	float v = 0.0f;
};


struct matRT
{
	Uint32 diff_colour = 0x000000ff;
	Uint32 spec_colour = 0xffffffff;
	float albedo[4] = { 0.75f, 0.25f, 0.0f, 0.0f };
	float spec_exp = 32.0f;
	float refractIndex = 1.0f;
};


struct vect3
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;


	float len()
	{
		return sqrtf(x * x + y * y + z * z);
	}

	float len2()
	{
		return x * x + y * y + z * z;
	}

	vect3 norm()
	{
		float s = 1.0f / this->len();
		return { x * s, y * s, z * s, 1.0f };
	}

	vect3 scale(float s)
	{
		return { x * s, y * s, z * s, 1.0f };
	}

	vect3 operator | (const vect3& p)
	{
		vect3 sum = { x + p.x, y + p.y, z + p.z, 1.0f };
		return sum.norm();
	}

	vect3 operator + (const vect3& p)
	{
		return { x + p.x, y + p.y, z + p.z, 1.0f };
	}

	vect3 operator - (const vect3& p)
	{
		return { x - p.x, y - p.y, z - p.z, 1.0f };
	}

	vect3 operator += (const vect3& p)
	{
		x += p.x;
		y += p.y;
		z += p.z;
		return *this;
	}

	vect3 operator -= (const vect3& p)
	{
		x -= p.x;
		y -= p.y;
		z -= p.z;
		return *this;
	}

	vect3 operator ^ (const vect3& p)				//Cross Product
	{
		return { y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x, 1.0f };
	}

	float operator * (const vect3& p)				//Dot Product
	{
		return x * p.x + y * p.y + z * p.z;
	}

	vect3 operator * (const float& s)			//Scale
	{
		return { x * s, y * s, z * s, 1.0f };
	}
};


struct mat4x4
{
	float _00 = 0.0f;
	float _01 = 0.0f;
	float _02 = 0.0f;
	float _03 = 0.0f;

	float _10 = 0.0f;
	float _11 = 0.0f;
	float _12 = 0.0f;
	float _13 = 0.0f;

	float _20 = 0.0f;
	float _21 = 0.0f;
	float _22 = 0.0f;
	float _23 = 0.0f;

	float _30 = 0.0f;
	float _31 = 0.0f;
	float _32 = 0.0f;
	float _33 = 0.0f;

	vect3 operator * (const vect3& v)
	{
		vect3 t;

		t.x = v.x * _00 + v.y * _01 + v.z * _02 + v.w * _03;
		t.y = v.x * _10 + v.y * _11 + v.z * _12 + v.w * _13;
		t.z = v.x * _20 + v.y * _21 + v.z * _22 + v.w * _23;
		t.w = v.x * _30 + v.y * _31 + v.z * _32 + v.w * _33;

		return t;
	}

	mat4x4 operator * (const mat4x4& m)
	{
		mat4x4 t;

		t._00 = _00 * m._00 + _01 * m._10 + _02 * m._20 + _03 * m._30;
		t._01 = _00 * m._01 + _01 * m._11 + _02 * m._21 + _03 * m._31;
		t._02 = _00 * m._02 + _01 * m._12 + _02 * m._22 + _03 * m._32;
		t._03 = _00 * m._03 + _01 * m._13 + _02 * m._23 + _03 * m._33;

		t._10 = _10 * m._00 + _11 * m._10 + _12 * m._20 + _13 * m._30;
		t._11 = _10 * m._01 + _11 * m._11 + _12 * m._21 + _13 * m._31;
		t._12 = _10 * m._02 + _11 * m._12 + _12 * m._22 + _13 * m._32;
		t._13 = _10 * m._03 + _11 * m._13 + _12 * m._23 + _13 * m._33;

		t._20 = _20 * m._00 + _21 * m._10 + _22 * m._20 + _23 * m._30;
		t._21 = _20 * m._01 + _21 * m._11 + _22 * m._21 + _23 * m._31;
		t._22 = _20 * m._02 + _21 * m._12 + _22 * m._22 + _23 * m._32;
		t._23 = _20 * m._03 + _21 * m._13 + _22 * m._23 + _23 * m._33;

		t._30 = _30 * m._00 + _31 * m._10 + _32 * m._20 + _33 * m._30;
		t._31 = _30 * m._01 + _31 * m._11 + _32 * m._21 + _33 * m._31;
		t._32 = _30 * m._02 + _31 * m._12 + _32 * m._22 + _33 * m._32;
		t._33 = _30 * m._03 + _31 * m._13 + _32 * m._23 + _33 * m._33;

		return t;
	}
};


bool match(const vect3& a, const vect3& b);


struct point3
{
	vect3 P;
	vect3 N;
	Uint32 colour = 0;
};


struct lump
{
	unsigned long offset = 0;
	unsigned long length = 0;
};


struct edge3d
{
	unsigned short startPoint = 0;
	unsigned short endPoint = 0;
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
	unsigned short 	plane_id = 0;
	unsigned short 	side = 0;
	long			ledge_id = 0;
	unsigned short	ledge_num = 0;
	unsigned short	texinfo_id = 0;
	unsigned char	typelight = 0;
	unsigned char	baselight = 0;
	unsigned char	light[2] = { 0, 0 };
	long			lightmap = 0;
};


struct surface_t
{
	vert			S;
	float			distS = 0.0f;
	vert			T;
	float			distT = 0.0f;
	unsigned long	texture_id = 0;
	unsigned long	animated = 0;
};


struct miptex_t
{
	char 			name[16] = { 0 };
	unsigned long 	width = 0;
	unsigned long 	height = 0;
	unsigned long 	offset_1 = 0;
	unsigned long 	offset_2 = 0;
	unsigned long 	offset_4 = 0;
	unsigned long 	offset_8 = 0;
};


struct face
{
	int			texture = 0;
	int			effect = 0;
	int			type = 0;
	int			firstVert = 0;
	int			nVert = 0;
	int			meshVert = 0;
	int			nMeshVert = 0;
	int			lightMap = 0;
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
	int ID = 0;

	int w = 0;
	int h = 0;

	Uint32* pixels = nullptr;
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

	Uint32 colour = 0;
	int texture = 0;
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
	float	x = 0.0f;
	float 	y = 0.0f;
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
	polygon4uv* leftChild = nullptr;
	polygon4uv* rightChild = nullptr;
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

	Uint32 h = 0;
	int texture = 0;

	float illumA = 0.0f;
	float illumB = 0.0f;
	float illumC = 0.0f;

	float illumFlat = 0.0f;

	textCoord At;
	textCoord Bt;
	textCoord Ct;
};

/*
struct transform3d
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	float sinAzm = 0.0f;
	float cosAzm = 0.0f;
	float sinAlt = 0.0f;
	float cosAlt = 0.0f;
	float sinRol = 0.0f;
	float cosRol = 0.0f;
};
*/
float distancePN(polyNode a, polyNode b);

polyNode subNodes(polyNode a, polyNode b);

polyNode addNodes(polyNode a, polyNode b);

polyNode unitNode(polyNode a);

float dotNodes(polyNode a, polyNode b);

polyNode getNormal(polyNode a, polyNode b);

bool checkPolygonForSplitting(int n, vect2* P, polyNode a, polyNode b);

bool checkPolygonForSplitting(polygon4uv P, polyNode a, polyNode b);

bool iSect2dLine(vect2 a, vect2 b, polyNode p, polyNode q, vect2* r);

void printCoord3(vect3);

vect3 invertVector(vect3 a);

vect3 addVectors(const vect3& a, const vect3& b);

vect3 subVectors(const vect3& a, const vect3& b);

vect3 halfwayPoint(vect3 a, vect3 b);

float distanceSquared(vect3 a, vect3 b);

float lengthSquared(vect3 v);

vect3 unitVector(vect3 v);

worldCoord unitVector2(worldCoord v);

vect3 dirVector(float azm, float alt);

float dotProduct(const vect3& a, const vect3& b);

worldCoord rotate2(worldCoord target, Side view, worldCoord origin, float angle);

float dotProductSquared(const vect3& a, const vect3& b);

float distPoint2Plane(vect3 P, triangle3dV T);

float distPoint2Line(worldCoord P, Side view, line3 L);

float distPoint2LineSquared(vect3 p, vect3 a, vect3 b);

bool pointIsAroundLine(worldCoord P, Side view, line3 L);

vect3 crossProduct(vect3 a, vect3 b);

vect3 midPoint(vect3 a, vect3 b);

coord2 view2screen(vect3 vertex, int width, int height, float hR, float vR);

Uint32 getColour(const unsigned char& a, const unsigned char& r, const unsigned char& g, const unsigned char& b);

Uint32 getColour(const float& r, const float& g, const float& b);

bool onScreen(coord2 test, int w, int h);

int sign(const float& a);

int roundInt(float a);

template <class T> T getMinN(int n, T* list);

vect3 getXYZmin(int n, triangle3dV* T, float padding);

vect3 getXYZmax(int n, triangle3dV* T, float padding);

bool checkBoundingBoxForCollision(vect3 p, boundingBox B);

float pow10(int n);

int pow2(int n);

int* getFractionals(float number, int nDecimals);

int* getIntegers(float number, int* n);

void freePointerToPolyArray(triangle3dV** ptr, int nDimensions);

bool* getSinglePETSCIICharacter(char char_no);
