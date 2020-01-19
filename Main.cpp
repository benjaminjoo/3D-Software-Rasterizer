#include <SDL/SDL.h>
#include <SDLImage/SDL_image.h>
#include <GLEW/glew.h>
#include <GLM/glm.hpp>
#include <stdlib.h>
#include <time.h>
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
#include "OpenGLCanvas.h"
#include "OpenGLCamera.h"
#include "OpenGLTransform.h"
#include "OpenGLAdapter.h"
#include "OpenGLMesh.h"
#include "OpenGLShader.h"
#include "Game.h"
#include "Player.h"
#include "ParticleSystem.h"
#include "DynamicMesh.h"


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

		Builder->updateScreen();
		Controls->HandleUserEvents();

		Screen->update();
	}

	IMG_Quit();

	SDL_Quit();
}


void water()
{
	auto Eye		= std::make_shared<Camera>(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.1f, PI * 0.5f, 0.01f, 999.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	Eye->addTexture(IMG_Load("Assets/Textures/blue.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/wolf001.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/wolf002.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/mosaic001.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/brick001.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/concrete001.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/timber.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/metal.jpg"));

	auto Screen		= std::make_shared<Canvas>("Waves", SCREEN_WIDTH, SCREEN_HEIGHT, 999.9f);

	Eye->linkToCanvas(Screen);

	auto Controls	= std::make_shared<EventHandler>(0.1f, 0.1f, 0.01f);

	auto Sun		= std::make_shared<LightSource>(300.0f, 75.0f, 0.95f);

	auto Viewer		= std::make_shared<Player>(15.0f, 20.0f, 17.5f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100);

	auto testWorld	= std::make_shared<Game>(Screen, Eye, Controls, Sun, Viewer, nullptr);

	testWorld->addTextScreen("Controls Text", std::make_shared<Text>("Assets/Txt/Controls_Text.txt", 480, 0x007f7fff));

	auto Sea		= std::make_shared<DynamicMesh>(36, 36, 8.0f, 0.25f, 5.0f, 2.5f, 0x00007fbf, false);

	testWorld->addDynamicSurface(Sea);

	//testWorld->buildMesh();

	while (!Controls->quit)
		testWorld->updateAll();

	//testWorld->destroyMesh();

	IMG_Quit();

	SDL_Quit();
}


void fps_game()
{
	auto Screen		= std::make_shared<Canvas>("3D Shooter", SCREEN_WIDTH, SCREEN_HEIGHT, 999.9f);

	auto Eye		= std::make_shared<Camera>(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.1f, PI * 0.5f, 0.01f, 999.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	Eye->linkToCanvas(Screen);

	auto Controls	= std::make_shared<EventHandler>(0.1f, 0.1f, 0.01f);

	auto Sun		= std::make_shared<LightSource>(300.0f, 22.5f, 1.0f);

	auto Hero		= std::make_shared<Player>(15.0f, 20.0f, 2.5f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100);	

	auto weapon		= std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.5f, 0.0f, PI * 0.5f, 0.0f, 0xff7f7fff, 7, 0.25f, 5.0f, 32);

	Hero->addPart(weapon);

	auto Observer	= std::make_shared<Player>(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100);

	auto cone		= std::make_shared<SolidCone>(0.0f, 0.0f, 0.0f, 0x000000ff);

	Observer->addPart(cone);
	
	auto fpsGame		= std::make_shared<Game>(Screen, Eye, Controls, Sun, Hero, Observer);

	//Fill up bullet pool
	fpsGame->loadProjectile(200);

#define _STL_READER_
//#define _PLANETS_
//#define _SHOOTER_
//#define _PARTICLES_
//#define _QUAKE_1_READER_
//#define _QUAKE_3_READER_
//#define	_BEZIER_PATCH_

#ifdef _QUAKE_1_READER_

	auto quake1Map = std::make_shared<BSP1Loader>("Assets/QuakeMaps/dm6.bsp", 0.01f, 0.01f, 0.01f);
	quake1Map->readData();
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

	auto teapot = std::make_shared<SolidSTL>(1.0f, 1.0f, 1.0f, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0x00ff7f00, 3, "Assets/Stl/utah_teapot.stl");
	teapot->readSTLfile();
	teapot->smoothSurfaces();
	fpsGame->addEntity(teapot);

	auto board = std::make_shared<SolidPrism>(1.0f, 1.0f, 1.0f, -15.0f, -15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0x0000007f, 6, 30.0f, 30.0f, 1.0f);
	fpsGame->addEntity(board);

#endif//_STL_READER_

#ifdef _PLANETS_

	//Load planet textures
	Eye->addTexture(IMG_Load("Assets/Textures/earth_large.jpg"));
	Eye->addTexture(IMG_Load("Assets/Textures/mars_large.jpg"));

	auto Earth = std::make_shared<SolidSphere>(	1.0f, 1.0f, 1.0f,
												0.0f, 0.5f, 0.0f,
												0.0f, 0.0f, 0.0f,
												0xffffff00, 8, 6.0f, 24);

	//Earth->setMotion(true);
	//Earth->setAngularVelocity({ 0.0f, 0.0f, -0.001f, 1.0f });

	fpsGame->addEntity(Earth);

	auto Mars = std::make_shared<SolidSphere>(	1.0f, 1.0f, 1.0f,
												15.0f, 12.5f, 5.0f,
												0.0f, 0.0f, 0.0f,
												0xffffff00, 9, 3.0f, 24);

	//Mars->setMotion(true);
	//Mars->setAngularVelocity({ 0.0f, 0.0f, -0.0005f, 1.0f });

	fpsGame->addEntity(Mars);

#endif//_PLANETS_

#ifdef _QUAKE_3_READER_

	auto quake3Map = std::make_shared<BSP3Loader>("Assets/QuakeMaps/13ground.bsp", 0.01f, 0.01f, 0.01f);
	quake3Map->readData();
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
	auto pedestal	= std::make_shared<SolidPrism>(1.0f, 1.0f, 1.0f, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0x0000ff00, 6, 20.0f, 30.0f, 5.0f);
	auto wall		= std::make_shared<SolidPrism>(1.0f, 1.0f, 1.0f, 5.0f, 19.5f, 5.0f, 0.0f, 0.0f, 0.0f, 0x00ff0000, 4, 20.0f, 1.0f, 7.5f);
	auto dome		= std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 15.0f, 12.5f, 5.0f, 0.0f, 0.0f, 0.0f, 0xffffff00, 9, 5.0f, 24);
	auto box		= std::make_shared<Room>(0.0f, 0.0f, 0.0f, 30.0f, 40.0f, 20.0f);
	box->calculateMesh();	

	fpsGame->addEntity(box);
	fpsGame->addEntity(pedestal);
	fpsGame->addEntity(wall);

	//Add some spheres floating around
	srand(unsigned int(time(NULL)));
	for (int i = 0; i < 10; i++)
	{
		double x = double(rand() % 20) * 1.0f + 5.0f;
		double y = double(rand() % 30) * 1.0f + 5.0f;
		double z = double(rand() % 10) * 1.0f + 5.0f;
	
		double vx = (-5.0f + double(rand() % 10)) * 0.05f;
		double vy = (-5.0f + double(rand() % 10)) * 0.05f;
		double vz = (-5.0f + double(rand() % 10)) * 0.05f;
	
		auto ball = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, x, y, z, 0.0f, 0.0f, 0.0f, 0xff7f7fff, 1, 1.0f, 8);
	
		ball->setBBRadius(1.0f);
		ball->setGravity(false);
		ball->setMotion(true);
		ball->setVelocity({ vx, vy, vz, 1.0f });
		ball->setAngularVelocity({ 0.01f, 0.0f, 0.0f, 1.0f });
		ball->setBehaviour(bounce);
		ball->setBreakability(true);
	
		fpsGame->addBall(ball);
	}

	//Create enemies
	auto Enemy1		= std::make_shared<Player>(20.0f, 20.0f, 10.0f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100);

	auto e_011		= std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xffffffff, 7, 1.5f, 8);
	auto e_021		= std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0.0f, 0xffff0000, 7, 0.25f, 3.0f, 4);
	auto e_031		= std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0xff00ff00, 7, 0.25f, 3.0f, 4);
	auto e_041		= std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0xff0000ff, 7, 0.25f, 3.0f, 4);

	Enemy1->addPart(e_011);
	Enemy1->addPart(e_021);
	Enemy1->addPart(e_031);
	Enemy1->addPart(e_041);

	fpsGame->addEnemy(Enemy1);

	auto Enemy2		= std::make_shared<Player>(40.0f, 50.0f, 60.0f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100);

	auto e_012		= std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xff7f7fff, 7, 1.5f, 8);
	auto e_022		= std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0.0f, 0xff0000ff, 7, 0.25f, 3.0f, 4);
	auto e_032		= std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0xff0000ff, 7, 0.25f, 3.0f, 4);
	auto e_042		= std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0xff0000ff, 7, 0.25f, 3.0f, 4);

	Enemy2->addPart(e_012);
	Enemy2->addPart(e_022);
	Enemy2->addPart(e_032);
	Enemy2->addPart(e_042);

	fpsGame->addEnemy(Enemy2);

	auto Enemy3 = std::make_shared<Player>(-40.0f, -50.0f, 30.0f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100);

	auto e_013 = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xffff0000, 7, 1.5f, 8);
	auto e_023 = std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0.0f, 0xffffff00, 7, 0.25f, 3.0f, 4);
	auto e_033 = std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0xffffff00, 7, 0.25f, 3.0f, 4);
	auto e_043 = std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0xffffff00, 7, 0.25f, 3.0f, 4);

	Enemy3->addPart(e_013);
	Enemy3->addPart(e_023);
	Enemy3->addPart(e_033);
	Enemy3->addPart(e_043);

	fpsGame->addEnemy(Enemy3);

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

	IMG_Quit();

	SDL_Quit();
}


void opengl_renderer()
{
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
	std::cout << "Do you want to play the Game <P>, see some waves <W> or use the Editor <E>?" << std::endl;
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
		water();
		break;
	case 'p':
	case 'P':
		fps_game();
		break;
	default:

		break;
	}

	return 0;
}