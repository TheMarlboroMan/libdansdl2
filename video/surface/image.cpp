#include "image.h"

using namespace ldv;

//Toma propiedad del puntero a partir de este momento..

image::image(SDL_Surface * psurface)
	:surface(psurface)
{

}

image::image(const std::string& path, SDL_Window * window)
	:surface(ldv::load_image(path, window))
{

}

image::image(const std::string& path)
	:surface(ldv::load_image(path))
{

}

image& image::operator=(const image& o)
{
	surface::operator=(o);
	return *this;
}

image::image(const image& o)
{
	surface::operator=(o);
}


image::~image()
{

}
