#include <SDL.h>
#include <stdio.h>
#include <time.h>
#include "Definitions.h"
#include "Sphere.h"
#include "Prism.h"
#include "Cube.h"

int main(int argc, char* argv[])
{
	//const double zFar = 999.0;
	//const double zVis = 99.9;
	const light sun{ 30.0, 60.0, 1.0 };
	vect3 sunVector = dirVector(sun.azm, sun.alt);

	Sphere balls[] = {	Sphere( 0.0,  0.0,  0.0, 5, 24),
						Sphere(20.0, 10.0, 10.0, 5, 48),
						Sphere(40.0, 20.0,  0.0, 5, 24),
						Sphere(60.0, 30.0, 10.0, 5, 48),
						Sphere(80.0, 40.0,  0.0, 5, 24) };

	int nSphere = sizeof(balls) / sizeof(Sphere);
	vect3** sphereVertices = new vect3 * [nSphere];
	for (int i = 0; i < nSphere; i++)
	{
		int n = balls[i].getTotal();
		printf("Number of vertices in current sphere: %d\n", n);
		sphereVertices[i] = new vect3[n];
		sphereVertices[i] = balls[i].getVertexData();
	}


	Prism boxes[] = {	Prism(-5.0, -5.0, 0.0, 5.0, 1.0, 1.0, 10),
						Prism(-5.0, -5.0, 10.0, 5.0, 1.0, 1.0, 10),
						Prism( 10.0, 15.0, 2.5, 3.0, 4.0, 5.0, 10),
						Prism( -10.0, -10.0, -1.0, 20.0, 40.0, 1.0, 20)};

	int nPrism = sizeof(boxes) / sizeof(Prism);
	vect3** prismVertices = new vect3 * [nPrism];
	for (int i = 0; i < nPrism; i++)
	{
		int n = boxes[i].getTotal();
		printf("Number of vertices in current prism: %d\n", n);
		prismVertices[i] = new vect3[n];
		prismVertices[i] = boxes[i].getVertexData();
	}


	Cube cubes[] = { Cube(50.0, 0.0, -2.0, 5.0, getColour(0, 0, 255, 0)),
					 Cube(60.0, 0.0, -2.0, 5.0, getColour(0, 255, 0, 0)),
					 Cube(50.0, 0.0,  8.0, 5.0, getColour(0, 0, 0, 255)),
					 Cube(60.0, 0.0,  8.0, 5.0, getColour(0, 128, 128, 128)),
					 Cube(50.0, 10.0,  8.0, 10.0, getColour(0, 255, 128, 0)),
					 Cube(50.0, 25.0,  8.0, 10.0, getColour(0, 15, 31, 223)) };

	
	int nCube = sizeof(cubes) / sizeof(Cube);
	printf("Number of cubes created: %d\n", nCube);
	
	triangle3d** cubeTriangles = new triangle3d * [nCube];
	for (int i = 0; i < nCube; i++)
	{
		cubeTriangles[i] = new triangle3d[12];
		cubeTriangles[i] = cubes[i].getTriangleData();
	}
	
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_ShowCursor(SDL_DISABLE);
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

	double* zBuffer = new double[SCREEN_WIDTH * SCREEN_HEIGHT];
	if (zBuffer == NULL) { return 1; }
	else
	{
		for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
		{
			zBuffer[i] = zFar;
		}
	}

	camera Player{ 0.0, 0.0, 0.0, 1.0, 0, 0.0, 0.0 };
	double turnH, turnV;
	turnH = 0.0;
	turnV = 0.0;

	double moveP, strafeP, riseP, stepP, tiltP;
	moveP = 0;
	strafeP = 0;
	riseP = 0;
	tiltP = 0;
	stepP = Player.step;

	Uint32 tc = getColour(0, 0, 255, 0);
	triangle2d crosshair;
	crosshair.a = { (int)(SCREEN_WIDTH / 2), (int)(SCREEN_HEIGHT / 2) };
	crosshair.b = { (int)(SCREEN_WIDTH / 2) + 10, (int)(SCREEN_HEIGHT / 2) };
	crosshair.c = { (int)(SCREEN_WIDTH / 2) + 10, (int)(SCREEN_HEIGHT / 2) + 10 };

	while (!quit)
	{
		for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
		{
			pixels[i] = 0;
			zBuffer[i] = zFar;
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

		Uint32 ballColour = getColour(0, 0, 0, 255);
		for (int i = 0; i < nSphere; i++)
		{
			int total = balls[i].getTotal();
			for (int k = 0; k < total; k++)
			{
				vect3 viewP;
				coord2	screenP;

				viewP = world2view(Player, sphereVertices[i][k]);

				if (viewP.z > 0)
				{
					screenP = view2screen(viewP, SCREEN_WIDTH, SCREEN_HEIGHT);

					if (screenP.y * SCREEN_WIDTH + screenP.x >= 0 &&
						screenP.y * SCREEN_WIDTH + screenP.x < SCREEN_WIDTH * SCREEN_HEIGHT &&
						(screenP.x > 0 && screenP.x < SCREEN_WIDTH) &&
						(screenP.y > 0 && screenP.y < SCREEN_HEIGHT))
					{
						pixels[screenP.y * SCREEN_WIDTH + screenP.x] = ballColour;
					}
				}
			}
		}

		Uint32 boxColour = getColour(0, 255, 0, 0);
		for (int i = 0; i < nPrism; i++)
		{
			int total = boxes[i].getTotal();
			for (int k = 0; k < total; k++)
			{
				vect3 viewP;
				coord2	screenP;

				viewP = world2view(Player, prismVertices[i][k]);

				if (viewP.z > 0)
				{
					screenP = view2screen(viewP, SCREEN_WIDTH, SCREEN_HEIGHT);

					if (screenP.y * SCREEN_WIDTH + screenP.x >= 0 &&
						screenP.y * SCREEN_WIDTH + screenP.x < SCREEN_WIDTH * SCREEN_HEIGHT &&
						(screenP.x > 0 && screenP.x < SCREEN_WIDTH) &&
						(screenP.y > 0 && screenP.y < SCREEN_HEIGHT))
					{
						pixels[screenP.y * SCREEN_WIDTH + screenP.x] = boxColour;
					}
				}
			}
		}

		
		for (int i = 0; i < nCube; i++)									//For every cube
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
				triangle3d viewT;
				triangle2d screenT;

				viewT.A = world2view(Player, cubeTriangles[i][k].A);	//For every vertex
				viewT.B = world2view(Player, cubeTriangles[i][k].B);
				viewT.C = world2view(Player, cubeTriangles[i][k].C);
				viewT.N = world2view(Player, cubeTriangles[i][k].N);	//And the surface normal

				//Perform backface culling here

				//if (viewT.A.z > 0 && viewT.B.z > 0 && viewT.C.z > 0)
				//if (viewT.A.z > 0 || viewT.B.z > 0 || viewT.C.z > 0)
				if(1)
				{
					if (dotProduct(viewT.N, viewT.B) < 0)				//Performing backface culling here
					{
						//illumination = dotProduct(viewT.N, sunVector) * sun.intensity;
						illum = dotProduct(cubeTriangles[i][k].N, sunVector) * sun.intensity;
						screenT.a = view2screen(viewT.A, SCREEN_WIDTH, SCREEN_HEIGHT);
						screenT.b = view2screen(viewT.B, SCREEN_WIDTH, SCREEN_HEIGHT);
						screenT.c = view2screen(viewT.C, SCREEN_WIDTH, SCREEN_HEIGHT);
						//r = (unsigned char)(r * illum);
						//g = (unsigned char)(g * illum);
						//b = (unsigned char)(b * illum);
						//screenT.h = getColour(0, r, g, b);
						screenT.h = getColour(0, r * illum, g * illum, b * illum);

						if (onScreen(screenT.a, SCREEN_WIDTH, SCREEN_HEIGHT) &&
							onScreen(screenT.b, SCREEN_WIDTH, SCREEN_HEIGHT) &&
							onScreen(screenT.c, SCREEN_WIDTH, SCREEN_HEIGHT))
						{
							fillTriangleZ(screenT, pixels, zBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, screenT.h);
							//fillTriangleZLit(screenT, pixels, zBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, illumination, r, g, b);
						}
					}
				}
			}
		}
		


		//fillTriangle(crosshair, pixels, SCREEN_WIDTH, SCREEN_HEIGHT, tc);
		drawCrosshair(pixels, SCREEN_WIDTH, SCREEN_HEIGHT, 4, 12, getColour(0, 127, 127, 127));

		SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
		SDL_RenderClear(screen);

		SDL_RenderCopy(screen, texture, NULL, NULL);
		SDL_RenderPresent(screen);

	}


	for (int i = 0; i < nSphere; i++){ delete[] sphereVertices[i]; }
	delete[] sphereVertices;

	for (int i = 0; i < nPrism; i++) { delete[] prismVertices[i]; }
	delete[] prismVertices;

	for (int i = 0; i < nCube; i++) { delete[] cubeTriangles[i]; }
	delete[] cubeTriangles;

	delete[] pixels;


	for (int j = 0; j < 30; j++)
	{
		for (int i = 0; i < 30; i++)
		{
			//printf("%3.2f ", zBuffer[(j + SCREEN_HEIGHT / 2) * SCREEN_WIDTH + SCREEN_WIDTH / 2 + i]);
		}
		//printf("\n");
	}



	delete[] zBuffer;

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(screen);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;

}