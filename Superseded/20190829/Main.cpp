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
#include "SolidRevolved.h"
#include "Room.h"
#include "Projectile.h"

#include "EventHandler.h"


void renderer()
{
	Shapes Solids;
	Shapes Actors;

	//SDL boilerplate
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_Window* window = SDL_CreateWindow("Rendering Engine",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_Renderer* screen = SDL_CreateRenderer(window, -1, 0);

	//Setting up display
	Canvas Screen(SCREEN_WIDTH, SCREEN_HEIGHT, getColour(0, 255, 255, 255), 999.9);
	Screen.resetPixelBuffer();
	Screen.resetDepthBuffer();

	//#include "gun.txt"
	#include "rooms.txt"
	//#include "bodies.txt"
	//#include "solar_system.txt"
  	
	Solids.addSpotLight(new SpotLight({ 5.0, 6.0, 3.0, 1.0 }, 15.0));
	Solids.addSpotLight(new SpotLight({ 5.0, 14.0, 3.0, 1.0 }, 15.0));

	Solids.addSpotLight(new SpotLight({ 18.0, -6.0, 4.0, 1.0 }, 15.0));
	Solids.addSpotLight(new SpotLight({ 18.0, 10.0, 4.0, 1.0 }, 15.0));


	Actors.addSolid(new SolidSphere(1.0, 1.0, 1.0, 3.0, 3.0, 2.0, 0.0, 0.0, 0.0, getColour(0, 127, 127, 255), 2, 0.25, 24));
	//Actors.addSolid(new SolidCube(0.5, 0.5, 0.5, 12.0, 4.0, 1.0, 0.0, 0.0, 0.0, getColour(0, 255, 0, 0), 6, 2.0));

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
			tempTexture.pixels = (Uint32*)image->pixels;
			tempTexture.ID = i;
			tempTexture.w = textures[i]->w;
			tempTexture.h = textures[i]->h;
			Solids.addTextureData(tempTexture);
			Actors.addTextureData(tempTexture);
		}
	}


	SDL_Texture* texture = SDL_CreateTexture(screen, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

		
	int				solidN							= Solids.getNEntities();	
	int*			solidPolyCount					= Solids.getPolyCountEntities();
	triangle3dV**	solidMesh						= new triangle3dV * [solidN];

	for (int i = 0; i < solidN; i++)
	{
		solidMesh[i] = new triangle3dV[solidPolyCount[i]];
	}
	Solids.getPolyData_(solidN, solidMesh);


	txt*			solidTexture					= Solids.getTextureData();
	int				solidLightN						= Solids.getNSpotLights();
	vect3*			solidLightPosition				= Solids.getSpotLightPositions();
	vect3*			solidLightPositionTransformed	= new vect3[solidLightN];
	double*			solidLightIntensity				= Solids.getSpotLightIntensities();
	//int				solidSupportN					= 0;
	//triangle3dV*	solidSupports					= Solids.getSupport(&solidSupportN);


	int				actorN;
	triangle3dV**	actorMesh;
	int*			actorPolyCount;
	txt*			actorTexture;


	//Setting up player object
	Camera player_01( 1.0, 1.0, 1.5, 1.0, 0.05, PI * 0.5, PI, 0.0, PI * 0.5, 0.25, 999.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );
	Camera player_02( 1.0, 1.0, 1.5, 1.0, 0.05, PI * 0.5, PI, 0.0, PI * 0.5, 5.00, 999.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );
	

	Camera Player = player_01;
	//EventHandler Controls(Player.step, Player.turn, &Player, &Screen, solidSupportN, solidSupports);
	EventHandler Controls(Player.step, Player.turn, &Player, &Screen, 0, NULL);
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


	double waveAngle = 0.0; 
	double zPositionOld1, zPositionNew1;
	double zPositionOld2, zPositionNew2;
	double deltaZ1, deltaZ2;

	
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
		
		//if (Controls.gravityOn) { Player.sufferFromGravity(solidSupportN, solidSupports); }

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


		if (!Controls.isPaused)
		{
			transformMesh(Solids.getPolyCount(0), solidMesh[0], 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,
				0.0, 0.0, 0.25 * PI / 360.0);
		}

		solidLightPosition[0].z = sin(waveAngle * PI / 180.0) * 1.5 + 3.00;
		solidLightPosition[1].z = sin((waveAngle + 180.0) * PI / 180.0) * 1.5 + 3.00;

		zPositionOld1 = sin(waveAngle * PI / 180.0) * 1.5 + 2.0;
		zPositionOld2 = sin((waveAngle + 180.0) * PI / 180.0) * 1.5 + 2.0;
		zPositionNew1 = sin((waveAngle + 1.00) * PI / 180.0) * 1.5 + 2.0;
		zPositionNew2 = sin((waveAngle + 180.0 + 1.00) * PI / 180.0) * 1.5 + 2.0;
		deltaZ1 = zPositionNew1 - zPositionOld1;
		deltaZ2 = zPositionNew2 - zPositionOld2;
		
		
		/*transformMesh(Solids.getPolyCount(2), solidMesh[2], 1.0, 1.0, 1.0, 0.0, 0.0, deltaZ1,
			0.0, 0.0, 0.0);

		transformMesh(Solids.getPolyCount(3), solidMesh[3], 1.0, 1.0, 1.0, 0.0, 0.0, deltaZ2,
			0.0, 0.0, 0.0);*/

		waveAngle += 2.50;
		

		for (int i = 0; i < solidN; i++)									//For every solid
		{	
			int totalPoly = solidPolyCount[i];
			for (int k = 0; k < totalPoly; k++)
			{
				if (Player.polyFacingCamera(solidMesh[i][k]))			//Backface culling is performed here
				{
					Uint32 colour = solidMesh[i][k].colour;
					triangle3dV viewT = Player.world2viewT(playerPosition, solidMesh[i][k]);

					Player.illuminatePoly(lights, &viewT, solidMesh[i][k], Controls.gouraudShading);

					int nVert = Player.clipToFrustum(viewT, solidVertexList, solidUVList, Frustum);

					int textureID = solidMesh[i][k].texture;
					Player.currentTexture = solidTexture[textureID];

					Player.projectPoly(nVert, solidVertexList, solidUVList, colour, Screen, hRatio, vRatio, Controls.gouraudShading,
						Controls.visualiseDepth, solidLightN, solidLightPositionTransformed, solidLightIntensity, viewT);
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
		Actors.getPolyData_(actorN, actorMesh);


		for (int i = 0; i < actorN; i++)									//For every actor
		{	
			int totalPoly = actorPolyCount[i];
			for (int k = 0; k < totalPoly; k++)
			{
				if (Player.polyFacingCamera(actorMesh[i][k]))			//Backface culling is performed here
				{
					Uint32 colour = actorMesh[i][k].colour;
					triangle3dV viewT = Player.world2viewT(playerPosition, actorMesh[i][k]);

					Player.illuminatePoly(lights, &viewT, actorMesh[i][k], Controls.gouraudShading);

					int nVert = Player.clipToFrustum(viewT, actorVertexList, actorUVList, Frustum);

					int textureID = actorMesh[i][k].texture;
					Player.currentTexture = actorTexture[textureID];

					Player.projectPoly(nVert, actorVertexList, actorUVList, colour, Screen, hRatio, vRatio, Controls.gouraudShading,
						Controls.visualiseDepth, solidLightN, solidLightPositionTransformed, solidLightIntensity, viewT);
				}
			}
		}


		freePointerToPolyArray(actorMesh, actorN);

		delete[] actorPolyCount;
		delete[] actorTexture;

		
		if (Controls.showCrosshair) { Screen.drawCrosshair(2, 6, getColour(0, 127, 127, 127)); }
		if (Controls.showFPS && frameTime > 0) { Screen.displayFps((double)(30000.0 / frameTime), 1); }
		if (Controls.showPosition)
		{
			Screen.displayPosition((double)Player.x, 1, 8, getColour(0, 255, 127, 127));
			Screen.displayPosition((double)Player.y, 1, 7, getColour(0, 127, 255, 127));
			Screen.displayPosition((double)Player.z, 1, 6, getColour(0, 127, 127, 255));

			azmToShow = Player.azm * 180.0 / PI;
			if (azmToShow > 360.0) { azmToShow -= 360.0; }
			if (azmToShow < -360.0) { azmToShow += 360.0; }
			altToShow = Player.alt * 180.0 / PI - 180.0;
			if (altToShow > 360.0) { altToShow -= 360.0; }
			if (altToShow < -360.0) { altToShow += 360.0; }
			rolToShow = Player.rol * 180.0 / PI;
			if (rolToShow > 360.0) { rolToShow -= 360.0; }
			if (rolToShow < -360.0) { rolToShow += 360.0; }

			Screen.displayPosition(azmToShow, 1, 4, getColour(0, 255, 127, 127));
			Screen.displayPosition(altToShow, 1, 3, getColour(0, 127, 255, 127));
			Screen.displayPosition(rolToShow, 1, 2, getColour(0, 127, 127, 255));
		}
				
		SDL_UpdateTexture(texture, NULL, Screen.pixelBuffer, Screen.getWidth() * sizeof(Uint32));
		SDL_RenderClear(screen);

		SDL_RenderCopy(screen, texture, NULL, NULL);
		SDL_RenderPresent(screen);

		frameCounter++;
	}

	delete[] solidPolyCount;
	//delete[] solidSupports;

	freePointerToPolyArray(solidMesh, solidN);
	delete[] solidLightIntensity;
	delete[] solidLightPosition;
	delete[] solidLightPositionTransformed;

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
	renderer();

	return 0;
}