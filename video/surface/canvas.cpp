#include "canvas.h"

using namespace ldv;

canvas::canvas()
	:surface(), generated(false)
{
}


canvas::canvas(const canvas& o)
	:surface(o), generated(o.generated)
{

}

canvas& canvas::operator=(const canvas& o)
	
{
	surface::operator=(o);
	generated=o.generated;
	return *this;
}

canvas::~canvas()
{	

}

canvas * canvas::create(int w, int h, const SDL_Surface * surface)
{
	return create(w, h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
}

canvas * canvas::create(int w, int h, int bpp, Uint32 r, Uint32 g, Uint32 b, Uint32 a)
{
	canvas * result=NULL;
	result=new canvas();
	result->generated=true;
	result->sdl_surface=SDL_CreateRGBSurface(0, w, h, bpp, r, g, b, a);
	return result;
}
