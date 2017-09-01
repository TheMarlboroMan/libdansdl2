#ifndef LIBDANSDL2_REAL_AUDIO_CHANNEL_H
#define LIBDANSDL2_REAL_AUDIO_CHANNEL_H

#include "../sound/sound.h"
#include "../audio_callback_interface/audio_callback_interface.h"

namespace lda
{
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
	void				set_stereo_volume(sound_panning);
	void				clear_panning();
	void 				do_callback();
	//!Assigns a callback listener, replacing any previously existing one. Of course, it is assumed that it will still exist after the sound is played.
	void				assign_callback_listener(audio_callback_interface& a) {callback_listener=&a;}
	//!Removes the callback listener (useful in case the listener is going to be deleted before the sound is done playing.
	void				clear_callback_listener() {callback_listener=nullptr;}

					~real_audio_channel(){sound_playing=nullptr;}
					real_audio_channel(const real_audio_channel&);
	private:
					real_audio_channel(int, const int&);
	void 				free();

	int				calculate_real_volume();

	
	int 				index,
	 				repeat, //!< -1 means it will repeat forever.
	 				volume;

	bool 				playing, /**< Wave is playing. */ 
	 				monitoring, /**< Monitored channels are not automatically freed. */ 
	 				paused;

	int const * 	 		main_sound_volume_ptr; //!< Cannot be written into... do not dare to change what it points to!!!.
	sound * 			sound_playing;
	audio_callback_interface *	callback_listener=nullptr;

	public:

	friend class			audio_controller;	//!< So that these can be created.
	friend class 			audio_channel;	//!< So that the audio_channel does not need to pass through accessors.
};
}

#endif
