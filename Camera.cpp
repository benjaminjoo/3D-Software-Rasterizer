#define _CRT_SECURE_NO_DEPRECATE

#include "Definitions.h"
#include "Camera.h"
#include "LightSource.h"
#include "ViewVolume.h"
#include "Canvas.h"
#include "ShadowVolume.h"


#define PI					3.141592654

Camera::Camera():
	x(0.0f), y(0.0f), z(0.0f), step(1.0f), turn(0.1f), azm(0.0f), alt(0.0f), rol(0.0f),
	fovH(PI* 0.5 / 90 * 85), zNear(1.0f), zFar(99.9f), w(320), h(200)
{
	std::cout << "Camera constructor called" << std::endl;

	Frustum.initFrustum(this->getFovH(), this->getFovV(), zNear, zFar);

	hRatio = this->getHRatio();
	vRatio = this->getVRatio();

	this->clearVertexList();
}


Camera::Camera(double cx, double cy, double cz, int width, int height, int s):
	x(cx), y(cy), z(cz), step(1.0f), turn(0.1f), azm(0.0f), alt(0.0f), rol(0.0f),
	fovH(PI* 0.5 / 90 * 85), zNear(1.0f), zFar(99.9f), w(width), h(height)
{
	std::cout << "Camera constructor called" << std::endl;

	Frustum.initFrustum(this->getFovH(), this->getFovV(), zNear, zFar);	//

	hRatio = this->getHRatio();
	vRatio = this->getVRatio();

	this->clearVertexList();
}


Camera::Camera(double cx, double cy, double cz, double az, double al, double rl, double stp, double trn,
	double fov, double nr, double fr, int width, int height, int s):
	x(cx), y(cy), z(cz), step(stp), turn(trn), azm(az), alt(al), rol(rl),
	fovH(fov), zNear(nr), zFar(fr), w(width), h(height)
{
	std::cout << "Camera constructor called" << std::endl;

	Frustum.initFrustum(this->getFovH(), this->getFovV(), zNear, zFar);	//

	hRatio = this->getHRatio();
	vRatio = this->getVRatio();

	this->clearVertexList();
}


void Camera::linkToCanvas(std::shared_ptr<Canvas> screen)
{
	pixelBuffer = screen->pixelBuffer;
	depthBuffer = screen->depthBuffer;

	Screen = screen;
}


Camera::~Camera()
{
	std::cout << "Camera destructor called" << std::endl;
}


double Camera::getFovH()
{
	return fovH;
}


double Camera::getFovV()
{
	return (atan(tan(fovH * 0.5) / (w / h))) * 2;
}


double Camera::getHRatio()
{
	return 1 / (tan(fovH / 2));
}


double Camera::getVRatio()
{
	return 1 / (tan(atan(tan(fovH * 0.5) / (w / h) * 2)));
}


void Camera::clearVertexList()
{
	for (int v = 0; v < MAXCLIPVERTS; v++)
	{
		vertexList[v] = { 0.0f, 0.0f, 0.0f, 0.0f };
		uvList[v] = { 0.0f, 0.0f };
	}
}


transform3d Camera::getTransformation()
{
	transform3d T;

	T.sinAzm = sin(azm + PI * 0.5);
	T.cosAzm = cos(azm + PI * 0.5);
	T.sinAlt = sin(alt + PI * 0.5);
	T.cosAlt = cos(alt + PI * 0.5);
	T.sinRol = sin(rol);
	T.cosRol = cos(rol);

	return T;
}


bool Camera::polyFacingCamera(const triangle3dV& P)
{
	vect3 eyeVector = subVectors(P.A, { x, y, z, 1.0 });
	return dotProduct(P.N, eyeVector) < 0.0f ? true : false;
}


void Camera::clipToFrustumL(line3d* line)
{
	plane currentPlane;

	currentPlane = Frustum.getNearPlane();
	this->clipLine(currentPlane, line);
	currentPlane = Frustum.getTopPlane();
	this->clipLine(currentPlane, line);
	currentPlane = Frustum.getBottomPlane();
	this->clipLine(currentPlane, line);
	currentPlane = Frustum.getLeftPlane();
	this->clipLine(currentPlane, line);
	currentPlane = Frustum.getRightPlane();
	this->clipLine(currentPlane, line);
	currentPlane = Frustum.getFarPlane();
	this->clipLine(currentPlane, line);
}


