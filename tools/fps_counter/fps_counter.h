#ifndef LIBDANSDL2_FPS_COUNTER_H
#define LIBDANSDL2_FPS_COUNTER_H

#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include <chrono>

namespace ldt
{

class fps_counter
{
	public:

			fps_counter();

	unsigned int 	get_frame_count() const {return frame_count;}
	float 		get_delta() const {return delta;}

	void 		set_apply(bool v) {apply=v;}
	void 		end_loop_step();		//Esto lo llamamos justo después del render (o antes, lo que sea).

	//Nueva interface...
	void 		init_loop_step(float=0);
	bool 		consume_loop(float);
	void 		set_max_timestep(float t) {max_timestep=t;}

	private:

	bool 		apply;

	std::chrono::high_resolution_clock::time_point 	ticks_count,
		 					ticks_begin;

	int 		frame_count,	//Este es el "visible"...
			frame_count_internal;	//Este es "no visible"...

	float 		delta,
			delta_acumulator,
	 		rest_acumulator,
	 		timestep,
	 		max_timestep;
};

}
#endif
