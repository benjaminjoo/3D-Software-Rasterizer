#define _CRT_SECURE_NO_DEPRECATE//

#include <SDL.h>
#include <SDL_image.h>

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "Definitions.h"
#include "Context.h"
#include "Shapes.h"
#include "Extruded.h"
#include "Camera.h"
#include "Canvas.h"
#include "LightSource.h"
#include "ViewVolume.h"

#include "SolidBody.h"
#include "SolidCube.h"
#include "SolidPrism.h"
#include "SolidCylinder.h"
#include "SolidCone.h"
#include "SolidSphere.h"
#include "SolidRevolved.h"


void renderer()
{
	Context World;

	Shapes Solids;

	#include "bodies.txt"


	//SDL boilerplate
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG);
	
	SDL_ShowCursor(SDL_DISABLE);
	//SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_Event event;
	SDL_Window* window = SDL_CreateWindow("Rendering Engine",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	SDL_Renderer* screen = SDL_CreateRenderer(window, -1, 0);

	SDL_Surface* textures[] = { IMG_Load("blue.jpg"),
								IMG_Load("wolf001.jpg"),
								IMG_Load("wolf002.jpg"),
								IMG_Load("timber.jpg"),
								IMG_Load("tisza001.jpg"),
								IMG_Load("vader001.jpg")	};

	int nTextures = sizeof(textures) / sizeof(SDL_Surface*);
	printf("%d x textures loaded...\n", nTextures);

	for (int i = 0; i < nTextures; i++)
	{
		if (textures[i] == NULL)
		{
			printf("Image loading failed...\n");
		}
		else
		{
			txt tempTexture;
			SDL_Surface* image = SDL_ConvertSurfaceFormat(textures[i], SDL_PIXELFORMAT_ARGB8888, 0);
			//Uint32* texturePixels = (Uint32*)image->pixels;
			tempTexture.pixels = (Uint32*)image->pixels;
			tempTexture.ID = i;
			tempTexture.w = textures[i]->w;
			tempTexture.h = textures[i]->h;
			Solids.addTextureData(tempTexture);
		}
	}



	SDL_Texture* texture = SDL_CreateTexture(screen, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);






	int nEntities = Solids.getNEntities();
	int* polyCountEntities = Solids.getPolyCountEntities();
	Uint32* colourEntities = Solids.getColourEntities();
	bool* gouraudEntities = Solids.getGouraudEntities();
	triangle3dV** meshTriangles = Solids.getPolyData();
	txt* meshTextures = Solids.getTextureData();




	int i = 2;
	Uint32* textures_ = meshTextures[i].pixels;
	FILE* f = fopen("fucked_texture.ppm", "w");
	if (f == NULL)
	{
		printf("Could not write to file...\n");
	}
	else
	{
		fprintf(f, "P3\n");
		fprintf(f, "%d %d\n", meshTextures[i].w, meshTextures[i].h);
		fprintf(f, "255\n");

		Uint32 currentPixel;
		Uint8 red, green, blue, alpha;

		//SDL_Surface* image = SDL_ConvertSurfaceFormat(textures_[i], SDL_PIXELFORMAT_ARGB8888, 0);
		//SDL_FreeSurface(textures_[i]);
		Uint32* texturePixels = textures_;

		int txtrW = meshTextures[i].w, txtrH = meshTextures[i].h;

		for (int j = 0; j < txtrH; j++)
		{
			for (int i = 0; i < txtrW; i++)
			{
				currentPixel = texturePixels[j * txtrW + i];
				red = (currentPixel << 8) >> 24;
				green = (currentPixel << 16) >> 24;
				blue = (currentPixel << 24) >> 24;
				fprintf(f, "%d %d %d\n", red, green, blue);
			}
		}

		fclose(f);
	}









	int nSupport = 0;
	triangle3dV* supports = Solids.getSupport(&nSupport);



	bool quit = false;

	//Setting up player object
	Camera player_01( 0.0, 0.0, 40.0, 1.0, -PI * 0.5, 0.0, 0.0, PI * 0.5, 1.0, 999.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );
	Camera player_02( 0.0, 0.0, 90.0, 1.0, -PI * 0.5, 0.0, 0.0, PI * 0.75, 1.0, 999.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );

	Camera Player = player_01;

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
	textCoord uvList[MAXCLIPVERTS];
	for (int v = 0; v < MAXCLIPVERTS; v++) { vertexList[v] = { 0.0, 0.0, 0.0, 0.0 }; uvList[v] = { 0.0, 0.0 }; }
	   

	double moveP, strafeP, riseP, stepP, tiltP;
	moveP = 0;
	strafeP = 0;
	riseP = 0;
	tiltP = 0;
	stepP = Player.step;

	const double playerSupport = 10.0;
	const double turnVmin = PI * 0.5;
	const double turnVmax = 3 * PI * 0.5;

	clock_t oldTime, newTime, frameTime;
	oldTime = 0;
	newTime = 0;
	frameTime = 0;
	int frameCounter = 0;

	double rotTestX = 3.0, rotTestY = 2.0, rotTestZ = 1.0;
	int liftCurrent = 0, liftMax = 100;
	double liftStep = 1.0;

	double hRatio = Player.getHRatio();
	double vRatio = Player.getVRatio();


	//Main Loop
	while (!quit)
	{
		if (frameCounter == 30)
		{
			oldTime = newTime;
			newTime = clock();
			frameTime = newTime - oldTime;
			frameCounter = 0;
		}

		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_Q] && !state[SDL_SCANCODE_E])
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
		if (state[SDL_SCANCODE_E] && !state[SDL_SCANCODE_Q])
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
					World.gouraudShading = World.gouraudShading ? false : true;
					break;
				case SDLK_z:
					World.visualiseDepth = World.visualiseDepth ? false : true;
					break;
				case SDLK_c:
					World.gravityOn = World.gravityOn ? false : true;
					break;
				case SDLK_TAB:
					World.showFPS = World.showFPS ? false : true;
					break;
				case SDLK_m:
					World.isPaused = World.isPaused ? false : true;
					break;
				case SDLK_p:
					Player.outputImage(Screen);
					break;
				case SDLK_KP_PLUS:
					Player.scaleUp();
					break;
				case SDLK_KP_MINUS:
					Player.scaleDown();
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

		//Flush previous frame from buffers
		Screen.resetPixelBuffer();
		Screen.resetDepthBuffer();
		
		if (World.gravityOn) { Player.sufferFromGravity(nSupport, supports); }

		Player.updatePosition(turnH, turnV, tiltP, moveP, strafeP, riseP);

		moveP = 0;
		strafeP = 0;
		riseP = 0;
		tiltP = 0;

		//double hRatio = Player.getHRatio();
		//double vRatio = Player.getVRatio();

		transform3d playerPosition;
		playerPosition.sinAzm = sin(Player.azm + PI / 2);
		playerPosition.cosAzm = cos(Player.azm + PI / 2);
		playerPosition.sinAlt = sin(Player.alt + PI / 2);
		playerPosition.cosAlt = cos(Player.alt + PI / 2);
		playerPosition.sinRol = sin(Player.rol);
		playerPosition.cosRol = cos(Player.rol);


		if (!World.isPaused)
		{
			for (int cb = 0; cb < 32 && cb < Solids.getNEntities(); cb++)
			{
				transformMesh(Solids.getPolyCount(cb), meshTriangles[cb], 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,
					rotTestX * PI / 360.0 * cb, rotTestY * PI / 360.0 * cb, rotTestZ * PI / 360.0);
			}

			liftCurrent++;
			if (liftCurrent == liftMax)
			{
				liftStep *= -1;
				liftCurrent = 0;
			}

			transformMesh(Solids.getPolyCount(17), meshTriangles[17], 1.0, 1.0, 1.0, 0.0, 0.0, liftStep,
				0.0, 0.0, 0.0);

			double scBall = 1.0 - (liftStep / liftMax);
			transformMesh(Solids.getPolyCount(0), meshTriangles[0], scBall, scBall, scBall, 0.0, 0.0, 0.0,
				0.0, 0.0, 0.0);
		}


		for (int i = 0; i < nEntities; i++)									//For every solid
		{	
			int totalPoly = polyCountEntities[i];
			Uint32 colour = colourEntities[i];
			for (int k = 0; k < totalPoly; k++)
			{
				if (Player.polyFacingCamera(meshTriangles[i][k]))			//Backface culling is performed here
				{

					triangle3dV viewT = Player.world2viewT(playerPosition, meshTriangles[i][k]);

					Player.illuminatePoly(lights, &viewT, meshTriangles[i][k], gouraudEntities[i]);

					int nVert = Player.clipToFrustum(viewT, vertexList, uvList, Frustum);

					int textureID = meshTriangles[i][k].texture;
					txt textureData = meshTextures[textureID];					

					Player.projectPoly(nVert, vertexList, uvList, colour, textureData, Screen, hRatio, vRatio,
											World.gouraudShading, World.visualiseDepth);
				}
			}
		}
		
		Screen.drawCrosshair(4, 12, getColour(0, 127, 127, 127));
		if (World.showFPS && frameTime > 0) { Screen.displayFps((double)(30000.0 / frameTime), 1); }
				
		SDL_UpdateTexture(texture, NULL, Screen.pixelBuffer, Screen.getWidth() * sizeof(Uint32));
		SDL_RenderClear(screen);

		SDL_RenderCopy(screen, texture, NULL, NULL);
		SDL_RenderPresent(screen);

		frameCounter++;
	}

	delete[] polyCountEntities;
	delete[] colourEntities;
	delete[] gouraudEntities;
	delete[] supports;

	for (int i = 0; i < nEntities; i++)
	{
		delete[] meshTriangles[i];
	}
	delete[] meshTriangles;

	//SDL clean-up
	for (int i = 0; i < nTextures; i++)
	{
		SDL_FreeSurface(textures[i]);
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(screen);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

}


int main(int argc, char* argv[])
{
	//getch();
	

	renderer();

	return 0;
}