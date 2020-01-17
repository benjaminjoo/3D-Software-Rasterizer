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
#include "Pong.h"
#include "Player.h"
#include "PelvisBone.h"
#include "LongBone.h"
#include "ParticleSystem.h"


void editor()
{
	auto Screen		= std::make_shared<Canvas>("Editor", EDITOR_WIDTH, EDITOR_HEIGHT, 999.9f);

	auto Renderer	= std::make_shared<Projection>();

	auto Viewer		= std::make_shared<Camera>(Renderer, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.01f, 999.0f, EDITOR_WIDTH, EDITOR_HEIGHT, 0);

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
	auto Screen		= std::make_shared<Canvas>("3D Shooter", SCREEN_WIDTH, SCREEN_HEIGHT, 999.9f);

	auto Renderer	= std::make_shared<Projection>();

	auto Eye		= std::make_shared<Camera>(Renderer, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.1f, PI * 0.5f, 0.01f, 999.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	Eye->linkToCanvas(Screen);

	auto Controls	= std::make_shared<EventHandler>(0.1f, 0.1f, 0.01f);

	auto Sun		= std::make_shared<LightSource>(300.0f, 45.0f, 0.95f);

	auto Hero		= std::make_shared<Player>(Renderer, 15.0f, 20.0f, 17.5f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100, nullptr);	

	auto weapon		= std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.5f, 0.0f, PI * 0.5f, 0.0f, 0xff7f7fff, 7, 0.25f, 5.0f, 4);

	Hero->addPart(weapon);

	auto Observer	= std::make_shared<Player>(Renderer, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100, nullptr);

	auto cone		= std::make_shared<SolidCone>(0.0f, 0.0f, 0.0f, 0x000000ff);

	Observer->addPart(cone);
	
	auto Game		= std::make_shared<Pong>(Screen, Eye, Renderer, Controls, Sun, Hero, Observer);

	//Fill up bullet pool
	Game->loadProjectile(200);

//#define _STL_READER_
//#define _PLANETS_
#define _SHOOTER_
//#define _PARTICLES_
//#define _QUAKE_1_READER_
//#define _QUAKE_3_READER_
//#define	_BEZIER_PATCH_

#ifdef _QUAKE_1_READER_

	auto quake1Map = std::make_shared<BSP1Loader>("Assets/QuakeMaps/dm6.bsp", 0.01f, 0.01f, 0.01f);
	quake1Map->readData();
	Game->addEntity(quake1Map);

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

	auto navalGun = std::make_shared<SolidSTL>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, getColour(0, 255, 255, 255), 3, "Assets/Stl/naval_gun.stl");
	navalGun->readSTLfile();
	//test_body.smoothSurfaces();
	Game->addEntity(navalGun);

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

	Game->addEntity(Earth);

	auto Mars = std::make_shared<SolidSphere>(	1.0f, 1.0f, 1.0f,
												15.0f, 12.5f, 5.0f,
												0.0f, 0.0f, 0.0f,
												0xffffff00, 9, 3.0f, 24);

	//Mars->setMotion(true);
	//Mars->setAngularVelocity({ 0.0f, 0.0f, -0.0005f, 1.0f });

	Game->addEntity(Mars);

#endif//_PLANETS_

#ifdef _QUAKE_3_READER_

	auto quake3Map = std::make_shared<BSP3Loader>("Assets/QuakeMaps/13ground.bsp", 0.01f, 0.01f, 0.01f);
	quake3Map->readData();
	Game->addEntity(quake3Map);

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

	Game->addEntity(patch);

#endif//_BEZIER_PATCH_

