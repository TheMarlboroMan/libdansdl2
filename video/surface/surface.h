#ifndef SUPERFICIE_SDL_H
#define SUPERFICIE_SDL_H

#include <SDL2/SDL.h>
//#include <string>
//#include <fstream>
#include <iostream>
#include "../sdl_video_tools/sdl_video_tools.h"

//Wrapper para una surface... La vamos a usar para algunas cosas muy
//concretas, como usar fuentes de bitmap.

namespace ldv
{

class surface
{
	public:

	enum class 	types{image, canvas};
	
			surface();
			surface(SDL_Surface *);
			virtual ~surface();
			surface& operator=(const surface&);
	
	SDL_Surface * 	get_surface() const {return sdl_surface;}
	SDL_Surface * 	get_surface() {return sdl_surface;}
	unsigned int 	get_w() const {return sdl_surface->w;}
	unsigned int 	get_h() const {return sdl_surface->h;}
	bool 		is_with_colorkey() const {return this->with_colorkey;}

	void 		set_colorkey(Uint32=0);
	void 		set_colorkey(Uint8=0, Uint8=255, Uint8=255);
	void 		set_alpha(Uint8);

	void 		clear_colorkey();
	void 		clear(Uint8=0, Uint8=255, Uint8=255);
	void 		clear(Uint32 c);

	void 		copy_from(surface const&);
	void 		copy_from(surface const&, SDL_Rect);
	void 		copy_from(surface const&, SDL_Rect, SDL_Rect);

	//Resulta ridículo que esto es lo único que hace abstracta a esta clase :D.
	virtual types 	get_type() const=0;

	protected:

	Uint32 		map_color(unsigned int r, unsigned int g, unsigned int b);

	SDL_Surface * 	sdl_surface;
	bool 		with_colorkey;

	
};

} //Fin namespace DLibV
#endif
