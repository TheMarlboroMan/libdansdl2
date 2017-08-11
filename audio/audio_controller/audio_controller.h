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

//!Configuration to instruct SDL2 on how to setup the audio framework. 

//!Refer to SDL2 reference for further information on their meaning.

struct audio_controller_config
{
	int 				ratio, 	/**< A sane default is 44100. */ 
	 				out,	/**< A sane default is 2. */ 
	 				buffers,	/**< A sane default is 1024. */ 
	 				channels,	/**< A sane default is 8... like Doom. */ 
					initial_volume; /**< A sane default is 128. */ 
 
	Uint16 				format; /**< A sane default is SDL2 value AUDIO_S16SYS. */ 
};

class audio_channel; //F-f-f-f-forward!.
class audio_controller;

//!Non constructible audio channel for internal use.

//!Can only be created from within the audio_controller class. 
//!There's another class: audio_channel, that provides the same public interface for it. 

class real_audio_channel
{
	public:

	int 				play(const sound_struct&);
	void 				pause();
	void 				resume();
	void 				stop();
	//!Fades out the sound according to the parameter.
	int 				fade_out(int p_ms) {return Mix_FadeOutChannel(index, p_ms);}
	//!Indicates whether the sound is looping or not.
	bool 				is_looping() const {return repeat==-1;}
	//!Busy channels are either playing a sound or finished playing but are being monitored. A non busy channel (idle) can be automatically picked from the channel pool.
	bool 				is_busy() const {return busy;}
	//!Channels can be monitored. A monitores channel does not call free() when SDL's callback is issued after the sound is played.
	bool 				is_monitored() const {return monitoring;}
	//!Indicates whether the channel is playing or not.
	bool 				is_playing() const {return playing;}
	//!Indicates whether the channel is paused or not.
	bool 				is_paused() const {return paused;}
	//!Gets the internal channel index.
	int 				get_index() const {return index;}
	//!Gets the number of specified repeats (-1 means it will repeat indefinitely).
	int 				get_repeat() const {return repeat;}
	//!Gets the channel volume.
	int 				get_volume() const {return volume;}
	//!Sets the monitoring value. A monitored channel does not call free() when SDL's callback is issued at the end of playback. This allows for finer, yet useless control in most cases (maybe you want to keep the channel from reverting to "idle").
	void 				set_monitoring(bool v) {monitoring=v;}
	void 				set_volume(int);
	void 				free();
	void 				do_callback();

					~real_audio_channel(){sound_playing=nullptr;}
					real_audio_channel(const real_audio_channel&);
					real_audio_channel& operator=(const real_audio_channel&);

	private:

					real_audio_channel(int, const audio_controller&);

	int 				index,
	 				repeat, //!< -1 means it will repeat forever.
	 				volume;

	bool 				playing, /**< Wave is playing. */ 
	 				busy,	/**< Busy channel: either playing or under monitoring from the application. */ 
	 				monitoring, /**< Monitored channels are not automatically freed. */ 
	 				paused;

	audio_controller const *	controller_ref;
	sound * 			sound_playing;

	public:

	friend class 			audio_controller; //To call callback reception.
	friend class 			audio_channel;
};

//!Non singleton audio controller. 

//!Unlike the video elements - which need to be loaded, queued and refreshed
//!in sequence - the audio_controller is interactive: each action takes place
//!instantly.
//!Only one instance can exist for each application. Provides access to all 
//!sound channels, music, volumes and debug information.

class audio_controller
{
	public:

						audio_controller(const audio_controller_config&);
						~audio_controller();

