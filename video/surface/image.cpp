#include "image.h"

using namespace ldv;

//!Creates an image from the surface.

//!The surface will be owned by the image from then on.

image::image(SDL_Surface * psurface)
	:surface(psurface)
{

}

//!Creates an image from a string path.

//!This is an old version pre-openGL where we would use the window color depth.

image::image(const std::string& path, SDL_Window * window)
	:surface(ldv::load_image(path, window))
{

}

//!Creates an image from a string path.

image::image(const std::string& path)
	:surface(ldv::load_image(path))
{

}

//!Assignment operator.

//!Makes a copy of the underlying surface without changing the reference.

image& image::operator=(const image& o)
{
	surface::operator=(o);
	return *this;
}

//!Copy constructor.

//!Makes a copy of the underlying surface without changing the reference.

image::image(const image& o)
	:surface(o)
{

}

image::~image()
{

}
