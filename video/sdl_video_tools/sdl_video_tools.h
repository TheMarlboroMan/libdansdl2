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

///Displays or hides the cursor.
void 		set_cursor_visible(bool v);
///Creates a SDL_Surface with the same format and size as the one given.
SDL_Surface * 	new_sdl_surface(SDL_Surface const *);
///Creates a SDL_Surface with the same format as the one given, the specified size (2nd) flags (3rd parameter) and background color in integer form (4th parameter, check SDL documentation on this).
SDL_Surface * 	new_sdl_surface(SDL_Surface const *, const SDL_Rect&, Uint32=0, Uint32=0);
///Copies a SDL_Surface.
SDL_Surface * 	copy_sdl_surface(const SDL_Surface *);
///Copies part of a SDL surface  with the specified flags.
SDL_Surface * 	copy_sdl_surface(const SDL_Surface *, const SDL_Rect&, Uint32=0, Uint32=0);
///Loads image into SDL_Surface. With old window parameter... Unused, really.
//TODO: Remove this.
SDL_Surface * 	load_image(const std::string&, const SDL_Window *);
///Loads image into SDL_Surface. Use this one.
SDL_Surface * 	load_image(const std::string&);
///Gets the uint32 value of a surface pixel (2nd and 3rd parameters are x and y) in a surface. Does not work with textures and is mostly obsolete.
Uint32 		SDL_GetPixel(SDL_Surface *, int, int);

} //Fin namespace DLibV
#endif
