#define _CRT_SECURE_NO_DEPRECATE

#include "Definitions.h"
#include "Camera.h"
#include "LightSource.h"
#include "ViewVolume.h"
#include "Canvas.h"


Camera::Camera() :
	x(0.0f), y(0.0f), z(0.0f), step(1.0f), turn(0.1f), azm(0.0f), alt(0.0f), rol(0.0f),
	fovH(PI * 0.5f / 90.0f * 85.0f), zNear(1.0f), zFar(99.9f), w(320), h(200)
{
	std::cout << "Camera constructor called" << std::endl;

	Frustum.initFrustum(this->getFovH(), this->getFovV(), zNear, zFar);

	hRatio = this->getHRatio();
	vRatio = this->getVRatio();

	this->clearVertexList();

	Renderer = std::make_shared<Projection>();
}


Camera::Camera(float cx, float cy, float cz, int width, int height, int s) :
	x(cx), y(cy), z(cz), step(1.0f), turn(0.1f), azm(0.0f), alt(0.0f), rol(0.0f),
	fovH(PI * 0.5f / 90.0f * 85.0f), zNear(1.0f), zFar(99.9f), w(width), h(height)
{
	std::cout << "Camera constructor called" << std::endl;

	Frustum.initFrustum(this->getFovH(), this->getFovV(), zNear, zFar);	//

	hRatio = this->getHRatio();
	vRatio = this->getVRatio();

	this->clearVertexList();

	Renderer = std::make_shared<Projection>();
}


Camera::Camera(float cx, float cy, float cz, float az, float al, float rl, float stp, float trn,
	float fov, float nr, float fr, int width, int height, int s) :
	x(cx), y(cy), z(cz), step(stp), turn(trn), azm(az), alt(al), rol(rl),
	fovH(fov), zNear(nr), zFar(fr), w(width), h(height)
{
	std::cout << "Camera constructor called" << std::endl;

	Frustum.initFrustum(this->getFovH(), this->getFovV(), zNear, zFar);	//

	hRatio = this->getHRatio();
	vRatio = this->getVRatio();

	this->clearVertexList();

	Renderer = std::make_shared<Projection>();
}


Camera::~Camera()
{
	std::cout << "Camera destructor called" << std::endl;
}


void Camera::linkToCanvas(std::shared_ptr<Canvas> screen)
{
	pixelBuffer = screen->pixelBuffer;
	depthBuffer = screen->depthBuffer;

	Screen = screen;
}


void Camera::addTexture(SDL_Surface* T)
{
	if (T == nullptr)
	{
		std::cout << "Image loading failed..." << std::endl;
	}
	else
	{
		txt tempTexture;
		SDL_Surface* tempImage = SDL_ConvertSurfaceFormat(T, SDL_PIXELFORMAT_ARGB8888, 0);
		tempTexture.pixels = (Uint32*)tempImage->pixels;
		tempTexture.ID = textureData.size();
		tempTexture.w = T->w;
		tempTexture.h = T->h;
		textureData.push_back(tempTexture);
	}
}


void Camera::addTexture(txt t)
{
	textureData.push_back(t);
}


float Camera::getFovH()
{
	return fovH;
}


float Camera::getFovV()
{
	return (atanf(tanf(fovH * 0.5f) / (w / h))) * 2.0f;
}


float Camera::getHRatio()
{
	return 1 / (tan(fovH / 2.0f));
}


float Camera::getVRatio()
{
	return 1 / (tanf(atanf(tanf(fovH * 0.5f) / (w / h) * 2.0f)));
}


void Camera::updateViewDirection()
{
	viewDirection = { cos(-alt) * cos(-azm), cos(-alt) * sin(-azm), sin(-alt), 0.0f };
}


void Camera::updateViewVolume()
{
	mat4x4 rotX = getRotation(axis::x, -alt - PI * 0.5f);
	
	mat4x4 rotY = getRotation(axis::y, rol);
	
	mat4x4 rotZ = getRotation(axis::z, azm - PI * 0.5f);
	
	mat4x4 R = rotZ * rotY * rotX;

	mat4x4 MOV = getTranslation({ x, y, z, 1.0f });

	mat4x4 MR = MOV * R;

	Frustum.transformFrustum(MR, R);
}


