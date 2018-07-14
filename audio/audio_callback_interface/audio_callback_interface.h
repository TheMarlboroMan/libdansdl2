#ifndef LIBDANSDL2_AUDIO_CALLBACK_INTERFACE_H
#define LIBDANSDL2_AUDIO_CALLBACK_INTERFACE_H

namespace lda
{

//!Defines a callback interface for audio monitoring.

//!Derivates of this class can be assigned to audio channels to their 
//!do_callback method is executed once the audio stops playing. This is 
//!specially useful for classes that own an audio_channel, so they can 
//!trigger specific actions (like freeing the real channel and unlinking it)
//!once audio is done. Any audio_callback_interface object tied to a real
//!channel will be unlinked once the real_audio_channel is freed.

class audio_callback_interface
{
	public:

	//!Callback method to be executed once the audio stops playing.
	virtual void		do_callback()=0;
};

}

#endif
