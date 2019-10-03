#include "Definitions.h"

Uint32 getColour(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
{
	return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}


vect3 multiplyMxV(mat4x4 m, vect3 v)
{
	vect3 t;

	t.x = v.x * m._00 + v.y * m._01 + v.z * m._02 + v.w * m._03;
	t.y = v.x * m._10 + v.y * m._11 + v.z * m._12 + v.w * m._13;
	t.z = v.x * m._20 + v.y * m._21 + v.z * m._22 + v.w * m._23;
	t.w = v.x * m._30 + v.y * m._31 + v.z * m._32 + v.w * m._33;

	return t;
}


vect3 multiplyVxM(mat4x4 m, vect3 v)
{
	vect3 t;

	t.x = v.x * m._00 + v.y * m._10 + v.z * m._20 + v.w * m._30;
	t.y = v.x * m._01 + v.y * m._11 + v.z * m._21 + v.w * m._31;
	t.z = v.x * m._02 + v.y * m._12 + v.z * m._22 + v.w * m._32;
	t.w = v.x * m._03 + v.y * m._13 + v.z * m._23 + v.w * m._33;

	return t;
}


vect3 rotX(double angle, vect3 v)
{
	vect3 t;

	double a = angle;// *PI / 180.0;
	mat4x4 m = {		1,          0,         0,          0,
						0,     cos(a),    sin(a),          0,
						0,    -sin(a),    cos(a),          0,
						0,          0,         0,          1 };
	t = multiplyVxM(m, v);

	return t;
}


vect3 rotY(double angle, vect3 v)
{
	vect3 t;

	double a = angle;// *PI / 180.0;
	mat4x4 m = {   cos(a),          0,   -sin(a),          0,
					    0,          1,         0,          0,
				   sin(a),          0,    cos(a),          0,
					    0,          0,         0,          1 };
	t = multiplyVxM(m, v);

	return t;
}


vect3 rotZ(double angle, vect3 v)
{
	vect3 t;

	double a = angle;// *PI / 180.0;
	mat4x4 m = {   cos(a),     sin(a),         0,          0,
				  -sin(a),     cos(a),         0,          0,
					    0,          0,         1,          0,
					    0,          0,         0,          1 };
	t = multiplyVxM(m, v);

	return t;
}


vect3 scale(double x, double y, double z, vect3 v)
{
	vect3 t;

	double scaleX = x;
	double scaleY = y;
	double scaleZ = z;
	mat4x4 m = {   scaleX,          0,         0,          0,
						0,     scaleY,         0,          0,
						0,          0,    scaleZ,          0,
						0,          0,         0,          1 };
	t = multiplyMxV(m, v);

	return t;
}


vect3 translate(double x, double y, double z, vect3 v)
{
	vect3 t;

	double deltaX = x;
	double deltaY = y;
	double deltaZ = z;
	mat4x4 m = {		1,          0,         0,     deltaX,
						0,          1,         0,     deltaY,
						0,          0,         1,     deltaZ,
						0,          0,         0,          1 };
	t = multiplyMxV(m, v);

	return t;
}


vect3 world2view(camera p, vect3 v)
{
	vect3 t;

	double azm = p.azm;// *PI / 180.0;
	double alt = p.alt;// *PI / 180.0;
	double rol = p.rol;// *PI / 180.0;

	//t = rotZ(rol, rotX(alt - PI * 0.5, rotZ(azm - PI * 0.5, translate(-p.x, -p.y, -p.z, v))));
	t = rotZ(rol, rotX(alt + PI * 0.5, rotZ(azm + PI / 2, translate(-p.x, -p.y, -p.z, v))));

	return t;
}


mat4x4 multiplyMxM(mat4x4 p, mat4x4 q)
{
	mat4x4 t;

	t._00 = p._00 * q._00 + p._01 * q._10 + p._02 * q._20 + p._03 * q._30;
	t._01 = p._00 * q._01 + p._01 * q._11 + p._02 * q._21 + p._03 * q._31;
	t._02 = p._00 * q._02 + p._01 * q._12 + p._02 * q._22 + p._03 * q._32;
	t._03 = p._00 * q._03 + p._01 * q._13 + p._02 * q._23 + p._03 * q._33;

	t._10 = p._10 * q._00 + p._11 * q._10 + p._12 * q._20 + p._13 * q._30;
	t._11 = p._10 * q._01 + p._11 * q._11 + p._12 * q._21 + p._13 * q._31;
	t._12 = p._10 * q._02 + p._11 * q._12 + p._12 * q._22 + p._13 * q._32;
	t._13 = p._10 * q._03 + p._11 * q._13 + p._12 * q._23 + p._13 * q._33;

	t._20 = p._20 * q._00 + p._21 * q._10 + p._22 * q._20 + p._23 * q._30;
	t._21 = p._20 * q._01 + p._21 * q._11 + p._22 * q._21 + p._23 * q._31;
	t._22 = p._20 * q._02 + p._21 * q._12 + p._22 * q._22 + p._23 * q._32;
	t._23 = p._20 * q._03 + p._21 * q._13 + p._22 * q._23 + p._23 * q._33;

	t._30 = p._30 * q._00 + p._31 * q._10 + p._32 * q._20 + p._33 * q._30;
	t._31 = p._30 * q._01 + p._31 * q._11 + p._32 * q._21 + p._33 * q._31;
	t._32 = p._30 * q._02 + p._31 * q._12 + p._32 * q._22 + p._33 * q._32;
	t._33 = p._30 * q._03 + p._31 * q._13 + p._32 * q._23 + p._33 * q._33;

	return t;
}



coord2 world2Plan(double x, double y, double scale)
{
	coord2 temp;
	temp.x = (int)(SCREEN_WIDTH / 2 + x * scale);
	temp.y = (int)(SCREEN_HEIGHT / 2 - y * scale);

	return temp;
}

/*
vect3 world2view(vect3 vertex, camera eye)
{
	vect3 temp;

	vertex.x += eye.x;
	vertex.y += eye.y;
	vertex.z += eye.z;

	temp.x = vertex.x * cos(eye.azm) - vertex.y * sin(eye.azm);		//z
	temp.y = vertex.x * sin(eye.azm) + vertex.y * cos(eye.azm);
	temp.z = vertex.z;

	vertex.x = temp.x;	vertex.y = temp.y;	vertex.z = temp.z;

	temp.x = vertex.z * sin(eye.rol) + vertex.x * cos(eye.rol);		//y
	temp.y = vertex.y;
	temp.z = vertex.z * cos(eye.rol) - vertex.x * sin(eye.rol);

	vertex.x = temp.x;	vertex.y = temp.y;	vertex.z = temp.z;

	temp.x = vertex.x;												//x
	temp.y = vertex.y * cos(PI / 2 + eye.alt) - vertex.z * sin(PI / 2 + eye.alt);
	temp.z = vertex.y * sin(PI / 2 + eye.alt) + vertex.z * cos(PI / 2 + eye.alt);

	vertex.x = temp.x;	vertex.y = temp.y;	vertex.z = temp.z;	temp.w = 1.0;

	return temp;
}
*/

coord2 view2screen(vect3 vertex, int width, int height)
{
	coord2 pixel;

	//pixel.x = (int)(SCREEN_WIDTH / 2 + scale * vertex.x / vertex.y);
	//pixel.y = (int)(SCREEN_HEIGHT / 2 - scale *	vertex.z / vertex.y);

	pixel.x = (int)(width * 0.5 + (vertex.x / vertex.z) * width * 0.5);
	pixel.y = (int)(height * 0.5 - (vertex.y / vertex.z) * width * 0.5);

	return pixel;
}