void Camera::clipLine(plane p, line3d* line)
{
	double t;

	//vect3 a		= line->A - p.P;
	vect3 a			= subVectors(line->A, p.P);
	//double sA	= a * p.N;
	double sA		= dotProduct(a, p.N);
	//vect3 b		= line->B - p.P;
	vect3 b			= subVectors(line->B, p.P);
	//double sB	= b * p.N;
	double sB		= dotProduct(b, p.N);

	if (sign(sA) != sign(sB))
	{
		//vect3 d = line->B - line->A;
		vect3 d = subVectors(line->B, line->A);
		//double dist = d * p.N;
		double dist = dotProduct(d, p.N);

		if (sA > 0)
		{
			if (dist)
			{
				t = (dist - sB) / dist;

				line->B.x = line->A.x + t * (line->B.x - line->A.x);
				line->B.y = line->A.y + t * (line->B.y - line->A.y);
				line->B.z = line->A.z + t * (line->B.z - line->A.z);

				//line->B = line->A + ((line->B - line->A) * t);
			}
		}
		if (sB > 0)
		{
			if (dist)
			{
				t = (-dist - sA) / dist;

				line->A.x = line->B.x - t * (line->A.x - line->B.x);
				line->A.y = line->B.y - t * (line->A.y - line->B.y);
				line->A.z = line->B.z - t * (line->A.z - line->B.z);

				//line->A = line->B - ((line->A - line->B) * t);
			}
		}
	}
}


bool Camera::insideFrustum(point3 point)
{
	if (this->assertPointVis(Frustum.getNearPlane(),	point) &&
		this->assertPointVis(Frustum.getTopPlane(),		point) &&
		this->assertPointVis(Frustum.getBottomPlane(),	point) &&
		this->assertPointVis(Frustum.getLeftPlane(),	point) &&
		this->assertPointVis(Frustum.getRightPlane(),	point) &&
		this->assertPointVis(Frustum.getFarPlane(),		point))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool Camera::assertPointVis(plane plane, point3 Point)
{
	return dotProduct(subVectors(Point.P, plane.P), plane.N) >= 0 ? true : false;
	//return ((Point.P - plane.P) * plane.N) >= 0.0f ? true : false;
}


int Camera::clipToFrustum(const triangle3dV& viewT, vect3* vertList, textCoord* uvList)
{
	memset(vertList, 0, MAXCLIPVERTS * sizeof(double));

	vertList[0] = viewT.A;	uvList[0] = viewT.At;
	vertList[1] = viewT.B;	uvList[1] = viewT.Bt;
	vertList[2] = viewT.C;	uvList[2] = viewT.Ct;

	int nVert = 3;
	plane currentPlane;

	currentPlane = Frustum.getNearPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);
	currentPlane = Frustum.getTopPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);
	currentPlane = Frustum.getBottomPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);
	currentPlane = Frustum.getLeftPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);
	currentPlane = Frustum.getRightPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);
	currentPlane = Frustum.getFarPlane();
	this->clipPoly(&nVert, vertList, uvList, currentPlane);

	return nVert;
}


inline void Camera::clipPoly(int* nVert, vect3 * vertList, textCoord * uvList, plane clippingPlane)
{
	int nResult = 0;
	vect3 vTemp[MAXCLIPVERTS];
	textCoord uvTemp[MAXCLIPVERTS];

	for (int i = 0; i < *nVert; i++)
	{
		if (i < (*nVert) - 1)
		{ this->clipEdge(clippingPlane,		vertList[i],	vertList[i + 1], uvList[i],		uvList[i + 1],		&nResult, vTemp, uvTemp); }
		else
		{ this->clipEdge(clippingPlane,		vertList[i],		vertList[0], uvList[i],			uvList[0],		&nResult, vTemp, uvTemp); }
	}
	for (int j = 0; j < nResult; j++)
	{
		vertList[j] = vTemp[j];
		uvList[j] = uvTemp[j];
	}

	*nVert = nResult;
}


