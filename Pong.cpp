#include "Pong.h"



Pong::Pong(std::shared_ptr<Canvas> screen, std::shared_ptr<Camera> eye, std::shared_ptr<EventHandler> controls,
			std::shared_ptr<LightSource> sun, std::shared_ptr<Player> hero, std::shared_ptr<Player> enemy):
	Screen(screen), Eye(eye), Controls(controls), Sun(sun), Hero(hero), Enemy(enemy)
{
	for (int v = 0; v < MAXCLIPVERTS; v++)
	{
		vertexList[v]	= { 0.0f, 0.0f, 0.0f, 0.0f };
		uvList[v]		= { 0.0f, 0.0f };
	}

	hRatio = Eye->getHRatio();
	vRatio = Eye->getVRatio();
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


void Pong::addBall(std::shared_ptr<SolidBody> ball)
{
	Balls.push_back(ball);
}


void Pong::loadProjectile(unsigned int n)
{
	for (unsigned int i = 0; i < n; i++)
	{
		auto bullet = std::make_shared<Bullet>(0.0f, 0.0f, 0.0f, 0.25f, 0.5f, 1.0f, 0xffffffff);
		bullet->setGravity(true);
		bullet->setVisibility(false);
		bullet->setBehaviour(stick);
		bullet->setBBRadius(0.05f);
		Projectiles.push_back(bullet);
		ammo++;
	}
	Hero->setAmmo(Projectiles.size());
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

	if (Hero != nullptr)
	{
		size_t nPlayerParts = Hero->Parts.size();
		playerMesh = new triangle3dV * [nPlayerParts];
		playerPolyCount = new unsigned int[nPlayerParts];
		for (unsigned int i = 0; i < nPlayerParts; i++)
		{
			unsigned int nCurrent = Hero->Parts[i]->getTotalPoly();
			playerMesh[i] = new triangle3dV[nCurrent];
			playerPolyCount[i] = nCurrent;
			Hero->Parts[i]->getTriangleData_(playerMesh[i]);
		}
	}

	if (Enemy != nullptr)
	{
		size_t nEnemyParts = Enemy->Parts.size();
		enemyMesh = new triangle3dV * [nEnemyParts];
		enemyPolyCount = new unsigned int[nEnemyParts];
		for (unsigned int i = 0; i < nEnemyParts; i++)
		{
			unsigned int nCurrent = Enemy->Parts[i]->getTotalPoly();
			enemyMesh[i] = new triangle3dV[nCurrent];
			enemyPolyCount[i] = nCurrent;
			Enemy->Parts[i]->getTriangleData_(enemyMesh[i]);
		}
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

	size_t nBalls = Balls.size();
	ballMesh = new triangle3dV* [nBalls];
	ballPolyCount = new unsigned int[nBalls];
	for (unsigned int i = 0; i < nBalls; i++)
	{
		unsigned int nCurrent = Balls[i]->getTotalPoly();
		ballMesh[i] = new triangle3dV[nCurrent];
		ballPolyCount[i] = nCurrent;
		Balls[i]->getTriangleData_(ballMesh[i]);
	}

	size_t nExplosions = Balls.size();
	explosionMesh = new triangle3dV* [nExplosions];
	explosionPolyCount = new unsigned int [nExplosions];
	for (unsigned int i = 0; i < nExplosions; i++)
	{
		double radius = Balls[i]->getBBRadius();

		auto ball_1 = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xffff0000, 1, radius * 0.8f, 12);
		unsigned int n_1 = ball_1->getTotalPoly();
		triangle3dV* mesh_1 = new triangle3dV[n_1];
		ball_1->getTriangleData_(mesh_1);

		auto ball_2 = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xffffff00, 1, radius * 0.6f, 8);
		unsigned int n_2 = ball_2->getTotalPoly();
		triangle3dV* mesh_2 = new triangle3dV[n_2];
		ball_2->getTriangleData_(mesh_2);

		unsigned int nCurrent = n_1 + n_2;
		explosionMesh[i] = new triangle3dV[nCurrent];
		explosionPolyCount[i] = nCurrent;

		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;

		srand(unsigned int(time(NULL)));

		for (unsigned int p = 0; p < n_1; p++)
		{
			explosionMesh[i][p] = mesh_1[p];

			r = 128 + rand() % 127;
			g = 128 + rand() % 127;

			explosionMesh[i][p].colour = getColour(0, r, g, 0);
		}
		for (unsigned int q = n_1; q < n_1 + n_2; q++)
		{
			explosionMesh[i][q] = mesh_2[q - n_1];
		
			r = 128 + rand() % 127;
			g = 128 + rand() % 127;
		
			explosionMesh[i][q].colour = getColour(0, r, g, 0);
		}
		delete[] mesh_1;
		delete[] mesh_2;
	}
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

	if (Hero != nullptr)
	{
		size_t nPlayerParts = Hero->Parts.size();
		for (unsigned int i = 0; i < nPlayerParts; i++)
		{
			delete playerMesh[i];
		}
		delete[] playerMesh;
		delete playerPolyCount;
	}
	
	if (Enemy != nullptr)
	{
		size_t nEnemyParts = Enemy->Parts.size();
		for (unsigned int i = 0; i < nEnemyParts; i++)
		{
			delete enemyMesh[i];
		}
		delete[] enemyMesh;
		delete enemyPolyCount;
	}

	size_t nProjectiles = Projectiles.size();
	for (unsigned int i = 0; i < nProjectiles; i++)
	{
		delete projectileMesh[i];
	}
	delete[] projectileMesh;
	delete projectilePolyCount;

	size_t nBalls = Balls.size();
	for (unsigned int i = 0; i < nBalls; i++)
	{
		delete ballMesh[i];
	}
	delete[] ballMesh;
	delete ballPolyCount;

	size_t nExplosions = Balls.size();
	for (unsigned int i = 0; i < nExplosions; i++)
	{
		delete explosionMesh[i];
	}
	delete[] explosionMesh;
	delete explosionPolyCount;
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

	for (unsigned int i = 0; i < Hero->Parts.size(); i++)
	{
		Hero->Parts[i]->setPosition({ Hero->x, Hero->y, Hero->z, 1.0f });
		Hero->Parts[i]->setRotation({ Hero->alt, Hero->rol, Hero->azm, 1.0f });
	}
}


