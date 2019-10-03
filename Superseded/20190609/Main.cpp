#include <SDL.h>
#include <stdio.h>
#include <time.h>
#include "Definitions.h"
#include "Sphere.h"
#include "Prism.h"
#include "Cube.h"




int main(int argc, char* argv[])
{
	int pCount = 0;
	double edge = 15.0;
	int num = 15;
	int total = num * num * num - (num - 2) * (num - 2) * (num - 2);
	double edgeUnit = edge / (double)num;
	vect3* cube = new vect3[total];
	for (int k = 0; k < num; k++)
	{
		for (int j = 0; j < num; j++)
		{
			for (int i = 0; i < num; i++)
			{
				if (i == 0 || i == num - 1 ||
					j == 0 || j == num - 1 ||
					k == 0 || k == num - 1)
				{
					cube[pCount].x = i * edgeUnit;
					cube[pCount].y = j * edgeUnit;
					cube[pCount].z = k * edgeUnit;
					cube[pCount].w = 1.0;
					pCount++;
				}
			}
		}
	}

	Sphere ball(25.0, 5.0, 2.5, 7.5, 48);

	Prism box(20.0, 15.0, 10.0, 30.0, 40.0, 50.0, 30);

	/*
	Cube cube_1(10, 10, 5, 3, 255);
	triangle3d* cubeArray = cube_1.getTriangleData();
	printf("%.2f\t%.2f\t%.2f\n", cubeArray[3].N.x, cubeArray[3].N.y, cubeArray[3].N.z);
	cube_1.getColour();
	*/

	Cube cubeArray[1] = {  };

	triangle3d* cubes = cubeArray[0].getTriangleData();

	int cnt = 0;
	double radius = 10.0;
	double angleh = 0.0, anglev = 0.0;
	for (int i = 1; i < 48; i++)
	{
		for (int j = 0; j < 48; j++) { cnt++; }
	}
	vect3* sphere = new vect3[cnt];
	cnt = 0;
	double x, y, z;
	for (int i = 1; i < 48; i++)
	{
		for (int j = 0; j < 48; j++)
		{
			anglev = (-90.0 + i * 3.75) *PI / 180.0;
			angleh = (j * 7.5) *PI / 180.0;
			x = radius * cos(anglev) * cos(angleh);
			y = radius * cos(anglev) * sin(angleh);
			z = radius * sin(anglev);
			sphere[cnt].x = x;
			sphere[cnt].y = y;
			sphere[cnt].z = z;
			sphere[cnt].w = 1.0;
			cnt++;
		}
	}


	printf("%d\n", cnt);


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

	coord2 triangle[3];
	triangle[0] = { (int)(SCREEN_WIDTH / 2), (int)(SCREEN_HEIGHT / 2) };
	triangle[1] = { (int)(SCREEN_WIDTH / 2) + 10, (int)(SCREEN_HEIGHT / 2) };
	triangle[2] = { (int)(SCREEN_WIDTH / 2) + 10, (int)(SCREEN_HEIGHT / 2) + 10};
	Uint32 tc = getColour(0, 0, 255, 0);

	//clock_t start = clock();
	//int frame_count = 0;

	while (!quit)
	{
		/*
		frame_count++;
		clock_t current = clock();
		int elapsed = ((float)current - (float)start) / 1000;
		if (elapsed > 1.0)
		{
			printf("FPS: %d\tElapsed time between two frames: %d\n", frame_count, current - start);
			start = clock();
			frame_count = 0;
		}
		*/


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
			tiltP = 0.03;// PI* 0.01;
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
			tiltP = -0.03;//-PI * 0.01;
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


		for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
		{
			pixels[i] = 0;
		}



		coord2 playerPos = world2Plan(Player.x, Player.y, 0.01);
		Player.azm = -turnH;
		Player.alt = -turnV;
		Player.rol = tiltP;

		Player.x -= moveP * cos(Player.azm) - strafeP * cos(Player.azm + PI * 0.5);
		Player.y += moveP * sin(Player.azm) - strafeP * sin(Player.azm + PI * 0.5);
		Player.z += riseP;

		moveP = 0;
		strafeP = 0;
		riseP = 0;
		tiltP = 0;
		




		Uint32 pc1 = getColour(0, 0, 0, 255);

		for (int k = 0; k < cnt; k++)
		{
			vect3	viewP;
			coord2	screenP;

			viewP = world2view(Player, sphere[k]);

			if(viewP.z > 0)
			{
				screenP = view2screen(viewP, SCREEN_WIDTH, SCREEN_HEIGHT);
				
				if (screenP.y * SCREEN_WIDTH + screenP.x >= 0 &&
					screenP.y * SCREEN_WIDTH + screenP.x < SCREEN_WIDTH * SCREEN_HEIGHT &&
					(screenP.x > 0 && screenP.x < SCREEN_WIDTH) &&
					(screenP.y > 0 && screenP.y < SCREEN_HEIGHT))
				{
					pixels[screenP.y * SCREEN_WIDTH + screenP.x] = pc1;					
				}
			}
		}


		Uint32 pc2 = getColour(0, 0, 255, 0);

		for (int k = 0; k < total; k++)
		{
			vect3	viewP;
			coord2	screenP;

			viewP = world2view(Player, cube[k]);

			if (viewP.z > 0)
			{
				screenP = view2screen(viewP, SCREEN_WIDTH, SCREEN_HEIGHT);

				if (screenP.y * SCREEN_WIDTH + screenP.x >= 0 &&
					screenP.y * SCREEN_WIDTH + screenP.x < SCREEN_WIDTH * SCREEN_HEIGHT &&
					(screenP.x > 0 && screenP.x < SCREEN_WIDTH) &&
					(screenP.y > 0 && screenP.y < SCREEN_HEIGHT))
				{
					pixels[screenP.y * SCREEN_WIDTH + screenP.x] = pc2;
				}
			}
		}
		

		Uint32 pc3 = getColour(0, 255, 0, 0);
		
		vect3* vertexData = ball.getVertexData();
		int total = ball.getTotal();
		if (vertexData != NULL)
		{
			for (int k = 0; k < total; k++)
			{
				vect3	viewP;
				coord2	screenP;

				viewP = world2view(Player, vertexData[k]);

				if (viewP.z > 0)
				{
					screenP = view2screen(viewP, SCREEN_WIDTH, SCREEN_HEIGHT);

					if (screenP.y * SCREEN_WIDTH + screenP.x >= 0 &&
						screenP.y * SCREEN_WIDTH + screenP.x < SCREEN_WIDTH * SCREEN_HEIGHT &&
						(screenP.x > 0 && screenP.x < SCREEN_WIDTH) &&
						(screenP.y > 0 && screenP.y < SCREEN_HEIGHT))
					{
						pixels[screenP.y * SCREEN_WIDTH + screenP.x] = pc3;
					}
				}
			}
		}
		




		vect3* boxData = box.getVertexData();
		int t = box.getTotal();
		if (boxData != NULL)
		{
			for (int k = 0; k < t; k++)
			{
				vect3	viewP;
				coord2	screenP;

				viewP = world2view(Player, boxData[k]);

				if (viewP.z > 0)
				{
					screenP = view2screen(viewP, SCREEN_WIDTH, SCREEN_HEIGHT);

					if (screenP.y * SCREEN_WIDTH + screenP.x >= 0 &&
						screenP.y * SCREEN_WIDTH + screenP.x < SCREEN_WIDTH * SCREEN_HEIGHT &&
						(screenP.x > 0 && screenP.x < SCREEN_WIDTH) &&
						(screenP.y > 0 && screenP.y < SCREEN_HEIGHT))
					{
						pixels[screenP.y * SCREEN_WIDTH + screenP.x] = pc3;
					}
				}
			}

		}
		
		fillTriangle(triangle, pixels, SCREEN_WIDTH, SCREEN_HEIGHT, tc);







		SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
		SDL_RenderClear(screen);

		SDL_RenderCopy(screen, texture, NULL, NULL);
		SDL_RenderPresent(screen);

	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(screen);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;

}