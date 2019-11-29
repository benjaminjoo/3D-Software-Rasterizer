#include "Pong.h"



Pong::Pong(std::shared_ptr<Canvas> screen, std::shared_ptr<Camera> eye, std::shared_ptr<EventHandler> controls,
			std::shared_ptr<LightSource> sun, std::shared_ptr<SolidSphere> ball):
	Screen(screen), Eye(eye), Controls(controls), Sun(sun), Ball(ball)
{
	for (int v = 0; v < MAXCLIPVERTS; v++)
	{
		vertexList[v]	= { 0.0f, 0.0f, 0.0f, 0.0f };
		uvList[v]		= { 0.0f, 0.0f };
	}

	hRatio = Eye->getHRatio();
	vRatio = Eye->getVRatio();

	ballRadius = Ball->getRadius();
}


Pong::~Pong()
{
}


void Pong::addTexture(SDL_Surface* T)
{
	if (T == nullptr)
	{
		printf("Image loading failed...\n");
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


void Pong::addEntity(std::shared_ptr<SolidBody> entity)
{
	Entities.push_back(entity);
}


void Pong::buildMesh()
{
	size_t nEntities = Entities.size();
	triangleMesh = new triangle3dV* [nEntities];
	polyCount = new unsigned int[nEntities];
	for (unsigned int i = 0; i < nEntities; i++)
	{
		unsigned int nCurrent = Entities[i]->getTotalPoly();
		triangleMesh[i] = new triangle3dV[nCurrent];
		polyCount[i] = nCurrent;
		Entities[i]->getTriangleData_(triangleMesh[i]);
	}
	ballPolyCount = Ball->getTotalPoly();
	ballMesh = new triangle3dV[ballPolyCount];
	Ball->getTriangleData_(ballMesh);
}


void Pong::destroyMesh()
{
	size_t nEntities = Entities.size();
	for (unsigned int i = 0; i < nEntities; i++)
	{
		delete triangleMesh[i];
	}
	delete[] triangleMesh;
	delete ballMesh;
	delete polyCount;
}


void Pong::updateCameraPosition()
{
	Eye->azm = -Controls->turnH;
	Eye->alt = -Controls->turnV;
	Eye->rol = Controls->tiltP;

	Eye->x -= Controls->moveP * cos(Eye->azm) - Controls->strafeP * cos(Eye->azm + PI * 0.5);
	Eye->y += Controls->moveP * sin(Eye->azm) - Controls->strafeP * sin(Eye->azm + PI * 0.5);
	Eye->z += Controls->riseP;
}


void Pong::updateEntities()
{
	for (unsigned int i = 0; i < Entities.size(); i++)
	{
		if (Entities[i]->isInMotion())
		{
			Entities[i]->updatePosition();
			Entities[i]->updateRotation();

			vect3 velocity = Entities[i]->getVelocity();
			vect3 angVelocity = Entities[i]->getAngularVelocity();
			unsigned int nCurrent = Entities[i]->getTotalPoly();
			transformMesh(nCurrent, triangleMesh[i], 1.0, 1.0, 1.0, velocity.x, velocity.y, velocity.z, angVelocity.x, angVelocity.y, angVelocity.z);
		}
	}
}



void Pong::updateBall()
{
	if (Controls->gravityOn)
		Ball->updateVelocity(gravity);

	vect3 displacement = Ball->getVelocity();
	vect3 oldPos = Ball->getPosition();
	vect3 newPos = addVectors(oldPos, displacement);

	size_t nEntities = Entities.size();
	for (unsigned int i = 0; i < nEntities; i++)
	{
		for (unsigned int j = 0; j < polyCount[i]; j++)
		{		
			if (ballApproachingWall(oldPos, displacement, i, j))
			{
				triangle3dV tempWall = triangleMesh[i][j];

				double oldDistWall = distPoint2Plane(oldPos, tempWall);
				double newDistWall = distPoint2Plane(newPos, tempWall);

				if (newDistWall <= ballRadius)
				{
					bounceCount++;

					double x = oldDistWall - ballRadius;
					double v2n = -dotProduct(displacement, tempWall.N);
					double safePercentage = x / v2n;

					vect3 toIntersection = scaleVector(safePercentage, displacement);
					Ball->updatePosition(toIntersection);
					transformMesh(ballPolyCount, ballMesh, toIntersection);

					vect3 oldVelocity = Ball->getVelocity();
					if (Controls->gravityOn)
					{
						vect3 newVelocity = subVectors(oldVelocity, scaleVector(1.75f * dotProduct(oldVelocity, tempWall.N), tempWall.N));
						Ball->setVelocity(newVelocity);
					}
					else
					{
						vect3 newVelocity = subVectors(oldVelocity, scaleVector(2.00f * dotProduct(oldVelocity, tempWall.N), tempWall.N));
						Ball->setVelocity(newVelocity);
					}				

					return;
				}
			}
		}
	}

	Ball->updatePosition();
	transformMesh(ballPolyCount, ballMesh, displacement);
}


bool Pong::ballApproachingWall(vect3 p, vect3 v, const unsigned int& i, const unsigned int& j)
{
	triangle3dV T = triangleMesh[i][j];
	double v2n = dotProduct(v, T.N);				//Magnitude of velocity projected to plane normal
	double dist = distPoint2Plane(p, T);			//Distance from point to plane
	if (dist >= 0.0f && v2n < 0.0f)
	{
		double t = dist / -v2n;
		vect3 intersection{ p.x + v.x * t, p.y + v.y * t, p.z + v.z * t, 1.0f };

		double sA = dotProduct(crossProduct(subVectors(T.A, T.C), T.N), subVectors(T.A, intersection));
		double sB = dotProduct(crossProduct(subVectors(T.B, T.A), T.N), subVectors(T.B, intersection));
		double sC = dotProduct(crossProduct(subVectors(T.C, T.B), T.N), subVectors(T.C, intersection));

		if ((sA <= 0.0f && sB <= 0.0f && sC <= 0.0f) ||
			(sA >= 0.0f && sB >= 0.0f && sC >= 0.0f))	//If point of intersection lies inside triangle T
		{
			return true;
		}
	}
	return false;
}


void Pong::renderMesh(transform3d eyePosition, int nPoly, triangle3dV* mesh)
{
	for (int i = 0; i < nPoly; i++)
	{
		if (Eye->polyFacingCamera(mesh[i]))
		{
			Uint32 colour = mesh[i].colour;

			triangle3dV viewT = Eye->world2viewT(eyePosition, mesh[i]);

			Eye->illuminatePoly(*Sun, &viewT, mesh[i], Controls->visualStyle);

			int nVert = Eye->clipToFrustum(viewT, vertexList, uvList);

			int textureID = mesh[i].texture;

			Eye->currentTexture = textureData[textureID];

			Eye->projectPoly(nVert, vertexList, uvList, colour, Screen->pixelBuffer, Screen->depthBuffer, 0, nullptr,
				hRatio, vRatio, Controls->visualStyle, Controls->torchIntensity, Controls->maxIllumination, viewT);
		}
	}
}


void Pong::renderAll()
{
	transform3d eyePosition = Eye->getTransformation();
	size_t nEntities = Entities.size();
	for (unsigned int i = 0; i < nEntities; i++)
	{
		renderMesh(eyePosition, polyCount[i], triangleMesh[i]);
	}
	renderMesh(eyePosition, ballPolyCount, ballMesh);
}


void Pong::updateAll()
{
	this->calculateFrametime();

	Controls->HandleUserEvents();

	Screen->resetPixelBuffer();

	Screen->resetDepthBuffer();

	this->updateCameraPosition();

	Controls->ceaseMotion();

	if (!Controls->isPaused)
	{
		this->updateEntities();
		this->updateBall();
	}	

	this->renderAll();

	this->displayStats(Controls->showCrosshair, Controls->showFPS, Controls->showPosition, Controls->showPolyN, Screen);

	this->resetPolyCounter();

	Screen->update();

	this->updateFrameCounter();
}


void Pong::updateFrameCounter()
{
	frameCounter++;
}


void Pong::resetPolyCounter()
{
	polyCounter = 0;
}


void Pong::incrementPolyCounter()
{
	polyCounter++;
}


void Pong::calculateFrametime()
{
	if (frameCounter == 30)
	{
		oldTime = newTime;
		newTime = clock();
		frameTime = newTime - oldTime;
		frameCounter = 0;
	}
}


void Pong::displayStats(bool crosshair, bool fps, bool position, bool polyN, std::shared_ptr<Canvas> Screen)
{
	double azmToShow, altToShow, rolToShow;
	if (crosshair) { Screen->drawCrosshair(2, 6, getColour(0, 127, 127, 127)); }
	if (position)
	{
		Screen->displayString("MAX ILLUMINATION", 30, 12, getColour(0, 255, 255, 255));
		Screen->displayValue(Controls->maxIllumination, 2, 2, 12, getColour(0, 255, 255, 255));
		Screen->displayString("TORCH INTENSITY", 30, 11, getColour(0, 255, 255, 255));
		Screen->displayValue(Controls->torchIntensity, 2, 2, 11, getColour(0, 255, 255, 255));

		Screen->displayString("POSITION X", 30, 9, getColour(0, 255, 127, 127));
		Screen->displayValue((double)Eye->x, 1, 2, 9, getColour(0, 255, 127, 127));
		Screen->displayString("POSITION Y", 30, 8, getColour(0, 127, 255, 127));
		Screen->displayValue((double)Eye->y, 1, 2, 8, getColour(0, 127, 255, 127));
		Screen->displayString("POSITION Z", 30, 7, getColour(0, 127, 127, 255));
		Screen->displayValue((double)Eye->z, 1, 2, 7, getColour(0, 127, 127, 255));

		azmToShow = Eye->azm * 180.0 / PI;
		if (azmToShow > 360.0) { azmToShow -= 360.0; }
		if (azmToShow < -360.0) { azmToShow += 360.0; }
		altToShow = Eye->alt * 180.0 / PI - 180.0;
		if (altToShow > 360.0) { altToShow -= 360.0; }
		if (altToShow < -360.0) { altToShow += 360.0; }
		rolToShow = Eye->rol * 180.0 / PI;
		if (rolToShow > 360.0) { rolToShow -= 360.0; }
		if (rolToShow < -360.0) { rolToShow += 360.0; }

		Screen->displayString("ROTATION X", 30, 5, getColour(0, 255, 63, 63));
		Screen->displayValue(azmToShow, 1, 2, 5, getColour(0, 255, 63, 63));
		Screen->displayString("ROTATION Y", 30, 4, getColour(0, 63, 255, 63));
		Screen->displayValue(altToShow, 1, 2, 4, getColour(0, 63, 255, 63));
		Screen->displayString("ROTATION Z", 30, 3, getColour(0, 63, 63, 255));
		Screen->displayValue(rolToShow, 1, 2, 3, getColour(0, 63, 63, 255));
	}
	if (fps)
	{
		Screen->displayFps((double)(30000.0 / frameTime), 1, 2, 0);
	}
	if (polyN)
	{
		Screen->displayValue(polyCounter, 1, 2, 53, getColour(0, 255, 0, 0));
	}
}
