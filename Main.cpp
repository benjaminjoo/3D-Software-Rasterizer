#define _CRT_SECURE_NO_DEPRECATE//


#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "Definitions.h"
#include "Camera.h"
#include "Canvas.h"
#include "LightSource.h"
#include "SpotLight.h"
#include "Shapes.h"
#include "SolidBody.h"
#include "SolidCube.h"
#include "SolidPrism.h"
#include "SolidCylinder.h"
#include "SolidCone.h"
#include "SolidSphere.h"
#include "SolidRevolved.h"
#include "Room.h"
#include "SolidSTL.h"
#include "EventHandler.h"
#include "Renderer.h"
#include "BSP3Loader.h"
#include "BSP1Loader.h"
#include "RayTracer.h"


void gameplay(char* mapName)
{
	SDL_Window*		window			= SDL_CreateWindow("Rendering Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	SDL_Renderer*	screen			= SDL_CreateRenderer(window, -1, 0);

	SDL_Texture*	sdl_texture		= SDL_CreateTexture(screen, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	Canvas Screen(SCREEN_WIDTH, SCREEN_HEIGHT, getColour(0, 255, 255, 255), 999.9);

	//Canvas* Screen = new Canvas(SCREEN_WIDTH, SCREEN_HEIGHT, getColour(0, 255, 255, 255), 999.9);

	Shapes Solids;

	Shapes Actors;



	//Camera			Player(3.0, 12.0, 1.5, 0.0, 0.0, 0.0, 0.1, 0.1, PI * 0.5, 1.00, 999.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	//EventHandler		Controls(Player.step, Player.turn, &Player, &Screen);

	#include "quake1map.txt"
	//#include "quake3map.txt"
	//#include "rooms.txt"
	//#include "test.txt"
	//#include "bbtest.txt"
	RayTracer PoshCamera(SCREEN_WIDTH, SCREEN_HEIGHT, &Player, &Screen, &Solids);
	EventHandler		Controls(Player.step, Player.turn, &Player, &Screen, &PoshCamera);
	Controls.torchIntensity = 10.0;

	//#include "st_loo.txt"
	//#include "meeting_room.txt"
	//#include "quake.txt"
	//#include "solar_system.txt"
	//#include "vader.txt"
	//#include "gun.txt"	
	//#include "rooms2.txt"
	//#include "bodies.txt"



	//Solids.textureLoader(sizeof(textures) / sizeof(SDL_Surface*), textures);

	//Actors.textureLoader(sizeof(textures) / sizeof(SDL_Surface*), textures);

	Renderer Game(&Solids, &Actors, &LightSource(115.0, 45.0, 0.95), &Player, &Controls);

	Game.updateShadowVolumes(solid);

	Game.updateShadowVolumes(actor);


	//Game.updateBoundingBoxes(solid, 0.5);
	//Solids.activateBBox(0);
	//Solids.activateBBox(1);


	while (!Controls.quit)
	{
		Game.calculateFrametime();

		Controls.HandleUserEvents();

		Screen.resetPixelBuffer();

		Screen.resetDepthBuffer();

		//Game.checkCameraCollision();

		Game.updateCameraPosition(Controls.turnH, Controls.turnV, Controls.tiltP, Controls.moveP, Controls.strafeP, Controls.riseP, true);

		Controls.ceaseMotion();

		Game.updateShadowVolumes(actor);

		Game.renderEntities(solid, Screen.pixelBuffer, Screen.depthBuffer);	

		if (!Controls.isPaused) { Game.updateEntities(actor); }

		Game.renderEntities(actor, Screen.pixelBuffer, Screen.depthBuffer);

		//PoshCamera.calculatePixel();

		//Game.renderLines(nLine, linePile, Screen.pixelBuffer, Screen.depthBuffer);

		//Game.renderPoints(nVert, pointCloud, Screen.pixelBuffer);

		Game.displayStats(Controls.showCrosshair, Controls.showFPS, Controls.showPosition, Controls.showPolyN, Screen);

		Game.resetPolyCounter();
				
		SDL_UpdateTexture(sdl_texture, nullptr, Screen.pixelBuffer, Screen.getWidth() * sizeof(Uint32));

		SDL_RenderClear(screen);

		SDL_RenderCopy(screen, sdl_texture, nullptr, nullptr);

		SDL_RenderPresent(screen);

		Game.updateFrameCounter();
	}

	//for (int i = 0; i < (sizeof(textures) / sizeof(SDL_Surface*)); i++){ SDL_FreeSurface(textures[i]); }

	//delete[] pointCloud;
	//delete[] linePile;

	SDL_DestroyTexture(sdl_texture);

	SDL_DestroyRenderer(screen);

	SDL_DestroyWindow(window);

	IMG_Quit();

	SDL_Quit();
}



int main(int argc, char** argv)
{
	if (argc > 1)
	{
		gameplay(argv[1]);
	}
	else
	{
		char map_name[] = "dm6.bsp";
		gameplay(map_name);
	}

	return 0;
}