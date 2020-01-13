#include "sdl_tools.h"

/*! \file sdl_tools.cpp
    \brief SDL tools.
    
Free wrapper functions to SDL basics.
*/

using namespace ldt;

//!Inits SDL and TTF with the given flags.
bool ldt::sdl_init(Uint32 p_flags)
{
	return SDL_Init(p_flags) >= 0 && TTF_Init()==0;
}

//!Shuts down SDL and TTF
void ldt::sdl_shutdown()
{
 	TTF_Quit();
	SDL_Quit();
}

//!Inits a SDL subsystem.
bool ldt::sdl_system_init(Uint32 p_flags)
{
	return SDL_InitSubSystem(p_flags)==0;
}

//!Checks if a subsystem is loaded.
bool ldt::is_sdl_system_init(Uint32 p_flags)
{
	return SDL_WasInit(p_flags)!=0;
}

//!Stops a subsystem.
void ldt::sdl_system_stop(Uint32 p_flags)
{
	SDL_QuitSubSystem(p_flags);
}
