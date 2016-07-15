#include "sdl_tools.h"

using namespace ldt;

bool ldt::sdl_init(Uint32 p_flags)
{
	return SDL_Init(p_flags) >= 0 && TTF_Init()==0;
}

void ldt::sdl_shutdown()
{
 	TTF_Quit();
	SDL_Quit();
}

bool ldt::sdl_system_init(Uint32 p_flags)
{
	return SDL_InitSubSystem(p_flags)==0;
}

bool ldt::is_sdl_system_init(Uint32 p_flags)
{
	return SDL_WasInit(p_flags)!=0;
}

void ldt::sdl_system_stop(Uint32 p_flags)
{
	SDL_QuitSubSystem(p_flags);
}