bool Camera::pointFacingCamera(const point3& p)
{
	vect3 eyeVector = subVectors(p.P, { x, y, z, 1.0 });
	return dotProduct(p.N, eyeVector) < 0.0f ? true : false;
}


bool Camera::polyFacingCamera(const triangle3dV& worldT)
{
	vect3 eyeVector = subVectors(worldT.A, { x, y, z, 1.0 });
	return dotProduct(worldT.N, eyeVector) < 0.0f ? true : false;
}


void Camera::clearVertexList()
{
	for (int v = 0; v < MAXCLIPVERTS; v++)
	{
		vertexListV[v]	= { 0.0f, 0.0f, 0.0f, 0.0f };
		vertexListW[v]	= { 0.0f, 0.0f, 0.0f, 0.0f };
		uvList[v]		= { 0.0f, 0.0f };
		specularList[v] = { 0.0f };
	}
}


int Camera::clipToFrustum(const triangle3dV& viewT, vect3* vertList, textCoord* uvList, float* specList)
{
	memset(vertList, 0, MAXCLIPVERTS * sizeof(float));

	vertList[0] = viewT.A;	uvList[0] = viewT.At;	specList[0] = illSpec[0];
	vertList[1] = viewT.B;	uvList[1] = viewT.Bt;	specList[1] = illSpec[1];
	vertList[2] = viewT.C;	uvList[2] = viewT.Ct;	specList[2] = illSpec[2];

	int nVert = 3;
	plane currentPlane;

	currentPlane = Frustum.getNearPlane();
	this->clipPoly(&nVert, vertList, uvList, specList, currentPlane);
	currentPlane = Frustum.getTopPlane();
	this->clipPoly(&nVert, vertList, uvList, specList, currentPlane);
	currentPlane = Frustum.getBottomPlane();
	this->clipPoly(&nVert, vertList, uvList, specList, currentPlane);
	currentPlane = Frustum.getLeftPlane();
	this->clipPoly(&nVert, vertList, uvList, specList, currentPlane);
	currentPlane = Frustum.getRightPlane();
	this->clipPoly(&nVert, vertList, uvList, specList, currentPlane);
	currentPlane = Frustum.getFarPlane();
	this->clipPoly(&nVert, vertList, uvList, specList, currentPlane);

	return nVert;
}


int Camera::clipToFrustumT(const triangle3dV& worldT, vect3* vertList, textCoord* uvList, float* specList)
{
	memset(vertList, 0, MAXCLIPVERTS * sizeof(float));

	vertList[0] = worldT.A;	uvList[0] = worldT.At;	specList[0] = illSpec[0];
	vertList[1] = worldT.B;	uvList[1] = worldT.Bt;	specList[1] = illSpec[1];
	vertList[2] = worldT.C;	uvList[2] = worldT.Ct;	specList[2] = illSpec[2];

	int nVert = 3;
	plane currentPlane;

	currentPlane = Frustum.getNearPlaneT();
	this->clipPoly(&nVert, vertList, uvList, specList, currentPlane);
	currentPlane = Frustum.getTopPlaneT();
	this->clipPoly(&nVert, vertList, uvList, specList, currentPlane);
	currentPlane = Frustum.getBottomPlaneT();
	this->clipPoly(&nVert, vertList, uvList, specList, currentPlane);
	currentPlane = Frustum.getLeftPlaneT();
	this->clipPoly(&nVert, vertList, uvList, specList, currentPlane);
	currentPlane = Frustum.getRightPlaneT();
	this->clipPoly(&nVert, vertList, uvList, specList, currentPlane);
	currentPlane = Frustum.getFarPlaneT();
	this->clipPoly(&nVert, vertList, uvList, specList, currentPlane);

	return nVert;
}


