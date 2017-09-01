#ifndef LIBDANSDL2_AUDIO_CHANNEL_H
#define LIBDANSDL2_AUDIO_CHANNEL_H

#include "../sound/sound.h"
#include "../audio_callback_interface/audio_callback_interface.h"

namespace lda
{

class real_audio_channel; //Forward.

//!Disposable references to real_audio_channels used by the application.
/**
Implements audio_channels, which can be used to hand references of specific 
audio channels to application classes (which would be valid until the audio_controller
itself shuts down.

Any sound must be played through an audio_channel. When a sound stops playing, the 
channel is automatically freed and made available to the channel pool unless it is
set under "monitoring" by the application logic.

When a channel is set under "monitoring" it will remain so until the application
logic frees it. In other words, deleting the channel does not unmonitor it.

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

				audio_channel();
				audio_channel(real_audio_channel&);
				audio_channel(const audio_channel&);
				audio_channel& operator=(const audio_channel&);

	int 			play(const sound_struct& e);
	void 			pause();
	void 			resume();
	void 			stop();
	int 			fade_out(int);
	bool 			is_looping() const;
	bool 			is_monitored() const;
	bool 			is_playing() const;
	bool 			is_paused() const;
	int 			get_index() const;
	int 			get_repeat() const;
	int 			get_volume() const;
	void 			set_volume(int);
	void 			set_monitoring(bool);
	void			set_stereo_volume(sound_panning);
	void			clear_panning();
	bool 			is_linked() const;
	void 			unlink();
	void			assign_callback_listener(audio_callback_interface&);
	void			clear_callback_listener();

	private:

	real_audio_channel * 	channel;
};
}

#endif
