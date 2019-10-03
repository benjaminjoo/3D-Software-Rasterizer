#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Definitions.h"
#include "Context.h"
#include "Sphere.h"
#include "Prism.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Cube.h"
#include "Model.h"
#include "Revolved.h"
#include "Camera.h"
#include "Canvas.h"
#include "LightSource.h"
#include "ViewVolume.h"


int main(int argc, char* argv[])
{
	Context World;	//To avoid having to use global variables

	LightSource lights[] = {	LightSource(-75.0, 45.0, 0.95) };	//Put all directional light sources in here

	int nEntities = 0;
	int* polyCountEntities = new int[MAXENTITIES];
	Uint32* colourEntities = new Uint32[MAXENTITIES];
	bool* gouraudEntities = new bool[MAXENTITIES];
	int currentEntity = 0;


	//3D Solids
	/*Sphere balls[] = {	Sphere(0.0,  0.0,  40.0, 15, 36, true, getColour(0, 0, 0, 255)),
						Sphere(20.0, 10.0, -20.0, 10, 36, true, getColour(0, 127, 127, 127)),
						Sphere(40.0, 20.0, -10.0, 5, 36, true, getColour(0, 127, 127, 0)),
						Sphere(60.0, 30.0, 25.0, 5, 16, true, getColour(0, 235, 25, 140)),
						Sphere(80.0, 40.0,  0.0, 2.5, 12, true, getColour(0, 255, 0, 0)) };


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


	Cylinder cylinders[] = { Cylinder(0.0, 0.0, -10.0, 1.25, 5.0, 36, true, getColour(0, 127, 127, 127)) };

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


	Cone cones[] = {	Cone(-35.0, -30.0, -40.0, 15.0, 17.5, 72, true, getColour(0, 127, 127, 255)),
						Cone(-22.5, -30.0, -40.0, 25.0, 25.0, 72, true, getColour(0, 255,   0,   0)) };

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
	}*/





	polyNode polyline[24] = {
{-6.042586993756, -90.76946724737,      1},
{-5.029692731495, -90.1475146302,      	1},
{-5.136313180154, -89.34786126525,      1},
{-4.656521161189, -89.34786126525,      1},
{-3.981258319682, -88.86806924629,    	900},
{           0, -211.8912642002,   	4001},
{-3.154446498535, -88.81744811438,      1},
{-2.257561066361, -88.7081385733,      	1},
{-1.60006829963, -89.06354006883,    	900},
{           0, 161.1904212596,   	4001},
{-1.092278645382, -89.61195289542,      1},
{0.265789551903, -90.68061687349,      	1},
{0.9232823186337, -88.86806924629,      1},
{1.634085309694, -88.54820790031,    	900},
{           0, -184.2446387961,   	4001},
{2.366613243955, -88.28183410603,      	1},
{2.753600020614, -88.60151812464,    	900},
{           0, -156.9752323643,   	4001},
{3.234795521921, -88.74437303909,      	1},
{3.411092787344, -89.18793059227,    	900},
{           0, -217.4566932194,   	4001},
{3.281285219175, -89.64724967963,      	1},
{4.068585554075, -91.26702934112,      	1},
{-6.042586993756, -90.76946724737,      1}
	};


	polyNode polyline_[6] = {
{1.0, 0.0,      1},
{1.5, 2.0,      1},
{2.0, 2.0,      1},
{2.0, 4.0,      1},
{1.0, 5.0,    	1},
{1.0, 6.0,   	1}
};


	polyNode cannon[64] = {
{        0,          116,      1},
{        0,        102.5,      1},
{                 1.7,        102.5,      1},
{                 1.7,         97.5,      1},
{                 2.2,           97,      1},
{                 2.2,            0,      1},
{               4.875,            0,      1},
{               4.875,        0.375,      1},
{              5.1875,        0.375,      1},
{              5.1875,            1,      1},
{              5.1875,            2,    900},
{                   0, 114.4040207538,   4001},
{        6.098154668798, 2.41316833639,      1},
{        17.02601069438, 7.37118837307,    900},
{                   0, -26.64164865977,   4001},
{        5.0351608088, 7.839716943631,      1},
{        5.168413669181,        11.25,      1},
{                5.25,        11.25,      1},
{                5.25,       11.375,      1},
{                5.25,           12,    900},
{                   0,          180,   4001},
{                5.25,       12.625,      1},
{                5.25,        12.75,      1},
{        5.227024431768,        12.75,      1},
{        6.91696808634,           56,      1},
{                   7,           56,      1},
{                7.25,           57,      1},
{                7.25,       58.125,      1},
{                   7,       58.125,      1},
{        7.49885670732,       77.375,      1},
{                7.75,       77.375,      1},
{                7.75,       78.625,      1},
{             7.53125,       78.625,      1},
{        8.05926067073,           99,      1},
{               8.125,           99,      1},
{               8.125,       99.125,      1},
{               8.125,        99.75,    900},
{                   0,          180,   4001},
{               8.125,      100.375,      1},
{               8.125,        100.5,      1},
{              8.0625,        100.5,      1},
{               8.125,          105,      1},
{             8.24375,          105,      1},
{             8.24375,      105.125,      1},
{             8.29375,      105.125,      1},
{             8.79375,      106.125,      1},
{             8.79375,       106.25,      1},
{             8.84375,       106.25,      1},
{             8.84375,          108,      1},
{        7.35568930474, 107.8111207602,    900},
{                   0, 56.29164337263,   4001},
{        8.024388467767, 109.1538200673,      1},
{        0, 93.04142764044,    900},
{                   0, 15.74096740388,   4001},
{        3.35234771882, 110.7264994461,      1},
{             3.59375,          112,    900},
{                   0, -124.2176324565,   4001},
{        2.676432415223, 112.9157549731,      1},
{        1.590336993585,          114,    900},
{                   0, 102.3318971694,   4001},
{        2.417602994138, 115.2926026101,      1},
{        0, 111.5150984475,    900},
{                   0, 32.6192466232,   4001},
{        0,          116,      1}
	};


	//Revolved weird_shape(0.0, 0.0, 0.0, -10.0, 0.0, 360.0, 72, 36, 24, polyline, 255);
	//Revolved weird_shape(0.0, 0.0, 0.0, -10.0, 0.0, 360.0, 72, 36, 6, polyline_, 255);
	//Revolved weird_shape(0.0, 0.0, 0.0, 0.0, 0.0, 360.0, 90, 180, 64, cannon, getColour(0, 127, 127, 127));
	Revolved weird_shape(0.0, 0.0, 0.0, 0.0, 0.0, 360.0, 36, 36, 64, cannon, getColour(0, 255, 0, 0));

	nEntities++;
	if (currentEntity < MAXENTITIES)
	{
		polyCountEntities[currentEntity] = weird_shape.getTotalPoly();
		colourEntities[currentEntity] = weird_shape.getColour();
		gouraudEntities[currentEntity] = true;
		currentEntity++;
	}







	   	  
	//Storage for triangle mesh
	triangle3dV** meshTriangles = new triangle3dV * [nEntities];
	for (int i = 0; i < nEntities; i++)
	{
		int totalPoly = polyCountEntities[i];
		meshTriangles[i] = new triangle3dV[totalPoly];
	}

	//Filling up triangle mesh
	currentEntity = 0;
	if (meshTriangles != NULL)
	{
		//for (int i = 0; i < nSphere; i++) { meshTriangles[currentEntity] = balls[i].getTriangleData(); 	currentEntity++; }
		//for (int i = 0; i < nCube; i++) { meshTriangles[currentEntity] = cubes[i].getTriangleData(); 	currentEntity++; }
		//for (int i = 0; i < nPrism; i++) { meshTriangles[currentEntity] = boxes[i].getTriangleData(); 	currentEntity++; }
		//for (int i = 0; i < nCylinder; i++) { meshTriangles[currentEntity] = cylinders[i].getTriangleData(); 	currentEntity++; }
		//for (int i = 0; i < nCone; i++) { meshTriangles[currentEntity] = cones[i].getTriangleData(); 	currentEntity++; }
		//meshTriangles[currentEntity] = weird_shape.getTriangleData(); currentEntity++;
		meshTriangles[currentEntity] = transformObject(weird_shape.getTotalPoly(), weird_shape.getTriangleDataN(),
			0.25, 0.25, 0.25,
			0.0, 0.0, -10.0,
			//PI / 180.0 * 100.0, 0.0, 0.0);
			0.0, 0.0, 0.0);
		weird_shape.get2dNormals();
		currentEntity++;
	}










	//Support provided by triangle mesh
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




	//SDL boilerplate
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

	//Setting up player object
	Camera Player{ 0.0, 0.0, 40.0, 1.0, -PI * 0.5, 0.0, 0.0, PI * 0.5, 1.0, 999.0, SCREEN_WIDTH, SCREEN_HEIGHT};

	double turnH, turnV;
	turnH = 0.0;
	turnV = 0.0;

	//Setting up view frustum object
	ViewVolume Frustum;
	Frustum.initFrustum(Player.getFovH(), Player.getFovV(), Player.zNear, Player.zFar);
	
	//Setting up display
	Canvas Screen(SCREEN_WIDTH, SCREEN_HEIGHT, getColour(0, 255, 255, 255), 999.9);
	Screen.resetPixelBuffer();
	Screen.resetDepthBuffer();

	//Initialising vertex list
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
	double turnVmin = PI * 0.5;
	double turnVmax = 3 * PI * 0.5;

	clock_t oldTime, newTime, frameTime;
	oldTime = 0;
	newTime = 0;
	frameTime = 0;
	int frameCounter = 0;
	//Main Loop
	while (!quit)
	{
		if (frameCounter == 10)
		{
			oldTime = newTime;
			newTime = clock();
			frameTime = newTime - oldTime;
			frameCounter = 0;
		}
		/*oldTime = newTime;
		newTime = clock();
		frameTime = newTime - oldTime;*/

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
		if (state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S])
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
		if (state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_W])
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
		if (state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D])
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
		if (state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_A])
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
				case SDLK_g:
					//gouraudShading = gouraudShading ? false : true;
					World.gouraudShading = World.gouraudShading ? false : true;
					break;
				case SDLK_z:
					//visualiseDepth = visualiseDepth ? false : true;
					World.visualiseDepth = World.visualiseDepth ? false : true;
					break;
				case SDLK_TAB:
					World.showFPS = World.showFPS ? false : true;
					break;
				case SDLK_SPACE:
					printf("FPS: %.2f\n", 10000.0 / frameTime);
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
				if ((double)event.motion.y * SENSITIVITY > turnVmin &&
					(double)event.motion.y* SENSITIVITY < turnVmax)
				{
					turnV = (double)event.motion.y * SENSITIVITY;
				}
				else
				{
					if ((double)event.motion.y * SENSITIVITY < turnVmin)
					{
						turnV = turnVmin;
					}
					else if ((double)event.motion.y * SENSITIVITY > turnVmax)
					{
						turnV = turnVmax;
					}
				}
			}

			if (event.type == SDL_QUIT) { quit = true; }
		}


			
		//playerSupport = Player.getSupport(nSupport, supports);
		//Player.sufferFromGravity(nSupport, supports);
		Player.updatePosition(turnH, turnV, tiltP, moveP, strafeP, riseP);


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
			for (int k = 0; k < totalPoly; k++)
			{
				if(Player.polyFacingCamera(meshTriangles[i][k]))			//Backface culling is performed here
				{

					triangle3dV viewT = Player.world2viewT(playerPosition, meshTriangles[i][k]);

					Player.illuminatePoly(lights, &viewT, meshTriangles[i][k], gouraudEntities[i]);

					int nVert = Player.clipToFrustum(viewT, vertexList, Frustum);

					Player.projectPoly(nVert, vertexList, colour, Screen, hRatio, vRatio, World.gouraudShading, World.visualiseDepth);

				}
			}
		}

   		 	  	  	 

		Screen.drawCrosshair(4, 12, getColour(0, 127, 127, 127));
		if (World.showFPS && frameTime > 0) { Screen.displayFps((double)(10000.0 / frameTime), 1); }

		
		SDL_UpdateTexture(texture, NULL, Screen.pixelBuffer, Screen.getWidth() * sizeof(Uint32));
		SDL_RenderClear(screen);

		SDL_RenderCopy(screen, texture, NULL, NULL);
		SDL_RenderPresent(screen);

		frameCounter++;
	}

	for (int i = 0; i < nEntities; i++) { delete[] meshTriangles[i]; }
	delete[] meshTriangles;

	delete[] polyCountEntities;
	delete[] colourEntities;
	delete[] supports;

	//SDL clean-up
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(screen);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;

}