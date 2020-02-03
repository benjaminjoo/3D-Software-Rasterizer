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
		tempTexture.w = T->w;
		tempTexture.h = T->h;
		tempTexture.pixelsH = new Uint32[tempTexture.w * tempTexture.h];
		tempTexture.pixelsH = (Uint32*)tempImage->pixels;
		tempTexture.ID = textureData.size();

		tempTexture.pixelsM = new Uint32[(tempTexture.w / 2) * (tempTexture.h / 2)];
		unsigned rAcc = 0;
		unsigned gAcc = 0;
		unsigned bAcc = 0;
		for (int j = 0; j < tempTexture.h; j += 2)
		{
			for (int i = 0; i < tempTexture.w; i += 2)
			{
				for (int y = 0; y < 2; y++)
				{
					for (int x = 0; x < 2; x++)
					{
						colour32 sample;
						sample.argb = tempTexture.pixelsH[(j + y) * tempTexture.w + i + x];
						rAcc += static_cast<int>(sample.c[2]);
						gAcc += static_cast<int>(sample.c[1]);
						bAcc += static_cast<int>(sample.c[0]);
					}
				}
				colour32 final;
				final.c[2] = static_cast<unsigned char>(static_cast<float>(rAcc) / 4.0f);
				final.c[1] = static_cast<unsigned char>(static_cast<float>(gAcc) / 4.0f);
				final.c[0] = static_cast<unsigned char>(static_cast<float>(bAcc) / 4.0f);
				tempTexture.pixelsM[(j / 2) * (tempTexture.w / 2) + (i / 2)] = final.argb;
				rAcc = 0;
				gAcc = 0;
				bAcc = 0;
			}
		}

		tempTexture.pixelsL = new Uint32[(tempTexture.w / 4) * (tempTexture.h / 4)];
		rAcc = 0;
		gAcc = 0;
		bAcc = 0;
		for (int j = 0; j < tempTexture.h; j += 4)
		{
			for (int i = 0; i < tempTexture.w; i += 4)
			{
				for (int y = 0; y < 4; y++)
				{
					for (int x = 0; x < 4; x++)
					{
						colour32 sample;
						sample.argb = tempTexture.pixelsH[(j + y) * tempTexture.w + i + x];
						rAcc += static_cast<int>(sample.c[2]);
						gAcc += static_cast<int>(sample.c[1]);
						bAcc += static_cast<int>(sample.c[0]);
					}
				}
				colour32 final;
				final.c[2] = static_cast<unsigned char>(static_cast<float>(rAcc) / 16.0f);
				final.c[1] = static_cast<unsigned char>(static_cast<float>(gAcc) / 16.0f);
				final.c[0] = static_cast<unsigned char>(static_cast<float>(bAcc) / 16.0f);
				tempTexture.pixelsL[(j / 4) * (tempTexture.w / 4) + (i / 4)] = final.argb;
				rAcc = 0;
				gAcc = 0;
				bAcc = 0;
			}
		}

		tempTexture.pixelsT = new Uint32[(tempTexture.w / 16) * (tempTexture.h / 16)];
		rAcc = 0;
		gAcc = 0;
		bAcc = 0;
		for (int j = 0; j < tempTexture.h; j += 16)
		{
			for (int i = 0; i < tempTexture.w; i += 16)
			{
				for (int y = 0; y < 16; y++)
				{
					for (int x = 0; x < 16; x++)
					{
						colour32 sample;
						sample.argb = tempTexture.pixelsH[(j + y) * tempTexture.w + i + x];
						rAcc += static_cast<int>(sample.c[2]);
						gAcc += static_cast<int>(sample.c[1]);
						bAcc += static_cast<int>(sample.c[0]);
					}
				}
				colour32 final;
				final.c[2] = static_cast<unsigned char>(static_cast<float>(rAcc) / 256.0f);
				final.c[1] = static_cast<unsigned char>(static_cast<float>(gAcc) / 256.0f);
				final.c[0] = static_cast<unsigned char>(static_cast<float>(bAcc) / 256.0f);
				tempTexture.pixelsT[(j / 16) * (tempTexture.w / 16) + (i / 16)] = final.argb;
				rAcc = 0;
				gAcc = 0;
				bAcc = 0;
			}
		}

		std::ofstream outputFileT(std::to_string(tempTexture.ID) + "tiny" + ".ppm");
		if (outputFileT.is_open())
		{
			outputFileT << "P3" << std::endl;
			outputFileT << tempTexture.w / 16 << " " << tempTexture.h / 16 << std::endl;
			outputFileT << "255" << std::endl;

			for (int j = 0; j < (tempTexture.h / 16); j++)
			{
				for (int i = 0; i < (tempTexture.w / 16); i++)
				{
					colour32 ccc;
					ccc.argb = tempTexture.pixelsT[j * (tempTexture.w / 16) + i];
					outputFileT << std::to_string(ccc.c[2]) << " "
								<< std::to_string(ccc.c[1]) << " "
								<< std::to_string(ccc.c[0]) << std::endl;
				}
			}

			outputFileT.close();
		}

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


