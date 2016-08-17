#ifndef CONTROLADOR_AUDIO_SDL_H
#define CONTROLADOR_AUDIO_SDL_H

#include <vector>
#include <map>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../sound/sound.h"
#include "../music/music.h"

namespace lda
{

void audio_play_callback(int pchannel);	//F-f-f-forward!.
class sound_queue;			//Re-f-f-f-forward!.

/*
Sobre el Controlador_audio:

No hay mucho que decir. Antes era un singleton.

Sobre el audio_channel.

El channel de audio lo podríamos usar para darles a otros elementos referencias
a los channels de audio originales que esté usando el controlador. El controlador
tendría un vector de tantos objetos como channels. Al solicitar la reproducción 
podríamos devolver un asa a uno de estos de forma que se pueda siempre
inspeccionar y actuar desde la propia entidad sin tener que hacer muchos puentes
entre clases.

Estos objetos serían válidos mientras el controlador de audio estuviera en 
marcha.

Esto reemplazaría a los vectores de estado de channels y demás y se convertiría
en el nuevo punto de acceso.

La implementación original inluía un vector de Canales de audio en el 
controlador. Una posterior es un vector de punteros a channels de audio. Esto es
así porque la interfaz pública hacía copia al devolver el objeto y no permitía
realmente llevar la cuenta de los mismos si se usan desde fuera del controlador.
Por ejemplo, al sacar un channel libre y usarlo desde el código cliente se hacía
una copia del mismo y al usarlo dejaba de reflejarse en debug_channels.

La implementación actual es bien diferente: el channel de audio que se guarda
en el controlador es del tipo real_audio_channel, y no se guarda como un puntero.
Para obtener referencias a los mismos desde el código cliente y que todo
funcione bien, se ha envuelto en un patrón de proxy de forma que un objeto
audio_channel tiene una referencia al real_audio_channel. audio_channel implementa
la misma interfaz pública que real_audio_channel de forma que es transparente
al uso. audio_channel puede ser copiable de un lado para otro sin problemas. 
audio_channel es amiga de real_audio_channel para poder acceder a sus propiedades
directamente. 

El objeto del tipo audio_channel es deshechable: no se devuelven por referencia
ni se guardan referencias al mismo, sino que se copian tantas veces como sea
necesario. Un detalle MUY importante es que puede crearse un objeto audio_channel
de la nada pero estaría "desvinculado" y todo aquello que llamemos que tenga
que afectar un channel causaría un crash seguro. Tenemos el método "es_vinculado()"
para asegurarnos de que tiene un channel de audio real asociado. Asimismo se
puede desvincular con el método "desvicular".

Objetos de audio_channel directamente obtenidos del controlador de audio estarían
siempre vinculados y listos. Objetos creados libremente estarán siempre
desvinculados a no ser que se creen mediante copia o constructor de copia de 
otro objeto audio_channel.
*/

struct audio_controller_config
{
	int 				ratio,
	 				out,
	 				buffers,
	 				channels,
					initial_volume;
 
	Uint16 				format;

/*	audio_controller_config()
		:ratio(44100), out(2), buffers(1024), channels(8),
		initial_volume(128), format(AUDIO_S16SYS)
	{
	}
*/

};

class audio_channel; //F-f-f-f-forward!.
class audio_controller;

//Non constructible audio channel. Can only be created from within the 
//audio_controller class. There's another class: audio_channel, that provides
//the same public interface for it.
class real_audio_channel
{
	public:

	int 				play(const sound_struct&);	//Pondría el puntero al audio y si está o no en loop.		
	void 				pause();
	void 				resume();
	void 				stop();	//Forzaría la parada del channel en el controlador.
	int 				fade_out(int p_ms) {return Mix_FadeOutChannel(index, p_ms);}

	bool 				is_looping() const {return repeat==-1;}
	bool 				is_busy() const {return busy;}
	bool 				is_monitored() const {return monitoring;}
	bool 				is_playing() const {return playing;}
	bool 				is_paused() const {return paused;}
	int 				get_index() const {return index;}
	int 				get_repeat() const {return repeat;}
	int 				get_volume() const {return volume;}

	void 				set_monitoring(bool v) {monitoring=v;}
	void 				set_volume(int);

