#include "Pong.h"



Pong::Pong(std::shared_ptr<Canvas> screen, std::shared_ptr<Camera> eye, std::shared_ptr<EventHandler> controls,
			std::shared_ptr<LightSource> sun, std::shared_ptr<Player> hero, std::shared_ptr<SolidSphere> ball):
	Screen(screen), Eye(eye), Controls(controls), Sun(sun), Hero(hero), Ball(ball)
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


void Pong::loadProjectile(unsigned int n)
{
	for (unsigned int i = 0; i < n; i++)
	{
		auto bullet = std::make_shared<SolidCube>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, PI * 0.25f, PI * 0.25f, 0.0f, 0xffffffff, 1, 0.25f);
		bullet->setVisibility(false);
		Projectiles.push_back(bullet);
		ammo++;
	}
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

	size_t nProjectiles = Projectiles.size();
	projectileMesh = new triangle3dV* [nProjectiles];
	projectilePolyCount = new unsigned int[nProjectiles];
	for (unsigned int i = 0; i < nProjectiles; i++)
	{
		unsigned int nCurrent = Projectiles[i]->getTotalPoly();
		projectileMesh[i] = new triangle3dV[nCurrent];
		projectilePolyCount[i] = nCurrent;
		Projectiles[i]->getTriangleData_(projectileMesh[i]);
	}

	ballPolyCount = Ball->getTotalPoly();
	ballMesh = new triangle3dV[ballPolyCount];
	Ball->getTriangleData_(ballMesh);

	playerPolyCount = Hero->boundingVolume->getTotalPoly();
	playerMesh = new triangle3dV[playerPolyCount];
	Hero->boundingVolume->getTriangleData_(playerMesh);
}


void Pong::destroyMesh()
{
	size_t nEntities = Entities.size();
	for (unsigned int i = 0; i < nEntities; i++)
	{
		delete triangleMesh[i];
	}
	delete[] triangleMesh;
	delete polyCount;

	size_t nProjectiles = Projectiles.size();
	for (unsigned int i = 0; i < nProjectiles; i++)
	{
		delete projectileMesh[i];
	}
	delete[] projectileMesh;
	delete projectilePolyCount;
	
	delete playerMesh;

	delete ballMesh;	
}


void Pong::updateCameraPosition(const std::shared_ptr<Player>& player)
{
	Eye->azm	= player->azm;
	Eye->alt	= player->alt;
	Eye->rol	= player->rol;

	Eye->x		= player->x;
	Eye->y		= player->y;
	Eye->z		= player->z;
}


void Pong::updatePlayerPosition()
{
	Hero->azm = -Controls->turnH;
	Hero->alt = -Controls->turnV;
	Hero->rol = Controls->tiltP;

	Hero->x -= Controls->moveP * cos(Hero->azm) - Controls->strafeP * cos(Hero->azm + PI * 0.5);
	Hero->y += Controls->moveP * sin(Hero->azm) - Controls->strafeP * sin(Hero->azm + PI * 0.5);
	Hero->z += Controls->riseP;

	//vect3 playerVelocity = {   -Controls->moveP * cos(Hero->azm) - Controls->strafeP * cos(Hero->azm + PI * 0.5),
	//							Controls->moveP * sin(Hero->azm) - Controls->strafeP * sin(Hero->azm + PI * 0.5),
	//							Controls->riseP,
	//							1.0f };
	//
	//Hero->boundingVolume->setVelocity(playerVelocity);
	//
	//updateMovingObject(Hero->boundingVolume, playerPolyCount, playerMesh);
	//
	//vect3 playerPosition = Hero->boundingVolume->getPosition();
	//
	//Hero->x = playerPosition.x;
	//Hero->y = playerPosition.y;
	//Hero->z = playerPosition.z;
}


void Pong::updateEntities()
{
	for (unsigned int i = 0; i < Entities.size(); i++)
	{
		if (Entities[i]->isInMotion() && Entities[i]->isVisible())
		{
			Entities[i]->updatePosition();
			Entities[i]->updateRotation();

			vect3 velocity = Entities[i]->getVelocity();
			vect3 angVelocity = Entities[i]->getAngularVelocity();
			unsigned int nCurrent = Entities[i]->getTotalPoly();
			transformMesh(nCurrent, triangleMesh[i], velocity);
		}
	}
}


void Pong::updateProjectiles()
{
	for (unsigned int i = 0; i < Projectiles.size(); i++)
	{
		if (Projectiles[i]->isInMotion())
		{
			//if (Controls->gravityOn)
			//	Projectiles[i]->updateVelocity(gravity);
			//
			//Projectiles[i]->updatePosition();
			//Projectiles[i]->updateRotation();
			//
			//vect3 velocity = Projectiles[i]->getVelocity();
			//unsigned int nCurrent = Projectiles[i]->getTotalPoly();
			//transformMesh(nCurrent, projectileMesh[i], velocity);
			updateMovingObject(Projectiles[i], Projectiles[i]->getTotalPoly(), projectileMesh[i]);
		}

	}
}