void Camera::update()
{
	updateViewDirection();
	updateViewVolume();

	//rotation = getRotation();
	//translation = getTranslation();
	//transformation = rotation * translation;

	float sinAz = sinf(rol);
	float cosAz = cosf(rol);

	float sinAx = sinf(-(alt + PI * 0.5f));
	float cosAx = cosf(-(alt + PI * 0.5f));

	float sinAz_ = sinf(-(azm + PI * 0.5f));
	float cosAz_ = cosf(-(azm + PI * 0.5f));

	rotation =
	{
		cosAz * cosAz_ + sinAz * cosAx * -sinAz_,	cosAz * sinAz_ + sinAz * cosAx * cosAz_,	sinAz * sinAx,	0.0f,
		-sinAz * cosAz_ + cosAz * cosAx * -sinAz_,	-sinAz * sinAz_ + cosAz * cosAx * cosAz_,	cosAz * sinAx,	0.0f,
		-sinAx * -sinAz_,							-sinAx * cosAz_,							cosAx,			0.0f,
		0.0f,										0.0f,										0.0f,			1.0f
	};

	rotationSIMD.col_0 = _mm_set_ps(	cosAz * cosAz_ + sinAz * cosAx * -sinAz_,
										-sinAz * cosAz_ + cosAz * cosAx * -sinAz_,
										-sinAx * -sinAz_,
										0.0f);

	rotationSIMD.col_1 = _mm_set_ps(	cosAz * sinAz_ + sinAz * cosAx * cosAz_,
										-sinAz * sinAz_ + cosAz * cosAx * cosAz_,
										-sinAx * cosAz_,
										0.0f);

	rotationSIMD.col_2 = _mm_set_ps(	sinAz * sinAx,
										cosAz * sinAx,
										cosAx,
										0.0f);

	rotationSIMD.col_3 = _mm_set_ps(	0.0f,
										0.0f,
										0.0f,
										1.0f);


	transformation =
	{
		cosAz * cosAz_ + sinAz * cosAx * -sinAz_,
		cosAz * sinAz_ + sinAz * cosAx * cosAz_,
		sinAz * sinAx,
		(cosAz * cosAz_ + sinAz * cosAx * -sinAz_) * (-x) + (cosAz * sinAz_ + sinAz * cosAx * cosAz_) * (-y) + (sinAz * sinAx) * (-z),

		-sinAz * cosAz_ + cosAz * cosAx * -sinAz_,
		-sinAz * sinAz_ + cosAz * cosAx * cosAz_,
		cosAz * sinAx,
		(-sinAz * cosAz_ + cosAz * cosAx * -sinAz_) * (-x) + (-sinAz * sinAz_ + cosAz * cosAx * cosAz_) * (-y) + (cosAz * sinAx) * (-z),

		-sinAx * -sinAz_,
		-sinAx * cosAz_,
		cosAx,
		-sinAx * sinAz_ * x + sinAx * cosAz_ * y - cosAx * z,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	};

	transformationSIMD.col_0 = _mm_set_ps(	cosAz * cosAz_ + sinAz * cosAx * -sinAz_,
											-sinAz * cosAz_ + cosAz * cosAx * -sinAz_,
											-sinAx * -sinAz_,
											0.0f);

	transformationSIMD.col_1 = _mm_set_ps(	cosAz * sinAz_ + sinAz * cosAx * cosAz_,
											-sinAz * sinAz_ + cosAz * cosAx * cosAz_,
											-sinAx * cosAz_,
											0.0f);

	transformationSIMD.col_2 = _mm_set_ps(	sinAz * sinAx,
											cosAz * sinAx,
											cosAx,
											0.0f);

	transformationSIMD.col_3 = _mm_set_ps(	(cosAz * cosAz_ + sinAz * cosAx * -sinAz_) * (-x) + (cosAz * sinAz_ + sinAz * cosAx * cosAz_) * (-y) + (sinAz * sinAx) * (-z),
											(-sinAz * cosAz_ + cosAz * cosAx * -sinAz_) * (-x) + (-sinAz * sinAz_ + cosAz * cosAx * cosAz_) * (-y) + (cosAz * sinAx) * (-z),
											-sinAx * sinAz_ * x + sinAx * cosAz_ * y - cosAx * z,
											1.0f);
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


mat4x4 Camera::getTranslation(vect3 mv) const
{
	mat4x4 result;

	result = {  1.0f,          0.0f,         0.0f,		 mv.x,
				0.0f,          1.0f,         0.0f,		 mv.y,
				0.0f,          0.0f,         1.0f,		 mv.z,
				0.0f,          0.0f,         0.0f,       1.0f };

	return result;
}


mat4x4 Camera::getTranslation() const
{
	mat4x4 result;

	result = {  1.0f,          0.0f,         0.0f,		-x,
				0.0f,          1.0f,         0.0f,		-y,
				0.0f,          0.0f,         1.0f,		-z,
				0.0f,          0.0f,         0.0f,       1.0f };

	return result;
}


mat4x4 Camera::getRotation(axis t, float a) const
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
		result = {  1.0f,       0.0f,       0.0f,       0.0f,
					0.0f,       cosA,       sinA,       0.0f,
					0.0f,      -sinA,       cosA,       0.0f,
					0.0f,       0.0f,       0.0f,       1.0f };
	}
	break;
	case axis::y:
	{
		result = {  cosA,       0.0f,      -sinA,       0.0f,
					0.0f,       1.0f,       0.0f,       0.0f,
					sinA,       0.0f,       cosA,       0.0f,
					0.0f,       0.0f,       0.0f,       1.0f };
	}
	break;
	case axis::z:
	{
		result = {  cosA,       sinA,       0.0f,       0.0f,
				   -sinA,       cosA,       0.0f,       0.0f,
					0.0f,       0.0f,       1.0f,       0.0f,
					0.0f,       0.0f,       0.0f,       1.0f };
	}
	break;
	}

	return result;
}


