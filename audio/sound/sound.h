#ifndef LIBDANSDL2_SOUND_H
#define LIBDANSDL2_SOUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include "../../tools/log/log.h"

/* Se definen dos cosas: la clase sound es un wrapper para un chunk de 
SDL_Mixer. La clase Estructura sonido envuelve a sound y le añade volumen,
repeticiones y otras historias para poder pasarlo a un canal y que haga 
todo lo que tenga que hacer.*/

namespace lda
{
class sound
{
	public:

				sound();
				sound(const std::string&);
	~sound();

	void 			load(const std::string&);

	std::string 		get_path() const {return path;}
	bool			is_ready() const {return ready;}
	Mix_Chunk * 		get_data() {return sound_data;}

	private:

	void 			free();

	Mix_Chunk * 		sound_data;
	std::string 		path;
	bool 			ready;
};

struct sound_struct
{
	sound&	 		sound_ref;
	short int 		volume;	//El volumen -1 es "no hagas cambios".
	short int 		repeat;
	int 			volume_left;
	int 			volume_right;
	int 			ms_fade;
	
	bool 			is_ready() 	{return sound_ref.is_ready();}

	sound_struct(sound& s, int v=-1, int r=0, int pvi=127, int pvd=127, int msf=0):
		sound_ref(s), volume(v), repeat(r), 
		volume_left(pvi), volume_right(pvd), ms_fade(msf) {}
};

}

#endif
