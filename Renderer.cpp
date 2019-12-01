#include "Renderer.h"
#include "ShadowVolume.h"
#include <stdio.h>


Renderer::Renderer(std::shared_ptr<Shapes> solids, std::shared_ptr<Shapes> actors, std::shared_ptr<LightSource> sun,
	std::shared_ptr<Camera> eye, std::shared_ptr<EventHandler> controls):
	Solids(solids), Actors(actors), Sun(sun), Eye(eye), Controls(controls)
{
	std::cout << "Renderer constructor called" << std::endl;

	hRatio = Eye->getHRatio();
	vRatio = Eye->getVRatio();

	solidN = Solids->getNEntities();
	actorN = Actors->getNEntities();

	std::shared_ptr<int[]> solidPolyCount(new int[Solids->getNEntities()]);
	Solids->getPolyCountEntities(solidPolyCount);

	std::shared_ptr<int[]> actorPolyCount(new int[Actors->getNEntities()]);
	Actors->getPolyCountEntities(actorPolyCount);

	solidMesh = new triangle3dV * [solidN];
	for (unsigned int i = 0; i < solidN; i++)
	{
		solidMesh[i] = new triangle3dV[solidPolyCount[i]];
	}
	Solids->getPolyData_(solidN, solidMesh);

	actorMesh = new triangle3dV * [actorN];
	for (unsigned int i = 0; i < actorN; i++)
	{
		actorMesh[i] = new triangle3dV[actorPolyCount[i]];
	}
	Actors->getPolyData_(actorN, actorMesh);

	triangleMesh = nullptr;
	shadowMesh = nullptr;

	nShadows = 0;
	vShadows = nullptr;

	sunVector = Sun->getVector();
}


Renderer::~Renderer()
{
	std::cout << "Renderer destructor called" << std::endl;

	solidN = Solids->getNEntities();
	actorN = Actors->getNEntities();

	for (unsigned int i = 0; i < solidN; i++)
	{
		delete[] solidMesh[i];
	}
	delete[] solidMesh;

	for (unsigned int i = 0; i < actorN; i++)
	{
		delete[] actorMesh[i];
	}
	delete[] actorMesh;
}


void Renderer::checkCameraCollision()
{
	int bodyCount = Solids->getNEntities();
	for (int i = 0; i < bodyCount; i++)
	{
		if (Solids->getBBState(i))
		{
			boundingBox tempBB = Solids->getBB(i);
			if ((Eye->x >= tempBB.minExt.x) &&
				(Eye->y >= tempBB.minExt.y) &&
				(Eye->z >= tempBB.minExt.z) &&
				(Eye->x <= tempBB.maxExt.x) &&
				(Eye->y <= tempBB.maxExt.y) &&
				(Eye->z <= tempBB.maxExt.z))
			{
				int nPoly = Solids->getPolyCount(i);
				for (int p = 0; p < nPoly; p++)
				{
					triangle3dV triangleChecked = solidMesh[i][p];
					vect3 pointChecked = { Eye->x, Eye->y, Eye->z, 1.0 };
					double dist = distPoint2Plane(pointChecked, triangleChecked);
					if (distPoint2Plane(pointChecked, triangleChecked) <= collisionDist)
					{
						printf(".");
					}
				}
			}
		}
	}
}


