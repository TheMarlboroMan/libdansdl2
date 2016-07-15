#ifndef LIBDANSDL2_FPS_COUNTER_H
#define LIBDANSDL2_FPS_COUNTER_H

#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>

/*
Incluye el controlador de frames y el cronómetro. El controlador nos servirá
para controlar los FPS y para obtener la cantidad de delta desde el último 
frame.

El crónometro_SDL está muy ligado al Controlador de FPS. A un controlador le
podemos asignar referencias a uno o más cronómetros y por cada turno del 
controlador se irá pasando el tiempo al crono. Más sencillo imposible. Estos
cronómetros deben mantener referencias fuera de la clase: sin ellas no 
podremos manejarlos correctamente.

OJO: En el momento en que adjuntamos un cronómetro al control de frames ese 
cronómetro no puede "salirse" de ámbito, por así decirlo. Allí donde exista
el control deben existir aún los cronómetros.
*/

//TODO: Retool with c++ chrono stuff.

namespace ldt
{

class fps_counter
{
	public:

			fps_counter(unsigned int p_f=60);

	unsigned int 	get_frame_count() const {return frame_count;}
	float 		get_delta() const {return delta;}
	float 		get_delta_for_time(Uint32) const;	//Esto es para obtener el valor delta después de turno();

	void 		set_apply(bool v) {apply=v;}
	void 		turn();		//Esto lo llamamos justo después del render (o antes, lo que sea).

	//Nueva interface...
	void 		init_loop_step();	//
	bool 		consume_loop(float);
	void 		set_max_timestep(float t) {max_timestep=t;}

	private:

	bool 		apply;
	Uint32 		ticks_count,
			ticks_end,
	 		ticks_begin,
			diff;

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
