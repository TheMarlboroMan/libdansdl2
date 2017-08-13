#include "fps_counter.h"
#include <iostream>

//#include "../log/log.h"

using namespace ldt;

fps_counter::fps_counter():
	apply(true), 
	ticks_count(std::chrono::high_resolution_clock::now()), 
	ticks_begin(ticks_count), 
	frame_count(0), frame_count_internal(0), delta(0.f),
	delta_acumulator(0.f), rest_acumulator(0.f), timestep(0.f)
{

}

void fps_counter::reset()
{
	ticks_count=std::chrono::high_resolution_clock::now();
	ticks_begin=ticks_count;
}

/*Timestep cap can be set by the caller to prevent very large timesteps from
happening. By default is 0.f.
*/

void fps_counter::init_loop_step(float timestep_cap)
{
	delta_acumulator+=delta;
	timestep=delta_acumulator; //Add the remainder of previous loop.
	if(timestep_cap && timestep > timestep_cap) timestep=timestep_cap;
	delta_acumulator=0.0;
}

bool fps_counter::consume_loop(float pdelta)
{
	if(timestep >= pdelta) //Si continuamos en el loop, restamos otro pico.
	{
		timestep-=pdelta;
		return true;
	}
	else //Si vamos a dejar el loop, guardamos lo que nos ha sobrado para la próxima.
	{
		delta_acumulator+=timestep;
		return false;
	}
}

void fps_counter::end_loop_step()
{
	if(!apply) return;

	auto ticks_end=std::chrono::high_resolution_clock::now();
	frame_count_internal++;

	
	//We see if a second has elapsed, to reset the framecount...
	//Integral duration: requires duration_cast
	auto int_ms=std::chrono::duration_cast<std::chrono::milliseconds>(ticks_end - ticks_count);
 
	if(int_ms.count() > 1000) //This measures a second
	{
		frame_count=frame_count_internal;
		frame_count_internal=0;
	
		//TODO: This would NOT be accurate... How about any reminder
		//of the substraction performed in the if?. Check this.

		ticks_count=std::chrono::high_resolution_clock::now();
	}

	
	auto diff=std::chrono::duration_cast<std::chrono::milliseconds>(ticks_end - ticks_begin);
	delta=diff.count() / 1000.f;

	ticks_begin=std::chrono::high_resolution_clock::now();
}