mat4x4 Camera::getRotation(vect3 rt) const
{
	const float sinAx = sinf(rt.x);
	const float cosAx = cosf(rt.x);
	const float sinAy = sinf(-rt.y);
	const float cosAy = cosf(-rt.y);
	const float sinAz = sinf(-rt.z);
	const float cosAz = cosf(-rt.z);

	const mat4x4 rotZrotYrotX =
	{
		cosAz* cosAy,	sinAz * cosAx + cosAz * -sinAy * -sinAx,	sinAz * sinAx + cosAz * -sinAy * cosAx,	0.0f,
		-sinAz * cosAy,	cosAz * cosAx + -sinAz * -sinAy * -sinAx,	cosAz * sinAx + -sinAz * -sinAy * cosAx,0.0f,
		sinAy,			cosAy * -sinAx,								cosAy * cosAx,							0.0f,
		0.0f,			0.0f,										0.0f,									1.0f
	};

	return rotZrotYrotX;
}


mat4x4 Camera::getRotation() const
{
	float sinAz = sinf(rol);
	float cosAz = cosf(rol);

	float sinAx = sinf(-(alt + PI * 0.5f));
	float cosAx = cosf(-(alt + PI * 0.5f));

	float sinAz_ = sinf(-(azm + PI * 0.5f));
	float cosAz_ = cosf(-(azm + PI * 0.5f));

	mat4x4 rotZrotXrotZ =
	{
		cosAz * cosAz_ + sinAz * cosAx * -sinAz_,	cosAz * sinAz_ + sinAz * cosAx * cosAz_,	sinAz * sinAx,	0.0f,
		-sinAz * cosAz_ + cosAz * cosAx * -sinAz_,	-sinAz * sinAz_ + cosAz * cosAx * cosAz_,	cosAz * sinAx,	0.0f,
		-sinAx * -sinAz_,							-sinAx * cosAz_,							cosAx,			0.0f,
		0.0f,										0.0f,										0.0f,			1.0f
	};

	return rotZrotXrotZ;
}


