#pragma once

#include <ldt/tools.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace ldv
{

struct video_display_mode {
	int w, h, refresh_rate;
};

//!Returns a video_display_mode with the current w, h and refresh_rate. Might
//!throw a std runtime error.
video_display_mode		get_display_info(int=0);

//!Displays or hides the cursor.
void 					set_cursor_visible(bool v);
void					set_vsync(bool v);
bool					get_vsync();
SDL_Surface * 			new_sdl_surface(int, int, int, bool=true);
SDL_Surface * 			new_sdl_surface(SDL_Surface const *);
SDL_Surface * 			new_sdl_surface(SDL_Surface const *, const SDL_Rect&, Uint32=0, Uint32=0);
SDL_Surface * 			copy_sdl_surface(const SDL_Surface *);
SDL_Surface * 			copy_sdl_surface(const SDL_Surface *, const SDL_Rect&, Uint32=0, Uint32=0);
SDL_Surface * 			load_image(const std::string&);
SDL_Surface * 			load_image_from_memory(const std::vector<unsigned char>& _seq);
SDL_Surface * 			load_image_from_memory(const unsigned char *, size_t);
Uint32 					get_pixel(SDL_Surface *, int, int);
void                    set_pixel(SDL_Surface *, int, int, Uint32);
std::string             get_opengl_version();

} //Fin namespace DLibV