bool Renderer::checkCameraCollision(vect3 v, int* body, int* poly)
{
	bool collisionTakingPlace = false;
	int bodyCount = Solids->getNEntities();
	double minDist = collisionDist;
	int collidingBody = 0;
	int collidingPoly = 0;

	for (int i = 0; i < bodyCount; i++)
	{
		if (Solids->getBBState(i))
		{
			boundingBox tempBB = Solids->getBB(i);
			if ((Eye->x >= tempBB.minExt.x) &&
				(Eye->y >= tempBB.minExt.y) &&
				(Eye->z >= tempBB.minExt.z) &&
				(Eye->x <= tempBB.maxExt.x) &&
				(Eye->y <= tempBB.maxExt.y) &&
				(Eye->z <= tempBB.maxExt.z))
			{
				int nPoly = Solids->getPolyCount(i);
				for (int p = 0; p < nPoly; p++)
				{
					triangle3dV polyChecked = solidMesh[i][p];
					vect3 pointChecked = { Eye->x + v.x, Eye->y + v.y, Eye->z + v.z, 1.0 };
					double dist = distPoint2Plane(pointChecked, polyChecked);
					if (dist <= collisionDist)
					{
						collisionTakingPlace = true;
						if (dist < minDist) { minDist = dist; collidingBody = i; collidingPoly = p; }
					}
				}
			}
		}
	}

	if (collisionTakingPlace)
	{
		*body = collidingBody;
		*poly = collidingPoly;
	}

	return collisionTakingPlace;
}


bool Renderer::checkCameraCollision(vect3* v, int* body, int* poly)
{
	bool collisionTakingPlace = false;

	int bodyCount = Solids->getNEntities();
	double minDist = collisionDist;
	int collidingBody = 0;
	int collidingPoly = 0;

	for (int i = 0; i < bodyCount; i++)												//FOR EACH SOLID BODIES
	{
		if (Solids->getBBState(i))													//CHECK IF CURRENT BODY HAS ACTIVE BOUNDING BOX
		{
			boundingBox tempBB = Solids->getBB(i);
			if ((Eye->x >= tempBB.minExt.x) &&									//CHECK IF CAMERA IS INSIDE BOUNDING BOX
				(Eye->y >= tempBB.minExt.y) &&
				(Eye->z >= tempBB.minExt.z) &&
				(Eye->x <= tempBB.maxExt.x) &&
				(Eye->y <= tempBB.maxExt.y) &&
				(Eye->z <= tempBB.maxExt.z))
			{
				int nPoly = Solids->getPolyCount(i);								//POLY COUNT OF CURRENT BODY
				for (int p = 0; p < nPoly; p++)										//FOR EACH POLYGONS
				{
					triangle3dV triangleChecked = solidMesh[i][p];					//TRIANGLE TO CHECK WITH

					vect3 cameraCurr = { Eye->x, Eye->y, Eye->z, 1.0 };						//CAMERA POSITION - CURRENT FRAME
					vect3 cameraNext = { Eye->x + v->x, Eye->y + v->y , Eye->z + v->z , 1.0 };	//CAMERA POSITION - NEXT FRAME

					double dist = distPoint2Plane(cameraCurr, triangleChecked);	//DISTANCE FROM CURRENT POSITION TO POLYGON'S PLANE

					double sCurr = dotProduct(subVectors(triangleChecked.A, cameraCurr), triangleChecked.N);
					double sNext = dotProduct(subVectors(triangleChecked.A, cameraNext), triangleChecked.N);

					if (sign(sCurr) != sign(sNext))	//IF CAMERA PATH INTERSECTS POLYGON'S PLANE IN BETWEEN THE CURRENT AND NEXT FRAME
					{
						double t = (dotProduct(subVectors(triangleChecked.A, cameraCurr), triangleChecked.N)) /
							(dotProduct(subVectors(cameraNext, cameraCurr), triangleChecked.N));
						vect3 intersection;

						vect3 cameraVector = subVectors(cameraNext, cameraCurr);

						intersection.x = cameraCurr.x + t * cameraVector.x;
						intersection.y = cameraCurr.y + t * cameraVector.y;
						intersection.z = cameraCurr.z + t * cameraVector.z;
						intersection.w = 1.0;		//INTERSECTION POINT

						double sA = dotProduct(subVectors(triangleChecked.A, intersection),
							subVectors(triangleChecked.A, triangleChecked.C));
						double sB = dotProduct(subVectors(triangleChecked.B, intersection),
							subVectors(triangleChecked.B, triangleChecked.A));
						double sC = dotProduct(subVectors(triangleChecked.C, intersection),
							subVectors(triangleChecked.C, triangleChecked.B));

						if (sA >= 0.0f && sB >= 0.0f && sC >= 0.0f)
						{
							collisionTakingPlace = true;
							collidingBody = i;
							collidingPoly = p;
						}
					}
				}
			}
		}
	}

	if (collisionTakingPlace)
	{
		*body = collidingBody;
		*poly = collidingPoly;
	}

	return collisionTakingPlace;
}