inline void Camera::clipEdge(const plane& p, const vect3& startV, const vect3& endV, const textCoord& startUV, const textCoord& endUV,
	int* nResult, vect3 * vTemp, textCoord * uvTemp)
{
	double t;
	//vect3 a = subVectors(startV, p.P);
	//vect3 a = startV - p.P;
	vect3 a = { startV.x - p.P.x, startV.y - p.P.y, startV.z - p.P.z, 1.0 };
	//double sStart = dotProduct(a, p.N);
	//double sStart = a * p.N;
	double sStart = a.x * p.N.x + a.y * p.N.y + a.z * p.N.z;
	//vect3 b = subVectors(endV, p.P);
	//vect3 b = endV - p.P;
	vect3 b = { endV.x - p.P.x, endV.y - p.P.y, endV.z - p.P.z, 1.0 };
	//double sEnd = dotProduct(b, p.N);	
	//double sEnd = b * p.N;
	double sEnd = b.x * p.N.x + b.y * p.N.y + b.z * p.N.z;
	if (sign(sStart) != sign(sEnd))
	{
		//vect3 d = subVectors(endV, startV);
		//vect3 d = endV - startV;
		vect3 d = { endV.x - startV.x, endV.y - startV.y, endV.z - startV.z, 1.0f };
		//double dist = dotProduct(d, p.N);	
		//double dist = d * p.N;
		double dist = d.x * p.N.x + d.y * p.N.y + d.z * p.N.z;
		
		if(sStart < 0)
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

				(*nResult)++;
			}
		}
		if(sEnd < 0)
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

			(*nResult)++;
		}
	}
}


inline coord2 Camera::view2screen(const vect3& vertex, const double& hR, const double& vR)
{
	coord2 pixel;

	double s = double(w) * 0.475f; //w* 0.5f* 0.95f

	pixel.x = (int)(w * 0.5 + (vertex.x / vertex.z) * s * hR);
	pixel.y = (int)(h * 0.5 - (vertex.y / vertex.z) * s * vR);
	pixel.z = 1 / vertex.z;

	return pixel;
}


void Camera::projectLine(line3d line, Uint32* pixelBuffer, double* depthBuffer, double hRatio, double vRatio)
{
	coord2 startP	= this->view2screen(line.A, hRatio, vRatio);
	coord2 endP		= this->view2screen(line.B, hRatio, vRatio);

	if ((startP.x	>= 0	&& startP.x		< w	)	&&
		(startP.y	>= 0	&& startP.y		< h	)	&&
		(endP.x		>= 0	&& endP.x		< w	)	&&
		(endP.y		>= 0	&& endP.y		< h	))
	{
		Screen->drawLine(startP, endP, line.colour);
	}
}


void Camera::projectPoint(point3 P, Uint32* pixelBuffer, double* depthBuffer)
{
	double hRatio = this->getHRatio();
	double vRatio = this->getVRatio();
	coord2 cp = this->view2screen(P.P, hRatio, vRatio);
	if ((cp.x >= 0) && (cp.x < w) && (cp.y >= 0) && (cp.y < h) && (1.0f / cp.z < depthBuffer[cp.y * w + cp.x]))
	{
		pixelBuffer[cp.y * w + cp.x] = P.colour;
		depthBuffer[cp.y * w + cp.x] = 1 / cp.z;
	}
}