void Pong::updateMovingObject(std::shared_ptr<SolidBody> object, int nPoly, triangle3dV* objectMesh)
{
	if (Controls->gravityOn)
		object->updateVelocity(gravity);

	vect3 displacement = object->getVelocity();
	vect3 oldPos = object->getPosition();
	vect3 newPos = addVectors(oldPos, displacement);

	double radius = object->getBBRadius();

	size_t nEntities = Entities.size();
	for (unsigned int i = 0; i < nEntities; i++)
	{
		for (unsigned int j = 0; j < polyCount[i]; j++)
		{
			if (objectApproachingWall(oldPos, displacement, i, j))
			{
				triangle3dV tempWall = triangleMesh[i][j];

				double oldDistWall = distPoint2Plane(oldPos, tempWall);
				double newDistWall = distPoint2Plane(newPos, tempWall);

				if (newDistWall <= radius)
				{
					double x = oldDistWall - radius;
					double v2n = -dotProduct(displacement, tempWall.N);
					double safePercentage = x / v2n;

					vect3 toIntersection = scaleVector(safePercentage, displacement);
					object->updatePosition(toIntersection);
					transformMesh(nPoly, objectMesh, toIntersection);

					vect3 oldVelocity = object->getVelocity();
					if (Controls->gravityOn)
					{
						vect3 newVelocity = subVectors(oldVelocity, scaleVector(1.50f * dotProduct(oldVelocity, tempWall.N), tempWall.N));
						object->setVelocity(newVelocity);
					}
					else
					{
						vect3 newVelocity = subVectors(oldVelocity, scaleVector(2.00f * dotProduct(oldVelocity, tempWall.N), tempWall.N));
						//vect3 newVelocity = { 0.0f, 0.0f, 0.0f, 1.0f };
						//object->stop();
						object->setVelocity(newVelocity);
					}

					return;
				}
			}
		}
	}

	object->updatePosition();
	transformMesh(nPoly, objectMesh, displacement);
}


bool Pong::objectApproachingWall(vect3 p, vect3 v, const unsigned int& i, const unsigned int& j)
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
		polyCounter++;
	}
}


void Pong::renderAll()
{
	polyCounter = 0;

	transform3d eyePosition = Eye->getTransformation();
	size_t nEntities = Entities.size();
	for (unsigned int i = 0; i < nEntities; i++)
	{
		if(Entities[i]->isVisible())
			renderMesh(eyePosition, polyCount[i], triangleMesh[i]);
	}
	size_t nProjectiles = Projectiles.size();
	for (unsigned int i = 0; i < nProjectiles; i++)
	{
		vect3 currentPos = Projectiles[i]->getPosition();
		if(Projectiles[i]->isVisible() && currentPos.z >= 0.0f)
			renderMesh(eyePosition, projectilePolyCount[i], projectileMesh[i]);
	}
	renderMesh(eyePosition, ballPolyCount, ballMesh);
}


void Pong::updateAll()
{
	this->calculateFrametime();

	Controls->HandleUserEvents();

	Screen->resetPixelBuffer();

	Screen->resetDepthBuffer();

	this->updatePlayerPosition();

	this->updateCameraPosition(Hero);

	if (!Controls->isPaused)
	{
		this->updateEntities();
		this->updateProjectiles();
		this->updateMovingObject(Ball, ballPolyCount, ballMesh);
		if (Hero->lastShot < 5)
			Hero->lastShot++;
		if (Controls->isFiring && Hero->lastShot >= 5)
			Hero->shoot(Projectiles, projectilePolyCount, projectileMesh);
	}	

	Controls->ceaseMotion();

	this->renderAll();

	this->displayStats(Controls->showCrosshair, Controls->showFPS, Controls->showPosition, Controls->showPolyN, Controls->showAmmo, Screen);

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


void Pong::displayStats(bool crosshair, bool fps, bool position, bool polyN, bool amm, std::shared_ptr<Canvas> Screen)
{
	double azmToShow, altToShow, rolToShow;
	if (crosshair) { Screen->drawCrosshair(2, 6,	0x007f7f7f); }
	if (position)
	{
		Screen->displayString("MAX ILLUMINATION", 30, 12,			0x00ffffff);
		Screen->displayValue(Controls->maxIllumination, 2, 2, 12,	0x00ffffff);
		Screen->displayString("TORCH INTENSITY", 30, 11,			0x00ffffff);
		Screen->displayValue(Controls->torchIntensity, 2, 2, 11,	0x00ffffff);

		Screen->displayString("POSITION X", 30, 9,		0x00ff7f7f);
		Screen->displayValue((double)Eye->x, 1, 2, 9,	0x00ff7f7f);
		Screen->displayString("POSITION Y", 30, 8,		0x007fff7f);
		Screen->displayValue((double)Eye->y, 1, 2, 8,	0x007fff7f);
		Screen->displayString("POSITION Z", 30, 7,		0x007f7fff);
		Screen->displayValue((double)Eye->z, 1, 2, 7,	0x007f7fff);

		azmToShow = Eye->azm * 180.0 / PI;
		if (azmToShow > 360.0) { azmToShow -= 360.0; }
		if (azmToShow < -360.0) { azmToShow += 360.0; }
		altToShow = Eye->alt * 180.0 / PI - 180.0;
		if (altToShow > 360.0) { altToShow -= 360.0; }
		if (altToShow < -360.0) { altToShow += 360.0; }
		rolToShow = Eye->rol * 180.0 / PI;
		if (rolToShow > 360.0) { rolToShow -= 360.0; }
		if (rolToShow < -360.0) { rolToShow += 360.0; }

		Screen->displayString("ROTATION X", 30, 5,	0x00ff3f3f);
		Screen->displayValue(azmToShow, 1, 2, 5,	0x00ff3f3f);
		Screen->displayString("ROTATION Y", 30, 4,	0x003fff3f);
		Screen->displayValue(altToShow, 1, 2, 4,	0x003fff3f);
		Screen->displayString("ROTATION Z", 30, 3,	0x003f3fff);
		Screen->displayValue(rolToShow,	1, 2, 3,	0x003f3fff);
	}
	if (fps)
	{
		Screen->displayFps((double)(30000.0 / frameTime), 1, 2, 0);
	}
	if (polyN)
	{
		Screen->displayValue(polyCounter, 0, 2, 53, 0x00ff0000);
	}
	if (amm)
	{
		Screen->displayValue(Hero->ammo, 0, 100, 3, 0x007f7f00);
	}
}



