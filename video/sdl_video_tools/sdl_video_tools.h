#ifndef LIBDANSDL_2_SDL_VIDEO_TOOLS_H
#define LIBDANSDL_2_SDL_VIDEO_TOOLS_H

#include <iostream>
#include <fstream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../../tools/log/log.h"
#include "../../tools/tools/tools.h"

namespace ldv
{

void 		set_cursor_visible(bool v);
SDL_Surface * 	new_sdl_surface(SDL_Surface const *);
SDL_Surface * 	new_sdl_surface(SDL_Surface const *, const SDL_Rect&, Uint32=0, Uint32=0);
SDL_Surface * 	copy_sdl_surface(const SDL_Surface *);
SDL_Surface * 	copy_sdl_surface(const SDL_Surface *, const SDL_Rect&, Uint32=0, Uint32=0);
SDL_Surface * 	load_image(const std::string&);
Uint32 		SDL_GetPixel(SDL_Surface *, int, int);

} //Fin namespace DLibV
#endif
