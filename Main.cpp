#include <SDL/SDL.h>
#include <SDLImage/SDL_image.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
//#include <GLEW/glew.h>
//#include <GLM/glm.hpp>
#include <stdlib.h>
#include <time.h>
#include "Definitions.h"
#include "Camera.h"
#include "Canvas.h"
#include "LightSource.h"
#include "Lamp.h"
#include "Shapes.h"
#include "SolidBody.h"
#include "SolidCube.h"
#include "SolidPrism.h"
#include "SolidCylinder.h"
#include "SolidCone.h"
#include "SolidSphere.h"
#include "Room.h"
#include "SolidSTL.h"
#include "SolidModel.h"
#include "SolidFrustum.h"
#include "Bullet.h"
#include "EventHandler.h"
#include "MessagePump.h"
#include "Editor.h"
#include "ModelElementBuffer.h"
#include "BSP3Loader.h"
#include "BSP1Loader.h"
#include "BezierPatch.h"
//#include "OpenGLCanvas.h"
//#include "OpenGLCamera.h"
//#include "OpenGLTransform.h"
//#include "OpenGLAdapter.h"
//#include "OpenGLMesh.h"
//#include "OpenGLShader.h"
#include "Game.h"
#include "Player.h"
#include "ParticleSystem.h"
#include "DynamicMesh.h"
#include "Terrain.h"
#include "PointCloud.h"
#include "RayTracer.h"
#include "Octree.h"


void octree_visualisation()
{
	auto Screen		= std::make_shared<Canvas>("Octree Visualisation", SCREEN_WIDTH, SCREEN_HEIGHT, 999.9f);

	auto Controls	= std::make_shared<EventHandler>(0.1f, 0.1f, 0.01f);

	auto catTree	= std::make_shared<Octree>("Assets/PointClouds/HighRes/cat9.vert", 10);

	int overallSize = 512;

	int xStart	= (SCREEN_WIDTH - overallSize) / 2;
	int yStart	= (SCREEN_HEIGHT - overallSize) / 2;
	int zStart	= -overallSize / 2;
	int xCentre = SCREEN_WIDTH / 2;
	int yCentre = SCREEN_HEIGHT / 2;
	int zCentre = 0;

	float modelSize = catTree->getSize();
	vect3 modelCentre = catTree->getCentre();

	int z = 0;

	while (!Controls->quit)
	{
		Controls->HandleUserEvents();

		Controls->ceaseMotion();

		for (int y = 0; y < overallSize; y++)
		{
			for (int x = 0; x < overallSize; x++)
			{
				float xCurrent = (static_cast<float>((xStart + x) - xCentre) / static_cast<float>(overallSize)) * modelSize;
				float yCurrent = (static_cast<float>((yStart + y) - yCentre) / static_cast<float>(overallSize)) * modelSize;
				float zCurrent = (static_cast<float>((zStart + z) - zCentre) / static_cast<float>(overallSize))* modelSize;

				vect3 vCurrent = { xCurrent, yCurrent, zCurrent, 1.0f };
				vCurrent += modelCentre;

				if (catTree->sampleTree(0, nullptr, { 0.0f, 0.0f, 0.0f, 1.0 }, vCurrent))
					Screen->putPixel(xStart + x, yStart + y, 0x000000ff);	
			}
		}

		if (!Controls->isPaused && z++ >= overallSize)
		{
			Screen->resetPixelBuffer();
			z = 0;
		}

		Screen->update();
	}
}


void editor()
{
	auto Screen = std::make_shared<Canvas>("Editor", EDITOR_WIDTH, EDITOR_HEIGHT, 999.9f);

	auto Viewer = std::make_shared<Camera>(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.01f, 999.0f, EDITOR_WIDTH, EDITOR_HEIGHT, 0);

	auto Drawing = std::make_shared<ModelElementBuffer>("test.wtf");

	auto Builder = std::make_shared<Editor>(0.05f, Viewer, Screen, Drawing);

	auto Controls = std::make_shared<MessagePump>(Builder);

	auto model_test_1 = std::make_shared<PointCloud>("Assets/PointClouds/HighRes/cat9", 0x0066664c, false);
	Drawing->addPointCloud(model_test_1);

	while (!Controls->quit)
	{
		Screen->resetPixelBuffer();

		Builder->updateScreen();
		Controls->HandleUserEvents();

		Screen->update();
	}
}