	void 					play_sound(sound_struct&);
	void 					stop_sound(int);
	void 					stop_sound(); //Stops all sounds
	void 					pause_sound(); //Pauses all sounds.
	void 					resume_sound(); //Restarts all sounds.
	void 					play_music(music&, int=-1);
	void 					stop_music();
	void 					set_main_sound_volume(int);
	void 					set_sound_volume(int p_vol, int pchannel); //p_vol de 0 a 128.
	void 					set_music_volume(int);
	//!Indicates whether music is playing.
	bool					is_music_playing() const {return music_playing;}
	//!Gets music volume.
	int 					get_music_volume() const {return Mix_VolumeMusic(-1);}
	//!Returns the number of channels requested to SDL by the controller.
	int 					get_requested_channels() const {return requested_channels;}
	//!Gets the main sound volume.
	unsigned int 				get_main_sound_volume() const {return main_sound_volume;}
	audio_channel 				get_channel(int);
	audio_channel 				get_free_channel();
	void 					debug_state();

	/////////////////////////////////////
	// Propiedades...

	private:

	//!Can't be copied.
						audio_controller(const audio_controller&)=delete;
	//!Can't be assigned.
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
	friend class 				audio_channel; //Para que pueda acceder a la definición de "real_audio_channel".
};

//!Disposable references to real_audio_channels used by the application.
/**
Implements audio_channels, which can be used to hand references of specific 
audio channels to application classes (which would be valid until the audio_controller
itself shuts down.

Any sound must be played through an audio_channel. When a sound stops playing, the 
channel is automatically freed and made available to the channel pool unless it is
set under "monitoring" by the application logic.

These references are implemented in terms of proxy objects: audio_channel is a 
proxy to real_audio_channel: both implement the same public structure. This
makes every audio_channel object suitable for deletion since they are 
not real references. On a related note, the audio_channels can be manually created but
that's a recipe for a crash, since they would not be related to the audio_controller 
in any way. The is_linked and unlink methods manipulate this aspect. There is no "link" method.

Every audio_channel retrieved from the audio_controller is linked. Manually created 
ones are not unless copied or copy-constructed.
**/

class audio_channel
{
	public:

	//!Plays the sound_struct. A bug in the Windows version of SDL audio prevents panning from working.
	int 			play(const sound_struct& e) {return channel->play(e);}
	//!Pauses the sound.
	void 			pause() {channel->pause();}
	//!Resumes after pausing.
	void 			resume() {channel->resume();}
	//!Forces sound stop.
	void 			stop() {channel->stop();}
	//!Fades out the sound according to the parameter.
	int 			fade_out(int ms) {return channel->fade_out(ms);}
	//!Indicated whether the sound is looping or not.
	bool 			is_looping() const {return channel->is_looping();}
	//!Busy channels are either playing a sound or finished playing but are being monitored. A non busy channel (idle) can be automatically picked from the channel pool.
	bool 			is_busy() const {return channel->busy;}
	//!Channels can be monitored. A monitores channel does not call free() when SDL's callback is issued after the sound is played.
	bool 			is_monitored() const {return channel->monitoring;}
	//!Indicates whether a wave is currently playing on the channel.
	bool 			is_playing() const {return channel->playing;}
	//!Indicates whether the channel is paused.
	bool 			is_paused() const {return channel->paused;}
	//!Gets the internal channel index.
	int 			get_index() const {return channel->index;}
	//!Indicates the number of repeats (a value of 0 loops over and over).
	int 			get_repeat() const {return channel->repeat;}
	//!Gets the channel volume (different from the master volume).
	int 			get_volume() const {return channel->volume;}
	//!Sets the channel volume (different from the master volume).
	void 			set_volume(int val) {channel->set_volume(val);}
	//!Sets the monitoring value. A monitored channel does not call free() when SDL's callback is issued at the end of playback. This allows for finer, yet useless control in most cases (maybe you want to keep the channel from reverting to "idle").
	void 			set_monitoring(bool v) {channel->set_monitoring(v);}
	//!Nullifies repeat, sets busy and monitoring to false and cleans pointer to played sound.
	void 			free() {channel->free();}
	//!Indicates whether the channel is linked to a real_audio_channel
	bool 			is_linked() const {return channel!=nullptr;}
	//!Unlinks the channel from its real_audio_channel (good thing to do if any object has a handle of this and can be copied).
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
