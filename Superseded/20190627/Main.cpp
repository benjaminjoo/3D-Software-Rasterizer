#include <SDL.h>
#include <stdio.h>
#include <time.h>
#include "Definitions.h"
#include "Context.h"
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
#include "VoxelObject.h"



int main(int argc, char* argv[])
{
	Context World;	//To avoid having to use global variables

	LightSource lights[] = {	LightSource(-75.0, 30.0, 0.95) };	//Put all directional light sources in here

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

	//Editable voxel maze
	VoxelObject Level(32, 16, 16, 5.0);
	/*Uint32 currentCube;
	int cubeCounter = 0;
	for (int z = 0; z < Level.getSizeZ(); z++)
	{
		for (int y = 0; y < Level.getSizeY(); y++)
		{
			for (int x = 0; x < Level.getSizeX(); x++)
			{
				currentCube = Level.voxelBuffer[z * Level.getSizeX() * Level.getSizeY() + y * Level.getSizeX() + x];
				if (currentCube)
				{
					cubeCounter++;
				}
			}
		}
	}*/

	//3D cursor
	Cube Cursor(Level.getCurrentX(), Level.getCurrentY(), Level.getCurrentZ(), Level.getUnitSize(), getColour(0, 255, 255, 255));

	triangle3dV* cursorTriangles = new triangle3dV[12];
	int totalSize = Level.getTotalSize();
	//Cube* voxels = new Cube[totalSize];
	Voxel* voxels = new Voxel[totalSize];
	
	triangle3dV** voxelTriangles = new triangle3dV*[totalSize];
	for (int i = 0; i < totalSize; i++)
	{
		//voxelTriangles[i] = new triangle3dV[12];
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
		/*for (int i = 0; i < nSphere; i++) { meshTriangles[currentEntity] = balls[i].getTriangleData(); 	currentEntity++; }
		for (int i = 0; i < nCube; i++) { meshTriangles[currentEntity] = cubes[i].getTriangleData(); 	currentEntity++; }
		for (int i = 0; i < nPrism; i++) { meshTriangles[currentEntity] = boxes[i].getTriangleData(); 	currentEntity++; }
		for (int i = 0; i < nCylinder; i++) { meshTriangles[currentEntity] = cylinders[i].getTriangleData(); 	currentEntity++; }
		for (int i = 0; i < nCone; i++) { meshTriangles[currentEntity] = cones[i].getTriangleData(); 	currentEntity++; }*/
		
		//for (int i = 0; i < nModel; i++) { meshTriangles[currentEntity] = models[i].getTriangleData(); 	currentEntity++; }		
	}


	//Support provided by triangle mesh
	/*int nSupport = 0;
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
	}*/

	int nSupport = Level.reportNSupport();
	triangle3dV* supports = new triangle3dV[totalSize];
	supports = Level.provideSupport();




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
	Camera Player{ 0.0, 0.0, 40.0, 1.0, -PI * 0.5, 0.0, 0.0, PI * 0.5, 1.0, 999.0, SCREEN_WIDTH, SCREEN_HEIGHT };

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

	int nVoxelNew, nVoxelOld;
	nVoxelOld = 0;

	//Main Loop
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
					//editingMode = editingMode ? false : true;
					World.editingMode = World.editingMode ? false : true;
					break;
				case SDLK_ESCAPE:
					printf("Number of voxels placed: %d\n", Level.countVoxels());
					quit = true;
					break;
				case SDLK_UP:
					if (Level.getCurrentY() < Level.getSizeY() - 1)
					{
						Level.stepCurrentY(1);
					}
					break;
				case SDLK_DOWN:
					if (Level.getCurrentY() > 0)
					{
						Level.stepCurrentY(-1);
					}
					break;
				case SDLK_LEFT:
					if (Level.getCurrentX() < Level.getSizeX() - 1)
					{
						Level.stepCurrentX(1);
					}
					break;
				case SDLK_RIGHT:
					if (Level.getCurrentX() > 0)
					{
						Level.stepCurrentX(-1);
					}
					break;
				case SDLK_HOME:
					if (Level.getCurrentZ() < Level.getSizeZ() - 1)
					{
						Level.stepCurrentZ(1);
					}
					break;
				case SDLK_END:
					if (Level.getCurrentZ() > 0)
					{
						Level.stepCurrentZ(-1);
					}
					break;
				case SDLK_RETURN:
					if (World.editingMode)
					{
						int x, y, z;
						x = Level.getCurrentX();
						y = Level.getCurrentY();
						z = Level.getCurrentZ();
						Level.putVoxel(x, y, z, 255);
					}
					break;
				case SDLK_DELETE:
					if (World.editingMode)
					{
						int x, y, z;
						x = Level.getCurrentX();
						y = Level.getCurrentY();
						z = Level.getCurrentZ();
						Level.delVoxel(x, y, z);
					}
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


		if (!World.editingMode)
		{			
			playerSupport = Player.getSupport(nSupport, supports);
			Player.sufferFromGravity(nSupport, supports);
		}
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
			for (int k = 0; k < totalPoly; k++)								//For every triangle (2 per side)
			{
				if(Player.polyFacingCamera(meshTriangles[i][k]))
				{

					triangle3dV viewT = Player.world2viewT(playerPosition, meshTriangles[i][k]);

					Player.illuminatePoly(lights, &viewT, meshTriangles[i][k], gouraudEntities[i]);

					int nVert = Player.clipToFrustum(viewT, vertexList, Frustum);

					Player.projectPoly(nVert, vertexList, colour, Screen, hRatio, vRatio, World.gouraudShading, World.visualiseDepth);

				}
			}
		}



		if (World.editingMode)
		{
			Cursor.updateX(Level.getCurrentX() * Level.getUnitSize());
			Cursor.updateY(Level.getCurrentY() * Level.getUnitSize());
			Cursor.updateZ(Level.getCurrentZ() * Level.getUnitSize());
			
			if (Level.cursorState < 0)
			{
				if (Level.getColour() >= 8)
				{
					Level.stepColour(-8);
				}
				else
				{
					Level.cursorState = 1;
					Level.setColour(8);
				}
			}
			else if (Level.cursorState > 0)
			{
				if (Level.getColour() <= 240)
				{
					Level.stepColour(8);
				}
				else
				{
					Level.cursorState = -1;
					Level.setColour(248);
				}
			}

			cursorTriangles = Cursor.getTriangleData();

			Uint32 colour = getColour(0, Level.getColour(), Level.getColour(), Level.getColour());

			for (int i = 0; i < 12; i++)
			{
				if (Player.polyFacingCamera(cursorTriangles[i]))
				{
					triangle3dV viewT = Player.world2viewT(playerPosition, cursorTriangles[i]);
					Player.illuminatePoly(lights, &viewT, cursorTriangles[i], false);
					int nVert = Player.clipToFrustum(viewT, vertexList, Frustum);
					Player.projectPoly(nVert, vertexList, colour, Screen, hRatio, vRatio, World.gouraudShading, World.visualiseDepth);
				}
			}
		}


		
		nVoxelNew = Level.countVoxels();
		if (nVoxelNew != nVoxelOld)
		{
			voxels = Level.getCubeData();
			if (nVoxelOld > 0)
			{
				for (int i = 0; i < nVoxelNew; i++)
				{
					delete[] voxelTriangles[i];
				}
			}

			for (int i = 0; i < nVoxelNew; i++)
			{
				if (i < totalSize)
				{
					voxelTriangles[i] = new triangle3dV[voxels[i].countFaces()];
					voxelTriangles[i] = voxels[i].getTriangleData();
				}
			}		
			nSupport = Level.reportNSupport();
			supports = Level.provideSupport();
		}
		if (nVoxelNew > 0)
		{
			for (int v = 0; v < nVoxelNew; v++)
			{
				for (int i = 0; i < voxels[v].countFaces(); i++)
				{
					//if(voxelTriangles[v][i])
					if (Player.polyFacingCamera(voxelTriangles[v][i]))
					{
						triangle3dV viewT = Player.world2viewT(playerPosition, voxelTriangles[v][i]);
						Player.illuminatePoly(lights, &viewT, voxelTriangles[v][i], false);
						int nVert = Player.clipToFrustum(viewT, vertexList, Frustum);
						Player.projectPoly(nVert, vertexList, 255, Screen, hRatio, vRatio, World.gouraudShading, World.visualiseDepth);
					}
				}
			}
		}
		nVoxelOld = nVoxelNew;



			   		 	  	  	 

		Screen.drawCrosshair(4, 12, getColour(0, 127, 127, 127));


		//SDL clean-up
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

	delete[] voxels;
	delete[] cursorTriangles;
	for (int i = 0; i < Level.getSizeX() * Level.getSizeY() * Level.getSizeZ(); i++)
	{
		if (i < totalSize)
		{
			delete[] voxelTriangles[i];
		}
	}
	delete[] voxelTriangles;

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(screen);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;

}