void ray_tracing()
{
	matRT ivory, red_rubber, mirror, shiny_blue, glass;

	ivory.diff_colour = getColour(0.4f, 0.4f, 0.3f);
	ivory.spec_colour = getColour(1.0f, 1.0f, 1.0f);
	ivory.albedo[0] = 0.6f;
	ivory.albedo[1] = 0.3f;
	ivory.albedo[2] = 0.1f;
	ivory.albedo[3] = 0.0f;
	ivory.spec_exp = 50.0f;
	ivory.refractIndex = 1.0f;

	red_rubber.diff_colour = getColour(0.3f, 0.1f, 0.1f);
	red_rubber.spec_colour = getColour(1.0f, 1.0f, 1.0f);
	red_rubber.albedo[0] = 0.9f;
	red_rubber.albedo[1] = 0.1f;
	red_rubber.albedo[2] = 0.0f;
	red_rubber.albedo[3] = 0.0f;
	red_rubber.spec_exp = 10.0f;
	red_rubber.refractIndex = 1.0f;

	mirror.diff_colour = getColour(1.0f, 1.0f, 1.0f);
	mirror.spec_colour = getColour(1.0f, 1.0f, 1.0f);
	mirror.albedo[0] = 0.0f;
	mirror.albedo[1] = 10.0f;
	mirror.albedo[2] = 0.8f;
	mirror.albedo[3] = 0.0f;
	mirror.spec_exp = 1425.0f;
	mirror.refractIndex = 1.0f;

	glass.diff_colour = getColour(0.6f, 0.7f, 0.8f);
	glass.spec_colour = getColour(1.0f, 1.0f, 1.0f);
	glass.albedo[0] = 0.0f;
	glass.albedo[1] = 0.5f;
	glass.albedo[2] = 0.1f;
	glass.albedo[3] = 0.8f;
	glass.spec_exp = 125.0f;
	glass.refractIndex = 1.5f;

	shiny_blue.diff_colour = getColour(0.5f, 0.5f, 1.0f);
	shiny_blue.spec_colour = getColour(1.0f, 1.0f, 1.0f);
	shiny_blue.albedo[0] = 0.6f;
	shiny_blue.albedo[1] = 0.3f;
	shiny_blue.albedo[2] = 0.1f;
	shiny_blue.albedo[2] = 0.0f;
	shiny_blue.spec_exp = 50.0f;
	shiny_blue.refractIndex = 1.0f;

	auto Controls = std::make_shared<EventHandler>(0.1f, 0.1f, 0.01f);
	auto Screen = std::make_shared<Canvas>("Waves", 320, 200, 999.9f);
	auto Eye = std::make_shared<RayTracer>(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.1f, PI * 0.5f, 0.01f, 999.0f, 320, 200);
	auto Model = std::make_shared<Scene>();

	auto sphere_1 = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, -3.0f, 0.0f, 16.0f, 0.0f, 0.0f, 0.0f, getColour(0.4f, 0.4f, 0.3f), 1, 1.0f, 8, true);
	auto sphere_2 = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, -1.0f, -1.5f, 12.0f, 0.0f, 0.0f, 0.0f, getColour(0.3f, 0.1f, 0.1f), 1, 2.0f, 8, true);
	auto sphere_3 = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 1.5f, -0.5f, 18.0f, 0.0f, 0.0f, 0.0f, getColour(0.3f, 0.1f, 0.1f), 1, 3.0f, 8, true);
	auto sphere_4 = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 7.0f, 5.0f, 18.0f, 0.0f, 0.0f, 0.0f, getColour(0.4f, 0.4f, 0.3f), 1, 4.0f, 8, true);

	sphere_1->setMaterial(ivory);
	sphere_2->setMaterial(glass);
	sphere_3->setMaterial(red_rubber);
	sphere_4->setMaterial(ivory);

	Model->addSphere(sphere_1);
	Model->addSphere(sphere_2);
	Model->addSphere(sphere_3);
	Model->addSphere(sphere_4);

	Model->addLightSource({ -20.0f, 20.0f, -20.0f, 1.5f });
	Model->addLightSource({ 30.0f, 50.0f, 25.0f, 1.8f });
	Model->addLightSource({ 30.0f, 20.0f, -30.0f, 1.7f });


	while (!Controls->quit)
	{
		Screen->resetPixelBuffer();
		Screen->resetDepthBuffer();
		Controls->HandleUserEvents();
		Eye->update(Controls);
		Controls->ceaseMotion();

		Eye->render(Screen, Model);

		if (true)
		{
			float azmToShow, altToShow, rolToShow;

			Screen->displayString("Position X", 30, 9, 0x00ff7f7f);
			Screen->displayValue((float)Eye->x, 1, 2, 9, 0x00ff7f7f);
			Screen->displayString("Position Y", 30, 8, 0x007fff7f);
			Screen->displayValue((float)Eye->y, 1, 2, 8, 0x007fff7f);
			Screen->displayString("Position Z", 30, 7, 0x007f7fff);
			Screen->displayValue((float)Eye->z, 1, 2, 7, 0x007f7fff);

			azmToShow = Eye->azm * 180.0f / PI;
			if (azmToShow > 360.0f) { azmToShow -= 360.0f; }
			if (azmToShow < -360.0f) { azmToShow += 360.0f; }
			altToShow = Eye->alt * 180.0f / PI - 180.0f;
			if (altToShow > 360.0f) { altToShow -= 360.0f; }
			if (altToShow < -360.0f) { altToShow += 360.0f; }
			rolToShow = Eye->rol * 180.0f / PI;
			if (rolToShow > 360.0f) { rolToShow -= 360.0f; }
			if (rolToShow < -360.0f) { rolToShow += 360.0f; }

			Screen->displayString("Rotation X", 30, 5, 0x00ff3f3f);
			Screen->displayValue(azmToShow, 1, 2, 5, 0x00ff3f3f);
			Screen->displayString("Rotation Y", 30, 4, 0x003fff3f);
			Screen->displayValue(altToShow, 1, 2, 4, 0x003fff3f);
			Screen->displayString("Rotation Z", 30, 3, 0x003f3fff);
			Screen->displayValue(rolToShow, 1, 2, 3, 0x003f3fff);

			Screen->displayFps((float)(30000.0f / Eye->frameTime), 1, 2, 0);
		}


		Screen->update();
	}
}


