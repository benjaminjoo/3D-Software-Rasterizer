#pragma once

#include <SDL/SDL.h>
#include <string>

class OpenGLCanvas
{
private:

	int				width;
	int				height;

	SDL_Window*		window;
	SDL_GLContext	glContext;

	bool			closed;

public:
	OpenGLCanvas(int w, int h, const std::string& title);
	~OpenGLCanvas();

	void clear(float r, float g, float b, float a);
	void update();
	bool isClosed();
	float getAspect();
};