	void 				free(); //Es público para poder llamarlo si haces monitoring manual.
	void 				do_callback(); 	//Se pondría como "no_playing" y quitaría el puntero del audio.

					~real_audio_channel(){sound_playing=nullptr;}
					real_audio_channel(const real_audio_channel&);
					real_audio_channel& operator=(const real_audio_channel&);

	private:

					real_audio_channel(int, const audio_controller&);

	int 				index,
	 				repeat,
	 				volume;	

	bool 				playing, //Reproduciendo es que la onda se está playing.
	 				busy,	//Activo es que el channel está busy, ya sea porque reproduce o porque está monitoring.
	 				monitoring, //Un channel monitoring no se libera del estado busy de forma automática.
	 				paused;

	audio_controller const *	controller_ref;
	sound * 			sound_playing;

	public:

	friend class 			audio_controller; //Para poder llamar a la recepción de callback.
	friend class 			audio_channel;
};

class audio_controller
{
	public:

	//Init and state.

						audio_controller(const audio_controller_config&);
						~audio_controller();

	//Sounds...	
	void 					play_sound(sound_struct&);
	void 					stop_sound(int);
	void 					pause_sound(); //Pauses all sounds.
	void 					resume_sound(); //Restarts all sounds.
	void 					stop_sound(); //Stops all sounds

	//Music.
	void 					play_music(music&, int=-1);
	void 					stop_music();

	//Volume...
	void 					set_main_sound_volume(int);
	void 					set_sound_volume(int p_vol, int pchannel); //p_vol de 0 a 128.
	void 					set_music_volume(int);

	//Getters.
	bool					is_music_playing() const {return music_playing;}
	int 					get_music_volume() const {return Mix_VolumeMusic(-1);}
	int 					get_requested_channels() const {return requested_channels;}
	unsigned int 				get_main_sound_volume() const {return main_sound_volume;}
	audio_channel 				get_channel(int);
	audio_channel 				get_free_channel();

	void 					debug_state();

	/////////////////////////////////////
	// Propiedades...

	private:

						audio_controller(const audio_controller&)=delete;
						audio_controller& operator=(const audio_controller&)=delete;

	int 					get_free_channel_index(int=0, int=-1);
	void					check_channel(int);


	int 					main_sound_volume,
	 					ratio,
	 					out,	//1 mono, 2 estéreo.
	 					buffers,
		 				requested_channels;

	Uint16 					format;

	bool 					music_playing; 	

	std::vector<real_audio_channel> 	channels;
	static std::map<int, real_audio_channel *>	callback_channels;

	///////////////////////////////////////
	// Amigos.

	friend void 				audio_play_callback(int pchannel);
	friend class 				sound_queue;
	friend class 				audio_channel; //Para que pueda acceder a la definición de "real_audio_channel".
};

//Exposición pública del channel de audio para usarlo fuera del controlador. 
//Tiene una referencia al channel real que asegura que todo se sincronice.

class audio_channel
{
	public:

	int 			play(const sound_struct& e) {return channel->play(e);}
	void 			pause() {channel->pause();}
	void 			resume() {channel->resume();}
	void 			stop() {channel->stop();}

	int 			fade_out(int ms) {return channel->fade_out(ms);}

	bool 			is_looping() const {return channel->is_looping();}
	bool 			is_busy() const {return channel->busy;}
	bool 			is_monitored() const {return channel->monitoring;}
	bool 			is_playing() const {return channel->playing;}
	bool 			is_paused() const {return channel->paused;}
	int 			get_index() const {return channel->index;}
	int 			get_repeat() const {return channel->repeat;}
	int 			get_volume() const {return channel->volume;}

	void 			set_volume(int val) {channel->set_volume(val);}
	void 			set_monitoring(bool v) {channel->set_monitoring(v);}
	void 			free() {channel->free();}

	bool 			is_linked() const {return channel!=nullptr;}
	void 			unlink() {channel=nullptr;}

				audio_channel():channel(nullptr){}
				audio_channel(real_audio_channel& c):channel(&c) {}
				audio_channel(const audio_channel& c):channel(c.channel){}
				audio_channel& operator=(const audio_channel& c)
	{
		channel=c.channel;
		return *this;
	}

	private:

	real_audio_channel * 	channel;

};

}
#endif