void globe()
{
	auto Eye = std::make_shared<Camera>(
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.1f, 0.1f,
		PI * 0.5f,
		0.01f, 999.0f,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		0
	);

	auto Screen = std::make_shared<Canvas>("Globe", SCREEN_WIDTH, SCREEN_HEIGHT, 999.9f);

	Eye->linkToCanvas(Screen);

	auto Controls = std::make_shared<EventHandler>(0.1f, 0.1f, 0.01f);

	auto Sun = std::make_shared<LightSource>(300.0f, 75.0f, 1.0f);

	auto Viewer = std::make_shared<Player>(15.0f, 20.0f, 17.5f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100);

	auto World = std::make_shared<Game>(Screen, Eye, Controls, Sun, Viewer, nullptr);

	World->addTextScreen("Controls Text", std::make_shared<Text>("Assets/Txt/Controls_Text.txt", 480, 0x007f7fff));


	//auto Earth = std::make_shared<PointCloud>(IMG_Load("Assets/Textures/earth_large.jpg"), 60.0f, 360);
	//World->addPointCloud(Earth);

	auto Earth_ = std::make_shared<PointCloud>("Assets/Gdl/globe.txt", -1000.0f, 1000.0f, 1000.0f);
	//Earth_->setRadius(60.0f);
	//Earth_->setGrid();
	//Earth_->addSurfacePoint(51.0f, 0.0f, 60.0f, { 0.0f, 0.0f, 0.0f, 1.0f }, 0x000000ff);
	//Earth_->addSurfacePoint(47.5f, 19.0f, 60.0f, { 0.0f, 0.0f, 0.0f, 1.0f }, 0x000000ff);
	//Earth_->addSurfacePoint(47.5f, 19.0f, 0.0025f, 0.0025f, 60.0f, { 0.0f, 0.0f, 0.0f, 1.0f }, 0x0000ff00);
	//Earth_->addSurfacePoint(-50.0f, 135.0f, 0.0025f, 0.0025f, 60.0f, { 0.0f, 0.0f, 0.0f, 1.0f }, 0x0000ff00);
	//Earth_->addSurfacePoint(60.0f, 60.0f, 0.0025f, 0.0025f, 60.0f, { 0.0f, 0.0f, 0.0f, 1.0f }, 0x0000ff00);
	//Earth_->addSurfacePoint(0.0f, 0.0f, 0.01f, 0.01f, 60.0f, { 0.0f, 0.0f, 0.0f, 1.0f }, 0x0000ff00);
	World->addPointCloud(Earth_);

	while (!Controls->quit)
	{
		World->updateAll();
	}
}


