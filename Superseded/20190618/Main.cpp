#include <SDL.h>
#include <stdio.h>
#include <time.h>
#include "Definitions.h"
#include "Sphere.h"
#include "Prism.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Torus.h"
#include "Cube.h"
#include "Camera.h"
#include "LightSource.h"

int main(int argc, char* argv[])
{

	LightSource lights[] = {LightSource(-75.0, 45.0, 1.0)};
	int nLight = sizeof(lights) / sizeof(LightSource);

	/*Cube cubes[] = { Cube(50.0, 0.0, -2.0, 5.0, getColour(0, 0, 255, 0)),
				 Cube(60.0, 0.0, -2.0, 5.0, getColour(0, 255, 0, 0)),
				 Cube(50.0, -8.0,  8.0, 5.0, getColour(0, 0, 0, 255)),
				 Cube(60.0, 0.0,  8.0, 5.0, getColour(0, 128, 128, 128)),
				 Cube(70.0, -20.0,  20.0, 1.0, getColour(0, 0, 0, 128)),
				 Cube(72.0, -20.0,  20.0, 1.0, getColour(0, 0, 0, 128)),
				 Cube(74.0, -20.0,  20.0, 1.0, getColour(0, 0, 0, 128)),
				 Cube(50.0, 10.0,  8.0, 10.0, getColour(0, 223, 223, 0)),
				 Cube(50.0, 25.0,  8.0, 10.0, getColour(0, 63, 63, 63)) };

	int nCube = sizeof(cubes) / sizeof(Cube);
	triangle3d** cubeTriangles = new triangle3d * [nCube];
	for (int i = 0; i < nCube; i++)
	{
		cubeTriangles[i] = new triangle3d[12];
		cubeTriangles[i] = cubes[i].getTriangleData();
	}*/


	/*Prism boxes[] = { Prism(-5.0, -5.0, 0.0, 50.0, 1.0, 1.0,		getColour(0, 0, 0, 255)),
					Prism(-5.0, -5.0, 10.0, 50.0, 1.0, 1.0,		getColour(0, 127, 127, 255)),
					Prism(10.0, 15.0, 2.5, 3.0, 4.0, 5.0,		getColour(0, 0, 0, 255)),
					Prism(-40.0, -20.0, -5.0, 20.0, 40.0, 1.0,	getColour(0, 0, 0, 127)) };

	int nPrism = sizeof(boxes) / sizeof(Prism);
	triangle3d** prismTriangles = new triangle3d * [nPrism];
	for (int i = 0; i < nPrism; i++)
	{
		prismTriangles[i] = new triangle3d[12];
		prismTriangles[i] = boxes[i].getTriangleData();
	}


	Sphere balls[] = {	Sphere( 0.0,  0.0,  40.0, 15, 36, getColour(0, 0, 0, 255)),
						Sphere(20.0, 10.0, -20.0, 10, 36, getColour(0, 127, 127, 127)),
						Sphere(40.0, 20.0, -10.0, 5, 36, getColour(0, 127, 127, 0)),
						Sphere(60.0, 30.0, 25.0, 5, 16, getColour(0, 235, 25, 140)),
						Sphere(80.0, 40.0,  0.0, 2.5, 12, getColour(0, 255, 0, 0)) };


	int nSphere = sizeof(balls) / sizeof(Sphere);
	triangle3d** sphereTriangles = new triangle3d * [nSphere];
	for (int i = 0; i < nSphere; i++)
	{
		int totalPoly = balls[i].getTotalPoly();
		sphereTriangles[i] = new triangle3d[totalPoly];
		sphereTriangles[i] = balls[i].getTriangleData();
	}*/


	Torus donuts[] = { Torus(0.0,  0.0,  10.0, 5.0, 1.0, 48, 48, getColour(0, 255, 0, 0)) };
	//Torus donuts[] = { Torus(0.0,  0.0,  10.0, getColour(0, 255, 0, 0)) };


	int nTorus = sizeof(donuts) / sizeof(Torus);
	triangle3d** torusTriangles = new triangle3d * [nTorus];
	for (int i = 0; i < nTorus; i++)
	{
		int totalPoly = donuts[i].getTotalPoly();
		torusTriangles[i] = new triangle3d[totalPoly];
		torusTriangles[i] = donuts[i].getTriangleData();
	}
	

	/*Cylinder cylinders[] = { Cylinder(0.0, 0.0, 0.0, 1.25, 5.0, 36, getColour(0, 127, 127, 127)) };

	int nCylinder = sizeof(cylinders) / sizeof(Cylinder);
	triangle3d** cylinderTriangles = new triangle3d * [nCylinder];
	for (int i = 0; i < nCylinder; i++)
	{
		int totalPoly = cylinders[i].getTotalPoly();
		cylinderTriangles[i] = new triangle3d[totalPoly];
		cylinderTriangles[i] = cylinders[i].getTriangleData();
	}*/


	/*Cone cones[] = { Cone(-30.0, -30.0, -30.0, 5.0, 25.0, 72, getColour(0, 127, 127, 255)) };

	int nCone = sizeof(cones) / sizeof(Cone);
	triangle3d** coneTriangles = new triangle3d * [nCone];
	for (int i = 0; i < nCone; i++)
	{
		int totalPoly = cones[i].getTotalPoly();
		coneTriangles[i] = new triangle3d[totalPoly];
		coneTriangles[i] = cones[i].getTriangleData();
	}*/

	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_Event event;
	SDL_Window* window = SDL_CreateWindow("Rendering Engine",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	SDL_Renderer* screen = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture* texture = SDL_CreateTexture(screen, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

	bool quit = false;
	Uint32* pixels = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
	if (pixels == NULL) { return 1; }
	else
	{
		for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
		{
			pixels[i] = 0;
		}
	}
	

	Camera Player{ 0.0, 0.0, 0.0, 1.0, -PI * 0.5, 0.0, 0.0, PI * 0.5, 1.0, 999.0, SCREEN_WIDTH, SCREEN_HEIGHT };
	double turnH, turnV;
	turnH = 0.0;
	turnV = 0.0;

	box Frustum;
	initFrustum(Player.getFovH(), Player.getFovV(), Player.zNear, Player.zFar, &Frustum);

	
	vect3 vertexList[MAXCLIPVERTS];
	const vect3 nullVector{ 0.0, 0.0, 0.0, 0.0 };
	for (int v = 0; v < MAXCLIPVERTS; v++)
	{
		vertexList[v] = { 0.0, 0.0, 0.0, 0.0 };
	}


	double moveP, strafeP, riseP, stepP, tiltP;
	moveP = 0;
	strafeP = 0;
	riseP = 0;
	tiltP = 0;
	stepP = Player.step;
	

	double* zBuffer = new double[SCREEN_WIDTH * SCREEN_HEIGHT];
	if (zBuffer == NULL) { return 1; }
	else
	{
		for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
		{
			zBuffer[i] = Player.zFar;
		}
	}


	while (!quit)
	{
		for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
		{
			pixels[i] = 0;
			zBuffer[i] = Player.zFar;
		}

		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_PAGEUP] && !state[SDL_SCANCODE_PAGEDOWN])
		{
			if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
			{
				riseP = stepP * 4;
			}
			else
			{
				riseP = stepP;
			}
		}
		if (state[SDL_SCANCODE_PAGEDOWN] && !state[SDL_SCANCODE_PAGEUP])
		{
			if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
			{
				riseP = -stepP * 4;
			}
			else
			{
				riseP = -stepP;
			}
		}
		if (state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN])
		{
			if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
			{
				moveP = stepP * 4;
			}
			else
			{
				moveP = stepP;
			}
		}
		if (state[SDL_SCANCODE_DOWN] && !state[SDL_SCANCODE_UP])
		{
			if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
			{
				moveP = -stepP * 4;
			}
			else
			{
				moveP = -stepP;
			}
		}
		if (state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT])
		{
			if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
			{
				strafeP = -stepP * 4;
			}
			else
			{
				strafeP = -stepP;
			}
			tiltP = 0.03;
		}
		if (state[SDL_SCANCODE_RIGHT] && !state[SDL_SCANCODE_LEFT])
		{
			if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
			{
				strafeP = stepP * 4;
			}
			else
			{
				strafeP = stepP;
			}
			tiltP = -0.03;
		}

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_TAB:

					break;
				case SDLK_ESCAPE:
					quit = true;
					break;
				}
			}

			if (event.type == SDL_MOUSEMOTION)
			{
				turnH = (double)event.motion.x * SENSITIVITY;
				turnV = (double)event.motion.y * SENSITIVITY;
			}

			if (event.type == SDL_QUIT) { quit = true; }
		}

		Player.azm = -turnH;
		Player.alt = -turnV;
		Player.rol = tiltP;

		Player.x -= moveP * cos(Player.azm) - strafeP * cos(Player.azm + PI * 0.5);
		Player.y += moveP * sin(Player.azm) - strafeP * sin(Player.azm + PI * 0.5);
		Player.z += riseP;

		moveP	= 0;
		strafeP = 0;
		riseP	= 0;
		tiltP	= 0;

		double sinAzm = sin(Player.azm + PI / 2);
		double cosAzm = cos(Player.azm + PI / 2);
		double sinAlt = sin(Player.alt + PI / 2);
		double cosAlt = cos(Player.alt + PI / 2);
		double sinRol = sin(Player.rol);
		double cosRol = cos(Player.rol);
		double hRatio = Player.getHRatio();
		double vRatio = Player.getVRatio();


		/*for (int i = 0; i < nCube; i++)									//For every cube
		{
			int total = 12;
			Uint32 cubeColour = cubes[i].getColour();
			unsigned char r, g, b;
			r = cubes[i].getRedValue();
			g = cubes[i].getGreenValue();
			b = cubes[i].getBlueValue();
			double illum;
			for (int k = 0; k < total; k++)								//For every triangle (2 per side)
			{				
				vect3 eyeVector = subVectors(cubeTriangles[i][k].A, { Player.x, Player.y, Player.z, 1.0 });
				if (dotProduct(cubeTriangles[i][k].N, eyeVector) < 0)	//Backface culling is performed here
				{
					triangle3d viewT;
					triangle2d screenT;

					viewT.A = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, cubeTriangles[i][k].A);
					viewT.B = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, cubeTriangles[i][k].B);
					viewT.C = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, cubeTriangles[i][k].C);
					viewT.N = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, cubeTriangles[i][k].N);

					for (int v = 0; v < MAXCLIPVERTS; v++)
					{
						vertexList[v] = { 0.0, 0.0, 0.0, 0.0 };
					}

					vertexList[0] = viewT.A;
					vertexList[1] = viewT.B;
					vertexList[2] = viewT.C;

					int nVert = clipToFrustum(vertexList, Frustum);

					illum = 0.0;
					vect3 n = cubeTriangles[i][k].N;
					for (int p = 0; p < nLight; p++)
					{
						illum += lights[p].getIllumination(n);
					}
					clampValue(&illum, 0.05, 1.0);
					
					for (int i = 0; i < nVert - 2; i++)
					{
						screenT.a = view2screen(vertexList[0],		SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);
						screenT.b = view2screen(vertexList[1 + i],	SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);
						screenT.c = view2screen(vertexList[2 + i],	SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);
						screenT.h = getColour(0, (byte)(r * illum), (byte)(g * illum), (byte)(b * illum));

						fillTriangleZ(screenT, pixels, zBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, screenT.h);
					}
				}			
			}
		}
		

		for (int i = 0; i < nPrism; i++)									//For every prism
		{
			int total = 12;
			Uint32 prismColour = boxes[i].getColour();
			unsigned char r, g, b;
			r = boxes[i].getRedValue();
			g = boxes[i].getGreenValue();
			b = boxes[i].getBlueValue();
			double illum;
			for (int k = 0; k < total; k++)								//For every triangle (2 per side)
			{
				vect3 eyeVector = subVectors(prismTriangles[i][k].A, { Player.x, Player.y, Player.z, 1.0 });
				if (dotProduct(prismTriangles[i][k].N, eyeVector) < 0)	//Backface culling is performed here
				{
					triangle3d viewT;
					triangle2d screenT;

					viewT.A = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, prismTriangles[i][k].A);
					viewT.B = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, prismTriangles[i][k].B);
					viewT.C = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, prismTriangles[i][k].C);
					viewT.N = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, prismTriangles[i][k].N);

					for (int v = 0; v < MAXCLIPVERTS; v++)
					{
						vertexList[v] = { 0.0, 0.0, 0.0, 0.0 };
					}

					vertexList[0] = viewT.A;
					vertexList[1] = viewT.B;
					vertexList[2] = viewT.C;

					int nVert = clipToFrustum(vertexList, Frustum);

					illum = 0.0;
					vect3 n = prismTriangles[i][k].N;
					for (int p = 0; p < nLight; p++)
					{
						illum += lights[p].getIllumination(n);
					}
					clampValue(&illum, 0.05, 1.0);

					for (int i = 0; i < nVert - 2; i++)
					{
						screenT.a = view2screen(vertexList[0], SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);
						screenT.b = view2screen(vertexList[1 + i], SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);
						screenT.c = view2screen(vertexList[2 + i], SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);
						screenT.h = getColour(0, (byte)(r * illum), (byte)(g * illum), (byte)(b * illum));

						fillTriangleZ(screenT, pixels, zBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, screenT.h);
					}
				}
			}
		}


		for (int i = 0; i < nSphere; i++)									//For every sphere
		{
			int totalPoly = balls[i].getTotalPoly();
			Uint32 ballColour = balls[i].getColour();
			unsigned char r, g, b;
			r = balls[i].getRedValue();
			g = balls[i].getGreenValue();
			b = balls[i].getBlueValue();
			double illum;
			for (int k = 0; k < totalPoly; k++)								//For every triangle (2 per side)
			{
				vect3 eyeVector = subVectors(sphereTriangles[i][k].A, { Player.x, Player.y, Player.z, 1.0 });
				if (dotProduct(sphereTriangles[i][k].N, eyeVector) < 0)	//Backface culling is performed here
				{
					triangle3d viewT;
					triangle2d screenT;

					viewT.A = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, sphereTriangles[i][k].A);
					viewT.B = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, sphereTriangles[i][k].B);
					viewT.C = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, sphereTriangles[i][k].C);
					viewT.N = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, sphereTriangles[i][k].N);

					for (int v = 0; v < MAXCLIPVERTS; v++)
					{
						vertexList[v] = { 0.0, 0.0, 0.0, 0.0 };
					}

					vertexList[0] = viewT.A;
					vertexList[1] = viewT.B;
					vertexList[2] = viewT.C;

					int nVert = clipToFrustum(vertexList, Frustum);

					illum = 0.0;
					vect3 n = sphereTriangles[i][k].N;
					for (int p = 0; p < nLight; p++)
					{
						illum += lights[p].getIllumination(n);
					}
					clampValue(&illum, 0.05, 1.0);

					for (int i = 0; i < nVert - 2; i++)
					{
						screenT.a = view2screen(vertexList[0],		SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);
						screenT.b = view2screen(vertexList[1 + i],	SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);
						screenT.c = view2screen(vertexList[2 + i],	SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);						
						screenT.h = getColour(0, (byte)(r * illum), (byte)(g * illum), (byte)(b * illum));

						fillTriangleZ(screenT, pixels, zBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, screenT.h);
					}
				}
			}
		}


		for (int i = 0; i < nCylinder; i++)									//For every cylinder
		{
			int totalPoly = cylinders[i].getTotalPoly();
			Uint32 cylinderColour = cylinders[i].getColour();
			unsigned char r, g, b;
			r = cylinders[i].getRedValue();
			g = cylinders[i].getGreenValue();
			b = cylinders[i].getBlueValue();
			double illum;
			for (int k = 0; k < totalPoly; k++)								//For every triangle (2 per side)
			{
				vect3 eyeVector = subVectors(cylinderTriangles[i][k].A, { Player.x, Player.y, Player.z, 1.0 });
				if (dotProduct(cylinderTriangles[i][k].N, eyeVector) < 0)	//Backface culling is performed here
				{
					triangle3d viewT;
					triangle2d screenT;

					viewT.A = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, cylinderTriangles[i][k].A);
					viewT.B = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, cylinderTriangles[i][k].B);
					viewT.C = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, cylinderTriangles[i][k].C);
					viewT.N = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, cylinderTriangles[i][k].N);

					for (int v = 0; v < MAXCLIPVERTS; v++)
					{
						vertexList[v] = { 0.0, 0.0, 0.0, 0.0 };
					}

					vertexList[0] = viewT.A;
					vertexList[1] = viewT.B;
					vertexList[2] = viewT.C;

					int nVert = clipToFrustum(vertexList, Frustum);

					illum = 0.0;
					vect3 n = cylinderTriangles[i][k].N;
					for (int p = 0; p < nLight; p++)
					{
						illum += lights[p].getIllumination(n);
					}
					clampValue(&illum, 0.05, 1.0);

					for (int i = 0; i < nVert - 2; i++)
					{
						screenT.a = view2screen(vertexList[0], SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);
						screenT.b = view2screen(vertexList[1 + i], SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);
						screenT.c = view2screen(vertexList[2 + i], SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);
						screenT.h = getColour(0, (byte)(r * illum), (byte)(g * illum), (byte)(b * illum));

						fillTriangleZ(screenT, pixels, zBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, screenT.h);
					}
				}
			}
		}


		for (int i = 0; i < nCone; i++)									//For every cylinder
		{
			int totalPoly = cones[i].getTotalPoly();
			Uint32 coneColour = cones[i].getColour();
			unsigned char r, g, b;
			r = cones[i].getRedValue();
			g = cones[i].getGreenValue();
			b = cones[i].getBlueValue();
			double illum;
			for (int k = 0; k < totalPoly; k++)								//For every triangle (2 per side)
			{
				vect3 eyeVector = subVectors(coneTriangles[i][k].A, { Player.x, Player.y, Player.z, 1.0 });
				if (dotProduct(coneTriangles[i][k].N, eyeVector) < 0)	//Backface culling is performed here
				{
					triangle3d viewT;
					triangle2d screenT;

					viewT.A = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, coneTriangles[i][k].A);
					viewT.B = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, coneTriangles[i][k].B);
					viewT.C = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, coneTriangles[i][k].C);
					viewT.N = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, coneTriangles[i][k].N);

					for (int v = 0; v < MAXCLIPVERTS; v++)
					{
						vertexList[v] = { 0.0, 0.0, 0.0, 0.0 };
					}

					vertexList[0] = viewT.A;
					vertexList[1] = viewT.B;
					vertexList[2] = viewT.C;

					int nVert = clipToFrustum(vertexList, Frustum);

					illum = 0.0;
					vect3 n = coneTriangles[i][k].N;
					for (int p = 0; p < nLight; p++)
					{
						illum += lights[p].getIllumination(n);
					}
					clampValue(&illum, 0.05, 1.0);

					for (int i = 0; i < nVert - 2; i++)
					{
						screenT.a = view2screen(vertexList[0], SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);
						screenT.b = view2screen(vertexList[1 + i], SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);
						screenT.c = view2screen(vertexList[2 + i], SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);
						screenT.h = getColour(0, (byte)(r * illum), (byte)(g * illum), (byte)(b * illum));

						fillTriangleZ(screenT, pixels, zBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, screenT.h);
					}
				}
			}
		}*/


		for (int i = 0; i < nTorus; i++)									//For every cylinder
		{
			int totalPoly = donuts[i].getTotalPoly();
			Uint32 torusColour = donuts[i].getColour();
			unsigned char r, g, b;
			r = donuts[i].getRedValue();
			g = donuts[i].getGreenValue();
			b = donuts[i].getBlueValue();
			double illum;
			for (int k = 0; k < totalPoly; k++)								//For every triangle (2 per side)
			{
				vect3 eyeVector = subVectors(torusTriangles[i][k].A, { Player.x, Player.y, Player.z, 1.0 });
				if (dotProduct(torusTriangles[i][k].N, eyeVector) < 0)	//Backface culling is performed here
				{
					triangle3d viewT;
					triangle2d screenT;

					viewT.A = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, torusTriangles[i][k].A);
					viewT.B = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, torusTriangles[i][k].B);
					viewT.C = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, torusTriangles[i][k].C);
					viewT.N = world2viewT(sinAzm, cosAzm, sinAlt, cosAlt, sinRol, cosRol, Player, torusTriangles[i][k].N);

					for (int v = 0; v < MAXCLIPVERTS; v++)
					{
						vertexList[v] = { 0.0, 0.0, 0.0, 0.0 };
					}

					vertexList[0] = viewT.A;
					vertexList[1] = viewT.B;
					vertexList[2] = viewT.C;

					int nVert = clipToFrustum(vertexList, Frustum);

					illum = 0.0;
					vect3 n = torusTriangles[i][k].N;
					for (int p = 0; p < nLight; p++)
					{
						illum += lights[p].getIllumination(n);
					}
					clampValue(&illum, 0.05, 1.0);

					for (int i = 0; i < nVert - 2; i++)
					{
						screenT.a = view2screen(vertexList[0], SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);
						screenT.b = view2screen(vertexList[1 + i], SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);
						screenT.c = view2screen(vertexList[2 + i], SCREEN_WIDTH, SCREEN_HEIGHT, hRatio, vRatio);
						screenT.h = getColour(0, (byte)(r * illum), (byte)(g * illum), (byte)(b * illum));

						fillTriangleZ(screenT, pixels, zBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, screenT.h);
					}
				}
			}
		}


		drawCrosshair(pixels, SCREEN_WIDTH, SCREEN_HEIGHT, 4, 12, getColour(0, 127, 127, 127));

		SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
		SDL_RenderClear(screen);

		SDL_RenderCopy(screen, texture, NULL, NULL);
		SDL_RenderPresent(screen);

	}

	/*for (int i = 0; i < nPrism; i++) { delete[] prismTriangles[i]; }
	delete[] prismTriangles;

	for (int i = 0; i < nCube; i++) { delete[] cubeTriangles[i]; }
	delete[] cubeTriangles;

	for (int i = 0; i < nSphere; i++) { delete[] sphereTriangles[i]; }
	delete[] sphereTriangles;

	for (int i = 0; i < nCylinder; i++) { delete[] cylinderTriangles[i]; }
	delete[] cylinderTriangles;

	for (int i = 0; i < nCone; i++) { delete[] coneTriangles[i]; }
	delete[] coneTriangles;*/

	for (int i = 0; i < nTorus; i++) { delete[] torusTriangles[i]; }
	delete[] torusTriangles;

	delete[] pixels;

	/*for (int j = 0; j < 30; j++)
	{
		for (int i = 0; i < 30; i++)
		{
			printf("%.2f ", zBuffer[(SCREEN_HEIGHT / 2 - 15 + j) * SCREEN_WIDTH + SCREEN_WIDTH / 2 -15 + i]);
		}
		printf("\n");
	}*/

	delete[] zBuffer;

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(screen);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;

}