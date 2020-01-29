#include "Game.h"



Game::Game(std::shared_ptr<Canvas> screen, std::shared_ptr<Camera> eye, std::shared_ptr<EventHandler> controls,
	std::shared_ptr<LightSource> sun, std::shared_ptr<Player> hero, std::shared_ptr<Player> enemy) :
	Screen(screen), Eye(eye), Controls(controls), Sun(sun), Hero(hero), Enemy(enemy)
{
	for (int v = 0; v < MAXCLIPVERTS; v++)
	{
		vertexList[v] = { 0.0f, 0.0f, 0.0f, 0.0f };
		uvList[v] = { 0.0f, 0.0f };
	}
	Renderer = std::make_shared<Projection>();
}


Game::~Game()
{
}


void Game::addEntity(std::shared_ptr<SolidBody> entity)
{
	Entities.push_back(entity);
	int n = entity->getTotalPoly();
	std::cout << "Adding " << n << " polygons to collision mesh..." << std::endl;
	triangle3dV* temp = new triangle3dV[n];
	entity->getTriangleData(temp);
	for (int i = 0; i < n; i++)
		collisionPlanes.push_back(temp[i]);
	delete[] temp;
}


void Game::addBall(std::shared_ptr<SolidBody> ball)
{
	Balls.push_back(ball);
}


void Game::addEnemy(std::shared_ptr<Player> enemy)
{
	Enemies.push_back(enemy);
}


void Game::addTextScreen(std::string tname, std::shared_ptr<Text> tscreen)
{
	TextScreens.insert({ tname, tscreen });
}


void Game::addEmitter(std::shared_ptr<ParticleSystem> em)
{
	Emitters.push_back(em);
}


void Game::addDynamicSurface(std::shared_ptr<DynamicMesh> s)
{
	DynamicSurfaces.push_back(s);
}


void Game::addStaticSurface(std::shared_ptr<Terrain> t)
{
	StaticSurfaces.push_back(t);
}


void Game::addPointCloud(std::shared_ptr<PointCloud> p)
{
	PointClouds.push_back(p);
}