void water()
{
	auto Eye = std::make_shared<Camera>(
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.1f, 0.1f,
		PI * 0.5f,
		0.01f, 999.0f,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		0
	);

	Eye->addTexture(IMG_Load("Assets/Textures/blue.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/wolf001.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/wolf002.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/mosaic001.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/brick001.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/concrete001.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/timber.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/metal.jpg"));

	auto Screen = std::make_shared<Canvas>("Waves", SCREEN_WIDTH, SCREEN_HEIGHT, 999.9f);

	Eye->linkToCanvas(Screen);

	auto Controls = std::make_shared<EventHandler>(0.1f, 0.1f, 0.01f);

	auto Sun = std::make_shared<LightSource>(300.0f, 75.0f, 1.0f);

	auto Viewer = std::make_shared<Player>(15.0f, 20.0f, 17.5f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100);

	auto testWorld = std::make_shared<Game>(Screen, Eye, Controls, Sun, Viewer, nullptr);

	testWorld->addTextScreen("Controls Text", std::make_shared<Text>("Assets/Txt/Controls_Text.txt", 480, 0x007f7fff));

	auto Sea = std::make_shared<DynamicMesh>(10, 10, 8.0f, 0.25f, 5.0f, 2.5f, 0x00007fbf, false);

	//auto asteroid_surface = std::make_shared<Terrain>("Assets/Textures/seafloor_large.jpg",
	//												256, 256, 2125.0f, 1600.0f, 2.0f, 1.0f, -50.0f, 0x000000ff, false);

	//testWorld->addStaticSurface(asteroid_surface);

	auto model_test_1 = std::make_shared<PointCloud>("Assets/PointClouds/HighRes/cat9", 0x0066664c, false);
	//auto route_1 = std::make_shared<PointCloud>("Assets/PointClouds/HighRes/route", 0x000000ff, false);
	//testWorld->addPointCloud(route_1);
	testWorld->addPointCloud(model_test_1);


	while (!Controls->quit)
	{
		testWorld->updateAll();
	}
}