/*
bool Renderer::checkCameraCollision(vect3* v, int* body, int* poly)
{
	bool collisionTakingPlace = false;

	int bodyCount		= Solids->getNEntities();
	int collidingBody	= 0;
	int collidingPoly	= 0;

	for (int i = 0; i < bodyCount; i++)												//FOR EACH SOLID BODIES
	{
		if (Solids->getBBState(i))													//CHECK IF CURRENT BODY HAS ACTIVE BOUNDING BOX
		{
			boundingBox tempBB = Solids->getBB(i);
			if ((Eye->x >= tempBB.minExt.x) &&									//CHECK IF CAMERA IS INSIDE BOUNDING BOX
				(Eye->y >= tempBB.minExt.y) &&
				(Eye->z >= tempBB.minExt.z) &&
				(Eye->x <= tempBB.maxExt.x) &&
				(Eye->y <= tempBB.maxExt.y) &&
				(Eye->z <= tempBB.maxExt.z))
			{
				int nPoly = Solids->getPolyCount(i);								//POLY COUNT OF CURRENT BODY
				for (int p = 0; p < nPoly; p++)										//FOR EACH POLYGONS
				{
					triangle3dV polyChecked = solidMesh[i][p];					//TRIANGLE TO CHECK WITH
					vect3 cameraCurr = { Eye->x, Eye->y, Eye->z, 1.0 };						//CAMERA POSITION IN THE CURRENT FRAME
					vect3 cameraPlus1 = { Eye->x + v->x, Eye->y + v->y , Eye->z + v->z , 1.0 };//CAMERA POSITION IN THE NEXT FRAME
					vect3 cameraPlus2 = { Eye->x + 2 * v->x, Eye->y + 2 * v->y, Eye->z + 2 * v->z, 1.0 };

					double dCurr = distPoint2Plane(cameraCurr, polyChecked);
					double dPlus1 = distPoint2Plane(cameraPlus1, polyChecked);
					double dPlus2 = distPoint2Plane(cameraPlus2, polyChecked);

					double sCurr	= dotProduct(subVectors(polyChecked.A, cameraCurr), polyChecked.N);
					double sPlus1	= dotProduct(subVectors(polyChecked.A, cameraPlus1), polyChecked.N);
					double sPlus2	= dotProduct(subVectors(polyChecked.A, cameraPlus2), polyChecked.N);

					if (sign(sPlus1) != sign(sPlus2))
					{
						double t = (dotProduct(subVectors(polyChecked.A, cameraPlus1), polyChecked.N)) /
							(dotProduct(subVectors(cameraPlus2, cameraPlus1), polyChecked.N));
						vect3 intersection;

						vect3 cameraVector = subVectors(cameraPlus2, cameraPlus1);

						intersection.x = cameraPlus1.x + t * cameraVector.x;
						intersection.y = cameraPlus1.y + t * cameraVector.y;
						intersection.z = cameraPlus1.z + t * cameraVector.z;
						intersection.w = 1.0;		//INTERSECTION POINT

						double sA = dotProduct(subVectors(polyChecked.A, intersection),
							subVectors(polyChecked.A, polyChecked.C));
						double sB = dotProduct(subVectors(polyChecked.B, intersection),
							subVectors(polyChecked.B, polyChecked.A));
						double sC = dotProduct(subVectors(polyChecked.C, intersection),
							subVectors(polyChecked.C, polyChecked.B));

						if (sA >= 0.0f && sB >= 0.0f && sC >= 0.0f)
						{
							double dColl = collisionDist;
							double sColl = (dCurr - dColl) / (dCurr - dPlus1);
							v->x *= sColl; v->y *= sColl; v->z *= sColl;	//VELOCITY REDUCED SO CAMERA LANDS ON COLLISION ZONE BOUNDARY NEXT
							collisionTakingPlace = true;
							collidingBody = i;
							collidingPoly = p;
						}
					}
				}
			}
		}
	}
	if (collisionTakingPlace)
	{
		*body = collidingBody;
		*poly = collidingPoly;
	}
	return collisionTakingPlace;
}
*/