void Pong::updateEnemyPosition()
{
	Enemy->azm = -Controls->turnH;
	Enemy->alt = -Controls->turnV;
	Enemy->rol = Controls->tiltP;

	Enemy->x -= Controls->moveP * cos(Enemy->azm) - Controls->strafeP * cos(Enemy->azm + PI * 0.5);
	Enemy->y += Controls->moveP * sin(Enemy->azm) - Controls->strafeP * sin(Enemy->azm + PI * 0.5);
	Enemy->z += Controls->riseP;

	for (unsigned int i = 0; i < Enemy->Parts.size(); i++)
	{
		Enemy->Parts[i]->setPosition({ Enemy->x, Enemy->y, Enemy->z, 1.0f });
		Enemy->Parts[i]->setRotation({ Enemy->alt, Enemy->rol, Enemy->azm, 1.0f });
	}
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
		}
	}
}


void Pong::updateBalls()
{
	for (unsigned int i = 0; i < Balls.size(); i++)
	{
		if (Balls[i]->isInMotion())
		{
			updateMovingObject(Balls[i], Balls[i]->getTotalPoly(), ballMesh[i]);
		}
		if (Balls[i]->isDestroyed() && !Balls[i]->isVanished())
		{
			
			Balls[i]->incrementTicksSinceHit();
			double ticks = double(Balls[i]->getTicksSinceHit());
			if (ticks >= 60.0f)
				Balls[i]->vanish();
			explodeDebris(2.0f / (ticks * ticks), Balls[i]->getPosition(), ballPolyCount[i], ballMesh[i]);
			explodeDebris(2.0f / (ticks * ticks), Balls[i]->getPosition(), explosionPolyCount[i], explosionMesh[i]);
		}
	}
}


void Pong::updateProjectiles()
{
	for (unsigned int i = 0; i < Projectiles.size(); i++)
	{
		if (Projectiles[i]->isInMotion())
		{
			vect3 currentPos = Projectiles[i]->getPosition();
			if (Projectiles[i]->getPosition().z <= 0.0f)
				Projectiles[i]->setMotion(false);
			if (updateMovingObject(Projectiles[i], Projectiles[i]->getTotalPoly(), projectileMesh[i]))
			{
			}			
			if (hitTest(Projectiles[i], Balls))
				std::cout << "+" << std::flush;
		}
	}
}


