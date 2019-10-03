#define _CRT_SECURE_NO_DEPRECATE//

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "Definitions.h"
#include "Camera.h"
#include "Canvas.h"
#include "LightSource.h"
#include "SpotLight.h"
#include "ViewVolume.h"
#include "Shapes.h"
#include "Extruded.h"
#include "SolidBody.h"
#include "SolidCube.h"
#include "SolidPrism.h"
#include "SolidCylinder.h"
#include "SolidCone.h"
#include "SolidSphere.h"
#include "SolidEgg.h"
#include "SolidRevolved.h"
#include "Room.h"
#include "Projectile.h"
#include "EventHandler.h"


void renderer()
{
	//SDL boilerplate
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_Window* window = SDL_CreateWindow("Rendering Engine",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_Renderer* screen = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture* sdl_texture = SDL_CreateTexture(screen, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Setting up display
	Canvas Screen(SCREEN_WIDTH, SCREEN_HEIGHT, getColour(0, 255, 255, 255), 999.9);
	Screen.resetPixelBuffer();
	Screen.resetDepthBuffer();

	Shapes Solids;
	Shapes Actors;

	/*LightSource lights[] = { LightSource(115.0, 45.0, 0.95) };
	SDL_Surface* textures[] = { IMG_Load("Textures/blue.jpg"),
								IMG_Load("Textures/wolf001.jpg"),
								IMG_Load("Textures/wolf002.jpg"),
								IMG_Load("Textures/timber.jpg"),
								IMG_Load("Textures/tisza001.jpg"),
								IMG_Load("Textures/vader001.jpg"),
								IMG_Load("Textures/brick001.jpg"),
								IMG_Load("Textures/wolf003.jpg"),
								IMG_Load("Textures/cat001.jpg"),
								IMG_Load("Textures/quake001.jpg"),
								IMG_Load("Textures/quake002.jpg"),
								IMG_Load("Textures/quake003.jpg"),
								IMG_Load("Textures/quake004.jpg"),
								IMG_Load("Textures/quake005.jpg"),
								IMG_Load("Textures/mosaic001.jpg"),
								IMG_Load("Textures/concrete001.jpg") };
	Solids.addSolid(new SolidSphere(1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, getColour(0, 0, 255, 0), 15, 5.0, 60));*/

	#include "gun.txt"
	//#include "rooms.txt"	
	//#include "bodies.txt"
	//#include "solar_system.txt"

	int nTextures = sizeof(textures) / sizeof(SDL_Surface*);
	printf("%d x textures loaded...\n", nTextures);
	for (int i = 0; i < nTextures; i++)
	{
		if (textures[i] == nullptr)
		{
			printf("Image loading failed...\n");
		}
		else
		{
			txt tempTexture;
			SDL_Surface* image = SDL_ConvertSurfaceFormat(textures[i], SDL_PIXELFORMAT_ARGB8888, 0);
			tempTexture.pixels = (Uint32*)image->pixels;
			tempTexture.ID = i;
			tempTexture.w = textures[i]->w;
			tempTexture.h = textures[i]->h;
			Solids.addTextureData(tempTexture);
			Actors.addTextureData(tempTexture);
		}
	}

		
	int				solidN							= Solids.getNEntities();	
	int*			solidPolyCount					= Solids.getPolyCountEntities();
	triangle3dV**	solidMesh						= new triangle3dV * [solidN];

	for (int i = 0; i < solidN; i++)
	{
		solidMesh[i] = new triangle3dV[solidPolyCount[i]];
	}
	Solids.getPolyData_(solidN, solidMesh);

	boundingBox*	solidBoxes						= new boundingBox[solidN];
	for (int i = 0; i < solidN; i++)
	{
		solidBoxes[i].minExt = getXYZmin(solidPolyCount[i], solidMesh[i], 1.0f);
		solidBoxes[i].maxExt = getXYZmax(solidPolyCount[i], solidMesh[i], 1.0f);
		printf("xMin = %.2f\tyMin = %.2f\tzMin = %.2f\n", solidBoxes[i].minExt.x, solidBoxes[i].minExt.y, solidBoxes[i].minExt.z);
		printf("xMax = %.2f\tyMax = %.2f\tzMax = %.2f\n", solidBoxes[i].maxExt.x, solidBoxes[i].maxExt.y, solidBoxes[i].maxExt.z);
	}

	txt*			solidTexture					= Solids.getTextureData();
	int				solidLightN						= Solids.getNSpotLights();
	vect3*			solidLightPosition				= Solids.getSpotLightPositions();
	vect3*			solidLightPositionTransformed	= new vect3[solidLightN];
	double*			solidLightIntensity				= Solids.getSpotLightIntensities();

	int				actorN			= 0;
	triangle3dV**	actorMesh		= nullptr;
	int*			actorPolyCount	= nullptr;
	txt*			actorTexture	= nullptr;


	//Setting up player object
	Camera Player( 1.0, 1.0, 1.5, 0.1, 0.05, PI * 0.5, PI, 0.0, PI * 0.5, 1.00, 999.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );

	EventHandler Controls(Player.step, Player.turn, &Player, &Screen);

	ViewVolume Frustum;
	Frustum.initFrustum(Player.getFovH(), Player.getFovV(), Player.zNear, Player.zFar);	

	//Initialising vertex list
	vect3 solidVertexList[MAXCLIPVERTS];
	textCoord solidUVList[MAXCLIPVERTS];
	for (int v = 0; v < MAXCLIPVERTS; v++) { solidVertexList[v] = { 0.0, 0.0, 0.0, 0.0 }; solidUVList[v] = { 0.0, 0.0 }; }

	vect3 actorVertexList[MAXCLIPVERTS];
	textCoord actorUVList[MAXCLIPVERTS];
	for (int v = 0; v < MAXCLIPVERTS; v++) { actorVertexList[v] = { 0.0, 0.0, 0.0, 0.0 }; actorUVList[v] = { 0.0, 0.0 }; }
	   

	double azmToShow, altToShow, rolToShow;
	const double playerSupport = 0.0;


	clock_t oldTime, newTime, frameTime;
	oldTime = 0;
	newTime = 0;
	frameTime = 0;
	int frameCounter = 0;
	

	double hRatio = Player.getHRatio();
	double vRatio = Player.getVRatio();
	
	//Main Loop
	while (!Controls.quit)
	{
		if (frameCounter == 30)
		{
			oldTime = newTime;
			newTime = clock();
			frameTime = newTime - oldTime;
			frameCounter = 0;
		}

		Controls.HandleUserEvents();

		//Flush previous frame from buffers
		Screen.resetPixelBuffer();
		Screen.resetDepthBuffer();

		Player.updatePosition(Controls.turnH, Controls.turnV, Controls.tiltP, Controls.moveP, Controls.strafeP, Controls.riseP);

		Controls.ceaseMotion();

		transform3d playerPosition;
		playerPosition.sinAzm = sin(Player.azm + PI * 0.5);
		playerPosition.cosAzm = cos(Player.azm + PI * 0.5);
		playerPosition.sinAlt = sin(Player.alt + PI * 0.5);
		playerPosition.cosAlt = cos(Player.alt + PI * 0.5);
		playerPosition.sinRol = sin(Player.rol);
		playerPosition.cosRol = cos(Player.rol);

		
		for (int lmp = 0; lmp < solidLightN; lmp++)
		{
			solidLightPositionTransformed[lmp] = Player.world2view(playerPosition, solidLightPosition[lmp]);
		}
		

		for (int i = 0; i < solidN; i++)								//For every solid
		{	
			int totalPoly = solidPolyCount[i];
			for (int k = 0; k < totalPoly; k++)
			{
				if (Player.polyFacingCamera(solidMesh[i][k]))			//Backface culling is performed here
				{
					Uint32 colour = solidMesh[i][k].colour;

					triangle3dV viewT = Player.world2viewT(playerPosition, solidMesh[i][k]);

					Player.illuminatePoly(lights, &viewT, solidMesh[i][k], Controls.gouraudShading);

					//Player.illuminatePoly(lights, &viewT, true);

					int nVert = Player.clipToFrustum(viewT, solidVertexList, solidUVList, Frustum);

					int textureID = solidMesh[i][k].texture;
					Player.currentTexture = solidTexture[textureID];

					Player.projectPoly(nVert, solidVertexList, solidUVList, colour, Screen, hRatio, vRatio, 
						Controls.visualStyle, solidLightN, solidLightPositionTransformed, solidLightIntensity, viewT);
				}
			}
		}


		actorN			= Actors.getNEntities();
		actorPolyCount	= Actors.getPolyCountEntities();
		actorTexture	= Actors.getTextureData();
		
		actorMesh		= new triangle3dV * [actorN];
		for (int j = 0; j < actorN; j++)
		{
			actorMesh[j] = new triangle3dV[actorPolyCount[j]];
		}

		Actors.updateBodyPosition();
		Actors.getPolyData_(actorN, actorMesh);


		for (int i = 0; i < actorN; i++)								//For every actor
		{	

			/*vect3 ballPosition = Actors.getPosition(i);
			printf("x: %.6f\ty: %.6f\tz: %.6f\n", ballPosition.x, ballPosition.y, ballPosition.z);
			bool collision = false;
			for (int b = 0; b < solidN; b++)
			{
				if (checkBoundingBoxForCollision(ballPosition, solidBoxes[b])) { collision = true; }
			}
			if (collision) { printf("ACHTUNG, FUCKERS! COLLISION!\n"); }
			else { printf("TAKE IT EASY, BITCHES...\n"); }*/

			int totalPoly = actorPolyCount[i];
			for (int k = 0; k < totalPoly; k++)
			{
				if (Player.polyFacingCamera(actorMesh[i][k]))			//Backface culling is performed here
				{
					Uint32 colour = actorMesh[i][k].colour;
					triangle3dV viewT = Player.world2viewT(playerPosition, actorMesh[i][k]);

					Player.illuminatePoly(lights, &viewT, actorMesh[i][k], Controls.gouraudShading);
					//Player.illuminatePoly(lights, &viewT, true);

					int nVert = Player.clipToFrustum(viewT, actorVertexList, actorUVList, Frustum);

					int textureID = actorMesh[i][k].texture;
					Player.currentTexture = actorTexture[textureID];

					Player.projectPoly(nVert, actorVertexList, actorUVList, colour, Screen, hRatio, vRatio, 
						 Controls.visualStyle, solidLightN, solidLightPositionTransformed, solidLightIntensity, viewT);
				}
			}
		}


		freePointerToPolyArray(actorMesh, actorN);

		delete[] actorPolyCount;
		delete[] actorTexture;

		
		if (Controls.showCrosshair) { Screen.drawCrosshair(2, 6, getColour(0, 127, 127, 127)); }
		if (Controls.showFPS && frameTime > 0) { Screen.displayFps((double)(30000.0 / frameTime), 1, 2, 0); }
		if (Controls.showPosition)
		{
			Screen.displayString("AaBbCcDdEeFfGgHhIiJjKkLlMm", 30, 18, getColour(0, 0, 255, 0));
			Screen.displayString("NnOoPpQqRrSsTtUuVvWwXxYyZz", 30, 16, getColour(0, 0, 255, 0));

			Screen.displayString("MAX ILLUMINATION", 30, 12,		getColour(0, 255, 255, 255));
			Screen.displayValue(Player.maxIllumination, 2, 2, 12,	getColour(0, 255, 255, 255));
			Screen.displayString("TORCH INTENSITY", 30, 11,			getColour(0, 255, 255, 255));
			Screen.displayValue(Player.torchIntensity, 2, 2, 11,	getColour(0, 255, 255, 255));

			Screen.displayString("POSITION X", 30, 9,				getColour(0, 255, 127, 127));
			Screen.displayValue((double)Player.x, 1, 2, 9,			getColour(0, 255, 127, 127));
			Screen.displayString("POSITION Y", 30, 8,				getColour(0, 127, 255, 127));
			Screen.displayValue((double)Player.y, 1, 2, 8,			getColour(0, 127, 255, 127));
			Screen.displayString("POSITION Z", 30, 7,				getColour(0, 127, 127, 255));
			Screen.displayValue((double)Player.z, 1, 2, 7,			getColour(0, 127, 127, 255));		

			azmToShow = Player.azm * 180.0 / PI;
			if (azmToShow > 360.0) { azmToShow -= 360.0; }
			if (azmToShow < -360.0) { azmToShow += 360.0; }
			altToShow = Player.alt * 180.0 / PI - 180.0;
			if (altToShow > 360.0) { altToShow -= 360.0; }
			if (altToShow < -360.0) { altToShow += 360.0; }
			rolToShow = Player.rol * 180.0 / PI;
			if (rolToShow > 360.0) { rolToShow -= 360.0; }
			if (rolToShow < -360.0) { rolToShow += 360.0; }

			Screen.displayString("ROTATION X", 30, 5,				getColour(0, 255, 127, 127));
			Screen.displayValue(azmToShow, 1, 2, 5,					getColour(0, 255, 127, 127));
			Screen.displayString("ROTATION Y", 30, 4,				getColour(0, 127, 255, 127));
			Screen.displayValue(altToShow, 1, 2, 4,					getColour(0, 127, 255, 127));
			Screen.displayString("ROTATION Z", 30, 3,				getColour(0, 127, 127, 255));
			Screen.displayValue(rolToShow, 1, 2, 3,					getColour(0, 127, 127, 255));
		}
				
		SDL_UpdateTexture(sdl_texture, nullptr, Screen.pixelBuffer, Screen.getWidth() * sizeof(Uint32));
		SDL_RenderClear(screen);

		SDL_RenderCopy(screen, sdl_texture, nullptr, nullptr);
		SDL_RenderPresent(screen);

		frameCounter++;
	}
	

	//Releasing dynamically allocated memory
	freePointerToPolyArray(solidMesh, solidN);
	delete[] solidPolyCount;
	delete[] solidLightIntensity;
	delete[] solidLightPosition;
	delete[] solidLightPositionTransformed;

	//SDL clean-up
	for (int i = 0; i < nTextures; i++)
	{
		SDL_FreeSurface(textures[i]);
	}

	SDL_DestroyTexture(sdl_texture);
	SDL_DestroyRenderer(screen);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

}


int main(int argc, char* argv[])
{
	renderer();

	return 0;
}