void Renderer::updateCameraPosition()
{
	Eye->azm = -Controls->turnH;
	Eye->alt = -Controls->turnV;
	Eye->rol = Controls->tiltP;

	Eye->x -= Controls->moveP * cos(Eye->azm) - Controls->strafeP * cos(Eye->azm + PI * 0.5);
	Eye->y += Controls->moveP * sin(Eye->azm) - Controls->strafeP * sin(Eye->azm + PI * 0.5);
	Eye->z += Controls->riseP;
}


void Renderer::updateCameraPosition(double turnH, double turnV, double tiltP, double moveP, double strafeP, double riseP, bool collisionCheck)
{
	Eye->azm = -turnH;
	Eye->alt = -turnV;
	Eye->rol = tiltP;

	vect3 velocity;
	velocity.x = -(moveP * cos(Eye->azm) - strafeP * cos(Eye->azm + PI * 0.5));
	velocity.y = (moveP * sin(Eye->azm) - strafeP * sin(Eye->azm + PI * 0.5));
	velocity.z = riseP;
	velocity.w = 1.0;

	vect3 counter = { 0.0, 0.0, 0.0, 1.0 };

	if (collisionCheck)
	{
		double dist = collisionDist;
		int bodyNo = 0;
		int polyNo = 0;
		if (this->checkCameraCollision(velocity, &bodyNo, &polyNo))
		{
			//printf("Colliding with body: %d poly: %d at %.2f", bodyNo, polyNo, dist);
			vect3 collisionNormal = solidMesh[bodyNo][polyNo].N;
			double vMagnitude = dotProduct(collisionNormal, velocity);
			if (vMagnitude < 0.0f)
			{
				counter.x = -collisionNormal.x * vMagnitude;
				counter.y = -collisionNormal.y * vMagnitude;
				counter.z = -collisionNormal.z * vMagnitude;
				counter.w = 1.0;
			}
		}
	}

	Eye->x += (velocity.x + counter.x);
	Eye->y += (velocity.y + counter.y);
	Eye->z += (velocity.z + counter.z);
}


bool Renderer::polyFacingLightsource(vect3 v, triangle3dV P)
{
	return (dotProduct(P.N, v) < 0) ? true : false;
}


