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
#include "SolidModel.h"
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
#include "Pong.h"
#include "Player.h"


void editor()
{
	auto Screen		= std::make_shared<Canvas>("Editor", EDITOR_WIDTH, EDITOR_HEIGHT, 999.9f);

	auto Viewer		= std::make_shared<Camera>(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.01f, 999.0f, EDITOR_WIDTH, EDITOR_HEIGHT, 0);

	auto Drawing	= std::make_shared<ModelElementBuffer>("test.wtf");

	auto Builder	= std::make_shared<Editor>(0.05f, Viewer, Screen, Drawing);

	auto Controls	= std::make_shared<MessagePump>(Builder);

	Shapes Solids;

	while (!Controls->quit)
	{
		Screen->resetPixelBuffer();
		Screen->resetDepthBuffer();

		Builder->updateScreen();
		Controls->HandleUserEvents();

		Screen->update();
	}

	Screen->cleanUp();

	IMG_Quit();

	SDL_Quit();
}


void pong3d()
{
	auto Screen		= std::make_shared<Canvas>("Pong 3D", SCREEN_WIDTH, SCREEN_HEIGHT, 999.9f);

	auto Eye		= std::make_shared<Camera>(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.1f, PI * 0.5f, 0.01f, 999.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	auto Controls	= std::make_shared<EventHandler>(0.1f, 0.1f, 0.01f);

	auto Sun		= std::make_shared<LightSource>(300.0f, 45.0f, 0.95f);

	auto Hero		= std::make_shared<Player>(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 100, 100);

	auto Ball		= std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 5.0f, 2.125f, 15.0f, 0.0f, 0.0f, 0.0f, 0xffffffff, 1, 0.3f, 24);
	
	auto Game		= std::make_shared<Pong>(Screen, Eye, Controls, Sun, Hero, Ball);

	auto pedestal	= std::make_shared<SolidPrism>(5.0f, 5.0f, 0.0f, 0x0000ff00, 20.0f, 30.0f, 5.0f);
	auto wall		= std::make_shared<SolidPrism>(5.0f, 19.5f, 5.0f, 0x00ff0000, 20.0f, 1.0f, 7.5f);
	auto dome		= std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 15.0f, 12.5f, 5.0f, 0.0f, 0.0f, 0.0f, 0xffffff00, 1, 5.0f, 24);
	auto box		= std::make_shared<Room>(0.0f, 0.0f, 0.0f, 30.0f, 40.0f, 20.0f);
	box->calculateMesh();

	Game->addEntity(box);
	Game->addEntity(pedestal);
	Game->addEntity(wall);
	Game->addEntity(dome);
	
	Ball->setInMotion();
	Ball->setVelocity({ 0.2f, 0.2f, -0.2f, 1.0f });

	Game->addTexture(IMG_Load("Textures/blue.jpg"));
	Game->addTexture(IMG_Load("Textures/wolf001.jpg"));
	Game->addTexture(IMG_Load("Textures/wolf002.jpg"));
	Game->addTexture(IMG_Load("Textures/mosaic001.jpg"));
	Game->addTexture(IMG_Load("Textures/brick001.jpg"));
	Game->addTexture(IMG_Load("Textures/concrete001.jpg"));
	Game->addTexture(IMG_Load("Textures/timber.jpg"));

	Game->loadProjectile(150);

	Game->buildMesh();

	while (!Controls->quit)
	{
		Game->updateAll();
	}

	Game->destroyMesh();

	Screen->cleanUp();

	IMG_Quit();

	SDL_Quit();
}



