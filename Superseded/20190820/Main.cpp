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

#include "Space.h"
#include "Room.h"

#include "EventHandler.h"


void renderer()
{
	Shapes Solids;

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

	/*int gridXSize = 5, gridYSize = 5, gridZSize = 5;
	for (int x = 0; x < gridXSize; x++)
	{
		for (int y = 0; y < gridYSize; y++)
		{
			for (int z = 0; z < gridZSize; z++)
			{
				Solids.addSolid(new SolidCube(1.0, 1.0, 1.0, x * 500.0, y * 500.0, z * 500.0, 0.0, 0.0, 0.0, getColour(0, 255, 255, 255), 0, 1.0));
			}
		}
	}*/
	

		
	int nEntities = Solids.getNEntities();
	//int* polyCountEntities = Solids.getPolyCountEntities();
	triangle3dV** meshTriangles = Solids.getPolyData();
	int* polyCountEntities = Solids.getPolyCountEntities();
	txt* meshTextures = Solids.getTextureData();
	int nSpotLights = Solids.getNSpotLights();
	vect3* spotLightPositions = Solids.getSpotLightPositions();
	vect3* spotLightPositionsTransformed = new vect3[nSpotLights];
	double* spotLightIntensities = Solids.getSpotLightIntensities();


	/*int texture_id = 0;
	Uint32* textures_ = meshTextures[texture_id].pixels;
	FILE* f = fopen("fucked_texture.ppm", "w");
	if (f == NULL)
	{
		printf("Could not write to file...\n");
	}
	else
	{
		fprintf(f, "P3\n");
		fprintf(f, "%d %d\n", meshTextures[texture_id].w, meshTextures[texture_id].h);
		fprintf(f, "255\n");

		Uint32 currentPixel;
		Uint8 red, green, blue;
		byte redShort, greenShort, blueShort;
		int blueStep, greenStep, redStep;

		redStep = 128;
		greenStep = 128;
		blueStep = 128;
		
		//SDL_Surface* image = SDL_ConvertSurfaceFormat(textures_[i], SDL_PIXELFORMAT_ARGB8888, 0);
		//SDL_FreeSurface(textures_[i]);
		Uint32* texturePixels = textures_;

		int txtrW = meshTextures[texture_id].w, txtrH = meshTextures[texture_id].h;

		for (int j = 0; j < txtrH; j++)
		{
			for (int i = 0; i < txtrW; i++)
			{
				currentPixel = texturePixels[j * txtrW + i];
				red = (currentPixel << 8) >> 24;
				green = (currentPixel << 16) >> 24;
				blue = (currentPixel << 24) >> 24;

				redShort = (red / redStep) * redStep;
				greenShort = (green / greenStep) * greenStep;
				blueShort = (blue / blueStep) * blueStep;

				fprintf(f, "%d %d %d\n", redShort, greenShort, blueShort);
			}
		}

		fclose(f);
	}*/


	int nSupport = 0;
	//printf("\n\nNow calculating supports...\n");
	//triangle3dV* supports = Solids.getSupport(&nSupport);

	//Setting up player object
	Camera player_01( 1.0, 1.0, 1.5, 1.0, 0.05, PI * 0.5, PI, 0.0, PI * 0.5, 0.25, 999.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );
	//Camera player_01(13000.0, 0.0, 0.0, 10.0, 0.05, -PI * 0.5, 0.0, 0.0, PI* 0.5, 1.0, 99999.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	

	Camera Player = player_01;
	//EventHandler Controls(Player.step, Player.turn, &Player, &Screen, nSupport, supports);
	EventHandler Controls(Player.step, Player.turn, &Player, &Screen, nSupport, NULL);
	ViewVolume Frustum;
	Frustum.initFrustum(Player.getFovH(), Player.getFovV(), Player.zNear, Player.zFar);
	

	//Initialising vertex list
	vect3 vertexList[MAXCLIPVERTS];
	textCoord uvList[MAXCLIPVERTS];
	for (int v = 0; v < MAXCLIPVERTS; v++) { vertexList[v] = { 0.0, 0.0, 0.0, 0.0 }; uvList[v] = { 0.0, 0.0 }; }
	   

	double azmToShow, altToShow, rolToShow;
	const double playerSupport = 0.0;


	clock_t oldTime, newTime, frameTime;
	oldTime = 0;
	newTime = 0;
	frameTime = 0;
	int frameCounter = 0;
	

	//double rotTestX = 3.0, rotTestY = 2.0, rotTestZ = 1.0;
	//int liftCurrent = 0, liftMax = 100;
	//double liftStep = 1.0;

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
		
		//if (Controls.gravityOn) { Player.sufferFromGravity(nSupport, supports); }
		if (Controls.gravityOn) { Player.sufferFromGravity(nSupport, NULL); }

		Player.updatePosition(Controls.turnH, Controls.turnV, Controls.tiltP, Controls.moveP, Controls.strafeP, Controls.riseP);

		Controls.ceaseMotion();

		transform3d playerPosition;
		playerPosition.sinAzm = sin(Player.azm + PI * 0.5);
		playerPosition.cosAzm = cos(Player.azm + PI * 0.5);
		playerPosition.sinAlt = sin(Player.alt + PI * 0.5);
		playerPosition.cosAlt = cos(Player.alt + PI * 0.5);
		playerPosition.sinRol = sin(Player.rol);
		playerPosition.cosRol = cos(Player.rol);



		/**/
		for (int lmp = 0; lmp < nSpotLights; lmp++)
		{
			spotLightPositionsTransformed[lmp] = Player.world2view(playerPosition, spotLightPositions[lmp]);
		}
		/**/

		if (!Controls.isPaused)
		{
			transformMesh(Solids.getPolyCount(0), meshTriangles[0], 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,
				0.0, 0.0, 0.25 * PI / 360.0);
		}

		spotLightPositions[0].z = sin(waveAngle * PI / 180.0) * 1.5 + 3.00;
		spotLightPositions[1].z = sin((waveAngle + 180.0) * PI / 180.0) * 1.5 + 3.00;

		zPositionOld1 = sin(waveAngle * PI / 180.0) * 1.5 + 2.0;
		zPositionOld2 = sin((waveAngle + 180.0) * PI / 180.0) * 1.5 + 2.0;
		zPositionNew1 = sin((waveAngle + 1.00) * PI / 180.0) * 1.5 + 2.0;
		zPositionNew2 = sin((waveAngle + 180.0 + 1.00) * PI / 180.0) * 1.5 + 2.0;
		deltaZ1 = zPositionNew1 - zPositionOld1;
		deltaZ2 = zPositionNew2 - zPositionOld2;


		transformMesh(Solids.getPolyCount(2), meshTriangles[2], 1.0, 1.0, 1.0, 0.0, 0.0, deltaZ1,
			0.0, 0.0, 0.0);

		transformMesh(Solids.getPolyCount(3), meshTriangles[3], 1.0, 1.0, 1.0, 0.0, 0.0, deltaZ2,
			0.0, 0.0, 0.0);

		waveAngle += 2.50;


		
		//if (waveAngle >= 900.0) { waveAngle = 0.0; }
		


		for (int i = 0; i < nEntities; i++)									//For every solid
		{	
			int totalPoly = polyCountEntities[i];
			for (int k = 0; k < totalPoly; k++)
			{
				if (Player.polyFacingCamera(meshTriangles[i][k]))			//Backface culling is performed here
				{
					Uint32 colour = meshTriangles[i][k].colour;
					triangle3dV viewT = Player.world2viewT(playerPosition, meshTriangles[i][k]);

					Player.illuminatePoly(lights, &viewT, meshTriangles[i][k], Controls.gouraudShading);

					int nVert = Player.clipToFrustum(viewT, vertexList, uvList, Frustum);

					int textureID = meshTriangles[i][k].texture;
					Player.currentTexture = meshTextures[textureID];

					Player.projectPoly(nVert, vertexList, uvList, colour, Screen, hRatio, vRatio, Controls.gouraudShading,
						Controls.visualiseDepth, nSpotLights, spotLightPositionsTransformed, spotLightIntensities, viewT);
				}
			}
		}
		
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

	delete[] polyCountEntities;
	//delete[] supports;

	for (int i = 0; i < nEntities; i++)
	{
		delete[] meshTriangles[i];
	}
	delete[] meshTriangles;
	delete[] spotLightIntensities;
	delete[] spotLightPositions;
	delete[] spotLightPositionsTransformed;

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
	
	vect2 A, B, R;
	polyNode P, Q;

	A.x = 4.00;		A.y = 6.00;		A.u = 4.00;		A.v = 6.00;
	B.x = 18.00;	B.y = 9.00;		B.u = 18.00;	B.v = 9.00;
	P.x = 9.00;		P.y = 13.00;	P.s = 0;
	Q.x = 12.00;	Q.y = 28.00;	Q.s = 0;

	//A.x = 4.00;		A.y = 6.00;		A.u = 4.00;		A.v = 6.00;
	//B.x = 4.00;		B.y = 9.00;		B.u = 18.00;	B.v = 9.00;
	//P.x = 2.00;		P.y = 1.00;		P.s = 0;
	//Q.x = 2.00;		Q.y = 8.00;		Q.s = 0;

	R.x = 0.0; R.y = 0.0; R.u = 0.0; R.v = 0.0;

	if (iSect2dLine(A, B, P, Q, &R))
	{
		printf("x: %.2f\ty: %.2f\n", R.x, R.y);
	}

	

	/*
	polygon4uv testPoly;

	testPoly.points[0].x = 0.0;
	testPoly.points[0].y = 0.0;
	testPoly.points[0].u = 0.0;
	testPoly.points[0].v = 0.0;

	testPoly.points[1].x = 2.0;
	testPoly.points[1].y = 0.0;
	testPoly.points[1].u = 2.0;
	testPoly.points[1].v = 0.0;

	testPoly.points[2].x = 2.0;
	testPoly.points[2].y = 1.0;
	testPoly.points[2].u = 2.0;
	testPoly.points[2].v = 1.0;

	testPoly.points[3].x = 0.0;
	testPoly.points[3].y = 1.0;
	testPoly.points[3].u = 0.0;
	testPoly.points[3].v = 1.0;

	testPoly.leftChild = testPoly.rightChild = NULL;

	if (checkPolygonForSplitting(testPoly, { 0.25, -1.0, 0 }, { 0.25, 5.0, 0 }))
	{
		printf("Polygon needs to be split!\n");
	}
	else { printf("Polygon doesn't need to be split!\n"); }
	*/


	renderer();

	return 0;
}