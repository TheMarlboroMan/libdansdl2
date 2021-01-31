#include <ldv/surface.h>
#include <stdexcept>

using namespace ldv;

//!Class constructor.

//!Creates a surface wrapper from a SDL_Surface.

surface::surface(SDL_Surface * s)
	:sdl_surface(s), with_colorkey(false)
{

}

//!Constructs an empty surface.

//!Useless unless there is surface copy involved down the road.

surface::surface()
	:sdl_surface(nullptr), with_colorkey(false)
{

}

//!Class destructor.

surface::~surface()
{
	if(sdl_surface)
	{
		SDL_FreeSurface(sdl_surface);
		sdl_surface=nullptr;
	}
}

surface::surface(const surface& _other):
	sdl_surface{ldv::copy_sdl_surface(_other.sdl_surface)},
	with_colorkey{_other.with_colorkey}
{

}

//!Assignment operator.

//!Copies the reference surface to the result. The reference surface remains
//!unchanged.

surface& surface::operator=(const surface& o)
{
	with_colorkey=o.with_colorkey;
	sdl_surface=ldv::copy_sdl_surface(o.sdl_surface);

	return *this;
}

//!Maps 3 unsigned ints to a color in 32 bit format.

//!The 32 bit format is expected in old SDL functions.

Uint32 surface::map_color(unsigned int r, unsigned int g, unsigned int b)
{
	return SDL_MapRGB(sdl_surface->format, r, g, b);
}

Uint32 surface::map_color(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
	return SDL_MapRGBA(sdl_surface->format, r, g, b, a);
}

//!Sets the colorkey.

//!The colorkey is the "transparent" pixel in surfaces without an alpha value.
//!May throw a std::runtime_error if no surface exists.

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

//!Sets the colorkey.

void surface::set_colorkey(Uint8 r, Uint8 g, Uint8 b)
{
	set_colorkey(map_color(r, g, b));
}

//!Fills the surface with the specified color.

//!May throw a std::runtime_error if no surface is present.

void surface::clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	if(sdl_surface)
	{
		clear(map_color(r,g,b,a));
	}
	else
	{
		throw std::runtime_error("calling clear(Uint8, Uint8, Uint8) with no sdl_surface");
	}
}

//!Fills the surface with the specified color.

//!The 32 bit integer is the SDL format, obtainable through map_color.
//!May throw a std::runtime_error if no surface is present.

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

//!Removes colorkey from surface.

//!May throw a std::runtime_error if no surface is present.

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

//!Sets the global transparency for the surface.

//!Values are between 0 and 255, with 0 being fully transparent.
//!This function may be buggy. I haven't touched surfaces in a while...

void surface::set_alpha(Uint8 pvalue)
{
	SDL_SetSurfaceAlphaMod(sdl_surface, pvalue);
	with_colorkey=pvalue; //TODO: What is this?.
}

//!Blits the parameter surface into the current surface.

//!The full surface is copied.

void surface::copy_from(surface const& p_res)
{
	SDL_BlitSurface(p_res.sdl_surface, nullptr, sdl_surface, nullptr);
}

//!Blits the parameter surface specifying clip.

//!Useful to compose surfaces, but less useful than the four parameter variant.

void surface::copy_from(surface const& p_recurso, SDL_Rect p_rect_origen)
{
	SDL_BlitSurface(p_recurso.sdl_surface, &p_rect_origen, sdl_surface, nullptr);
}

//!Blits the parameter surface specifying clip and destination.

//!Useful to compose surfaces.

void surface::copy_from(surface const& p_recurso, SDL_Rect p_rect_origen, SDL_Rect p_rect_destino)
{
	SDL_BlitSurface(p_recurso.sdl_surface, &p_rect_origen, sdl_surface, &p_rect_destino);
}

void surface::set_blend(
	surface::blends _blend
) {
	if(!sdl_surface) {

		throw std::runtime_error("calling set_blend() with no sdl_surface");
	}

	switch(_blend) {

		case surface::blends::add: SDL_SetSurfaceBlendMode(sdl_surface, SDL_BLENDMODE_ADD); break;
		case surface::blends::mod: SDL_SetSurfaceBlendMode(sdl_surface, SDL_BLENDMODE_MOD); break;
		case surface::blends::none: SDL_SetSurfaceBlendMode(sdl_surface, SDL_BLENDMODE_NONE); break;
		case surface::blends::alpha: SDL_SetSurfaceBlendMode(sdl_surface, SDL_BLENDMODE_BLEND); break;
	}
}