#include "fps_counter.h"
#include <iostream>

using namespace ldt;

fps_counter::fps_counter(unsigned int p_f):
	apply(true), ticks_count(SDL_GetTicks()), ticks_end(0), ticks_begin(ticks_count), 
	diff(0), frame_count(0), frame_count_internal(0), delta(0.f),
	delta_acumulator(0.f), rest_acumulator(0.f), timestep(0.f),
	max_timestep(0.f)
{

}

void fps_counter::init_loop_step()
{
	delta_acumulator+=delta;
	timestep=delta_acumulator;
	if(max_timestep && timestep > max_timestep) timestep=max_timestep;
	delta_acumulator=0.0;
}

bool fps_counter::consume_loop(float delta)
{
	bool go_on=timestep >= delta;

	if(go_on) //Si continuamos en el loop, restamos otro pico.
	{
		timestep-=delta;
	}
	else //Si vamos a dejar el loop, guardamos lo que nos ha sobrado para la próxima.
	{
		delta_acumulator+=timestep;
	}

	return go_on;
}

float fps_counter::get_delta_for_time(Uint32 p_ticks) const
{
	Uint32 now=SDL_GetTicks();
	Uint32 diff=now - p_ticks;
	float delta=diff / 1000.f;
	return delta;
}

void fps_counter::turn()
{
	if(!apply) return;

	ticks_end=SDL_GetTicks();  //Cortar FPS aquí.
	frame_count_internal++;

	if( (ticks_end - ticks_count) > 1000)
	{
		frame_count=frame_count_internal;
		frame_count_internal=0;
		ticks_count=SDL_GetTicks();
	}

	diff=ticks_end - ticks_begin;
	delta=diff / 1000.f;

	ticks_begin=SDL_GetTicks();
}