bool Pong::hitTest(const std::shared_ptr<SolidBody>& bullet, std::vector<std::shared_ptr<SolidBody>> targets)
{
	vect3 displacement = bullet->getVelocity();
	vect3 oldPos = bullet->getPosition();
	vect3 newPos = oldPos + displacement;

	for (unsigned int i = 0; i < targets.size(); i++)
	{
		if (!targets[i]->isDestroyed())
		{
			double targetRadius = targets[i]->getBBRadius();
			vect3 targetPosition = targets[i]->getPosition();
			vect3 currentV = newPos - oldPos;
			double sOld = (oldPos - targetPosition) * currentV;
			double sNew = (newPos - targetPosition) * currentV;
			if (distPoint2LineSquared(targetPosition, oldPos, newPos) <= targetRadius * targetRadius &&
				sign(sOld) != sign(sNew))
			{
				targets[i]->destroy();
				targets[i]->setMotion(false);
				targets[i]->setBehaviour(penetrate);

				return true;
			}
		}		
	}

	return false;
}


bool Pong::updateMovingObject(std::shared_ptr<SolidBody> object, int nPoly, triangle3dV* objectMesh)
{
	if (Controls->gravityOn && object->isGravitating())
		object->updateVelocity(gravity);

	vect3 displacement = object->getVelocity();
	vect3 oldPos = object->getPosition();
	vect3 newPos = oldPos + displacement;

	double radius = object->getBBRadius();

	size_t nEntities = Entities.size();
	for (unsigned int i = 0; i < nEntities; i++)
	{
		vect3 sc = Entities[i]->getScale();
		vect3 mv = Entities[i]->getPosition();
		vect3 rt = Entities[i]->getRotation();
		for (unsigned int j = 0; j < polyCount[i]; j++)
		{
			triangle3dV tempWall = Eye->object2worldT(sc, mv, rt, triangleMesh[i][j]);

			if (objectApproachingWall(oldPos, displacement, tempWall))
			{
				double oldDistWall = distPoint2Plane(oldPos, tempWall);
				double newDistWall = distPoint2Plane(newPos, tempWall);

				if (newDistWall <= radius)
				{
					double x = oldDistWall - radius;
					double v2n = -(displacement * tempWall.N);
					double safePercentage = x / v2n;

					vect3 toIntersection = displacement * safePercentage;
					object->updatePosition(toIntersection);

					vect3 oldVelocity = object->getVelocity();

					switch (object->getBehaviour())
					{
						case penetrate:
						{

						}
						break;
						case stick:
						{								
							vect3 newVelocity = { 0.0f, 0.0f, 0.0f, 1.0f };
							object->setMotion(false);
							object->setVelocity(newVelocity);
						}
						break;
						case bounce:
						{
							if (Controls->gravityOn)
							{
								vect3 newVelocity = oldVelocity - (tempWall.N * (1.50f * (oldVelocity * tempWall.N)));
								object->setVelocity(newVelocity);
							}
							else
							{
								vect3 newVelocity = oldVelocity - (tempWall.N * (2.00f * (oldVelocity * tempWall.N)));
								object->setVelocity(newVelocity);
							}
						}
						break;
						case slide:
						{

						}
						break;
						default:				
						break;
					}

					return true;
				}
			}
		}
	}

	object->updatePosition();
	object->updateRotation();

	return false;
}


bool Pong::objectApproachingWall(vect3 p, vect3 v, triangle3dV T)
{
	double v2n = v * T.N;							//Magnitude of velocity projected to plane normal
	double dist = distPoint2Plane(p, T);			//Distance from point to plane
	if (dist >= 0.0f && v2n < 0.0f)
	{
		double t = dist / -v2n;
		vect3 intersection{ p.x + v.x * t, p.y + v.y * t, p.z + v.z * t, 1.0f };

		double sA = ((T.A - T.C) ^ T.N) * (T.A - intersection);
		double sB = ((T.B - T.A) ^ T.N) * (T.B - intersection);
		double sC = ((T.C - T.B) ^ T.N) * (T.C - intersection);

		if ((sA <= 0.0f && sB <= 0.0f && sC <= 0.0f) ||
			(sA >= 0.0f && sB >= 0.0f && sC >= 0.0f))	//If point of intersection lies inside triangle T
		{
			return true;
		}
	}
	return false;
}


void Pong::explodeMesh(double velocity, vect3 centre, int nPoly, triangle3dV* mesh)
{
	for (int i = 0; i < nPoly; i++)
	{
		vect3 dA = (mesh[i].A - centre) * velocity;
		vect3 dB = (mesh[i].B - centre) * velocity;
		vect3 dC = (mesh[i].C - centre) * velocity;

		mesh[i].A += dA;
		mesh[i].B += dB;
		mesh[i].C += dC;
	}
}