void fps_game()
{
	auto Screen = std::make_shared<Canvas>("3D Shooter", SCREEN_WIDTH, SCREEN_HEIGHT, 999.9f);

	auto Eye = std::make_shared<Camera>(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.1f, PI * 0.5f, 0.01f, 999.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	Eye->linkToCanvas(Screen);

	auto Controls = std::make_shared<EventHandler>(0.1f, 0.1f, 0.01f);

	auto Sun = std::make_shared<LightSource>(300.0f, 22.5f, 1.0f);

	auto Lamp1 = std::make_shared<Lamp>(-30.0f, 60.0f, 45.0f,
										PI * 0.375f, PI * 0.25f, 0.0f,
										PI * 0.5f,
										1.0f, 100.0f,
										1200, 600);

	auto Hero = std::make_shared<Player>(0.0f, 0.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100);

	auto weapon = std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.5f, 0.0f, PI * 0.5f, 0.0f, 0xff7f7fff, 7, 0.25f, 5.0f, 32, true);

	Hero->addPart(weapon);
	Hero->setTrailColour(0x00ffff00);

	auto Observer = std::make_shared<Player>(10.0f, 10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100);

	auto cone = std::make_shared<SolidCone>(0.0f, 0.0f, 0.0f, 0x000000ff);

	Observer->addPart(cone);

	auto fpsGame = std::make_shared<Game>(Screen, Eye, Controls, Sun, Hero, Observer);

	fpsGame->addLamp(Lamp1);
	//Fill up bullet pool
	fpsGame->addProjectile(200);
	fpsGame->addImpact(50);
	//Fill up explosion pool
	fpsGame->addExplosion(5);

#define _STL_READER_
//#define _PLANETS_
//#define _SHOOTER_
//#define _PARTICLES_
//#define _QUAKE_1_READER_
//#define _QUAKE_3_READER_
//#define	_BEZIER_PATCH_

#ifdef _QUAKE_1_READER_

	auto quake1Map = std::make_shared<BSP1Loader>("Assets/QuakeMaps/dm6.bsp", 0.1f, 0.1f, 0.1f);
	//quake1Map->readData();
	fpsGame->addEntity(quake1Map);

	int nTxt = quake1Map->getTotalText();
	txt* quakeTextures = new txt[nTxt];
	for (int i = 0; i < nTxt; i++)
		Eye->addTexture(quake1Map->getTextureData(i));

#endif//_QUAKE_1_READER_

	//Load textures
	Eye->addTexture(IMG_Load("Assets/Textures/blue.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/wolf001.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/wolf002.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/mosaic001.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/brick001.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/concrete001.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/timber.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/metal.jpg"));

#ifdef _STL_READER_

	auto teapot = std::make_shared<SolidSTL>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0x00ff7f00, 3, "Assets/Stl/utah_teapot.stl");
	fpsGame->addEntity(teapot);

	auto revolver = std::make_shared<SolidSTL>(1.0f, 1.0f, 1.0f, 10.0f, 12.0f, 1.5f, PI, 0.0f, 0.0f, 0x007f7f7f, 3, "Assets/Stl/Frame.stl");
	fpsGame->addEntity(revolver);

	auto board = std::make_shared<SolidPrism>(1.0f, 1.0f, 1.0f, -30.0f, -30.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0x0000007f, 6, 60.0f, 60.0f, 1.0f);
	fpsGame->addEntity(board);

#endif//_STL_READER_

#ifdef _PLANETS_

	//Load planet textures
	Eye->addTexture(IMG_Load("Assets/Textures/earth_large.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/mars_large.jpg"));

	auto Earth = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0xffffff00, 8, 6.0f, 24);

	//Earth->setMotion(true);
	//Earth->setAngularVelocity({ 0.0f, 0.0f, -0.001f, 1.0f });

	fpsGame->addEntity(Earth);

	auto Mars = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f,
		15.0f, 12.5f, 5.0f,
		0.0f, 0.0f, 0.0f,
		0xffffff00, 9, 3.0f, 24);

	//Mars->setMotion(true);
	//Mars->setAngularVelocity({ 0.0f, 0.0f, -0.0005f, 1.0f });

	fpsGame->addEntity(Mars);

#endif//_PLANETS_