void Camera::projectPoly(int n, Uint32 colour, projectionStyle style, double torchI, double maxI, triangle3dV originalPoly)
{
	triangle2dG screenT;
	triangle3dV originalT;	//To be passed on to polygon filling routine

	if (style == wireframe)
	{
		coord2 startP, endP;
		for (int i = 0; i < n; i++)
		{
			if (i < n - 1)
			{
				startP = this->view2screen(vertexList[i], hRatio, vRatio);
				endP = this->view2screen(vertexList[i + 1], hRatio, vRatio);
			}
			else
			{
				startP = this->view2screen(vertexList[i], hRatio, vRatio);
				endP = this->view2screen(vertexList[0], hRatio, vRatio);
			}
			Screen->drawLine(startP, endP, colour);
		}
	}
	else
	{
		for (int i = 0; i < n - 2; i++)
		{
			screenT.a = this->view2screen(vertexList[0], hRatio, vRatio);
			screenT.b = this->view2screen(vertexList[1 + i], hRatio, vRatio);
			screenT.c = this->view2screen(vertexList[2 + i], hRatio, vRatio);

			originalT.A = vertexList[0];
			originalT.B = vertexList[1 + i];
			originalT.C = vertexList[2 + i];

			originalT.N = originalPoly.N;

			screenT.illumA = vertexList[0].w;
			screenT.illumB = vertexList[1 + i].w;
			screenT.illumC = vertexList[2 + i].w;

			screenT.At = uvList[0];
			screenT.Bt = uvList[1 + i];
			screenT.Ct = uvList[2 + i];

			screenT.h = colour;
			bool d = false;

			switch (style)
			{
			case solid_colour:
			{
				Projection::fillTriangleSolidColour(originalT, screenT, Screen, hRatio, vRatio);
			}
			break;
			case checkerboard:
			{
				Projection::fillTriangleCheckerboard(originalT, screenT, Screen, hRatio, vRatio);
			}
			break;
			case flat_shaded:
			{
				Projection::fillTriangleFlatShaded(screenT, Screen);
			}
			break;
			case gouraud_shaded:
			{
				Projection::fillTriangleGouraudShaded(screenT, Screen, hRatio, vRatio);
			}
			break;
			case depth_visualised:
			{
				Projection::fillTriangleDepthVisualised(originalT, screenT, Screen, zNear, hRatio, vRatio);
			}
			break;
			case sunlight:
			{
				Projection::fillTriangleSunlight(originalT, screenT, Screen, hRatio, vRatio, currentTexture);
			}
			break;
			case torchlight:
			{
				Projection::fillTriangleTorchlight(originalT, screenT, Screen, hRatio, vRatio, currentTexture, torchI, maxI);
			}
			break;
			case torchlight_solid:
			{
				Projection::fillTriangleTorchlightSolidColour(originalT, screenT, Screen, hRatio, vRatio, torchI, maxI);
			}
			break;
			case test:
			{
				
			}
			break;
			default:
			{
				Projection::fillTriangleFlatShaded(screenT, Screen);
			}
			break;
			}
		}
	}
}


vect3 Camera::getPosition()
{
	return { x, y, z, 1.0f };
}


void Camera::updatePosition(double turnH, double turnV, double tiltP, double moveP, double strafeP, double riseP)
{
	azm = -turnH;
	alt = -turnV;
	rol = tiltP;

	x -= moveP * cos(azm) - strafeP * cos(azm + PI * 0.5);
	y += moveP * sin(azm) - strafeP * sin(azm + PI * 0.5);
	z += riseP;
}


void Camera::centreLook()
{
	alt = 0.0;
}


void Camera::renderPoint(point3 p, Uint32* pixelBuffer, double* depthBuffer)
{
	point3 viewP = Projection::world2viewP(getTransformation(), p, x, y, z);
	projectPoint(viewP, pixelBuffer, depthBuffer);
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

void Camera::addTexture(txt T)
{
	textureData.push_back(T);
}


void Camera::renderPolygon(transform3d T, triangle3dV& viewT, LightSource Sun, unsigned textureID,
				const projectionStyle& visualStyle, double torchIntensity, double maxIllumination)
{
	triangle3dV worldT = viewT;

	Projection::world2view(viewT, T, x, y, z);

	Projection::illuminatePoly(Sun, &viewT, worldT, visualStyle);
	
	int nVert = this->clipToFrustum(viewT, vertexList, uvList);

	Uint32 colour = worldT.colour;

	currentTexture = &textureData[textureID];

	this->projectPoly(nVert, colour, visualStyle, torchIntensity, maxIllumination, viewT);
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
			r = colour >> 16	& 0xFF;
			g = colour >> 8		& 0xFF;
			b = colour			& 0xFF;
			fprintf(f, "%d %d %d\n", r, g, b);
		}

		fclose(f);
	}
}