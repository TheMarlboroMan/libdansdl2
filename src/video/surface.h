#pragma once

#include "sdl_video_tools.h"
#include <SDL2/SDL.h>
#include <iostream>

namespace ldv
{

//!Wrapper for a SDL_Surface and base class for image and canvas.

class surface
{
	public:

			surface();
			surface(SDL_Surface *);
			virtual ~surface();
			surface& operator=(const surface&);

	//!Returns the underlying surface.
	SDL_Surface * 	get_surface() const {return sdl_surface;}
	//!Returns the underlying surface.
	SDL_Surface * 	get_surface() {return sdl_surface;}
	//!Shortcut for get_surface()->w.
	unsigned int 	get_w() const {return sdl_surface->w;}
	//!Shortcut for get_surface()->h.
	unsigned int 	get_h() const {return sdl_surface->h;}
	//!Checks whether colokeying is active.
	bool 		is_with_colorkey() const {return with_colorkey;}
	void 		set_colorkey(Uint32=0);
	void 		set_colorkey(Uint8=0, Uint8=255, Uint8=255);
	void 		set_alpha(Uint8);

	void 		clear_colorkey();
	void 		clear(Uint8=0, Uint8=255, Uint8=255);
	void 		clear(Uint32 c);

	void 		copy_from(surface const&);
	void 		copy_from(surface const&, SDL_Rect);
	void 		copy_from(surface const&, SDL_Rect, SDL_Rect);

	protected:

	Uint32 		map_color(unsigned int r, unsigned int g, unsigned int b);

	SDL_Surface * 	sdl_surface;	//!< Underlying SDL_Surface.

	private:

	bool 		with_colorkey;	//!< Indicates if colorkeying will be applied.

	
};

} //Fin namespace DLibV