inline void Camera::clipPoly(int* nVert, vect3* vertList, textCoord* uvList, float* specList, plane clippingPlane)
{
	int nResult = 0;
	vect3 vTemp[MAXCLIPVERTS];
	textCoord uvTemp[MAXCLIPVERTS];
	float specTemp[MAXCLIPVERTS];

	for (int i = 0; i < *nVert; i++)
	{
		if (i < (*nVert) - 1)
		{
			this->clipEdge(clippingPlane,
				vertList[i], vertList[i + 1],
				uvList[i], uvList[i + 1],
				specList[i], specList[i + 1],
				&nResult, vTemp, uvTemp, specTemp);
		}
		else
		{
			this->clipEdge(clippingPlane,
				vertList[i], vertList[0],
				uvList[i], uvList[0],
				specList[i], specList[0],
				&nResult, vTemp, uvTemp, specTemp);
		}
	}
	for (int j = 0; j < nResult; j++)
	{
		vertList[j] = vTemp[j];
		uvList[j] = uvTemp[j];
		specList[j] = specTemp[j];
	}

	*nVert = nResult;
}


inline void Camera::clipEdge(const plane& p, const vect3& startV, const vect3& endV, const textCoord& startUV, const textCoord& endUV,
	const float& startSpec, const float& endSpec, int* nResult, vect3* vTemp, textCoord* uvTemp, float* specTemp)
{
	float t;
	//vect3 a = subVectors(startV, p.P);
	//vect3 a = startV - p.P;
	vect3 a = { startV.x - p.P.x, startV.y - p.P.y, startV.z - p.P.z, 1.0 };
	//float sStart = dotProduct(a, p.N);
	//float sStart = a * p.N;
	float sStart = a.x * p.N.x + a.y * p.N.y + a.z * p.N.z;
	//vect3 b = subVectors(endV, p.P);
	//vect3 b = endV - p.P;
	vect3 b = { endV.x - p.P.x, endV.y - p.P.y, endV.z - p.P.z, 1.0 };
	//float sEnd = dotProduct(b, p.N);	
	//float sEnd = b * p.N;
	float sEnd = b.x * p.N.x + b.y * p.N.y + b.z * p.N.z;
	if (sign(sStart) != sign(sEnd))
	{
		//vect3 d = subVectors(endV, startV);
		//vect3 d = endV - startV;
		vect3 d = { endV.x - startV.x, endV.y - startV.y, endV.z - startV.z, 1.0f };
		//float dist = dotProduct(d, p.N);	
		//float dist = d * p.N;
		float dist = d.x * p.N.x + d.y * p.N.y + d.z * p.N.z;

		if (sStart < 0)
		{
			if (dist)
			{
				t = (dist - sEnd) / dist;

				vTemp[*nResult].x = startV.x + t * (endV.x - startV.x);
				vTemp[*nResult].y = startV.y + t * (endV.y - startV.y);
				vTemp[*nResult].z = startV.z + t * (endV.z - startV.z);
				vTemp[*nResult].w = startV.w + t * (endV.w - startV.w);

				//vTemp[*nResult] = startV + ((endV - startV) * t);

				uvTemp[*nResult].u = startUV.u + t * (endUV.u - startUV.u);
				uvTemp[*nResult].v = startUV.v + t * (endUV.v - startUV.v);

				specTemp[*nResult] = startSpec + t * (endSpec - startSpec);

				(*nResult)++;
			}
		}
		if (sEnd < 0)
		{
			if (dist)
			{
				t = (-dist - sStart) / dist;

				vTemp[*nResult].x = endV.x - t * (startV.x - endV.x);
				vTemp[*nResult].y = endV.y - t * (startV.y - endV.y);
				vTemp[*nResult].z = endV.z - t * (startV.z - endV.z);
				vTemp[*nResult].w = endV.w - t * (startV.w - endV.w);

				//vTemp[*nResult] = endV - ((startV - endV) * t);

				uvTemp[*nResult].u = endUV.u - t * (startUV.u - endUV.u);
				uvTemp[*nResult].v = endUV.v - t * (startUV.v - endUV.v);

				specTemp[*nResult] = endSpec - t * (startSpec - endSpec);

				(*nResult)++;
			}
			else
			{

				vTemp[*nResult].x = endV.x;
				vTemp[*nResult].y = endV.y;
				vTemp[*nResult].z = endV.z;
				vTemp[*nResult].w = endV.w;

				//vTemp[*nResult] = endV;

				uvTemp[*nResult].u = endUV.u;
				uvTemp[*nResult].v = endUV.v;

				specTemp[*nResult] = endSpec;

				(*nResult)++;
			}
		}
		else
		{

			vTemp[*nResult].x = endV.x;
			vTemp[*nResult].y = endV.y;
			vTemp[*nResult].z = endV.z;
			vTemp[*nResult].w = endV.w;

			//vTemp[*nResult] = endV;

			uvTemp[*nResult].u = endUV.u;
			uvTemp[*nResult].v = endUV.v;

			specTemp[*nResult] = endSpec;

			(*nResult)++;
		}
	}
	else
	{
		if (sign(sStart) > 0 && sign(sEnd) > 0)
		{

			vTemp[*nResult].x = endV.x;
			vTemp[*nResult].y = endV.y;
			vTemp[*nResult].z = endV.z;
			vTemp[*nResult].w = endV.w;

			//vTemp[*nResult] = endV;

			uvTemp[*nResult].u = endUV.u;
			uvTemp[*nResult].v = endUV.v;

			specTemp[*nResult] = endSpec;

			(*nResult)++;
		}
	}
}