#ifdef _QUAKE_3_READER_

	auto quake3Map = std::make_shared<BSP3Loader>("Assets/QuakeMaps/13ground.bsp", 0.1f, 0.1f, 0.1f);
	fpsGame->addEntity(quake3Map);

#endif//_QUAKE_3_READER_

#ifdef _BEZIER_PATCH_

	auto patch = std::make_shared<BezierPatch>(32, 6, getColour(0, 127, 127, 255));

	patch->setPosition({ -20.0f, -10.0f, 0.0f, 0.0f });

	patch->setControlPoint(0, { 0.0f, -0.5f, 0.0f, 1.0f });
	patch->setControlPoint(1, { 2.0f, 0.0f, 1.5f, 1.0f });
	patch->setControlPoint(2, { 8.0f, 0.5f, 3.0f, 1.0f });

	patch->setControlPoint(3, { 0.0f, 2.0f, 0.0f, 1.0f });
	patch->setControlPoint(4, { 2.0f, 2.0f, 2.5f, 1.0f });
	patch->setControlPoint(5, { 8.0f, 2.0f, 1.0f, 1.0f });

	patch->setControlPoint(6, { 0.0f, 4.5f, 0.0f, 1.0f });
	patch->setControlPoint(7, { 2.0f, 4.0f, 2.0f, 1.0f });
	patch->setControlPoint(8, { 8.0f, 3.5f, 0.0f, 1.0f });

	patch->calculateMesh();

	fpsGame->addEntity(patch);

#endif//_BEZIER_PATCH_

