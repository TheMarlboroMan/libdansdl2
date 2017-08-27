#include "audio_channel.h"
#include "../real_audio_channel/real_audio_channel.h"

using namespace lda;

//!Default constructor, creates an unlinked channel.
audio_channel::audio_channel()
	:channel(nullptr)
{

}

//!Real channel constructor.
audio_channel::audio_channel(real_audio_channel& c)
	:channel(&c) 
{

}

//!Copy constructor. Duplicates the real channel reference.
audio_channel::audio_channel(const audio_channel& c)
	:channel(c.channel)
{

}

//!Assignment operator. Duplicates the real channel reference.
audio_channel& audio_channel::operator=(const audio_channel& c)
{
	channel=c.channel;
	return *this;
}

//!Plays the sound_struct. A bug in the Windows version of SDL audio prevents panning from working.
int audio_channel::play(const sound_struct& e) 
{	
	return channel->play(e);
}
//!Pauses the sound.
void audio_channel::pause() 
{
	channel->pause();
}

//!Resumes after pausing.
void audio_channel::resume() 
{
	channel->resume();
}

//!Forces sound stop.
void audio_channel::stop()
{
	channel->stop();
}

//!Fades out the sound according to the parameter.
int audio_channel::fade_out(int ms) 
{
	return channel->fade_out(ms);
}

//!Indicates whether the sound is looping or not.
bool audio_channel::is_looping() const 
{
	return channel->is_looping();
}

//!Busy channels are either playing a sound or finished playing but are being monitored. A non busy channel (idle) can be automatically picked from the channel pool.
bool audio_channel::is_busy() const 
{
	return channel->busy;
}

//!Channels can be monitored. A monitores channel does not call free() when SDL's callback is issued after the sound is played.
bool audio_channel::is_monitored() const 
{
	return channel->monitoring;
}

//!Indicates whether a wave is currently playing on the channel.
bool audio_channel::is_playing() const 
{
	return channel->playing;
}

//!Indicates whether the channel is paused.
bool audio_channel::is_paused() const 
{
	return channel->paused;
}

//!Gets the internal channel index.
int audio_channel::get_index() const 
{
	return channel->index;
}

//!Indicates the number of repeats (a value of 0 loops over and over).
int audio_channel::get_repeat() const 
{
	return channel->repeat;
}

//!Gets the channel volume (different from the master volume).
int audio_channel::get_volume() const 
{
	return channel->volume;
}

//!Sets the channel volume (different from the master volume).
void audio_channel::set_volume(int val) 
{
	channel->set_volume(val);
}

//!Sets the monitoring value. A monitored channel does not call free() when SDL's callback is issued at the end of playback. This allows for finer, yet useless control in most cases (maybe you want to keep the channel from reverting to "idle").
void audio_channel::set_monitoring(bool v) 
{
	channel->set_monitoring(v);
}

//!Nullifies repeat, sets busy and monitoring to false and cleans pointer to played sound.
void audio_channel::free() 
{
	channel->free();
}

//!Indicates whether the channel is linked to a real_audio_channel
bool audio_channel::is_linked() const 
{
	return channel!=nullptr;
}

//!Unlinks the channel from its real_audio_channel (good thing to do if any object has a handle of this and can be copied).
void audio_channel::unlink() 
{
	channel=nullptr;
}

//!Assigns a callback listener, replacing any previously existing one. Of course, it is assumed that it will still exist after the sound is played.
void audio_channel::assign_callback_listener(audio_callback_interface& a) 
{
	channel->assign_callback_listener(a);
}

//!Removes the callback listener (useful in case the listener is going to be deleted before the sound is done playing.
void audio_channel::clear_callback_listener() 
{
	channel->clear_callback_listener();
}

//!127 is the maximum for both parameters.
//!According to the docs, "The easiest way to do true panning is to call 
//!Mix_SetPanning(channel, left, 254 - left); so that the total volume is 
//!correct, if you consider the maximum volume to be 127 per channel for center, 
//!or 254 max for left, this works, but about halves the effective volume.

void audio_channel::set_panning(int volume_left, int volume_right)
{
	channel->set_panning(volume_left, volume_right);
}

//!Removes panning from the channel.

void audio_channel::clear_panning()
{
	channel->clear_panning();
}