bool Camera::pointInsideFrustum(vect3& V)
{
	plane currentPlane;

	currentPlane = Frustum.getNearPlane();
	if (pointBehindPlane(currentPlane, V))
		return false;

	currentPlane = Frustum.getTopPlane();
	if (pointBehindPlane(currentPlane, V))
		return false;

	currentPlane = Frustum.getBottomPlane();
	if (pointBehindPlane(currentPlane, V))
		return false;

	currentPlane = Frustum.getLeftPlane();
	if (pointBehindPlane(currentPlane, V))
		return false;

	currentPlane = Frustum.getRightPlane();
	if (pointBehindPlane(currentPlane, V))
		return false;

	currentPlane = Frustum.getFarPlane();
	if (pointBehindPlane(currentPlane, V))
		return false;

	return true;
}


bool Camera::pointBehindPlane(const plane& p, vect3& V)
{
	vect3 a = V - p.P;
	float sa = a * p.N;
	if (sa <= 0.0f)
		return true;
	else
		return false;
}


void Camera::centreLook()
{
	alt = 0.0f;
}


mat4x4 Camera::getTranslation(vect3 mv)
{
	mat4x4 result;

	result = { 1.0f,          0.0f,         0.0f,		 mv.x,
				0.0f,          1.0f,         0.0f,		 mv.y,
				0.0f,          0.0f,         1.0f,		 mv.z,
				0.0f,          0.0f,         0.0f,       1.0f };

	return result;
}


mat4x4 Camera::getTranslation()
{
	mat4x4 result;

	result = { 1.0f,          0.0f,         0.0f,		-x,
				0.0f,          1.0f,         0.0f,		-y,
				0.0f,          0.0f,         1.0f,		-z,
				0.0f,          0.0f,         0.0f,       1.0f };

	return result;
}


mat4x4 Camera::getRotation(axis t, float a)
{
	mat4x4 result = { 1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f };

	float sinA = sinf(a);
	float cosA = cosf(a);

	switch (t)
	{
	case axis::x:
	{
		result = { 1.0f,       0.0f,       0.0f,       0.0f,
					0.0f,       cosA,       sinA,       0.0f,
					0.0f,      -sinA,       cosA,       0.0f,
					0.0f,       0.0f,       0.0f,       1.0f };
	}
	break;
	case axis::y:
	{
		result = { cosA,       0.0f,      -sinA,       0.0f,
					0.0f,       1.0f,       0.0f,       0.0f,
					sinA,       0.0f,       cosA,       0.0f,
					0.0f,       0.0f,       0.0f,       1.0f };
	}
	break;
	case axis::z:
	{
		result = { cosA,       sinA,       0.0f,       0.0f,
				   -sinA,       cosA,       0.0f,       0.0f,
					0.0f,       0.0f,       1.0f,       0.0f,
					0.0f,       0.0f,       0.0f,       1.0f };
	}
	break;
	}

	return result;
}


mat4x4 Camera::getRotation()
{
	mat4x4 result = getRotation(axis::z, rol) *
		getRotation(axis::x, -(alt + PI * 0.5f)) *
		getRotation(axis::z, -(azm + PI * 0.5f));

	return result;
}


void Camera::object2world(mat4x4& MR, mat4x4& R, triangle3dV& T)
{
	T.A = MR * T.A;
	T.B = MR * T.B;
	T.C = MR * T.C;
	T.An = R * T.An;
	T.Bn = R * T.Bn;
	T.Cn = R * T.Cn;
	T.N = R * T.N;
}