#ifdef _SHOOTER_

	//Add some basic shapes
	auto pedestal = std::make_shared<SolidPrism>(1.0f, 1.0f, 1.0f, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0x0000ff00, 6, 20.0f, 30.0f, 5.0f);
	auto wall = std::make_shared<SolidPrism>(1.0f, 1.0f, 1.0f, 5.0f, 19.5f, 5.0f, 0.0f, 0.0f, 0.0f, 0x00ff0000, 4, 20.0f, 1.0f, 7.5f);
	auto dome = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 15.0f, 12.5f, 5.0f, 0.0f, 0.0f, 0.0f, 0xffffff00, 7, 5.0f, 24);
	auto box = std::make_shared<Room>(0.0f, 0.0f, 0.0f, 30.0f, 40.0f, 20.0f);
	box->calculateMesh();

	fpsGame->addEntity(box);
	fpsGame->addEntity(pedestal);
	fpsGame->addEntity(wall);
	//fpsGame->addEntity(dome);

	//Add some spheres floating around
	srand(unsigned int(time(NULL)));
	for (int i = 0; i < 10; i++)
	{
		float x = float(rand() % 20) * 1.0f + 5.0f;
		float y = float(rand() % 30) * 1.0f + 5.0f;
		float z = float(rand() % 10) * 1.0f + 5.0f;

		float vx = (-5.0f + float(rand() % 10)) * 0.01f;
		float vy = (-5.0f + float(rand() % 10)) * 0.01f;
		float vz = (-5.0f + float(rand() % 10)) * 0.01f;

		auto ball = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, x, y, z, 0.0f, 0.0f, 0.0f, 0xff7f7fff, 1, 1.0f, 8, true);

		ball->setBBRadius(1.0f);
		ball->setGravity(false);
		ball->setMotion(true);
		ball->setVelocity({ vx, vy, vz, 1.0f });
		ball->setAngularVelocity({ 0.01f, 0.0f, 0.0f, 1.0f });
		ball->setBehaviour(hit_response::bounce);
		ball->setBreakability(true);

		fpsGame->addBall(ball);
	}

	auto ball_01 = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xff7f7fff, 1, 4.0f, 16, true);
	ball_01->setBBRadius(4.0f);
	ball_01->setGravity(false);
	ball_01->setMotion(true);
	ball_01->setAngularVelocity({ 0.0f, 0.0f, 0.01f, 1.0f });
	ball_01->setBehaviour(hit_response::bounce);
	ball_01->setBreakability(true);
	fpsGame->addBall(ball_01);

	//Create enemies
	auto Enemy1 = std::make_shared<Player>(-20.0f, 60.0f, 10.0f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100);

	auto e_011 = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xffffffff, 7, 1.5f, 8);
	auto e_021 = std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0.0f, 0xffff0000, 7, 0.25f, 3.0f, 4, true);
	auto e_031 = std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0xff00ff00, 7, 0.25f, 3.0f, 4, true);
	auto e_041 = std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0xff0000ff, 7, 0.25f, 3.0f, 4, true);

	Enemy1->addPart(e_011);
	Enemy1->addPart(e_021);
	Enemy1->addPart(e_031);
	Enemy1->addPart(e_041);

	Enemy1->setTrailColour(0x00ff3f3f);

	fpsGame->addEnemy(Enemy1);

	auto Enemy2 = std::make_shared<Player>(40.0f, 50.0f, 60.0f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100);

	auto e_012 = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xff7f7fff, 7, 1.5f, 8);
	auto e_022 = std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0.0f, 0xff0000ff, 7, 0.25f, 3.0f, 4, true);
	auto e_032 = std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0xff0000ff, 7, 0.25f, 3.0f, 4, true);
	auto e_042 = std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0xff0000ff, 7, 0.25f, 3.0f, 4, true);

	Enemy2->addPart(e_012);
	Enemy2->addPart(e_022);
	Enemy2->addPart(e_032);
	Enemy2->addPart(e_042);

	Enemy2->setTrailColour(0x007fff7f);

	fpsGame->addEnemy(Enemy2);

	auto Enemy3 = std::make_shared<Player>(-40.0f, -50.0f, 30.0f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100);

	auto e_013 = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xffff0000, 7, 1.5f, 8);
	auto e_023 = std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0.0f, 0xffffff00, 7, 0.25f, 3.0f, 4, true);
	auto e_033 = std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0xffffff00, 7, 0.25f, 3.0f, 4, true);
	auto e_043 = std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0xffffff00, 7, 0.25f, 3.0f, 4, true);

	Enemy3->addPart(e_013);
	Enemy3->addPart(e_023);
	Enemy3->addPart(e_033);
	Enemy3->addPart(e_043);

	Enemy3->setTrailColour(0x007f7fff);

	fpsGame->addEnemy(Enemy3);

	//auto cockpit = std::make_shared<SolidSTL>(0.1f, 0.1f, 0.1f, -0.2f, 0.0f, 0.0f, 0.0f, PI * 0.25f, 0.0f, 0x00ff7f00, 3, "Assets/Stl/cockpit.stl");
	//Hero->addPart(cockpit);

#endif//_SHOOTER_

#ifdef _PARTICLES_

	//Add room
	auto box2 = std::make_shared<Room>(0.0f, 0.0f, 0.0f, 30.0f, 40.0f, 20.0f);
	box2->setTexture(3, 3, 3, 3, 3, 3);
	box2->setTextureScale(10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f);
	box2->calculateMesh();
	fpsGame->addEntity(box2);

	//Add particle emitter
	auto waterFountain = std::make_shared<ParticleSystem>(0.2f, 1.0f, 3000, 3, 0x007f7fff);
	waterFountain->setOrigin({ 1.0f, 1.0f, 1.0f, 1.0f });
	waterFountain->setDirection({ 3.0f, 4.0f, 5.0f, 1.0f });
	waterFountain->setDispersion(0.01f);
	waterFountain->activate();
	waterFountain->setGravity(true);

	//Game->buildMesh();
	fpsGame->addEmitter(waterFountain);

#endif//_PARTICLES_

	//Add text screens
	fpsGame->addTextScreen("Controls Text", std::make_shared<Text>("Assets/Txt/Controls_Text.txt", 480, 0x007f7fff));

	//Main loop
	while (!Controls->quit)
	{
		fpsGame->updateAll();
	}
}