#ifdef _SHOOTER_

	//Add some basic shapes
	auto pedestal	= std::make_shared<SolidPrism>(1.0f, 1.0f, 1.0f, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0x0000ff00, 6, 20.0f, 30.0f, 5.0f);
	auto wall		= std::make_shared<SolidPrism>(1.0f, 1.0f, 1.0f, 5.0f, 19.5f, 5.0f, 0.0f, 0.0f, 0.0f, 0x00ff0000, 4, 20.0f, 1.0f, 7.5f);
	auto dome		= std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 15.0f, 12.5f, 5.0f, 0.0f, 0.0f, 0.0f, 0xffffff00, 9, 5.0f, 24);
	auto box		= std::make_shared<Room>(0.0f, 0.0f, 0.0f, 30.0f, 40.0f, 20.0f);
	box->calculateMesh();	

	Game->addEntity(box);
	Game->addEntity(pedestal);
	Game->addEntity(wall);

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
	
		Game->addBall(ball);
	}

	//Create enemies
	auto Enemy1		= std::make_shared<Player>(Renderer, 20.0f, 20.0f, 10.0f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100, nullptr);

	auto e_011		= std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xffffffff, 7, 1.5f, 8);
	auto e_021		= std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0.0f, 0xffff0000, 7, 0.25f, 3.0f, 4);
	auto e_031		= std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0xff00ff00, 7, 0.25f, 3.0f, 4);
	auto e_041		= std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0xff0000ff, 7, 0.25f, 3.0f, 4);

	Enemy1->addPart(e_011);
	Enemy1->addPart(e_021);
	Enemy1->addPart(e_031);
	Enemy1->addPart(e_041);

	Game->addEnemy(Enemy1);

	auto Enemy2		= std::make_shared<Player>(Renderer, 40.0f, 50.0f, 60.0f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100, nullptr);

	auto e_012		= std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xff7f7fff, 7, 1.5f, 8);
	auto e_022		= std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0.0f, 0xff0000ff, 7, 0.25f, 3.0f, 4);
	auto e_032		= std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0xff0000ff, 7, 0.25f, 3.0f, 4);
	auto e_042		= std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0xff0000ff, 7, 0.25f, 3.0f, 4);

	Enemy2->addPart(e_012);
	Enemy2->addPart(e_022);
	Enemy2->addPart(e_032);
	Enemy2->addPart(e_042);

	Game->addEnemy(Enemy2);

	auto Enemy3 = std::make_shared<Player>(Renderer, -40.0f, -50.0f, 30.0f, 0.0f, 0.0f, 0.0f, 1.5f, 100, 100, nullptr);

	auto e_013 = std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xffff0000, 7, 1.5f, 8);
	auto e_023 = std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0.0f, 0xffffff00, 7, 0.25f, 3.0f, 4);
	auto e_033 = std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0.0f, 0xffffff00, 7, 0.25f, 3.0f, 4);
	auto e_043 = std::make_shared<SolidCylinder>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, PI * 0.5f, 0xffffff00, 7, 0.25f, 3.0f, 4);

	Enemy3->addPart(e_013);
	Enemy3->addPart(e_023);
	Enemy3->addPart(e_033);
	Enemy3->addPart(e_043);

	Game->addEnemy(Enemy3);

#endif//_SHOOTER_

#ifdef _PARTICLES_

	//Add room
	auto box2 = std::make_shared<Room>(0.0f, 0.0f, 0.0f, 30.0f, 40.0f, 20.0f);
	box2->setTexture(3, 3, 3, 3, 3, 3);
	box2->setTextureScale(10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f);
	box2->calculateMesh();
	Game->addEntity(box2);

	//Add particle emitter
	auto waterFountain = std::make_shared<ParticleSystem>(0.2f, 1.0f, 3000, 3, 0x007f7fff);
	waterFountain->setOrigin({ 1.0f, 1.0f, 1.0f, 1.0f });
	waterFountain->setDirection({ 3.0f, 4.0f, 5.0f, 1.0f });
	waterFountain->setDispersion(0.01f);
	waterFountain->activate();
	waterFountain->setGravity(true);

	Game->addEmitter(waterFountain);

#endif//_PARTICLES_

	//Add text screens
	Game->addTextScreen("Controls Text", std::make_shared<Text>("Assets/Txt/Controls_Text.txt", 480, 0x007f7fff));

	//Build geometry
	Game->buildMesh();

	//Main loop
	while (!Controls->quit)
	{
		Game->updateAll();
	}

	//Destroy geometry
	Game->destroyMesh();

	Screen->cleanUp();

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
	std::cout << "Do you want to play the Game <P> or use the Editor <E>?" << std::endl;
	char userInput;
	std::cin >> userInput;

	switch (userInput)
	{
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