void Camera::object2world(const mat4x4& MR, const mat4x4& R, triangle3dV& T) const
{
	T.A = MR * T.A;
	T.B = MR * T.B;
	T.C = MR * T.C;
	T.An = R * T.An;
	T.Bn = R * T.Bn;
	T.Cn = R * T.Cn;
	T.N = R * T.N;
}


void Camera::world2view(triangle3dV& T) const
{
	T.A = transformation * T.A;
	T.B = transformation * T.B;
	T.C = transformation * T.C;
	T.An = rotation * T.An;
	T.Bn = rotation * T.Bn;
	T.Cn = rotation * T.Cn;
	T.N = rotation * T.N;
}


void Camera::world2viewSIMD(triangle3dV& T) const
{
	const mat4x4SIMD& TR	= transformationSIMD;
	const mat4x4SIMD& R		= rotationSIMD;

	//Packing triangle

	__m128 xxxxA = _mm_set_ps1(T.A.x);
	__m128 yyyyA = _mm_set_ps1(T.A.y);
	__m128 zzzzA = _mm_set_ps1(T.A.z);
	__m128 wwwwA = _mm_set_ps1(T.A.w);

	__m128 xxxxB = _mm_set_ps1(T.B.x);
	__m128 yyyyB = _mm_set_ps1(T.B.y);
	__m128 zzzzB = _mm_set_ps1(T.B.z);
	__m128 wwwwB = _mm_set_ps1(T.B.w);

	__m128 xxxxC = _mm_set_ps1(T.C.x);
	__m128 yyyyC = _mm_set_ps1(T.C.y);
	__m128 zzzzC = _mm_set_ps1(T.C.z);
	__m128 wwwwC = _mm_set_ps1(T.C.w);

	__m128 xxxxN = _mm_set_ps1(T.N.x);
	__m128 yyyyN = _mm_set_ps1(T.N.y);
	__m128 zzzzN = _mm_set_ps1(T.N.z);
	__m128 wwwwN = _mm_set_ps1(T.N.w);

	__m128 xxxxAn = _mm_set_ps1(T.An.x);
	__m128 yyyyAn = _mm_set_ps1(T.An.y);
	__m128 zzzzAn = _mm_set_ps1(T.An.z);
	__m128 wwwwAn = _mm_set_ps1(T.An.w);

	__m128 xxxxBn = _mm_set_ps1(T.Bn.x);
	__m128 yyyyBn = _mm_set_ps1(T.Bn.y);
	__m128 zzzzBn = _mm_set_ps1(T.Bn.z);
	__m128 wwwwBn = _mm_set_ps1(T.Bn.w);

	__m128 xxxxCn = _mm_set_ps1(T.Cn.x);
	__m128 yyyyCn = _mm_set_ps1(T.Cn.y);
	__m128 zzzzCn = _mm_set_ps1(T.Cn.z);
	__m128 wwwwCn = _mm_set_ps1(T.Cn.w);

	//Calculation

	__m128 resultA = _mm_add_ps(_mm_add_ps(_mm_mul_ps(xxxxA, TR.col_0), _mm_mul_ps(yyyyA, TR.col_1)), _mm_add_ps(_mm_mul_ps(zzzzA, TR.col_2), _mm_mul_ps(wwwwA, TR.col_3)));
	__m128 resultB = _mm_add_ps(_mm_add_ps(_mm_mul_ps(xxxxB, TR.col_0), _mm_mul_ps(yyyyB, TR.col_1)), _mm_add_ps(_mm_mul_ps(zzzzB, TR.col_2), _mm_mul_ps(wwwwB, TR.col_3)));
	__m128 resultC = _mm_add_ps(_mm_add_ps(_mm_mul_ps(xxxxC, TR.col_0), _mm_mul_ps(yyyyC, TR.col_1)), _mm_add_ps(_mm_mul_ps(zzzzC, TR.col_2), _mm_mul_ps(wwwwC, TR.col_3)));

	__m128 resultN = _mm_add_ps(_mm_add_ps(_mm_mul_ps(xxxxN, R.col_0), _mm_mul_ps(yyyyN, R.col_1)), _mm_add_ps(_mm_mul_ps(zzzzN, R.col_2), _mm_mul_ps(wwwwN, R.col_3)));

	__m128 resultAn = _mm_add_ps(_mm_add_ps(_mm_mul_ps(xxxxAn, R.col_0), _mm_mul_ps(yyyyAn, R.col_1)), _mm_add_ps(_mm_mul_ps(zzzzAn, R.col_2), _mm_mul_ps(wwwwAn, R.col_3)));
	__m128 resultBn = _mm_add_ps(_mm_add_ps(_mm_mul_ps(xxxxBn, R.col_0), _mm_mul_ps(yyyyBn, R.col_1)), _mm_add_ps(_mm_mul_ps(zzzzBn, R.col_2), _mm_mul_ps(wwwwBn, R.col_3)));
	__m128 resultCn = _mm_add_ps(_mm_add_ps(_mm_mul_ps(xxxxCn, R.col_0), _mm_mul_ps(yyyyCn, R.col_1)), _mm_add_ps(_mm_mul_ps(zzzzCn, R.col_2), _mm_mul_ps(wwwwCn, R.col_3)));

	//Unpacking triangle

	T.A.x = resultA.m128_f32[3];
	T.A.y = resultA.m128_f32[2];
	T.A.z = resultA.m128_f32[1];
	T.A.w = resultA.m128_f32[0];

	T.B.x = resultB.m128_f32[3];
	T.B.y = resultB.m128_f32[2];
	T.B.z = resultB.m128_f32[1];
	T.B.w = resultB.m128_f32[0];

	T.C.x = resultC.m128_f32[3];
	T.C.y = resultC.m128_f32[2];
	T.C.z = resultC.m128_f32[1];
	T.C.w = resultC.m128_f32[0];

	T.N.x = resultN.m128_f32[3];
	T.N.y = resultN.m128_f32[2];
	T.N.z = resultN.m128_f32[1];
	T.N.w = resultN.m128_f32[0];

	T.An.x = resultAn.m128_f32[3];
	T.An.y = resultAn.m128_f32[2];
	T.An.z = resultAn.m128_f32[1];
	T.An.w = resultAn.m128_f32[0];

	T.Bn.x = resultBn.m128_f32[3];
	T.Bn.y = resultBn.m128_f32[2];
	T.Bn.z = resultBn.m128_f32[1];
	T.Bn.w = resultBn.m128_f32[0];

	T.Cn.x = resultCn.m128_f32[3];
	T.Cn.y = resultCn.m128_f32[2];
	T.Cn.z = resultCn.m128_f32[1];
	T.Cn.w = resultCn.m128_f32[0];
}