void Camera::world2view(mat4x4& RM, mat4x4& R, triangle3dV& T)
{
	T.A = RM * T.A;
	T.B = RM * T.B;
	T.C = RM * T.C;
	T.An = R * T.An;
	T.Bn = R * T.Bn;
	T.Cn = R * T.Cn;
	T.N = R * T.N;
}


void Camera::world2view(mat4x4& RM, int n)
{
	for (int i = 0; i < n; i++)
		vertexListV[i] = RM * vertexListW[i];
}


void Camera::world2viewPointM(point3& P, mat4x4& RM)
{
	P.P = RM * P.P;
}


void Camera::renderPoint(point3 p, mat4x4& RM, Uint32* pixelBuffer, float* depthBuffer)
{
	this->world2viewPointM(p, RM);
	if (pointInsideFrustum(p.P))
		projectPoint(p, pixelBuffer, depthBuffer);
}


void Camera::renderVisiblePoint(point3 p, mat4x4& RM, Uint32* pixelBuffer, float* depthBuffer)
{
	this->world2viewPointM(p, RM);
	if (pointInsideFrustum(p.P) && pointFacingCamera(p))
		projectPoint(p, pixelBuffer, depthBuffer);
}


void Camera::projectPoint(point3 P, Uint32* pixelBuffer, float* depthBuffer)
{
	coord2 cp = this->view2screen(P.P, hRatio, vRatio);
	//if ((cp.x >= 0) && (cp.x < w) && (cp.y >= 0) && (cp.y < h) && (1.0f / cp.z < depthBuffer[cp.y * w + cp.x]))
	//{
	//	pixelBuffer[cp.y * w + cp.x] = P.colour;
	//	depthBuffer[cp.y * w + cp.x] = 1 / cp.z;
	//}
	if ((cp.x >= 1) && (cp.x < w - 1) && (cp.y >= 1) && (cp.y < h - 1) && (1.0f / cp.z < depthBuffer[cp.y * w + cp.x]))
	{
		pixelBuffer[(cp.y - 1) * w + cp.x] = P.colour;
		pixelBuffer[cp.y * w + cp.x - 1] = P.colour;
		pixelBuffer[cp.y * w + cp.x] = P.colour;
		pixelBuffer[cp.y * w + cp.x + 1] = P.colour;
		pixelBuffer[(cp.y + 1) * w + cp.x] = P.colour;

		depthBuffer[cp.y * w + cp.x] = 1 / cp.z;
	}
}


void Camera::renderMesh(const int& nPoly, triangle3dV* mesh, mat4x4& rot, mat4x4& mov, std::shared_ptr<Lamp> spotlight,
	LightSource sun, const projectionStyle& visualStyle, float torchIntensity, float maxIllumination)
{
	mat4x4 RM = rot * mov;
	//mat4x4 M = mov;

	for (int i = 0; i < nPoly; i++)
	{
		triangle3dV worldT = mesh[i];

		if (polyFacingCamera(worldT))
			renderPolygon(RM, rot, worldT, spotlight, sun, visualStyle, torchIntensity, maxIllumination);
			//renderPolygon(RM, M, worldT, spotlight, sun, visualStyle, torchIntensity, maxIllumination);
	}
}


void Camera::renderMesh(const int& nPoly, triangle3dV* mesh, mat4x4& rot, mat4x4& mov, vect3 mv, vect3 rt, std::shared_ptr<Lamp> spotlight,
	LightSource sun, const projectionStyle& visualStyle, float torchIntensity, float maxIllumination)
{
	mat4x4 rotX = getRotation(axis::x, rt.x);

	mat4x4 rotY = getRotation(axis::y, -rt.y);

	mat4x4 rotZ = getRotation(axis::z, -rt.z);

	mat4x4 R = rotZ * rotY * rotX;

	mat4x4 MOV = getTranslation(mv);

	mat4x4 MR = MOV * R;

	mat4x4 RM = rot * mov;
	//mat4x4 M = mov;

	for (int i = 0; i < nPoly; i++)
	{
		triangle3dV worldT = mesh[i];

		this->object2world(MR, R, worldT);

		if (polyFacingCamera(worldT))
			renderPolygon(RM, rot, worldT, spotlight, sun, visualStyle, torchIntensity, maxIllumination);
			//renderPolygon(RM, M, worldT, spotlight, sun, visualStyle, torchIntensity, maxIllumination);
	}
}


