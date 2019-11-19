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

//#define _BEZIER_PATCH_
//#define _QUAKE_1_READER_
#define _STL_READER_

void editor()
{
	SDL_Window* window = SDL_CreateWindow("Software Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, EDITOR_WIDTH, EDITOR_HEIGHT, 0);

	SDL_Renderer* screen = SDL_CreateRenderer(window, -1, 0);

	SDL_Texture* sdl_texture = SDL_CreateTexture(screen, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, EDITOR_WIDTH, EDITOR_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);

	Canvas Screen(EDITOR_WIDTH, EDITOR_HEIGHT, 999.9);

	Camera Viewer(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.01, 999.0, EDITOR_WIDTH, EDITOR_HEIGHT, 0);

	ModelElementBuffer Drawing("test.wtf");

	Editor Builder(0.05, &Viewer, &Screen, &Drawing);

	MessagePump Controls(&Builder);

	Shapes Solids;

	while (!Controls.quit)
	{
		Screen.resetPixelBuffer();
		Screen.resetDepthBuffer();

		Builder.updateScreen();
		Controls.HandleUserEvents();

		//Screen.update();

		SDL_UpdateTexture(sdl_texture, nullptr, Screen.pixelBuffer, Screen.getWidth() * sizeof(Uint32));
		SDL_RenderClear(screen);
		SDL_RenderCopy(screen, sdl_texture, nullptr, nullptr);
		SDL_RenderPresent(screen);
	}

	//Screen.cleanUp();

	SDL_DestroyTexture(sdl_texture);
	SDL_DestroyRenderer(screen);
	SDL_DestroyWindow(window);

	IMG_Quit();

	SDL_Quit();
}


void software_renderer()
{
	SDL_Window* window = SDL_CreateWindow("Rendering Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	SDL_Renderer* screen = SDL_CreateRenderer(window, -1, 0);

	SDL_Texture* sdl_texture = SDL_CreateTexture(screen, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);

	Canvas Screen(SCREEN_WIDTH, SCREEN_HEIGHT, 999.9);

	Shapes Solids;

	Shapes Actors;

	Camera			Player(0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.1, PI * 0.5, 0.01, 999.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	EventHandler Controls(0.1f, 0.1f, 0.01f);

	Controls.torchIntensity = 10.0;

#ifdef _BEZIER_PATCH_

	LightSource lights[] = { LightSource(300.0, 60.0, 1.00) };

	Room room_01;
	SolidBody* room_ptr_1 = &room_01;
	room_01.setPosition(-5.0, -5.0, -5.0);
	room_01.setDimension(10.0, 10.0, 10.0);
	room_01.setSideOn(1, 1, 1, 1, 1, 1);
	room_01.setTexture(2, 3, 1, 1, 1, 1);
	room_01.setColour(getColour(0, 255, 0, 0), getColour(0, 255, 0, 0), getColour(0, 0, 255, 0), getColour(0, 0, 255, 0),
		getColour(0, 0, 0, 255), getColour(0, 0, 0, 255));
	room_01.setTextureScale(1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
	room_01.calculateMesh();
	Solids.addSolid(room_ptr_1);


	BezierPatch patch01(32, 3, getColour(0, 255, 127, 0));

	patch01.setPosition({ 0.0f, 4.0f, 0.0f, 1.0f });

	patch01.setControlPoint(0, { 0.0f, 0.0f, 0.0f, 1.0f });
	patch01.setControlPoint(1, { 2.0f, 0.0f, 2.0f, 1.0f });
	patch01.setControlPoint(2, { 8.0f, 0.0f, 0.0f, 1.0f });

	patch01.setControlPoint(3, { 0.0f, 0.0f, 2.0f, 1.0f });
	patch01.setControlPoint(4, { 2.0f, 0.0f, 3.0f, 1.0f });
	patch01.setControlPoint(5, { 8.0f, 0.0f, 2.0f, 1.0f });

	patch01.setControlPoint(6, { 0.0f, 0.0f, 4.0f, 1.0f });
	patch01.setControlPoint(7, { 2.0f, 0.0f, 4.0f, 1.0f });
	patch01.setControlPoint(8, { 8.0f, 0.0f, 4.0f, 1.0f });

	Solids.addSolid(&patch01);


	BezierPatch patch02(32, 6, getColour(0, 127, 127, 255));;

	patch02.setControlPoint(0, { 0.0f, 0.0f, 0.0f, 1.0f });
	patch02.setControlPoint(1, { 2.0f, 0.0f, 1.5f, 1.0f });
	patch02.setControlPoint(2, { 8.0f, 0.0f, 3.0f, 1.0f });

	patch02.setControlPoint(3, { 0.0f, 2.0f, 0.0f, 1.0f });
	patch02.setControlPoint(4, { 2.0f, 2.0f, 2.5f, 1.0f });
	patch02.setControlPoint(5, { 8.0f, 2.0f, 1.0f, 1.0f });

	patch02.setControlPoint(6, { 0.0f, 4.0f, 0.0f, 1.0f });
	patch02.setControlPoint(7, { 2.0f, 4.0f, 2.0f, 1.0f });
	patch02.setControlPoint(8, { 8.0f, 4.0f, 0.0f, 1.0f });

	Solids.addSolid(&patch02);


	BezierPatch patch03(32, 3, getColour(0, 127, 127, 255));;

	patch03.setPosition({ 20.0f, 20.0f, 0.0f, 1.0f });

	patch03.setControlPoint(0, { 0.0f, 0.0f, 0.0f, 1.0f });
	patch03.setControlPoint(1, { 2.0f, 0.0f, 0.5f, 1.0f });
	patch03.setControlPoint(2, { 4.0f, 0.0f, 0.0f, 1.0f });

	patch03.setControlPoint(3, { 0.0f, 2.0f, 0.5f, 1.0f });
	patch03.setControlPoint(4, { 2.0f, 2.0f, 2.0f, 1.0f });
	patch03.setControlPoint(5, { 4.0f, 2.0f, 0.5f, 1.0f });

	patch03.setControlPoint(6, { 0.0f, 4.0f, 0.0f, 1.0f });
	patch03.setControlPoint(7, { 2.0f, 4.0f, 0.5f, 1.0f });
	patch03.setControlPoint(8, { 4.0f, 4.0f, 0.0f, 1.0f });

	Solids.addSolid(&patch03);


	BezierPatch patch04(32, 4, getColour(0, 127, 127, 255));;

	patch04.setPosition({ 24.0f, 20.0f, 0.0f, 1.0f });

	patch04.setControlPoint(0, { 0.0f, 0.0f, 0.0f, 1.0f });
	patch04.setControlPoint(1, { 2.0f, 0.0f, 0.5f, 1.0f });
	patch04.setControlPoint(2, { 4.0f, 0.0f, 0.0f, 1.0f });

	patch04.setControlPoint(3, { 0.0f, 2.0f, 0.5f, 1.0f });
	patch04.setControlPoint(4, { 2.0f, 2.0f, 2.0f, 1.0f });
	patch04.setControlPoint(5, { 4.0f, 2.0f, 0.5f, 1.0f });

	patch04.setControlPoint(6, { 0.0f, 4.0f, 0.0f, 1.0f });
	patch04.setControlPoint(7, { 2.0f, 4.0f, 0.5f, 1.0f });
	patch04.setControlPoint(8, { 4.0f, 4.0f, 0.0f, 1.0f });

	Solids.addSolid(&patch04);


	BezierPatch patch05(32, 5, getColour(0, 127, 127, 255));;

	patch05.setPosition({ 20.0f, 24.0f, 0.0f, 1.0f });

	patch05.setControlPoint(0, { 0.0f, 0.0f, 0.0f, 1.0f });
	patch05.setControlPoint(1, { 2.0f, 0.0f, 0.5f, 1.0f });
	patch05.setControlPoint(2, { 4.0f, 0.0f, 0.0f, 1.0f });

	patch05.setControlPoint(3, { 0.0f, 2.0f, 0.5f, 1.0f });
	patch05.setControlPoint(4, { 2.0f, 2.0f, 2.0f, 1.0f });
	patch05.setControlPoint(5, { 4.0f, 2.0f, 0.5f, 1.0f });

	patch05.setControlPoint(6, { 0.0f, 4.0f, 0.0f, 1.0f });
	patch05.setControlPoint(7, { 2.0f, 4.0f, 0.5f, 1.0f });
	patch05.setControlPoint(8, { 4.0f, 4.0f, 0.0f, 1.0f });

	Solids.addSolid(&patch05);


	BezierPatch patch06(32, 6, getColour(0, 127, 127, 255));;

	patch06.setPosition({ 24.0f, 24.0f, 0.0f, 1.0f });

	patch06.setControlPoint(0, { 0.0f, 0.0f, 0.0f, 1.0f });
	patch06.setControlPoint(1, { 2.0f, 0.0f, 0.5f, 1.0f });
	patch06.setControlPoint(2, { 4.0f, 0.0f, 0.0f, 1.0f });

	patch06.setControlPoint(3, { 0.0f, 2.0f, 0.5f, 1.0f });
	patch06.setControlPoint(4, { 2.0f, 2.0f, 2.0f, 1.0f });
	patch06.setControlPoint(5, { 4.0f, 2.0f, 0.5f, 1.0f });

	patch06.setControlPoint(6, { 0.0f, 4.0f, 0.0f, 1.0f });
	patch06.setControlPoint(7, { 2.0f, 4.0f, 0.5f, 1.0f });
	patch06.setControlPoint(8, { 4.0f, 4.0f, 0.0f, 1.0f });

	Solids.addSolid(&patch06);

#endif //_BEZIER_PATCH_



#ifdef _QUAKE_1_READER_

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

#endif //_QUAKE_1_READER_



#ifdef _STL_READER_

	SolidSTL test_body(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, getColour(0, 255, 255, 255), 3, "naval_gun.stl");
	test_body.readSTLfile();
	//test_body.smoothSurfaces();
	Solids.addSolid(&test_body);

#endif //_STL_READER_

	SDL_Surface* textures[] = { IMG_Load("Textures/blue.jpg"),			//00
								IMG_Load("Textures/wolf001.jpg"),		//01
								IMG_Load("Textures/wolf002.jpg"),		//02
								IMG_Load("Textures/mosaic001.jpg"),		//03
								IMG_Load("Textures/brick001.jpg"),		//04
								IMG_Load("Textures/concrete001.jpg"),	//05
								IMG_Load("Textures/timber.jpg") };		//06

	Solids.textureLoader(sizeof(textures) / sizeof(SDL_Surface*), textures);

	Actors.textureLoader(sizeof(textures) / sizeof(SDL_Surface*), textures);

	Renderer Game(&Solids, &Actors, &LightSource(300.0, 45.0, 0.95), &Player, &Controls);

	Game.updateShadowVolumes(solid);

	Game.updateShadowVolumes(actor);


	//Game.updateBoundingBoxes(solid, 0.5);
	//Solids.activateBBox(0);
	//Solids.activateBBox(1);
	//Solids.activateBBox(2);

	std::cout << "Game loop starts here..." << std::endl;

	while (!Controls.quit)
	{
		Game.calculateFrametime();

		Controls.HandleUserEvents();

		Screen.resetPixelBuffer();

		Screen.resetDepthBuffer();

		Game.updateCameraPosition();

		Controls.ceaseMotion();

		//Game.updateShadowVolumes(actor);

		Game.renderEntities(solid, Screen.pixelBuffer, Screen.depthBuffer);	

		if (!Controls.isPaused) { Game.updateEntities(actor); }

		Game.renderEntities(actor, Screen.pixelBuffer, Screen.depthBuffer);

		Game.displayStats(Controls.showCrosshair, Controls.showFPS, Controls.showPosition, Controls.showPolyN, Screen);

		Game.resetPolyCounter();

		//Screen.update();


		SDL_UpdateTexture(sdl_texture, nullptr, Screen.pixelBuffer, Screen.getWidth() * sizeof(Uint32));

		SDL_RenderClear(screen);

		SDL_RenderCopy(screen, sdl_texture, nullptr, nullptr);

		SDL_RenderPresent(screen);

		Game.updateFrameCounter();
	}

	for (int i = 0; i < (sizeof(textures) / sizeof(SDL_Surface*)); i++){ SDL_FreeSurface(textures[i]); }

	//Screen.cleanUp();

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