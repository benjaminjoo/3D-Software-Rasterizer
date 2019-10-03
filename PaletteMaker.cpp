#include "PaletteMaker.h"

#include <SDL.h>
#include <SDL_image.h>



PaletteMaker::PaletteMaker()
{
	SDL_Surface* palette = IMG_Load("Palette/quake1palette.jpg");
}


PaletteMaker::~PaletteMaker()
{
}
