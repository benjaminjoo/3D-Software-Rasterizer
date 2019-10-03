#include <SDL.h>
#include <stdio.h>
#include <time.h>
#include "Definitions.h"
#include "Sphere.h"
#include "Prism.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Cube.h"
#include "Model.h"
#include "Camera.h"
#include "Canvas.h"
#include "LightSource.h"
#include "ViewVolume.h"
#include "World.h"



int main(int argc, char* argv[])
{
	World level01;

	/*level01.lamps[0] = { LightSource(-75.0, 30.0, 0.95) };*/

	LightSource lights[] = {	LightSource(-75.0, 30.0, 0.95) };


	int nEntities = 0;
	int* polyCountEntities = new int[MAXENTITIES];
	Uint32* colourEntities = new Uint32[MAXENTITIES];
	bool* gouraudEntities = new bool[MAXENTITIES];
	int currentEntity = 0;

	
	Sphere balls[] = {	Sphere(0.0,  0.0,  40.0, 15, 36, true, getColour(0, 0, 0, 255)),
						Sphere(20.0, 10.0, -20.0, 10, 36, true, getColour(0, 127, 127, 127)),
						Sphere(40.0, 20.0, -10.0, 5, 36, false, getColour(0, 127, 127, 0)),
						Sphere(60.0, 30.0, 25.0, 5, 16, true, getColour(0, 235, 25, 140)),
						Sphere(80.0, 40.0,  0.0, 2.5, 12, true, getColour(0, 255, 0, 0)) };

	//level01.spheres[0] = Sphere(0.0, 0.0, 40.0, 15, 36, true, getColour(0, 0, 0, 255));
	//level01.spheres[1] = Sphere(20.0, 10.0, -20.0, 10, 36, true, getColour(0, 127, 127, 127));
	//level01.spheres[2] = Sphere(40.0, 20.0, -10.0, 5, 36, false, getColour(0, 127, 127, 0));
	//level01.spheres[3] = Sphere(60.0, 30.0, 25.0, 5, 16, true, getColour(0, 235, 25, 140));
	//level01.spheres[4] = Sphere(80.0, 40.0, 0.0, 2.5, 12, true, getColour(0, 255, 0, 0));


	int nSphere = sizeof(balls) / sizeof(Sphere);
	nEntities += nSphere;
	for (int i = 0; i < nSphere; i++)
	{
		if (currentEntity < MAXENTITIES)
		{
			polyCountEntities[currentEntity] = balls[i].getTotalPoly();
			colourEntities[currentEntity] = balls[i].getColour();
			gouraudEntities[currentEntity] = balls[i].isGouraudOn();
			currentEntity++;
		}
	}


	Cube cubes[] = { Cube(50.0, 0.0, -2.0, 5.0, getColour(0, 0, 255, 0)),
			 Cube(60.0, 0.0, -2.0, 5.0, getColour(0, 255, 0, 0)),
			 Cube(50.0, -8.0,  8.0, 5.0, getColour(0, 0, 0, 255)),
			 Cube(60.0, 0.0,  8.0, 5.0, getColour(0, 128, 128, 128)),
			 Cube(70.0, -20.0,  20.0, 1.0, getColour(0, 0, 0, 128)),
			 Cube(72.0, -20.0,  20.0, 1.0, getColour(0, 0, 0, 128)),
			 Cube(74.0, -20.0,  20.0, 1.0, getColour(0, 0, 0, 128)),
			 Cube(50.0, 10.0,  8.0, 10.0, getColour(0, 223, 223, 0)),
			 Cube(50.0, 25.0,  8.0, 10.0, getColour(0, 63, 63, 63)) };

	int nCube = sizeof(cubes) / sizeof(Cube);
	nEntities += nCube;
	for (int i = 0; i < nCube; i++)
	{
		if (currentEntity < MAXENTITIES)
		{
			polyCountEntities[currentEntity] = cubes[i].getTotalPoly();
			colourEntities[currentEntity] = cubes[i].getColour();
			gouraudEntities[currentEntity] = true;
			currentEntity++;
		}
	}


	Prism boxes[] = {	Prism(-5.0, -5.0, 0.0, 50.0, 1.0, 1.0,		getColour(0, 0, 0, 255)),
						Prism(-5.0, -5.0, 10.0, 50.0, 1.0, 1.0,		getColour(0, 127, 127, 255)),
						Prism(10.0, 15.0, 2.5, 3.0, 4.0, 5.0,		getColour(0, 0, 0, 255)),
						Prism(-20.0, -20.0, -5.0, 40.0, 20.0, 1.0,	getColour(0, 0, 0, 127)),
						Prism(-30.0, -20.0, -10.0, 10.0, 20.0, 1.0,	getColour(0, 0, 0, 127)), 
						Prism(-40.0, -20.0, -15.0, 10.0, 20.0, 1.0,	getColour(0, 0, 0, 127)),
						Prism(-50.0, -20.0, -20.0, 10.0, 20.0, 1.0,	getColour(0, 0, 0, 127)),
						Prism(-60.0, -20.0, -25.0, 10.0, 20.0, 1.0,	getColour(0, 0, 0, 127)),
						Prism(-70.0, -20.0, -30.0, 10.0, 20.0, 1.0,	getColour(0, 0, 0, 127)),
						Prism(-80.0, -20.0, -35.0, 10.0, 20.0, 1.0,	getColour(0, 0, 0, 127)),
						Prism(-90.0, -20.0, -40.0, 10.0, 20.0, 1.0,	getColour(0, 0, 0, 127))
	};


	int nPrism = sizeof(boxes) / sizeof(Prism);
	nEntities += nPrism;
	for (int i = 0; i < nPrism; i++)
	{
		if (currentEntity < MAXENTITIES)
		{
			polyCountEntities[currentEntity] = boxes[i].getTotalPoly();	//TO DO: Add getTotalPoly() to Prism class, returning 12!
			colourEntities[currentEntity] = boxes[i].getColour();
			gouraudEntities[currentEntity] = true;
			currentEntity++;
		}
	}


	Cylinder cylinders[] = { Cylinder(0.0, 0.0, 0.0, 1.25, 5.0, 36, true, getColour(0, 127, 127, 127)) };

	int nCylinder = sizeof(cylinders) / sizeof(Cylinder);
	nEntities += nCylinder;
	for (int i = 0; i < nCylinder; i++)
	{
		if (currentEntity < MAXENTITIES)
		{
			polyCountEntities[currentEntity] = cylinders[i].getTotalPoly();
			colourEntities[currentEntity] = cylinders[i].getColour();
			gouraudEntities[currentEntity] = true;
			currentEntity++;
		}
	}


	Cone cones[] = {	Cone(-30.0, -30.0, -30.0, 15.0, 15.0, 72, true, getColour(0, 127, 127, 255)),
						Cone(-22.5, -30.0, -30.0, 15.0, 15.0, 72, true, getColour(0, 255,   0,   0)) };

	int nCone = sizeof(cones) / sizeof(Cone);
	nEntities += nCone;
	for (int i = 0; i < nCone; i++)
	{
		if (currentEntity < MAXENTITIES)
		{
			polyCountEntities[currentEntity] = cones[i].getTotalPoly();
			colourEntities[currentEntity] = cones[i].getColour();
			gouraudEntities[currentEntity] = true;
			currentEntity++;
		}
	}


	/*Model models[] = { Model("test.stl", 0.0, 0.0, 0.0, 0.1, getColour(0, 255, 255, 255)) };

	int nModel = sizeof(models) / sizeof(Model);
	nEntities += nModel;
	for (int i = 0; i < nModel; i++)
	{
		if (currentEntity < MAXENTITIES)
		{
			polyCountEntities[currentEntity] = models[i].getTotalPoly();
			colourEntities[currentEntity] = models[i].getColour();
			gouraudEntities[currentEntity] = false;
			currentEntity++;
		}
	}*/
	
	   	  

	triangle3dV** meshTriangles = new triangle3dV * [nEntities];
	for (int i = 0; i < nEntities; i++)
	{
		int totalPoly = polyCountEntities[i];
		meshTriangles[i] = new triangle3dV[totalPoly];
	}

	currentEntity = 0;

	if (meshTriangles != NULL)
	{
		for (int i = 0; i < nSphere; i++) { meshTriangles[currentEntity] = balls[i].getTriangleData(); 	currentEntity++; }
		for (int i = 0; i < nCube; i++) { meshTriangles[currentEntity] = cubes[i].getTriangleData(); 	currentEntity++; }
		for (int i = 0; i < nPrism; i++) { meshTriangles[currentEntity] = boxes[i].getTriangleData(); 	currentEntity++; }
		for (int i = 0; i < nCylinder; i++) { meshTriangles[currentEntity] = cylinders[i].getTriangleData(); 	currentEntity++; }
		for (int i = 0; i < nCone; i++) { meshTriangles[currentEntity] = cones[i].getTriangleData(); 	currentEntity++; }
		//for (int i = 0; i < nModel; i++) { meshTriangles[currentEntity] = models[i].getTriangleData(); 	currentEntity++; }
		
	}

	int nSupport = 0;
	for (int i = 0; i < nEntities; i++)
	{
		int totalPoly = polyCountEntities[i];
		for (int j = 0; j < totalPoly; j++)
		{
			triangle3dV currentNormal = meshTriangles[i][j];
			if (currentNormal.N.z >= 0.866)
			{
				nSupport++;
			}
		}
	}
	printf("Number of supporting triangles: %d\n", nSupport);
	triangle3dV* supports = new triangle3dV[nSupport];
	int sCnt = 0;
	for (int i = 0; i < nEntities; i++)
	{
		int totalPoly = polyCountEntities[i];
		for (int j = 0; j < totalPoly; j++)
		{
			triangle3dV currentNormal = meshTriangles[i][j];
			if (currentNormal.N.z >= 0.866)
			{
				supports[sCnt] = meshTriangles[i][j];
				sCnt++;
			}
		}
	}





	SDL_Init(SDL_INIT_VIDEO);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_Event event;
	SDL_Window* window = SDL_CreateWindow("Rendering Engine",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	SDL_Renderer* screen = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture* texture = SDL_CreateTexture(screen, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

	bool quit = false;


	Camera Player{ 0.0, 0.0, 40.0, 1.0, -PI * 0.5, 0.0, 0.0, PI * 0.5, 1.0, 999.0, SCREEN_WIDTH, SCREEN_HEIGHT };

	double turnH, turnV;
	turnH = 0.0;
	turnV = 0.0;

	ViewVolume Frustum;
	Frustum.initFrustum(Player.getFovH(), Player.getFovV(), Player.zNear, Player.zFar);
	
	Canvas Screen(SCREEN_WIDTH, SCREEN_HEIGHT, getColour(0, 255, 255, 255), 999.9);
	Screen.resetPixelBuffer();
	Screen.resetDepthBuffer();

	vect3 vertexList[MAXCLIPVERTS];
	const vect3 nullVector{ 0.0, 0.0, 0.0, 0.0 };
	for (int v = 0; v < MAXCLIPVERTS; v++)
	{
		vertexList[v] = { 0.0, 0.0, 0.0, 0.0 };
	}


	double moveP, strafeP, riseP, stepP, tiltP;
	moveP = 0;
	strafeP = 0;
	riseP = 0;
	tiltP = 0;
	stepP = Player.step;

	double playerSupport = 10.0;

	while (!quit)
	{

		Screen.resetPixelBuffer();
		Screen.resetDepthBuffer();

		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_PAGEUP] && !state[SDL_SCANCODE_PAGEDOWN])
		{
			if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
			{
				riseP = stepP * 4;
			}
			else
			{
				riseP = stepP;
			}
		}
		if (state[SDL_SCANCODE_PAGEDOWN] && !state[SDL_SCANCODE_PAGEUP])
		{
			if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
			{
				riseP = -stepP * 4;
			}
			else
			{
				riseP = -stepP;
			}
		}
		if (state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN])
		{
			if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
			{
				moveP = stepP * 4;
			}
			else
			{
				moveP = stepP;
			}
		}
		if (state[SDL_SCANCODE_DOWN] && !state[SDL_SCANCODE_UP])
		{
			if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
			{
				moveP = -stepP * 4;
			}
			else
			{
				moveP = -stepP;
			}
		}
		if (state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT])
		{
			if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
			{
				strafeP = -stepP * 4;
			}
			else
			{
				strafeP = -stepP;
			}
			tiltP = 0.03;
		}
		if (state[SDL_SCANCODE_RIGHT] && !state[SDL_SCANCODE_LEFT])
		{
			if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
			{
				strafeP = stepP * 4;
			}
			else
			{
				strafeP = stepP;
			}
			tiltP = -0.03;
		}

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_TAB:

					break;
				case SDLK_ESCAPE:
					quit = true;
					break;
				}
			}

			if (SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_RIGHT)
				{
					Player.jump(nSupport, supports);
				}
			}

			if (event.type == SDL_MOUSEMOTION)
			{
				turnH = (double)event.motion.x * SENSITIVITY;
				turnV = (double)event.motion.y * SENSITIVITY;
			}

			if (event.type == SDL_QUIT) { quit = true; }
		}

		playerSupport = Player.getSupport(nSupport, supports);
		//printf("Player is supported at: %.2f\n", playerSupport);
		Player.sufferFromGravity(nSupport, supports);

		Player.updatePosition(turnH, turnV, tiltP, moveP, strafeP, riseP);
		Player.sufferFromGravity(nSupport, supports);

		moveP = 0;
		strafeP = 0;
		riseP = 0;
		tiltP = 0;

		double hRatio = Player.getHRatio();
		double vRatio = Player.getVRatio();

		transform3d playerPosition;
		playerPosition.sinAzm = sin(Player.azm + PI / 2);
		playerPosition.cosAzm = cos(Player.azm + PI / 2);
		playerPosition.sinAlt = sin(Player.alt + PI / 2);
		playerPosition.cosAlt = cos(Player.alt + PI / 2);
		playerPosition.sinRol = sin(Player.rol);
		playerPosition.cosRol = cos(Player.rol);

		for (int i = 0; i < nEntities; i++)									//For every solid
		{
			int totalPoly = polyCountEntities[i];
			Uint32 colour = colourEntities[i];
			for (int k = 0; k < totalPoly; k++)								//For every triangle (2 per side)
			{
				if(Player.polyFacingCamera(meshTriangles[i][k]))
				{

					triangle3dV viewT = Player.world2viewT(playerPosition, meshTriangles[i][k]);

					Player.illuminatePoly(lights, &viewT, meshTriangles[i][k], gouraudEntities[i]);

					int nVert = Player.clipToFrustum(viewT, vertexList, Frustum);

					Player.projectPoly(nVert, vertexList, colour, Screen, hRatio, vRatio);

				}
			}
		}

		Screen.drawCrosshair(4, 12, getColour(0, 127, 127, 127));

		SDL_UpdateTexture(texture, NULL, Screen.pixelBuffer, Screen.getWidth() * sizeof(Uint32));
		SDL_RenderClear(screen);

		SDL_RenderCopy(screen, texture, NULL, NULL);
		SDL_RenderPresent(screen);

	}

	for (int i = 0; i < nEntities; i++) { delete[] meshTriangles[i]; }
	delete[] meshTriangles;

	delete[] polyCountEntities;
	delete[] colourEntities;
	delete[] supports;

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(screen);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;

}