/*
void opengl_renderer()
{
	OpenGLCanvas Screen(1200, 600, "OpenGL Renderer");
	OpenGLCamera Eye(glm::vec3(0.0f, 0.0f, 0.0f), 70.0f, Screen.getAspect(), 0.1f, 100.0f);
	OpenGLTransform View(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	EventHandler Controls(0.01f, 0.01f, 0.01f);

	Shapes Solids;
	Shapes Actors;

	//BSP3Loader q3map("Assets/QuakeMaps/13ground.bsp", 0.1f, 0.1f, 0.1f);
	//SolidBody* map = &q3map;
	////BSP1Loader quakeMap("Assets/QuakeMaps/dm6.bsp", { 0.01, 0.01, 0.01, 1.0 });
	////quakeMap.readData();
	////SolidBody* map = &quakeMap;	
	//Solids.addSolid(map);

	PointCloud cat_01("Assets/PointClouds/HighRes/cat9", 0x0066664c, false);
	cat_01.invertFaces();
	SolidBody* person = &cat_01;
	Solids.addSolid(person);

	Solids.addTextureData(IMG_Load("Assets/Textures/blue.jpg"));
	Solids.addTextureData(IMG_Load("Assets/Textures/wolf001.jpg"));
	Solids.addTextureData(IMG_Load("Assets/Textures/wolf002.jpg"));
	Solids.addTextureData(IMG_Load("Assets/Textures/mosaic001.jpg"));
	Solids.addTextureData(IMG_Load("Assets/Textures/brick001.jpg"));
	Solids.addTextureData(IMG_Load("Assets/Textures/concrete001.jpg"));
	Solids.addTextureData(IMG_Load("Assets/Textures/timber.jpg"));
	Solids.addTextureData(IMG_Load("Assets/Textures/metal.jpg"));


	//int nTxt = quakeMap.getTotalText();
	//txt* quakeTextures = new txt[nTxt];
	//for (int i = 0; i < nTxt; i++)
	//{
	//	quakeTextures[i] = quakeMap.getTextureData(i);
	//}
	//Solids.textureLoaderQ(nTxt, quakeTextures);


	OpenGLAdapter VertexPump(&Solids, &Actors);
	unsigned int nVert = VertexPump.getNVert(solid, 0);
	Vertex* vertices = new Vertex[nVert];
	VertexPump.getVertices(solid, 0, vertices);
	OpenGLMesh mesh_01(vertices, nVert);

	OpenGLShader shader_01("./Shaders/basicShader");

	SDL_Surface* textures[] = { IMG_Load("Textures/blue.jpg"),
								IMG_Load("Textures/timber.jpg") };

	float z = 0.001f;

	while (!Controls.quit)
	{
		Screen.clear(0.5f, 0.5f, 1.0f, 0.0f);

		Controls.HandleUserEvents();
		View.updateAll(Controls);


		shader_01.bind();
		shader_01.update(View, Eye);
		mesh_01.drawMesh();

		Controls.ceaseMotion();

		Screen.update();
	}

	delete[] vertices;

	for (int i = 0; i < (sizeof(textures) / sizeof(SDL_Surface*)); i++) { SDL_FreeSurface(textures[i]); }
}
*/


int main(int argc, char** argv)
{
	std::cout << "Do you want to play the Game <P>, test the Octree Visualisation tool <O>, see some point clouds <W> or use the Editor <E>?" << std::endl;
	char userInput;
	std::cin >> userInput;

	switch (userInput)
	{
	case 'e':
	case 'E':
		editor();
		break;
	case 'w':
	case 'W':
		//water();
		globe();
		break;
	case 'o':
	case 'O':
		octree_visualisation();
		break;
	case 'p':
	case 'P':
		fps_game();
		break;
	case 'r':
	case 'R':
		ray_tracing();
		break;
	default:

		break;
	}

	return 0;
}