void Game::loadProjectile(unsigned n)
{
	for (unsigned i = 0; i < n; i++)
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


void Game::addExplosion(unsigned n)
{
	for (unsigned i = 0; i < n; i++)
	{
		auto explosion = std::make_shared<Explosion>(5000, 0.1f, 60.0f);
		Explosions.push_back(explosion);
	}
}


void Game::updateCameraPosition(const std::shared_ptr<Player>& player)
{
	Eye->azm = player->azm;
	Eye->alt = player->alt;
	Eye->rol = player->rol;

	Eye->x = player->x;
	Eye->y = player->y;
	Eye->z = player->z;

	Eye->updateViewDirection();
}


void Game::updateHeroPosition()
{
	Hero->azm = -Controls->turnH;
	Hero->alt = -Controls->turnV;
	Hero->rol = Controls->tiltP;

	Hero->x -= Controls->moveP * (float)cos(Hero->azm) - Controls->strafeP * (float)cos(Hero->azm + PI * 0.5);
	Hero->y += Controls->moveP * (float)sin(Hero->azm) - Controls->strafeP * (float)sin(Hero->azm + PI * 0.5);
	Hero->z += Controls->riseP;

	//if (!Controls->isFiring)
	//	Hero->idle();

	updatePlayerModel(Hero);
}


void Game::updateEnemyPosition()
{
	Enemy->azm = -Controls->turnH;
	Enemy->alt = -Controls->turnV;
	Enemy->rol = Controls->tiltP;

	Enemy->x -= Controls->moveP * (float)cos(Enemy->azm) - Controls->strafeP * (float)cos(Enemy->azm + PI * 0.5);
	Enemy->y += Controls->moveP * (float)sin(Enemy->azm) - Controls->strafeP * (float)sin(Enemy->azm + PI * 0.5);
	Enemy->z += Controls->riseP;

	updatePlayerModel(Enemy);
}


void Game::updateEnemiesPositionsAI(aiGoal goal)
{
	for (unsigned int i = 0; i < Enemies.size(); i++)
	{
		if (!Enemies[i]->isDestroyed())
		{
			if (Enemies[i]->isUnderAttack())
			{
				Enemies[i]->isFiring = false;
				Enemies[i]->moveOutOfHarmsWay();
			}
			else
			{
				switch (goal)
				{
				case be_idle:
				{
					Enemies[i]->isFiring = false;
					Enemies[i]->idle();
					break;
				}
				case follow_player:
				{
					Enemies[i]->lockOnTarget(vect3{ Hero->x, Hero->y, Hero->z, 1.0f });
					Enemies[i]->idle();
					break;
				}
				case kill_player:
				{
					if (Enemies[i]->lockOnTarget(vect3{ Hero->x, Hero->y, Hero->z, 1.0f }))
						Enemies[i]->isFiring = true;
					else
					{
						Enemies[i]->isFiring = false;
						Enemies[i]->idle();
					}
					break;
				}
				case follow_others:
				{
					Enemies[i]->isFiring = false;
					unsigned int targetIndex = Enemies[i]->pickTarget(Balls);
					if (targetIndex < Balls.size())
					{
						Enemies[i]->lockOnTarget(Balls[targetIndex]->getPosition());
						Enemies[i]->idle();
					}
					else
						Enemies[i]->idle();
					break;
				}
				case kill_others:
				{
					unsigned int targetIndex = Enemies[i]->pickTarget(Balls);

					if (targetIndex < Balls.size())
					{
						if (Enemies[i]->lockOnTarget(Balls[targetIndex]->getPosition()))
							Enemies[i]->isFiring = true;
						else
						{
							Enemies[i]->isFiring = false;
							Enemies[i]->idle();
						}
					}
					else
					{
						Enemies[i]->isFiring = false;
						Enemies[i]->idle();
					}
					break;
				}
				case follow_each_other:
				{
					Enemies[i]->isFiring = false;
					unsigned int targetIndex = Enemies[i]->pickTarget(Enemies, i);
					if (targetIndex < Enemies.size() && targetIndex != i)
					{
						Enemies[i]->lockOnTarget(Enemies[targetIndex]->getPosition());
						Enemies[i]->idle();
					}
					else
						Enemies[i]->idle();
					break;
				}
				case kill_each_other:
				{
					unsigned int targetIndex = Enemies[i]->pickTarget(Enemies, i);

					if (targetIndex < Enemies.size() && targetIndex != i)
					{
						if (Enemies[i]->lockOnTarget(Enemies[targetIndex]->getPosition()))
							Enemies[i]->isFiring = true;
						else
						{
							Enemies[i]->isFiring = false;
							Enemies[i]->idle();
						}
					}
					else
					{
						Enemies[i]->isFiring = false;
						Enemies[i]->idle();
					}
					break;
				}
				default:
				{
					Enemies[i]->isFiring = false;
					Enemies[i]->idle();
					break;
				}
				}
			}

			for (unsigned int j = 0; j < Enemies.size(); j++)
				if (j != i)
					Enemies[i]->keepDistanceFrom(Enemies[j]->getPosition());

			updatePlayerModel(Enemies[i]);
		}
	}
}


void Game::updatePlayerModel(std::shared_ptr<Player> character)
{
	for (auto& Part : character->Parts)
	{
		Part->setPosition({ character->x, character->y, character->z, 1.0f });
		Part->setRotation({ -character->rol, character->alt, -character->azm, 1.0f });
	}
}


void Game::updateEntities()
{
	for (auto& Entity : Entities)
	{
		if (Entity->isInMotion() && Entity->isVisible())
		{
			Entity->updatePosition();
			Entity->updateRotation();
		}
	}
}


void Game::updateBalls()
{
	for (auto& Ball : Balls)
	{
		if (Ball->isInMotion())
			updateMovingObject(Ball);
		//if (Ball->isDestroyed() && !Ball->isVanished())
		//{			
		//	Ball->explode();
		//	Ball->incrementTicksSinceHit();
		//	float ticks = float(Ball->getTicksSinceHit());
		//	if (ticks >= 60.0f)
		//		Ball->vanish();
		//}
	}
}


void Game::updateProjectiles()
{
	for (auto& P : Projectiles)
	{
		if (P->isFired())
		{
			if (P->isInMotion())
			{
				vect3 currentPos = P->getPosition();
				updateMovingObject(P);
				hitTest(P, Balls);
				hitTest(P, Hero);
				hitTest(P, Enemy);
				for (auto& E : Enemies)
					hitTest(P, E);
			}

			P->incrementTicksSinceFired();

			if (P->getTicksSinceFired() > 1000)
			{
				P->setFired(false);
				P->setTicksSinceFired(0);
				P->setPosition(vect3{ 0.0f, 0.0f, 0.0f, 1.0f });
				P->setVelocity(vect3{ 0.0f, 0.0f, 0.0f, 1.0f });
				P->setVisibility(false);
				P->setMotion(false);
				ammo++;
			}
		}
	}
}


void Game::updateExplosions()
{
	for (auto& Exp : Explosions)
		if (Exp->isActive())
			Exp->update();
}


bool Game::hitTest(const std::shared_ptr<SolidBody>& bullet, std::shared_ptr<Player> actor)
{
	vect3 displacement = bullet->getVelocity();
	vect3 oldPos = bullet->getPosition();
	vect3 newPos = oldPos + displacement;

	if (actor != nullptr && !actor->isDestroyed())
	{
		float targetRadius = actor->getBBRadius();
		vect3 targetPosition = actor->getPosition();
		vect3 currentV = newPos - oldPos;
		float sOld = (oldPos - targetPosition) * currentV;
		float sNew = (newPos - targetPosition) * currentV;
		if (distPoint2LineSquared(targetPosition, oldPos, newPos) <= targetRadius * targetRadius &&
			sign(sOld) != sign(sNew))
		{
			actor->takeDamage(1);
			actor->setUnderAttack(true);
			actor->gotHitFrom(bullet->getVelocity());

			return true;
		}
	}

	return false;
}


bool Game::hitTest(const std::shared_ptr<SolidBody>& bullet, std::vector<std::shared_ptr<SolidBody>> targets)
{
	vect3 displacement = bullet->getVelocity();
	vect3 oldPos = bullet->getPosition();
	vect3 newPos = oldPos + displacement;

	for (unsigned int i = 0; i < targets.size(); i++)
	{
		if (!targets[i]->isDestroyed())
		{
			float targetRadius = targets[i]->getBBRadius();
			vect3 targetPosition = targets[i]->getPosition();
			vect3 currentV = newPos - oldPos;
			float sOld = (oldPos - targetPosition) * currentV;
			float sNew = (newPos - targetPosition) * currentV;
			if (distPoint2LineSquared(targetPosition, oldPos, newPos) <= targetRadius * targetRadius &&
				sign(sOld) != sign(sNew))
			{
				targets[i]->destroy();
				targets[i]->setMotion(false);
				targets[i]->setBehaviour(penetrate);
				for (auto& Exp : Explosions)
					if (!Exp->isActive() && !Exp->isUsed())
					{
						targets[i]->vanish();
						Exp->activate(targets[i]->getPosition());
						break;
					}

				return true;
			}
		}
	}

	return false;
}


bool Game::updateMovingObject(std::shared_ptr<SolidBody> object)
{
	if (Controls->gravityOn && object->isGravitating())
		object->updateVelocity(gravity);

	vect3 displacement = object->getVelocity();
	vect3 oldPos = object->getPosition();
	vect3 newPos = oldPos + displacement;

	float radius = object->getBBRadius();

	for (auto& tempWall : collisionPlanes)
	{
		if (objectApproachingWall(oldPos, displacement, tempWall))
		{
			float oldDistWall = distPoint2Plane(oldPos, tempWall);
			float newDistWall = distPoint2Plane(newPos, tempWall);

			if (newDistWall <= radius)
			{
				float x = oldDistWall - radius;
				float v2n = -(displacement * tempWall.N);
				float safePercentage = x / v2n;

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

	object->updatePosition();
	object->updateRotation();

	return false;
}


bool Game::objectApproachingWall(vect3& p, vect3& v, triangle3dV& T)
{
	float v2n = v * T.N;							//Magnitude of velocity projected to plane normal
	float dist = distPoint2Plane(p, T);			//Distance from point to plane
	if (dist >= 0.0f && v2n < 0.0f)
	{
		float t = dist / -v2n;
		vect3 intersection{ p.x + v.x * t, p.y + v.y * t, p.z + v.z * t, 1.0f };

		float sA = ((T.A - T.C) ^ T.N) * (T.A - intersection);
		float sB = ((T.B - T.A) ^ T.N) * (T.B - intersection);
		float sC = ((T.C - T.B) ^ T.N) * (T.C - intersection);

		if ((sA <= 0.0f && sB <= 0.0f && sC <= 0.0f) ||
			(sA >= 0.0f && sB >= 0.0f && sC >= 0.0f))	//If point of intersection lies inside triangle T
		{
			return true;
		}
	}
	return false;
}


void Game::renderAll()
{
	polyCounter = 0;

	mat4x4 RotationM = Eye->getRotation();
	mat4x4 TranslationM = Eye->getTranslation();
	mat4x4 RM = RotationM * TranslationM;

	for (auto& E : Entities)
		if (E->isVisible())
			E->render(Eye, false, RotationM, TranslationM, *Sun,
				Controls->visualStyle, Controls->torchIntensity, Controls->maxIllumination);

	if (Hero != nullptr)
		for (auto& P : Hero->Parts)
			if (P->isVisible())
				P->render(Eye, true, RotationM, TranslationM, *Sun,
					Controls->visualStyle, Controls->torchIntensity, Controls->maxIllumination);

	for (auto& E : Enemies)
		if (!E->isDestroyed())
			for (auto& P : E->Parts)
				if (P->isVisible())
					P->render(Eye, true, RotationM, TranslationM, *Sun,
						Controls->visualStyle, Controls->torchIntensity, Controls->maxIllumination);

	if (Enemy != nullptr)
		for (auto& P : Enemy->Parts)
			if (P->isVisible())
				P->render(Eye, true, RotationM, TranslationM, *Sun,
					Controls->visualStyle, Controls->torchIntensity, Controls->maxIllumination);

	for (auto& P : Projectiles)
		if (P->isVisible())
			P->render(Eye, true, RotationM, TranslationM, *Sun,
				Controls->visualStyle, Controls->torchIntensity, Controls->maxIllumination);

	for (auto& Exp : Explosions)
		if (Exp->isActive())
			Exp->render(Eye, Screen, RM);

	for (auto& B : Balls)
		if (B->isVisible() && !B->isVanished())
			B->render(Eye, true, RotationM, TranslationM, *Sun,
				Controls->visualStyle, Controls->torchIntensity, Controls->maxIllumination);

	if (Emitters.size())
	{
		for (auto& e : Emitters)
			e->render(Eye, Screen, RM);
	}

	if (DynamicSurfaces.size())
		for (auto& s : DynamicSurfaces)
			s->renderMesh(Eye, RotationM, TranslationM, *Sun, Controls->visualStyle,
				Controls->torchIntensity, Controls->maxIllumination);

	if (StaticSurfaces.size())
		for (auto& t : StaticSurfaces)
		{
			t->renderGrid(Eye, Screen, RM);
			//t->renderMesh(Eye, RotationM, TranslationM, *Sun, Controls->visualStyle,
			//	Controls->torchIntensity, Controls->maxIllumination);
		}

	if (PointClouds.size())
		for (auto& p : PointClouds)
		{
			p->renderCloud(Eye, Screen, RM);
			//p->renderMesh(Eye, RotationM, TranslationM, *Sun, Controls->visualStyle,
			//	Controls->torchIntensity, Controls->maxIllumination);
		}


}


void Game::updateAll()
{
	this->calculateFrametime();

	Controls->HandleUserEvents();

	Screen->resetPixelBuffer();

	Screen->resetDepthBuffer();

	if (Controls->playerControlled)
		this->updateHeroPosition();
	else if (Controls->enemyControlled && Enemy != nullptr)
		this->updateEnemyPosition();

	this->updateCameraPosition(Hero);

	//Hero->Parts[Hero->Parts.size() - 1]->position.x += Controls->mx;

	if (!Controls->isPaused)
	{
		Sun->update();

		this->updateBalls();

		if (Controls->playerControlled)
		{
			this->updateEnemiesPositionsAI(Controls->purposeOfAI);
		}

		this->updateProjectiles();
		this->updateExplosions();

		if (Hero->lastShot < 5)
			Hero->lastShot++;
		if (Controls->isFiring)
		{
			if (Controls->playerControlled && Hero->lastShot >= 5)
			{
				Hero->shoot(Projectiles);
				ammo--;
			}
		}

		if (Enemy != nullptr)
		{
			if (Enemy->lastShot < 5)
				Enemy->lastShot++;
			if (Controls->playerControlled)
			{
				if (Enemy->isFiring && Enemy->lastShot >= 5)
				{
					Enemy->shoot(Projectiles);
					ammo--;
				}
			}
		}

		for (auto en : Enemies)
		{
			if (en->lastShot < 5)
				en->lastShot++;
			if (Controls->playerControlled)

			{
				if (en->isFiring && en->lastShot >= 5)
				{
					en->shoot(Projectiles);
					ammo--;
				}
			}
		}

		if (Controls->enemyControlled && Enemy != nullptr)
		{
			if (Controls->isFiring && Enemy->lastShot >= 5)
			{
				Enemy->shoot(Projectiles);
				ammo--;
			}
		}

		for (auto& e : Emitters)
		{
			e->update();
			e->setGravity(Controls->gravityOn);
		}

		for (auto& s : DynamicSurfaces)
			s->update();
	}

	Controls->ceaseMotion();

	this->renderAll();

	if (Controls->showHelp)
		TextScreens["Controls Text"]->print(Screen);


	this->displayStats(Controls->showCrosshair, Controls->showFPS, Controls->showPosition, Controls->showPolyN, Controls->showAmmo, Screen);

	this->resetPolyCounter();

	Screen->update();

	this->updateFrameCounter();
}


void Game::updateFrameCounter()
{
	frameCounter++;
}


void Game::resetPolyCounter()
{
	polyCounter = 0;
}


void Game::incrementPolyCounter()
{
	polyCounter++;
}


void Game::calculateFrametime()
{
	if (frameCounter == 30)
	{
		oldTime = newTime;
		newTime = clock();
		frameTime = newTime - oldTime;
		frameCounter = 0;
	}
}


void Game::displayStats(bool crosshair, bool fps, bool position, bool polyN, bool amm, std::shared_ptr<Canvas> Screen)
{
	int screenWChar = Screen->getWidth() / 8;
	int screenHChar = Screen->getHeight() / 8;

	float azmToShow, altToShow, rolToShow;
	if (crosshair && !Controls->showHelp)
		Screen->drawCrosshair(2, 6, 0x007f7f7f);
	if (position)
	{
		Screen->displayString("MAX ILLUMINATION", 30, 12, 0x00ffffff);
		Screen->displayValue(Controls->maxIllumination, 2, 2, 12, 0x00ffffff);
		Screen->displayString("TORCH INTENSITY", 30, 11, 0x00ffffff);
		Screen->displayValue(Controls->torchIntensity, 2, 2, 11, 0x00ffffff);

		Screen->displayString("POSITION X", 30, 9, 0x00ff7f7f);
		Screen->displayValue((float)Eye->x, 1, 2, 9, 0x00ff7f7f);
		Screen->displayString("POSITION Y", 30, 8, 0x007fff7f);
		Screen->displayValue((float)Eye->y, 1, 2, 8, 0x007fff7f);
		Screen->displayString("POSITION Z", 30, 7, 0x007f7fff);
		Screen->displayValue((float)Eye->z, 1, 2, 7, 0x007f7fff);

		azmToShow = Eye->azm * 180.0f / PI;
		if (azmToShow > 360.0f) { azmToShow -= 360.0f; }
		if (azmToShow < -360.0f) { azmToShow += 360.0f; }
		altToShow = Eye->alt * 180.0f / PI - 180.0f;
		if (altToShow > 360.0f) { altToShow -= 360.0f; }
		if (altToShow < -360.0f) { altToShow += 360.0f; }
		rolToShow = Eye->rol * 180.0f / PI;
		if (rolToShow > 360.0f) { rolToShow -= 360.0f; }
		if (rolToShow < -360.0f) { rolToShow += 360.0f; }

		Screen->displayString("ROTATION X", 30, 5, 0x00ff3f3f);
		Screen->displayValue(azmToShow, 1, 2, 5, 0x00ff3f3f);
		Screen->displayString("ROTATION Y", 30, 4, 0x003fff3f);
		Screen->displayValue(altToShow, 1, 2, 4, 0x003fff3f);
		Screen->displayString("ROTATION Z", 30, 3, 0x003f3fff);
		Screen->displayValue(rolToShow, 1, 2, 3, 0x003f3fff);
	}
	if (fps)
	{
		Screen->displayFps((float)(30000.0f / frameTime), 1, 2, 0);
	}
	if (polyN)
	{
		Screen->displayValue(polyCounter, 0, 2, 53, 0x00ff0000);
	}
	if (amm)
	{
		Screen->displayValue(ammo, 0, 100, 3, 0x007f7f00);
	}

	Screen->displayValue(Hero->health, 0, 90, 3, 0x000000ff);

	for (unsigned int i = 0; i < Enemies.size(); i++)
		Screen->displayValue(Enemies[i]->health, 0, 80, 3 + i, 0x00ffffff);

	switch (Controls->purposeOfAI)
	{
	case follow_player:
	{
		Screen->displayString("<FOLLOW PLAYER>", 60, 3, 0x000000ff);
		break;
	}
	case kill_player:
	{
		Screen->displayString("<KILL PLAYER>", 60, 3, 0x00ff0000);
		break;
	}
	case follow_others:
	{
		Screen->displayString("<FOLLOW BALLS>", 60, 3, 0x000000ff);
		break;
	}
	case kill_others:
	{
		Screen->displayString("<SHOOT BALLS>", 60, 3, 0x00ff0000);
		break;
	}
	case follow_each_other:
	{
		Screen->displayString("<FOLLOW EACH OTHER>", 60, 3, 0x000000ff);
		break;
	}
	case kill_each_other:
	{
		Screen->displayString("<SHOOT EACH OTHER>", 60, 3, 0x00ff0000);
		break;
	}
	}

	Screen->displayString("PRESS H for HELP", 30, screenHChar - 2, 0x007f7fff);
}