void Renderer::updateShadowVolumes(model E)
{
	int bodyCount = 0;
	int* polyCount = nullptr;

	nShadows = 0;

	vect3 sunInView;

	transform3d T = Eye->getTransformation();
	sunInView = rotZrad(T.sinRol, T.cosRol, rotXrad(T.sinAlt, T.cosAlt, rotZrad(T.sinAzm, T.cosAzm, sunVector)));
	//sunInView = sunVector;

	if (vShadows != nullptr)
	{
		delete[] vShadows;	//printf("-\n");
		vShadows = nullptr;
	}
	   
	switch (E)
	{
	case solid:
	{
		shadowMesh = solidMesh;
		polyCount = Solids->getPolyCountEntities();
		Entities = Solids;
	}
	break;
	case actor:
	{
		shadowMesh = actorMesh;
		polyCount = Actors->getPolyCountEntities();
		Entities = Actors;
	}
	break;
	default:
	{
		shadowMesh = solidMesh;
		polyCount = Solids->getPolyCountEntities();
		Entities = Solids;
	}
	break;
	}

	bodyCount = Entities->getNEntities();

	for (int j = 0; j < bodyCount; j++)
		if (polyCount != nullptr)
			for (int i = 0; i < polyCount[j]; i++)
			{
				triangle3dV tempPoly = Eye->world2viewT(T, shadowMesh[j][i]);
				//triangle3dV tempPoly = shadowMesh[j][i];
				if (Entities->assertShadowCasting(j) && this->polyFacingLightsource(sunInView, tempPoly))
					nShadows++;
			}
	vShadows = new ShadowVolume[nShadows];	//printf("+\n");
	int shCount = 0;
	for (int j = 0; j < bodyCount; j++)
	{
		if (polyCount != nullptr)
		{
			for (int i = 0; i < polyCount[j]; i++)
			{
				triangle3dV tempPoly = Eye->world2viewT(T, shadowMesh[j][i]);
				//triangle3dV tempPoly = shadowMesh[j][i];
				if (Entities->assertShadowCasting(j) && this->polyFacingLightsource(sunInView, tempPoly))
				{
					if (shCount < nShadows)
					{
						plane tempPlane;

						tempPlane.P = tempPoly.A;
						tempPlane.N = crossProduct(unitVector(subVectors(tempPoly.B, tempPoly.A)), sunInView);
						vShadows[shCount].setPlaneA(tempPlane);

						tempPlane.P = tempPoly.B;
						tempPlane.N = crossProduct(unitVector(subVectors(tempPoly.C, tempPoly.B)), sunInView);
						vShadows[shCount].setPlaneB(tempPlane);

						tempPlane.P = tempPoly.C;
						tempPlane.N = crossProduct(unitVector(subVectors(tempPoly.A, tempPoly.C)), sunInView);
						vShadows[shCount].setPlaneC(tempPlane);

						tempPlane.P = tempPoly.A;
						tempPlane.N = tempPoly.N;
						vShadows[shCount].setPlaneC(tempPlane);
						//printf("Shadow volume no. %d calculated\n", shCount);
						shCount++;
					}
				}
			}
		}
	}

	//printf("%d\t\t%.2f\t%.2f\t%.2f\n", nShadows, sunVector.x, sunVector.y, sunVector.z);

}


void Renderer::updateBoundingBoxes(model E, double padding)
{
	int* polyCount = nullptr;

	switch (E)
	{
	case solid:
		Entities = Solids;
		polyCount = Solids->getPolyCountEntities();
		triangleMesh = solidMesh;
		break;
	case actor:
		Entities = Actors;
		polyCount = Actors->getPolyCountEntities();
		triangleMesh = actorMesh;
		break;
	default:
		Entities = Solids;
		polyCount = Solids->getPolyCountEntities();
		triangleMesh = solidMesh;
		break;
	}

	int bodyCount = Entities->getNEntities();

	for (int i = 0; i < bodyCount; i++)
	{
		int nPoly = polyCount[i];
		vect3 minExt = getXYZmin(nPoly, triangleMesh[i], padding);
		vect3 maxExt = getXYZmax(nPoly, triangleMesh[i], padding);
		Entities->updateBBox(i, minExt, maxExt);
	}
}


void Renderer::updateEntities(model E)
{
	switch (E)
	{
	case solid:
		Entities = Solids;
		triangleMesh = solidMesh;
		break;
	case actor:
		Entities = Actors;
		triangleMesh = actorMesh;
		break;
	default:
		Entities = Solids;
		triangleMesh = solidMesh;
		break;
	}

	if (Entities != nullptr)
	{
		int* polyCount = Entities->getPolyCountEntities();

		for (unsigned int i = 0; i < Entities->getNEntities(); i++)				//For every entity
		{
			vect3 velocity = Entities->getVelocity(i);
			vect3 angVelocity = Entities->getAngularVelocity(i);
			transformMesh(polyCount[i], triangleMesh[i], 1.0, 1.0, 1.0, velocity.x, velocity.y, velocity.z,
				angVelocity.x, angVelocity.y, angVelocity.z);
		}
	}
}


