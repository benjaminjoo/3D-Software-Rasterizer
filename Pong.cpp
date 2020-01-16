#include "Pong.h"



Pong::Pong(std::shared_ptr<Canvas> screen, std::shared_ptr<Camera> eye, std::shared_ptr<Projection> renderer, std::shared_ptr<EventHandler> controls,
			std::shared_ptr<LightSource> sun, std::shared_ptr<Player> hero, std::shared_ptr<Player> enemy):
	Screen(screen), Eye(eye), Renderer(renderer), Controls(controls), Sun(sun), Hero(hero), Enemy(enemy)
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


void Pong::addEntity(std::shared_ptr<SolidBody> entity)
{
	Entities.push_back(entity);
}


void Pong::addBall(std::shared_ptr<SolidBody> ball)
{
	Balls.push_back(ball);
}


void Pong::addEnemy(std::shared_ptr<Player> enemy)
{
	Enemies.push_back(enemy);
}


void Pong::addTextScreen(std::string tname, std::shared_ptr<Text> tscreen)
{
	TextScreens.insert({ tname, tscreen });
}


void Pong::addEmitter(std::shared_ptr<ParticleSystem> em)
{
	Emitter = em;
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
		Renderer->transformMesh(nCurrent, triangleMesh[i], Entities[i]->scale.x, Entities[i]->scale.y, Entities[i]->scale.z,
													Entities[i]->position.x, Entities[i]->position.y, Entities[i]->position.z,
													Entities[i]->rotation.x, Entities[i]->rotation.y, Entities[i]->rotation.z);
	}

	if (Hero != nullptr)
	{
		size_t nPlayerParts = Hero->Parts.size();
		playerMesh = new triangle3dV * [nPlayerParts];
		playerPolyCount = new unsigned int[nPlayerParts];
		for (unsigned int i = 0; i < nPlayerParts; i++)
		{
			std::shared_ptr<SolidBody> B = Hero->Parts[i];
			unsigned int nCurrent = B->getTotalPoly();
			playerMesh[i] = new triangle3dV[nCurrent];
			playerPolyCount[i] = nCurrent;
			B->getTriangleData_(playerMesh[i]);

			Renderer->transformMesh(nCurrent, playerMesh[i], B->scale.x, B->scale.y, B->scale.z,
														B->position.x, B->position.y, B->position.z,
														B->rotation.x, B->rotation.y, B->rotation.z);
		}
	}

	if (Enemy != nullptr)
	{
		size_t nEnemyParts = Enemy->Parts.size();
		enemyMesh = new triangle3dV * [nEnemyParts];
		enemyPolyCount = new unsigned int[nEnemyParts];
		for (unsigned int i = 0; i < nEnemyParts; i++)
		{
			std::shared_ptr<SolidBody> B = Enemy->Parts[i];
			unsigned int nCurrent = B->getTotalPoly();
			enemyMesh[i] = new triangle3dV[nCurrent];
			enemyPolyCount[i] = nCurrent;
			B->getTriangleData_(enemyMesh[i]);

			Renderer->transformMesh(nCurrent, enemyMesh[i], B->scale.x, B->scale.y, B->scale.z,
													B->position.x, B->position.y, B->position.z,
													B->rotation.x, B->rotation.y, B->rotation.z);
		}
	
		if (Enemy->skeleton != nullptr)
		{
			int nSkeletonMesh = Enemy->skeleton->getNMesh();
	
			skeletonMesh = new triangle3dV* [nSkeletonMesh];
			skeletonPolyCount = new unsigned int[nSkeletonMesh];
	
			Enemy->skeleton->getPoly(skeletonPolyCount, skeletonMesh);
		}
	}

	size_t nEnemies = Enemies.size();										//Total number of enemies
	enemiesPartCount = new unsigned int[nEnemies];							//Number of parts per individual enemy [enemyIndex]
	enemiesPolyCount = new unsigned int* [nEnemies];						//Number of polygons per individual part [enemyIndex][partIndex]
	enemiesMeshIndices = new unsigned int* [nEnemies];						//Indexes into enemiesMesh [partIndex][meshIndex]
	size_t nAllEnemyParts = 0;												//Total number of parts
	unsigned int partIndex = 0;
	for (unsigned int i = 0; i < nEnemies; i++)								//For each enemy
	{
		size_t nParts = Enemies[i]->Parts.size();							//Number of parts in current enemy
		nAllEnemyParts += nParts;											//Number of parts added to total
		enemiesPartCount[i] = nParts;										//Number of parts written into enemiesPartCount[enemyIndex]
		enemiesPolyCount[i] = new unsigned int[nParts];						//Storage allocated for enemiesPolyCount[][]
		enemiesMeshIndices[i] = new unsigned int[nParts];					//Storage allocated for enemiesMeshIndices[][]
		for (unsigned int j = 0; j < nParts; j++)							//For each part in current enemy
		{
			size_t nPoly = Enemies[i]->Parts[j]->getTotalPoly();			//Number of polygons in current part
			enemiesPolyCount[i][j] = nPoly;									//Number written into enemiesPolyCount[enemyIndex][partIndex]
			enemiesMeshIndices[i][j] = partIndex;							//partIndex written into enemiesMeshIndices[enemyIndex][partIndex]
			partIndex++;													//Increment partIndex
		}
	}
	enemiesMesh = new triangle3dV* [nAllEnemyParts];						//Polygon storage for all parts [partIndex][polyIndex]
	partIndex = 0;															//Set partIndex to 0
	for (unsigned int i = 0; i < nEnemies; i++)								//For each enemy
	{
		for (unsigned int j = 0; j < Enemies[i]->Parts.size(); j++)			//For each part in current enemy
		{
			std::shared_ptr<SolidBody> B = Enemies[i]->Parts[j];
			int nCurrentPoly = B->getTotalPoly();							//Number of polygons in current part
			enemiesMesh[partIndex] = new triangle3dV[nCurrentPoly];			//Storage allocated for nCurrentPoly number of polygons
			B->getTriangleData_(enemiesMesh[partIndex]);					//enemiesMesh[partIndex][] filled up with polygon data

			Renderer->transformMesh(nCurrentPoly, enemiesMesh[partIndex], B->scale.x, B->scale.y, B->scale.z,
																		B->position.x, B->position.y, B->position.z,
																		B->rotation.x, B->rotation.y, B->rotation.z);

			partIndex++;													//Increment partIndex
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

	delete enemiesPartCount;
	size_t nEnemies = Enemies.size();
	size_t nAllEnemyParts = 0;
	for (unsigned int i = 0; i < nEnemies; i++)
	{
		delete enemiesPolyCount[i];
		delete enemiesMeshIndices[i];
		nAllEnemyParts += Enemies[i]->Parts.size();
	}
	delete[] enemiesPolyCount;
	delete[] enemiesMeshIndices;
	for (unsigned int i = 0; i < nAllEnemyParts; i++)
	{
		delete enemiesMesh[i];
	}	
	delete[] enemiesMesh;

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


void Pong::updateHeroPosition()
{
	Hero->azm = -Controls->turnH;
	Hero->alt = -Controls->turnV;
	Hero->rol = Controls->tiltP;

	Hero->x -= Controls->moveP * cos(Hero->azm) - Controls->strafeP * cos(Hero->azm + PI * 0.5);
	Hero->y += Controls->moveP * sin(Hero->azm) - Controls->strafeP * sin(Hero->azm + PI * 0.5);
	Hero->z += Controls->riseP;

	//if (!Controls->isFiring)
	//	Hero->idle();

	updatePlayerModel(Hero);
}


void Pong::updateEnemyPosition()
{
	Enemy->azm = -Controls->turnH;
	Enemy->alt = -Controls->turnV;
	Enemy->rol = Controls->tiltP;

	Enemy->x -= Controls->moveP * cos(Enemy->azm) - Controls->strafeP * cos(Enemy->azm + PI * 0.5);
	Enemy->y += Controls->moveP * sin(Enemy->azm) - Controls->strafeP * sin(Enemy->azm + PI * 0.5);
	Enemy->z += Controls->riseP;

	updatePlayerModel(Enemy);
}


void Pong::updateEnemyPositionAI(aiGoal goal)
{
	switch (goal)
	{
		case be_idle:
		{
			Enemy->isFiring = false;
			Enemy->idle();
			break;
		}
		case follow_player:
		{
			Enemy->lockOnTarget(vect3{ Hero->x, Hero->y, Hero->z, 1.0f });
			Enemy->idle();
			break;
		}
		case kill_player:
		{
			if (Enemy->lockOnTarget(vect3{ Hero->x, Hero->y, Hero->z, 1.0f }))
				Enemy->isFiring = true;
			else
			{
				Enemy->isFiring = false;
				Enemy->idle();
			}				
			break;
		}
		case follow_others:
		{
			Enemy->isFiring = false;
			unsigned int targetIndex = Enemy->pickTarget(Balls);
			if (targetIndex < Balls.size())
			{
				Enemy->lockOnTarget(Balls[targetIndex]->getPosition());
				Enemy->idle();
			}			
			else
				Enemy->idle();
			break;
		}
		case kill_others:
		{
			unsigned int targetIndex = Enemy->pickTarget(Balls);
			
			if (targetIndex < Balls.size())
			{
				if (Enemy->lockOnTarget(Balls[targetIndex]->getPosition()))
					Enemy->isFiring = true;
				else
				{
					Enemy->isFiring = false;
					Enemy->idle();
				}
			}
			else
			{
				Enemy->isFiring = false;
				Enemy->idle();
			}					
			break;
		}
		default:
		{
			Enemy->isFiring = false;
			Enemy->idle();
			break;
		}
	}

	updatePlayerModel(Enemy);
}


void Pong::updateEnemiesPositionsAI(aiGoal goal)
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


void Pong::updatePlayerModel(std::shared_ptr<Player> character)
{
	for (unsigned int i = 0; i < character->Parts.size(); i++)
	{
		character->Parts[i]->setPosition({ character->x, character->y, character->z, 1.0f });
		character->Parts[i]->setRotation({ -character->rol, character->alt, -character->azm, 1.0f });
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
		if (Projectiles[i]->isFired())
		{
			if (Projectiles[i]->isInMotion())
			{
				vect3 currentPos = Projectiles[i]->getPosition();

				updateMovingObject(Projectiles[i], Projectiles[i]->getTotalPoly(), projectileMesh[i]);
				hitTest(Projectiles[i], Balls);
				hitTest(Projectiles[i], Hero);
				hitTest(Projectiles[i], Enemy);
				for (unsigned int p = 0; p < Enemies.size(); p++)
					hitTest(Projectiles[i], Enemies[p]);
			}

			Projectiles[i]->incrementTicksSinceFired();

			if (Projectiles[i]->getTicksSinceFired() > 1000)
			{
				Projectiles[i]->setFired(false);
				Projectiles[i]->setTicksSinceFired(0);
				Projectiles[i]->setPosition(vect3{ 0.0f, 0.0f, 0.0f, 1.0f });
				Projectiles[i]->setVelocity(vect3{ 0.0f, 0.0f, 0.0f, 1.0f });
				Projectiles[i]->setVisibility(false);
				Projectiles[i]->setMotion(false);
				Projectiles[i]->getTriangleData_(projectileMesh[i]);
				ammo++;
			}
		}
	}
}


bool Pong::hitTest(const std::shared_ptr<SolidBody>& bullet, std::shared_ptr<Player> actor)
{
	vect3 displacement = bullet->getVelocity();
	vect3 oldPos = bullet->getPosition();
	vect3 newPos = oldPos + displacement;

	if (actor != nullptr && !actor->isDestroyed())
	{
		double targetRadius = actor->getBBRadius();
		vect3 targetPosition = actor->getPosition();
		vect3 currentV = newPos - oldPos;
		double sOld = (oldPos - targetPosition) * currentV;
		double sNew = (newPos - targetPosition) * currentV;
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
		for (unsigned int j = 0; j < polyCount[i]; j++)
		{
			triangle3dV tempWall = triangleMesh[i][j];

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

		Renderer->movePoly(displacement, mesh[i]);
	}
}


void Pong::renderAll()
{
	polyCounter = 0;

	mat4x4 RotationM = Eye->getRotation();
	mat4x4 TranslationM = Eye->getTranslation();

	size_t nEntities = Entities.size();
	for (unsigned int i = 0; i < nEntities; i++)
		if (Entities[i]->isVisible())
			Eye->renderMesh(polyCount[i], triangleMesh[i], RotationM, TranslationM,
				*Sun, Controls->visualStyle, Controls->torchIntensity, Controls->maxIllumination);

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

				Eye->renderMesh(playerPolyCount[i], playerMesh[i], RotationM, TranslationM,
					sc, mv, rt, *Sun, Controls->visualStyle, Controls->torchIntensity, Controls->maxIllumination);
			}			
		}
	}

	size_t nEnemies = Enemies.size();
	for (unsigned int i = 0; i < nEnemies; i++)
	{
		if (!Enemies[i]->isDestroyed())
		{
			size_t nEnemyParts = Enemies[i]->Parts.size();
			for (unsigned int j = 0; j < nEnemyParts; j++)
			{
				if (Enemies[i]->Parts[j]->isVisible())
				{
					vect3 sc = Enemies[i]->Parts[j]->getScale();
					vect3 mv = Enemies[i]->Parts[j]->getPosition();
					vect3 rt = Enemies[i]->Parts[j]->getRotation();

					unsigned int partIndex = enemiesMeshIndices[i][j];
					unsigned int nPoly = enemiesPolyCount[i][j];

					Eye->renderMesh(enemiesPolyCount[i][j], enemiesMesh[partIndex], RotationM, TranslationM,
						sc, mv, rt, *Sun, Controls->visualStyle, Controls->torchIntensity, Controls->maxIllumination);
				}
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

				Eye->renderMesh(enemyPolyCount[i], enemyMesh[i], RotationM, TranslationM,
					sc, mv, rt, *Sun, Controls->visualStyle, Controls->torchIntensity, Controls->maxIllumination);
			}			
		}

		//if (Enemy->skeleton != nullptr)
		//{
		//	int nSkeletonMesh = Enemy->skeleton->getNMesh();
		//
		//	vect3* sc = new vect3[nSkeletonMesh];
		//	vect3* mv = new vect3[nSkeletonMesh];
		//	vect3* rt = new vect3[nSkeletonMesh];
		//
		//	Enemy->skeleton->getBonePosition(sc, mv, rt);
		//
		//	for (int i = 0; i < nSkeletonMesh; i++)
		//		Eye->renderMesh(skeletonPolyCount[i], skeletonMesh[i], T, sc[i], mv[i], rt[i], *Sun, Controls->visualStyle, Controls->torchIntensity, Controls->maxIllumination);
		//
		//	delete[] sc;
		//	delete[] mv;
		//	delete[] rt;
		//}
	}

	size_t nProjectiles = Projectiles.size();
	for (unsigned int i = 0; i < nProjectiles; i++)
	{
		if (Projectiles[i]->isVisible())
		{
			vect3 sc = Projectiles[i]->getScale();
			vect3 mv = Projectiles[i]->getPosition();
			vect3 rt = Projectiles[i]->getRotation();

			Eye->renderMesh(projectilePolyCount[i], projectileMesh[i], RotationM, TranslationM,
				sc, mv, rt, *Sun, Controls->visualStyle, Controls->torchIntensity, Controls->maxIllumination);

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

			Eye->renderMesh(ballPolyCount[i], ballMesh[i], RotationM, TranslationM,
				sc, mv, rt, *Sun, Controls->visualStyle, Controls->torchIntensity, Controls->maxIllumination);

			if(Balls[i]->isDestroyed())
				Eye->renderMesh(explosionPolyCount[i], explosionMesh[i], RotationM, TranslationM,
					sc, mv, rt, *Sun, Controls->visualStyle, Controls->torchIntensity, Controls->maxIllumination);
		}
			
	}

	if (Emitter != nullptr)
		Emitter->render(Eye, Screen);
}


