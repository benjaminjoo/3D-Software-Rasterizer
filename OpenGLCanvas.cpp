#include "OpenGLCanvas.h"
#include <GLEW/glew.h>
#include <iostream>


OpenGLCanvas::OpenGLCanvas(int w, int h, const std::string& title)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,		8	);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,		8	);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,		8	);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,		8	);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,		32	);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,		16	);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,	1	);

	width		= w;
	height		= h;

	window		= SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	glContext	= SDL_GL_CreateContext(window);
	closed		= false;

	GLenum status = glewInit();

	if (status != GLEW_OK)
	{
		std::cerr << "Glew failed to initialise!" << std::endl;
	}

	SDL_ShowCursor(SDL_DISABLE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
}


OpenGLCanvas::~OpenGLCanvas()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void OpenGLCanvas::clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void OpenGLCanvas::update()
{
	SDL_GL_SwapWindow(window);

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			closed = true;
		}

		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				closed = true;
				break;
			}
		}
	}
}


bool OpenGLCanvas::isClosed()
{
	return closed;
}


float OpenGLCanvas::getAspect()
{
	return (float)width / (float)height;
}