void Camera::renderPolygon(mat4x4& RM, mat4x4& R, triangle3dV& viewT, std::shared_ptr<Lamp> spotlight, LightSource sun,
	const projectionStyle& visualStyle, float torchIntensity, float maxIllumination)
{
	if (visualStyle == projectionStyle::shadow_test)
	{
		triangle3dV worldT = viewT;

		int nVert = this->clipToFrustumT(worldT, vertexListW, uvList, specularList); //CLIPPING IN WORLD SPACE

		this->world2view(RM, nVert);	//WORLD SPACE TO VIEW SPACE TRANSFORMATION

		Uint32 colour = worldT.colour;

		currentTexture = &textureData[viewT.texture];

		this->projectPoly(nVert, colour, spotlight, visualStyle, torchIntensity, maxIllumination, viewT);
	}
	else
	{
		triangle3dV worldT = viewT;

		this->world2view(RM, R, viewT);	//WORLD SPACE TO VIEW SPACE TRANSFORMATION

		if (visualStyle == projectionStyle::blinn_phong)
		{
			Renderer->illuminatePoly(sun, &viewT, worldT, visualStyle, 0.0f);
			getSpecular(illSpec, sun, worldT, 16.0f);
		}
		else
			Renderer->illuminatePoly(sun, &viewT, worldT, visualStyle, 0.1f);

		int nVert = this->clipToFrustum(viewT, vertexListV, uvList, specularList); //CLIPPING IN VIEW SPACE

		Uint32 colour = worldT.colour;

		currentTexture = &textureData[viewT.texture];

		this->projectPoly(nVert, colour, spotlight, visualStyle, torchIntensity, maxIllumination, viewT);
	}
}


void Camera::getSpecular(float* illSp, LightSource Light, const triangle3dV& worldT, float shine)
{
	illSp[0] = Light.getBlinnSpecular(worldT.An, viewDirection, shine);
	illSp[1] = Light.getBlinnSpecular(worldT.Bn, viewDirection, shine);
	illSp[2] = Light.getBlinnSpecular(worldT.Cn, viewDirection, shine);
}


inline coord2 Camera::view2screen(const vect3& vertex, const float& hR, const float& vR)
{
	coord2 pixel;

	float s = float(w) * 0.475f; //w* 0.5f* 0.95f

	pixel.x = (int)(w * 0.5f + static_cast<float>(vertex.x / vertex.z) * s * hR);
	pixel.y = (int)(h * 0.5f - static_cast<float>(vertex.y / vertex.z) * s * vR);
	pixel.z = 1 / vertex.z;

	return pixel;
}