void software_renderer(bool exportFile, const std::string& fileName)
{

//#define _CUBE_
//#define _BEZIER_PATCH_
//#define _QUAKE_1_READER_
#define _STL_READER_

	auto Screen = std::make_shared<Canvas>("Software Renderer", SCREEN_WIDTH, SCREEN_HEIGHT, 999.9);

	auto Solids = std::make_shared<Shapes>();

	auto Actors = std::make_shared<Shapes>();

	auto Eye	= std::make_shared<Camera>(0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.1, PI * 0.5, 0.01, 999.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	auto Controls = std::make_shared<EventHandler>(0.1f, 0.1f, 0.01f);
	Controls->torchIntensity = 10.0;

	auto Sun = std::make_shared<LightSource>(300.0, 45.0, 0.95);

#ifdef _CUBE_

	LightSource lights[] = { LightSource(300.0, 60.0, 1.00) };
	SolidCube cube_01;
	cube_01.setTexture(2);
	Solids->addSolid(&cube_01);

#endif //_CUBE_


#ifdef _BEZIER_PATCH_

	LightSource lights[] = { LightSource(300.0, 60.0, 1.00) };

	Room room_01(-5.0, -5.0, -5.0, 10.0, 10.0, 10.0);
	SolidBody* room_ptr_1 = &room_01;
	room_01.setTexture(2, 3, 1, 1, 1, 1);
	room_01.calculateMesh();
	Solids->addSolid(room_ptr_1);


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

	Solids->addSolid(&patch01);


	BezierPatch patch02(32, 6, getColour(0, 127, 127, 255));

	patch02.setPosition({ -20.0f, -10.0f, 0.0f, 0.0f });

	patch02.setControlPoint(0, { 0.0f, -0.5f, 0.0f, 1.0f });
	patch02.setControlPoint(1, { 2.0f, 0.0f, 1.5f, 1.0f });
	patch02.setControlPoint(2, { 8.0f, 0.5f, 3.0f, 1.0f });

	patch02.setControlPoint(3, { 0.0f, 2.0f, 0.0f, 1.0f });
	patch02.setControlPoint(4, { 2.0f, 2.0f, 2.5f, 1.0f });
	patch02.setControlPoint(5, { 8.0f, 2.0f, 1.0f, 1.0f });

	patch02.setControlPoint(6, { 0.0f, 4.5f, 0.0f, 1.0f });
	patch02.setControlPoint(7, { 2.0f, 4.0f, 2.0f, 1.0f });
	patch02.setControlPoint(8, { 8.0f, 3.5f, 0.0f, 1.0f });

	Solids->addSolid(&patch02);


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

	Solids->addSolid(&patch03);


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

	Solids->addSolid(&patch04);


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

	Solids->addSolid(&patch05);


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

	Solids->addSolid(&patch06);

#endif //_BEZIER_PATCH_


#ifdef _QUAKE_1_READER_

	BSP1Loader quakeMap("dm6.bsp", { 0.01, 0.01, 0.01, 1.0 });
	quakeMap.readData();
	SolidBody* map = &quakeMap;
	Solids->addSolid(map);
	int nTxt = quakeMap.getTotalText();
	txt* quakeTextures = new txt[nTxt];
	for (int i = 0; i < nTxt; i++)
	{
		quakeTextures[i] = quakeMap.getTextureData(i);
	}
	Solids->textureLoaderQ(nTxt, quakeTextures);

#endif //_QUAKE_1_READER_


#ifdef _STL_READER_

	SolidSTL test_body(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, getColour(0, 255, 255, 255), 3, "cockpit.stl");
	test_body.readSTLfile();
	test_body.smoothSurfaces();
	Solids->addSolid(&test_body);

#endif //_STL_READER_

	SolidModel test_model(10.0f, 10.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, getColour(0, 127, 127, 255), 0);
	
	if (exportFile)
	{
		test_model.readModelFile(fileName);
		Solids->addSolid(&test_model);
	}


	SDL_Surface* textures[] = { IMG_Load("Textures/blue.jpg"),			//00
								IMG_Load("Textures/wolf001.jpg"),		//01
								IMG_Load("Textures/wolf002.jpg"),		//02
								IMG_Load("Textures/mosaic001.jpg"),		//03
								IMG_Load("Textures/brick001.jpg"),		//04
								IMG_Load("Textures/concrete001.jpg"),	//05
								IMG_Load("Textures/timber.jpg") };		//06

	Solids->textureLoader(sizeof(textures) / sizeof(SDL_Surface*), textures);
	Actors->textureLoader(sizeof(textures) / sizeof(SDL_Surface*), textures);

	auto Game = std::make_shared<Renderer>(Solids, Actors, Sun, Eye, Controls);

	Game->updateShadowVolumes(solid);

	Game->updateShadowVolumes(actor);


	//Game->updateBoundingBoxes(solid, 0.5);
	//Solids->activateBBox(0);
	//Solids->activateBBox(1);
	//Solids->activateBBox(2);

	//Game->importMesh("export_test.dat");

	while (!Controls->quit)
	{
		Game->calculateFrametime();

		Controls->HandleUserEvents();

		Screen->resetPixelBuffer();

		Screen->resetDepthBuffer();

		Game->updateCameraPosition();

		Controls->ceaseMotion();

		//Game->updateShadowVolumes(actor);
		
		Game->renderEntities(solid, Screen->pixelBuffer, Screen->depthBuffer);

		if (!Controls->isPaused) { Game->updateEntities(actor); }

		Game->renderEntities(actor, Screen->pixelBuffer, Screen->depthBuffer);

		Game->displayStats(Controls->showCrosshair, Controls->showFPS, Controls->showPosition, Controls->showPolyN, Screen);

		Game->resetPolyCounter();

		Screen->update();

		Game->updateFrameCounter();
	}

	char saveModel;
	std::cout << "Do you want to save model? (Y/N)" << std::flush;
	std::cin >> saveModel;
	if(saveModel == 'y' || saveModel == 'Y')
		Game->exportMesh("export_test.dat");

	for (int i = 0; i < (sizeof(textures) / sizeof(SDL_Surface*)); i++){ SDL_FreeSurface(textures[i]); }

	Screen->cleanUp();
	
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
	OpenGLCamera Eye(glm::vec3(0.0f, 0.0f, 0.0f), 70.0f, Screen.getAspect(), 0.1f, 100.0f);
	OpenGLTransform View(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	EventHandler Controls(0.01f, 0.01f, 0.01f);

	Shapes Solids;
	Shapes Actors;


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


	OpenGLAdapter VertexPump(&Solids, &Actors);
	unsigned int nVert = VertexPump.getNVert(solid, 0);
	Vertex* vertices = new Vertex[nVert];
	VertexPump.getVertices(solid, 0, vertices);
	OpenGLMesh mesh_01(vertices, nVert);

	OpenGLShader shader_01("./Shaders/basicShader");

	SDL_Surface* textures[] = { IMG_Load("Textures/blue.jpg"),
								IMG_Load("Textures/timber.jpg") };

	while(!Controls.quit)
	{
		Screen.clear(0.5f, 0.5f, 1.0f, 0.0f);

		shader_01.bind();
		shader_01.update(View, Eye);
		mesh_01.drawMesh();
		
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
	std::cout << "Do you want to use the Software Renderer <S> the OpenGL Renderer <O> or the Editor <E> or play Pong <P> ?" << std::endl;
	char userInput;
	std::cin >> userInput;

	std::string importFileName = "";
	if (argc == 2)
	{
		importFileName += argv[1];
	}

	switch (userInput)
	{
	case 's':
	case 'S':
		{
			if (argc == 2)
			{
				software_renderer(true, argv[1]);
			}
			else
			{
				software_renderer(false, "");
			}			
		}		
		break;
	case 'o':
	case 'O':
		opengl_renderer();
		break;
	case 'e':
	case 'E':
		editor();
		break;
	case 'p':
	case 'P':
		pong3d();
		break;
	default:

		break;
	}

	return 0;
}