void Camera::world2view(int n)
{
	for (int i = 0; i < n; i++)
		vertexListV[i] = transformation * vertexListW[i];
}


void Camera::world2viewPointM(point3& P) const
{
	P.P = transformation * P.P;
}


void Camera::renderPoint(point3 p, Uint32* pixelBuffer, float* depthBuffer)
{
	this->world2viewPointM(p);
	if (pointInsideFrustum(p.P))
		projectPoint(p, pixelBuffer, depthBuffer);
}


void Camera::renderVisiblePoint(point3 p, Uint32* pixelBuffer, float* depthBuffer)
{
	this->world2viewPointM(p);
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


void Camera::renderMesh(const int& nPoly, triangle3dV* mesh, std::shared_ptr<Lamp> spotlight,
	LightSource sun, const projectionStyle& visualStyle, float torchIntensity, float maxIllumination)
{
	if (visualStyle == projectionStyle::shadow_test)
	{
		for (int i = 0; i < nPoly; i++)
		{
			triangle3dV worldT = mesh[i];

			if (polyFacingCamera(worldT))
				renderPolygonShadows(worldT, spotlight, sun, visualStyle, torchIntensity, maxIllumination);
		}
	}
	else
	{
		for (int i = 0; i < nPoly; i++)
		{
			triangle3dV worldT = mesh[i];

			if (polyFacingCamera(worldT))
				renderPolygon(worldT, spotlight, sun, visualStyle, torchIntensity, maxIllumination);
		}
	}
}


void Camera::renderMesh(const int& nPoly, triangle3dV* mesh, vect3 mv, vect3 rt, std::shared_ptr<Lamp> spotlight,
	LightSource sun, const projectionStyle& visualStyle, float torchIntensity, float maxIllumination)
{
	mat4x4 R = getRotation(rt);

	mat4x4 MOV = getTranslation(mv);

	mat4x4 MR = MOV * R;

	if (visualStyle == projectionStyle::shadow_test)
	{
		for (int i = 0; i < nPoly; i++)
		{
			triangle3dV worldT = mesh[i];

			this->object2world(MR, R, worldT);

			if (polyFacingCamera(worldT))
				renderPolygonShadows(worldT, spotlight, sun, visualStyle, torchIntensity, maxIllumination);
		}
	}
	else
	{
		for (int i = 0; i < nPoly; i++)
		{
			triangle3dV worldT = mesh[i];

			this->object2world(MR, R, worldT);

			if (polyFacingCamera(worldT))
				renderPolygon(worldT, spotlight, sun, visualStyle, torchIntensity, maxIllumination);
		}
	}
}


void Camera::renderPolygon(triangle3dV& viewT, std::shared_ptr<Lamp> spotlight, LightSource sun,
	const projectionStyle& visualStyle, float torchIntensity, float maxIllumination)
{
	triangle3dV worldT = viewT;

	//this->world2view(viewT);	//WORLD SPACE TO VIEW SPACE TRANSFORMATION

	this->world2viewSIMD(viewT);

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


void Camera::renderPolygonShadows(triangle3dV& viewT, std::shared_ptr<Lamp> spotlight, LightSource sun,
	const projectionStyle& visualStyle, float torchIntensity, float maxIllumination)
{
	triangle3dV worldT = viewT;

	int nVert = this->clipToFrustumT(worldT, vertexListW, uvList, specularList); //CLIPPING IN WORLD SPACE

	this->world2view(nVert);	//WORLD SPACE TO VIEW SPACE TRANSFORMATION

	Uint32 colour = worldT.colour;

	currentTexture = &textureData[viewT.texture];

	this->projectPoly(nVert, colour, spotlight, visualStyle, torchIntensity, maxIllumination, viewT);
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