void Camera::projectPoly(int n, Uint32 colour, std::shared_ptr<Lamp> spotlight, projectionStyle style, float torchI, float maxI, triangle3dV originalPoly)
{
	if (style == projectionStyle::shadow_test)
	{		
		triangle3dV worldT;
		triangle3dV viewT;
		triangle2dG screenT;

		for (int i = 0; i < n - 2; i++)
		{
			worldT.A = vertexListW[0];
			worldT.B = vertexListW[1 + i];
			worldT.C = vertexListW[2 + i];

			worldT.N = ((worldT.B - worldT.A) ^ (worldT.C - worldT.A)).norm();

			viewT.A = vertexListV[0];
			viewT.B = vertexListV[1 + i];
			viewT.C = vertexListV[2 + i];

			screenT.a = this->view2screen(vertexListV[0], hRatio, vRatio);
			screenT.b = this->view2screen(vertexListV[1 + i], hRatio, vRatio);
			screenT.c = this->view2screen(vertexListV[2 + i], hRatio, vRatio);

			screenT.At = uvList[0];
			screenT.Bt = uvList[1 + i];
			screenT.Ct = uvList[2 + i];

			screenT.h = colour;

			Renderer->fillTriangleShadows(worldT, viewT, screenT, spotlight, Screen, hRatio, vRatio);
		}
	}
	else if (style == projectionStyle::wireframe)
	{
		coord2 startP, endP;
		for (int i = 0; i < n; i++)
		{
			if (i < n - 1)
			{
				startP = this->view2screen(vertexListV[i], hRatio, vRatio);
				endP = this->view2screen(vertexListV[i + 1], hRatio, vRatio);
			}
			else
			{
				startP = this->view2screen(vertexListV[i], hRatio, vRatio);
				endP = this->view2screen(vertexListV[0], hRatio, vRatio);
			}
			Screen->drawLine(startP, endP, colour);
		}
	}
	else
	{
		triangle2dG screenT;
		triangle3dV viewT;	//To be passed on to polygon filling routine
	
		for (int i = 0; i < n - 2; i++)
		{
			screenT.a = this->view2screen(vertexListV[0], hRatio, vRatio);
			screenT.b = this->view2screen(vertexListV[1 + i], hRatio, vRatio);
			screenT.c = this->view2screen(vertexListV[2 + i], hRatio, vRatio);
	
			viewT.A = vertexListV[0];
			viewT.B = vertexListV[1 + i];
			viewT.C = vertexListV[2 + i];
	
			viewT.N = originalPoly.N;
	
			screenT.illumA = vertexListV[0].w;
			screenT.illumB = vertexListV[1 + i].w;
			screenT.illumC = vertexListV[2 + i].w;
	
			screenT.At = uvList[0];
			screenT.Bt = uvList[1 + i];
			screenT.Ct = uvList[2 + i];
	
			screenT.h = colour;
	
			switch (style)
			{
			case projectionStyle::solid_colour:
			{
				Renderer->fillTriangleSolidColour(viewT, screenT, Screen, hRatio, vRatio);
			}
			break;
			case projectionStyle::checkerboard:
			{
				Renderer->fillTriangleCheckerboard(viewT, screenT, Screen, hRatio, vRatio);
			}
			break;
			case projectionStyle::flat_shaded:
			{
				Renderer->fillTriangleFlatShaded(screenT, Screen);
			}
			break;
			case projectionStyle::gouraud_shaded:
			{
				Renderer->fillTriangleGouraudShaded(screenT, Screen, hRatio, vRatio);
			}
			break;
			case projectionStyle::blinn_phong:
			{
				float specular[3];
				specular[0] = specularList[0];
				specular[1] = specularList[1 + i];
				specular[2] = specularList[2 + i];
				Renderer->fillTriangleBlinnPhong(screenT, specular, Screen, hRatio, vRatio);
			}
			break;
			case projectionStyle::depth_visualised:
			{
				Renderer->fillTriangleDepthVisualised(viewT, screenT, Screen, zNear, hRatio, vRatio);
			}
			break;
			case projectionStyle::sunlight:
			{
				Renderer->fillTriangleSunlight(viewT, screenT, Screen, hRatio, vRatio, currentTexture);
			}
			break;
			case projectionStyle::torchlight:
			{
				Renderer->fillTriangleTorchlight(viewT, screenT, Screen, hRatio, vRatio, currentTexture, torchI, maxI);
			}
			break;
			case projectionStyle::torchlight_solid:
			{
				Renderer->fillTriangleTorchlightSolidColour(viewT, screenT, Screen, hRatio, vRatio, torchI, maxI);
			}
			break;
			case projectionStyle::test:
			{
	
			}
			break;
			default:
			{
				Renderer->fillTriangleFlatShaded(screenT, Screen);
			}
			break;
			}
		}
	}
}


void Camera::outputImage(Canvas screen)
{
	FILE* f = fopen("screenshot_scanline.ppm", "w");
	if (f == NULL)
	{
		printf("Could not write to file...\n");
	}
	else
	{
		fprintf(f, "P3\n");
		fprintf(f, "%d %d\n", w, h);
		fprintf(f, "255\n");

		byte r, g, b;
		Uint32 colour;
		for (int i = 0; i < w * h; i++)
		{
			colour = screen.pixelBuffer[i];
			r = colour >> 16 & 0xFF;
			g = colour >> 8 & 0xFF;
			b = colour & 0xFF;
			fprintf(f, "%d %d %d\n", r, g, b);
		}

		fclose(f);
	}
}