void Renderer::renderPoints(int nPoints, point3 * Points, Uint32 * pixelBuffer)
{
	transform3d eyePosition = Eye->getTransformation();
	for (int i = 0; i < nPoints; i++)
	{
		point3 viewP = Eye->world2viewP(eyePosition, Points[i]);
		if (Eye->insideFrustum(viewP)) { Eye->projectPoint(viewP, pixelBuffer, hRatio, vRatio); }
	}
}


void Renderer::renderEntities(model E, Uint32 * pixelBuffer, double* depthBuffer)
{
	switch (E)
	{
	case solid:
		Entities = Solids;
		triangleMesh = solidMesh;
		break;
	case actor:
		Entities = Actors;
		triangleMesh = actorMesh;
		break;
	default:
		Entities = Solids;
		triangleMesh = solidMesh;
		break;
	}

	if(Entities->getNEntities())
	{
		std::shared_ptr<int[]> polyCount(new int[Entities->getNEntities()]);
		Entities->getPolyCountEntities(polyCount);

		transform3d eyePosition = Eye->getTransformation();

		for (unsigned int i = 0; i < Entities->getNEntities(); i++)				//For every entity
		{
			int totalPoly = polyCount[i];
			for (int k = 0; k < totalPoly; k++)
			{
				if (Eye->polyFacingCamera(triangleMesh[i][k]))		//Backface culling is performed here
				{
					Uint32 colour = triangleMesh[i][k].colour;

					triangle3dV viewT = Eye->world2viewT(eyePosition, triangleMesh[i][k]);					

					Eye->illuminatePoly(*Sun, &viewT, triangleMesh[i][k], Controls->visualStyle);

					int nVert = Eye->clipToFrustum(viewT, Entities->vertexList, Entities->uvList);

					int textureID = triangleMesh[i][k].texture;

					Eye->currentTexture = Entities->getTextureData(textureID);

					Eye->projectPoly(nVert, Entities->vertexList, Entities->uvList, colour, pixelBuffer, depthBuffer, nShadows, vShadows,
						hRatio, vRatio, Controls->visualStyle, Controls->torchIntensity, Controls->maxIllumination, viewT);

					this->incrementPolyCounter();
				}
			}
		}
	}
}


void Renderer::displayStats(bool crosshair, bool fps, bool position, bool polyN, std::shared_ptr<Canvas> Screen)
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


void Renderer::updateFrameCounter()
{
	frameCounter++;
}


void Renderer::resetPolyCounter()
{
	polyCounter = 0;
}


void Renderer::incrementPolyCounter()
{
	polyCounter++;
}


void Renderer::calculateFrametime()
{
	if (frameCounter == 30)
	{
		oldTime = newTime;
		newTime = clock();
		frameTime = newTime - oldTime;
		frameCounter = 0;
	}
}


void Renderer::exportMesh(const std::string& fileName)
{
	std::ofstream exportFile(fileName, std::ios_base::binary);

	if (exportFile.is_open())
	{
		exportFile.write((char*) &solidN, sizeof(solidN));

		unsigned int tempCount = 0;
		std::shared_ptr<int[]> solidPolyCount(new int[solidN]);
		Solids->getPolyCountEntities(solidPolyCount);

		for (unsigned int j = 0; j < solidN; j++)
		{
			int nCurrent = solidPolyCount[j];

			exportFile.write((char*) &nCurrent, sizeof(int));

			for (int i = 0; i < nCurrent; i++)
			{
				triangle3dV temp = solidMesh[j][i];

				temp.colour = getColour(0, 127, 127, 255);
				temp.texture = 0;

				exportFile.write((char*)& temp, sizeof(triangle3dV));
			}
		}

		std::cout << "Model file saved..." << std::endl;
	}
	else
	{
		std::cout << "Could not write model file " << fileName << std::endl;
	}

}