void Pong::explodeDebris(double velocity, vect3 centre, int nPoly, triangle3dV* mesh)
{
	for (int i = 0; i < nPoly; i++)
	{
		vect3 temp = mesh[i].A;
		
		double x = double(rand() % 10) * 0.025f;
		double y = double(rand() % 20) * 0.025f;
		double z = double(rand() % 30) * 0.025f;
		
		vect3 mod = { x, y, z, 1.0f };
		vect3 displacement = (temp + mod) * velocity;

		if (Controls->gravityOn)
			displacement = addVectors(displacement, gravity);

		movePoly(displacement, mesh[i]);
	}
}


void Pong::renderMesh(const transform3d& eyePosition, const vect3& sc, const vect3& mv, const vect3& rt, const int& nPoly, triangle3dV* mesh)
{
	for (int i = 0; i < nPoly; i++)
	{
		triangle3dV worldT = Eye->object2worldT(sc, mv, rt, mesh[i]);
		if (Eye->polyFacingCamera(worldT))
		{
			Uint32 colour = mesh[i].colour;

			triangle3dV viewT = Eye->world2viewT(eyePosition, worldT);

			Eye->illuminatePoly(*Sun, &viewT, worldT, Controls->visualStyle);

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
		if (Entities[i]->isVisible())
		{
			vect3 sc = Entities[i]->getScale();
			vect3 mv = Entities[i]->getPosition();
			vect3 rt = Entities[i]->getRotation();
			renderMesh(eyePosition, sc, mv, rt, polyCount[i], triangleMesh[i]);
		}			
	}

	if (Hero != nullptr)
	{
		size_t nPlayerParts = Hero->Parts.size();
		for (unsigned int i = 0; i < nPlayerParts; i++)
		{
			if (Hero->Parts[i]->isVisible())
			{
				vect3 sc = Hero->Parts[i]->getScale();
				vect3 mv = Hero->Parts[i]->getPosition();
				vect3 rt = Hero->Parts[i]->getRotation();
				renderMesh(eyePosition, sc, mv, rt, playerPolyCount[i], playerMesh[i]);
			}			
		}
	}

	if (Enemy != nullptr)
	{
		size_t nEnemyParts = Enemy->Parts.size();
		for (unsigned int i = 0; i < nEnemyParts; i++)
		{
			if (Enemy->Parts[i]->isVisible())
			{
				vect3 sc = Enemy->Parts[i]->getScale();
				vect3 mv = Enemy->Parts[i]->getPosition();
				vect3 rt = Enemy->Parts[i]->getRotation();
				renderMesh(eyePosition, sc, mv, rt, enemyPolyCount[i], enemyMesh[i]);
			}			
		}
	}

	size_t nProjectiles = Projectiles.size();
	for (unsigned int i = 0; i < nProjectiles; i++)
	{
		if (Projectiles[i]->isVisible())
		{
			vect3 sc = Projectiles[i]->getScale();
			vect3 mv = Projectiles[i]->getPosition();
			vect3 rt = Projectiles[i]->getRotation();
			renderMesh(eyePosition, sc, mv, rt, projectilePolyCount[i], projectileMesh[i]);
		}			
	}

	size_t nBalls = Balls.size();
	for (unsigned int i = 0; i < nBalls; i++)
	{
		if (Balls[i]->isVisible() && !Balls[i]->isVanished())
		{
			vect3 sc = Balls[i]->getScale();
			vect3 mv = Balls[i]->getPosition();
			vect3 rt = Balls[i]->getRotation();
			renderMesh(eyePosition, sc, mv, rt, ballPolyCount[i], ballMesh[i]);
			if(Balls[i]->isDestroyed())
				renderMesh(eyePosition, sc, mv, rt, explosionPolyCount[i], explosionMesh[i]);
		}
			
	}
}


void Pong::updateAll()
{
	this->calculateFrametime();

	Controls->HandleUserEvents();

	Screen->resetPixelBuffer();

	Screen->resetDepthBuffer();

	if (Controls->playerControlled)
	{
		this->updatePlayerPosition();
	}
	else if (Controls->enemyControlled)
	{
		this->updateEnemyPosition();
	}

	this->updateCameraPosition(Hero);

	if (!Controls->isPaused)
	{
		this->updateEntities();
		this->updateProjectiles();
		this->updateBalls();
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



