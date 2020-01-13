#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <cmath>

namespace ldt
{
bool sdl_init(Uint32=(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK));
void sdl_shutdown();
bool sdl_system_init(Uint32);
bool is_sdl_system_init(Uint32);
void sdl_system_stop(Uint32);
}
