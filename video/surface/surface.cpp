#include "surface.h"
#include <stdexcept>

using namespace ldv;

surface::surface(SDL_Surface * s)
	:sdl_surface(s), with_colorkey(false)
{

}

surface::surface()
	:sdl_surface(nullptr), with_colorkey(false)
{

}

surface::~surface()
{
	if(sdl_surface)
	{
		SDL_FreeSurface(sdl_surface);
		sdl_surface=nullptr;
	}
}

surface& surface::operator=(const surface& o)
{	
	with_colorkey=o.with_colorkey;
	sdl_surface=ldv::copy_sdl_surface(o.sdl_surface);

	return *this;
}

Uint32 surface::map_color(unsigned int r, unsigned int g, unsigned int b)
{
	return SDL_MapRGB(sdl_surface->format, r, g, b);
}

void surface::set_colorkey(Uint32 color)
{
	if(sdl_surface)
	{
		SDL_SetColorKey(sdl_surface, SDL_TRUE, color);
	}
	else 
	{
		throw std::runtime_error("calling set_colorkey(Uint8, Uint8, Uint8, bool) with no sdl_surface.");
	}
}

void surface::set_colorkey(Uint8 r, Uint8 g, Uint8 b)
{
	set_colorkey(map_color(r, g, b));
}

void surface::clear(Uint8 r, Uint8 g, Uint8 b)
{
	if(sdl_surface) 
	{
		clear(map_color(r,g,b));
	}
	else
	{
		throw std::runtime_error("calling clear(Uint8, Uint8, Uint8) with no sdl_surface");
	}
}

void surface::clear(Uint32 c)
{
	if(sdl_surface) 
	{
		SDL_FillRect(sdl_surface, nullptr, c);
	}
	else 
	{
		throw std::runtime_error("calling clear(Uint32) with no sdl_surface");
	}
}

void surface::clear_colorkey()
{
	if(sdl_surface) 
	{
		SDL_SetColorKey(sdl_surface, SDL_FALSE, 0); 
		with_colorkey=false;
	}
	else 
	{
		throw std::runtime_error("calling clear_colorkey() with no sdl_surface");
	}
}

void surface::set_alpha(Uint8 pvalue)
{
	SDL_SetSurfaceAlphaMod(sdl_surface, pvalue);
	with_colorkey=pvalue;
}

void surface::copy_from(surface const& p_res)
{
	SDL_BlitSurface(p_res.sdl_surface, nullptr, sdl_surface, nullptr);
}

void surface::copy_from(surface const& p_recurso, SDL_Rect p_rect_origen)
{
	SDL_BlitSurface(p_recurso.sdl_surface, &p_rect_origen, sdl_surface, nullptr);
}

void surface::copy_from(surface const& p_recurso, SDL_Rect p_rect_origen, SDL_Rect p_rect_destino)
{
	SDL_BlitSurface(p_recurso.sdl_surface, &p_rect_origen, sdl_surface, &p_rect_destino);
}