void Pong::updateAll()
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

	if (!Controls->isPaused)
	{
		this->updateBalls();

		if (Controls->playerControlled)
		{
			this->updateEnemiesPositionsAI(Controls->purposeOfAI);
		}

		this->updateProjectiles();

		if (Hero->lastShot < 5)
			Hero->lastShot++;
		if (Controls->isFiring)
		{
			if (Controls->playerControlled && Hero->lastShot >= 5)
			{
				Hero->shoot(Projectiles, projectilePolyCount, projectileMesh);
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
					Enemy->shoot(Projectiles, projectilePolyCount, projectileMesh);
					ammo--;
				}
			}
		}

		for (unsigned int i = 0; i < Enemies.size(); i++)
		{
			if (Enemies[i]->lastShot < 5)
				Enemies[i]->lastShot++;
			if (Controls->playerControlled)

			{
				if (Enemies[i]->isFiring && Enemies[i]->lastShot >= 5)
				{
					Enemies[i]->shoot(Projectiles, projectilePolyCount, projectileMesh);
					ammo--;
				}
			}
		}

		if (Controls->enemyControlled && Enemy != nullptr)
		{
			if (Controls->isFiring && Enemy->lastShot >= 5)
			{
				Enemy->shoot(Projectiles, projectilePolyCount, projectileMesh);
				ammo--;
			}
		}

		if (Emitter != nullptr)
		{
			Emitter->update();
			Emitter->setGravity(Controls->gravityOn);
		}
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
	int screenWChar = Screen->getWidth() / 8;
	int screenHChar = Screen->getHeight() / 8;

	double azmToShow, altToShow, rolToShow;
	if (crosshair && !Controls->showHelp)
		Screen->drawCrosshair(2, 6, 0x007f7f7f);
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



