#include "canvas.h"

using namespace ldv;

//!Default constructor.

//!Creates an empty canvas... It is private so can only be called from 
//!statis functions.

canvas::canvas()
	:surface(), generated(false)
{
}

//!Copy constructor.

canvas::canvas(const canvas& o)
	:surface(o), generated(o.generated)
{

}

//!Assignment operator.

//!Copies the referenced canvas without changing it. The underlying surface
//!is also copied.

canvas& canvas::operator=(const canvas& o)
	
{
	surface::operator=(o);
	generated=o.generated;
	return *this;
}

//!Class destructor.

canvas::~canvas()
{

}

//!Creates a canvas with the specified size and surface parameters.

//!bpp and rgba masks are copied from the surface.

canvas * canvas::create(int w, int h, const SDL_Surface * surface)
{
	return create(w, h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
}

//!Creates a canvas specifying all parameters.

canvas * canvas::create(int w, int h, int bpp, Uint32 r, Uint32 g, Uint32 b, Uint32 a)
{
	canvas * result=NULL;
	result=new canvas();
	result->generated=true;
	result->sdl_surface=SDL_CreateRGBSurface(0, w, h, bpp, r, g, b, a);
	return result;
}
