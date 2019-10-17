#define _CRT_SECURE_NO_DEPRECATE//


#include <SDL/SDL.h>
#include <SDLImage/SDL_image.h>
#include <GLEW/glew.h>
#include <GLM/glm.hpp>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>
#include "Definitions.h"
#include "Camera.h"
#include "Canvas.h"
#include "LightSource.h"
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
#include "MessagePump.h"
#include "Editor.h"
#include "ModelElementBuffer.h"
#include "Renderer.h"
#include "BSP3Loader.h"
#include "BSP1Loader.h"
#include "BezierPatch.h"
#include "OpenGLCanvas.h"
#include "OpenGLCamera.h"
#include "OpenGLTransform.h"
#include "OpenGLAdapter.h"
#include "OpenGLMesh.h"
#include "OpenGLShader.h"

void editor()
{
	SDL_Window* window = SDL_CreateWindow("Software Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, EDITOR_WIDTH, EDITOR_HEIGHT, 0);

	SDL_Renderer* screen = SDL_CreateRenderer(window, -1, 0);

	SDL_Texture* sdl_texture = SDL_CreateTexture(screen, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, EDITOR_WIDTH, EDITOR_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);

	Camera Viewer(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.01, 999.0, EDITOR_WIDTH, EDITOR_HEIGHT, 0);

	Canvas Screen(EDITOR_WIDTH, EDITOR_HEIGHT, 999.9);

	ModelElementBuffer Drawing("test.wtf");

	Editor Builder(0.05, &Viewer, &Screen, &Drawing);

	MessagePump Controls(&Builder);

	//SDL_Surface* textures[] = { IMG_Load("Textures/blue.jpg"),			//00
	//							IMG_Load("Textures/wolf001.jpg"),			//01
	//							IMG_Load("Textures/wolf002.jpg"),			//02
	//							IMG_Load("Textures/mosaic001.jpg"),			//03
	//							IMG_Load("Textures/brick001.jpg"),			//04
	//							IMG_Load("Textures/concrete001.jpg"),		//05
	//							IMG_Load("Textures/timber.jpg") };			//06

	Shapes Solids;
	Solids.addSolid(new SolidSphere(1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, getColour(0, 127, 127, 255), 5, 5.0, 36));

	while (!Controls.quit)
	{
		Screen.resetPixelBuffer();
		Screen.resetDepthBuffer();

		Builder.updateScreen();
		Controls.HandleUserEvents();

		SDL_UpdateTexture(sdl_texture, nullptr, Screen.pixelBuffer, Screen.getWidth() * sizeof(Uint32));
		SDL_RenderClear(screen);
		SDL_RenderCopy(screen, sdl_texture, nullptr, nullptr);
		SDL_RenderPresent(screen);
	}

	printf("%d points added in total...\n", Drawing.getVertex3BufferSize());

	SDL_DestroyTexture(sdl_texture);

	SDL_DestroyRenderer(screen);

	SDL_DestroyWindow(window);

	IMG_Quit();

	SDL_Quit();
}


void software_renderer()
{
	SDL_Window*		window			= SDL_CreateWindow("Rendering Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	SDL_Renderer*	screen			= SDL_CreateRenderer(window, -1, 0);

	SDL_Texture*	sdl_texture		= SDL_CreateTexture(screen, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);

	Canvas Screen(SCREEN_WIDTH, SCREEN_HEIGHT, 999.9);

	Shapes Solids;

	Shapes Actors;

	#include "bbtest.txt"
	/**/
	//Camera	Player(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.1, PI * 0.5, 0.1, 99999.9, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	//LightSource lights[] = { LightSource(115.0, 45.0, 1.25) };
	//BSP1Loader quakeMap("dm6.bsp", { 0.01, 0.01, 0.01, 1.0 });
	//quakeMap.readData();
	//SolidBody* map = &quakeMap;
	//Solids.addSolid(map);
	//int nTxt = quakeMap.getTotalText();
	//txt* quakeTextures = new txt[nTxt];
	//for (int i = 0; i < nTxt; i++)
	//{
	//	quakeTextures[i] = quakeMap.getTextureData(i);
	//}
	//Solids.textureLoaderQ(nTxt, quakeTextures);
	/**/

	//EventHandler		Controls(Player.step, Player.turn, &Player, &Screen);

	EventHandler Controls(0.1f, 0.1f, 0.01f);

	Controls.torchIntensity = 10.0;

	Solids.textureLoader(sizeof(textures) / sizeof(SDL_Surface*), textures);

	Actors.textureLoader(sizeof(textures) / sizeof(SDL_Surface*), textures);

	Renderer Game(&Solids, &Actors, &LightSource(300.0, 45.0, 0.95), &Player, &Controls);

	Game.updateShadowVolumes(solid);

	Game.updateShadowVolumes(actor);


	//Game.updateBoundingBoxes(solid, 0.5);
	//Solids.activateBBox(0);
	//Solids.activateBBox(1);
	//Solids.activateBBox(2);


	while (!Controls.quit)
	{
		Game.calculateFrametime();

		Controls.HandleUserEvents();

		Screen.resetPixelBuffer();

		Screen.resetDepthBuffer();

		//Game.checkCameraCollision();

		Game.updateCameraPosition();

		Controls.ceaseMotion();

		Game.updateShadowVolumes(actor);

		Game.renderEntities(solid, Screen.pixelBuffer, Screen.depthBuffer);	

		if (!Controls.isPaused) { Game.updateEntities(actor); }

		Game.renderEntities(actor, Screen.pixelBuffer, Screen.depthBuffer);

		//Game.renderPoints(nVert, pointCloud, Screen.pixelBuffer);

		Game.displayStats(Controls.showCrosshair, Controls.showFPS, Controls.showPosition, Controls.showPolyN, Screen);

		Game.resetPolyCounter();
				
		SDL_UpdateTexture(sdl_texture, nullptr, Screen.pixelBuffer, Screen.getWidth() * sizeof(Uint32));

		SDL_RenderClear(screen);

		SDL_RenderCopy(screen, sdl_texture, nullptr, nullptr);

		SDL_RenderPresent(screen);

		Game.updateFrameCounter();
	}

	for (int i = 0; i < (sizeof(textures) / sizeof(SDL_Surface*)); i++){ SDL_FreeSurface(textures[i]); }

	//delete[] pointCloud;

	SDL_DestroyTexture(sdl_texture);

	SDL_DestroyRenderer(screen);

	SDL_DestroyWindow(window);

	IMG_Quit();

	SDL_Quit();
}


void opengl_renderer()
{
	//glm::vec4* pixelBuffer = new glm::vec4[SCREEN_WIDTH * SCREEN_HEIGHT];	   
	//for (auto i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)					   
	//{																		   
	//	pixelBuffer[i] = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);					   
	//}																
	//for (int i = 100000; i < 120000; i++)
	//{
	//	pixelBuffer[i] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	//}
					   


	OpenGLCanvas Screen(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Renderer");
	OpenGLCamera Player(glm::vec3(0.0f, 0.0f, 0.0f), 70.0f, Screen.getAspect(), 0.1f, 100.0f);
	OpenGLTransform View(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	EventHandler Controls(0.01f, 0.01f, 0.001f);

	Shapes Solids;
	Shapes Actors;

	//Solids.addSolid(new SolidCube(1.0, 1.0, 1.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, getColour(0, 255, 0, 0), 1, 2.0));
	//Solids.addSolid(new SolidSphere(1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, getColour(0, 127, 127, 255), 1, 3.0, 36));


	BSP1Loader quakeMap("dm6.bsp", { 0.01, 0.01, 0.01, 1.0 });
	quakeMap.readData();
	SolidBody* map = &quakeMap;
	Solids.addSolid(map);
	int nTxt = quakeMap.getTotalText();
	txt* quakeTextures = new txt[nTxt];
	for (int i = 0; i < nTxt; i++)
	{
		quakeTextures[i] = quakeMap.getTextureData(i);
	}
	Solids.textureLoaderQ(nTxt, quakeTextures);

	//#include "glbbtest.txt"

	OpenGLAdapter VertexPump(&Solids, &Actors);
	unsigned int nVert = VertexPump.getNVert(solid, 0);
	Vertex* vertices = new Vertex[nVert];
	VertexPump.getVertices(solid, 0, vertices);
	OpenGLMesh mesh_01(vertices, nVert);

	OpenGLShader shader_01("./Shaders/basicShader");

	//LightSource lights[] = { LightSource(150.0, 45.0, 0.95) };

	SDL_Surface* textures[] = { IMG_Load("Textures/blue.jpg"),
								IMG_Load("Textures/timber.jpg") };

	while(!Controls.quit)
	{
		Screen.clear(0.5f, 0.5f, 1.0f, 0.0f);

		shader_01.bind();
		shader_01.update(View, Player);
		mesh_01.drawMesh();

		//glDrawPixels(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_INT, pixelBuffer);
		
		Controls.HandleUserEvents();
		View.updateAll(Controls);
		Controls.ceaseMotion();

		Screen.update();
	}

	delete[] vertices;

	for (int i = 0; i < (sizeof(textures) / sizeof(SDL_Surface*)); i++) { SDL_FreeSurface(textures[i]); }
}


int main(int argc, char** argv)
{
	std::cout << "Do you want to use the Software Renderer <S> the OpenGL Renderer <O> or the Editor <E> ?" << std::endl;
	char userInput;
	std::cin >> userInput;

	switch (userInput)
	{
	case 's':
	case 'S':
		software_renderer();
		break;
	case 'o':
	case 'O':
		opengl_renderer();
		break;
	case 'e':
	case 'E':
		editor();
		break;
	default:

		break;
	}

	return 0;
}