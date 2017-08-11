#include "audio_controller.h"

#include <stdexcept>
#include "../../tools/log/log.h"

using namespace lda;

real_audio_channel::real_audio_channel(int i, const audio_controller& ac)
	:index(i), repeat(0), volume(128), playing(false), busy(false), 
	monitoring(false), paused(false), controller_ref(&ac),
	sound_playing(nullptr) 
{

}


real_audio_channel::real_audio_channel(const real_audio_channel& o)
	:index(o.index), repeat(o.repeat), volume(o.volume), 
	playing(o.playing), busy(o.busy), monitoring(o.monitoring),
	paused(o.paused), controller_ref(o.controller_ref),
	sound_playing(o.sound_playing)
{

}

real_audio_channel& real_audio_channel::operator=(const real_audio_channel& o)
{
	index=o.index;
	repeat=o.repeat;
	volume=o.volume;
	playing=o.playing;
	busy=o.busy;
	monitoring=o.monitoring;
	paused=o.paused;
	controller_ref=o.controller_ref;
	sound_playing=o.sound_playing;

	return *this;
}

//!Frees a channel after the playback is done. 

//!This is part of the SDL_Audio callback system, which needs a callback 
//!function to call after a sound has been played. Must be manually called on 
//!monitored channels. Nullifies repeat, sets busy and monitoring to false and 
//!cleans pointer to played sound.

void real_audio_channel::free()
{
	sound_playing=nullptr;
	repeat=0;
	busy=false;
	monitoring=false;
}

//!Forces sound stop.

//!Forces sound stop. Internally calls Mix_HaltChannel, sets playing to false
//!and calls "free" if the channel is unmonitored.

void real_audio_channel::stop()
{
	Mix_HaltChannel(index);
	playing=false;
	if(!monitoring) free();
}

//!Clears the "playing" flag and calls free if not monitored. 

//!This is part of SDL_Audio callback system.

void real_audio_channel::do_callback()
{
	playing=false;
	if(!monitoring) free();
}

//!Sets the volume for the channel. 

//!The volume for the channel is mixed with the overall master volume set on the 
//!controller. Maximum loudness is achieved when both are set at their maximum values.

void real_audio_channel::set_volume(int v)
{
	volume=v;
	int overall_volume=controller_ref->get_main_sound_volume();
	Mix_Volume(index, (float) volume * ((float)volume / (float)overall_volume));
}

//!Plays the sound_struct. 

//!A bug in the SDL Audio library (or in my code) prevents the panning from 
//!working on Windows builds.

int real_audio_channel::play(const sound_struct& e)
{
	int overall_volume=controller_ref->get_main_sound_volume();
	sound_playing=e.sound_ptr;
	repeat=e.repeat;
	
	if(e.volume!=-1) volume=e.volume;
	Mix_Volume(index, (float) volume * ((float)volume / (float)overall_volume));

	//TODO: try to play first, pan later... This is a Windows bug.
#ifndef WINCOMPIL
	if(e.volume_left!=-1 && e.volume_right!=-1) 
	{
		Mix_SetPanning(index, e.volume_left, e.volume_right);
	}
#endif

	playing=true;
	
	if(!e.ms_fade) return Mix_PlayChannel(index, (*sound_playing).get_data(), repeat);
	else return Mix_FadeInChannel(index, (*sound_playing).get_data(), repeat, e.ms_fade);
}

//!Pauses the sound. 

//!Internally calls Mix_Pause for the channel and sets the paused flag to true.

void real_audio_channel::pause()
{
	Mix_Pause(index);
	paused=true;
}

//!Resumes playing after pausing. 

//!Internally calls Mix_Resume for the channel and unsets the pause flag.

void real_audio_channel::resume()
{
	Mix_Resume(index);
	paused=false;
}

/*******************************************************************************
Audio controller implementation begins.
*/

std::map<int, real_audio_channel *> audio_controller::callback_channels;

//!Constructor for audio_controller.

//!Requires an audio_controller_config object. May throw if the system cannot
//!be started or the audio device is unavailable.

audio_controller::audio_controller(const audio_controller_config& c):
	main_sound_volume(c.initial_volume),
	ratio(c.ratio), //22050), 
	out(c.out), 
	buffers(c.buffers), //1024), //2048), 
	requested_channels(c.channels), 
	format(c.format), 
	music_playing(false)
{
	
	ldt::log_lsdl::get()<<"Init audio controller with "<<requested_channels<<" channels, "<<buffers<<" buffers, "<<out<<" outputs and a ratio of "<<ratio<<std::endl;

	int i=callback_channels.size();
	//Create all channels.
	while(i < requested_channels)
	{
		channels.push_back(std::move(real_audio_channel(i++, *this)));
	}

	//Fill the callback channel static object.
	for(auto& ch : channels)
	{
		callback_channels[ch.get_index()]=&ch;
	}

	//Comprobar que el audio está arrancado.
	if(SDL_WasInit(SDL_INIT_AUDIO)==0)
	{
		ldt::log_lsdl::get()<<"Starting sdl audio subsystem."<<std::endl;
		if(SDL_InitSubSystem(SDL_INIT_AUDIO)==-1)
		{
			throw std::runtime_error("Unable to init audio system");
		}
	}


	if(Mix_OpenAudio(ratio, format, out, buffers) == -1)
	{
		throw std::runtime_error("Unable to open audio device");
	}

	ldt::log_lsdl::get()<<"Starting sdl mixer and setting callback function."<<std::endl;
	Mix_ChannelFinished(audio_play_callback);
}

audio_controller::~audio_controller()
{
	ldt::log_lsdl::get()<<"Unmounting audio controller..."<<std::endl;

	stop_music();

	for(const auto& c: channels)
	{
		stop_sound(c.get_index());
		callback_channels.erase(c.get_index());
	}

	ldt::log_lsdl::get()<<callback_channels.size()<<" callback channels remain."<<std::endl;

	channels.clear();

	//Check if there are still channels before closing the mixer.
	if(!callback_channels.size())
	{
		ldt::log_lsdl::get()<<"Closing sdl mixer."<<std::endl;
		Mix_CloseAudio();
	}
}

//!SDL2 audio callback.

//!The sdl audio system needs a callback for the channel. We provide it in way
//1that still allows for singletons to be avoided.

void lda::audio_play_callback(int pchannel)
{
	if(audio_controller::callback_channels.count(pchannel))
	{
		audio_controller::callback_channels[pchannel]->do_callback();
	}
}

//!Plays sound in a free channel.

//!Attempts to locate a free channel. If no free channel is available the
//!function will throw a std::runtime_error.s
//!The sound_struct is checked before played. If it is not ready no sound will be
//!Played.

void audio_controller::play_sound(sound_struct& pstruct)
{
	if(!pstruct.is_ready())
	{
		ldt::log_lsdl::get()<<"Sound reference not ready to be played"<<std::endl;
		return;
	}

	if(channels.at(get_free_channel_index()).play(pstruct)==-1)
	{ 
		throw std::runtime_error("Error while playing sound :"+std::string(Mix_GetError()));
	}
}

//!Gets a free channel index.

//!pfrom will be used to start the search and pto will be the last channel
//!searched if provided. Returns -1 when no channels are available.

int audio_controller::get_free_channel_index(int pfrom, int pto)
{
	int l=(pto==-1) ? (int)channels.size()-1 : (pto >= (int)channels.size() ? channels.size()-1 : pto);

	for(; pfrom < l; pfrom++)
	{
		real_audio_channel& c=channels.at(pfrom);
		if(!c.is_busy() && !c.is_playing())
		{
			return pfrom;
		}
	}

	throw std::runtime_error("No free channels available");
}

//!Stops the sound at the indicated channel.

//!The channel index is checked before execution, which may throw a std::runtime_error
//!if the channel does not exist.

void audio_controller::stop_sound(int pchannel)
{
	check_channel(pchannel);
	channels.at(pchannel).stop();
}

//!Checks that a channel with the given index exists.

//!Index for channels are always consecutive and always start at 0.

void audio_controller::check_channel(int pchannel)
{
	if(pchannel < 0 || pchannel >= (int)channels.size())
	{
		throw std::runtime_error("Unable access channel out of bounds");
	}
}

//!Plays music.

//!Plays the music structure with as many repeats as indicated in prepeat. -1
//!will loop the music until stopped by "stop_music". Internally uses 
//!Mix_PlayMusic.

void audio_controller::play_music(music &pmusic, int prepeat)
{
	if(!pmusic.is_ready())
	{
		ldt::log_lsdl::get()<<"Sound reference not ready to be played"<<std::endl;
		return; 
	}

	Mix_PlayMusic(pmusic.get_data(), prepeat);
	music_playing=true;
}

//!Stops music.

//!Stops the music through Mix_HaltMusic.

void audio_controller::stop_music()
{
	if(music_playing)
	{
		Mix_HaltMusic();
		music_playing=false;
	}
}
 
//!Sets the main sound volume that affects all channels.

//!The volume must be in the range 0-128. The main sound volume will be 
//!blended with each channel volume: to achieve maximum loudness both must
//!be at their maximum value.

void audio_controller::set_main_sound_volume(int p_vol)
{
	main_sound_volume=p_vol;
}

//!Sets the volume for a channel.

//!Volume is in the range 0-128. Channel will be checked, which could result
//!in a std::runtime_error being thrown if it doesn't exist.

void audio_controller::set_sound_volume(int p_vol, int pchannel)	//p_vol de 0 a 128.
{
	check_channel(pchannel);
	channels.at(pchannel).set_volume(p_vol);
}

//!Sets the music volume.

//!The valid range is 0-128. Forwards to Mix_VolumeMusic.

void audio_controller::set_music_volume(int p_vol)
{
	Mix_VolumeMusic(p_vol);
}

//!Prints a crude representation of channel state to stout.

//!The representation looks like this:
//!C[8] = PPB___|_
//!Where 8 is the number of channels and |=Paused, P=playing, B=busy, _=free.

void audio_controller::debug_state()
{
	std::cout<<"C["<<channels.size()<<"] = ";

	for(real_audio_channel& c : channels)
	{
		if(c.is_paused()) std::cout<<"|"<<std::endl;
		else if(c.is_playing()) std::cout<<"P";
		else if(c.is_busy()) std::cout<<"B";
		else std::cout<<"_";
	}

	std::cout<<std::endl;
}

//!Gets a handle for the specified audio channel.

//!An audio_channel object (a proxy for a real_audio_channel) will be returned.
//!Channel index will be checked, which may result in a std::runtime_error being
//!thrown.

audio_channel audio_controller::get_channel(int pchannel)// throw()
{
	check_channel(pchannel);
	return audio_channel(channels.at(pchannel));
}

//!Gets a free channel. 

//!If no channels are available will throw with std::runtime_error, since it
//is implemented on terms of get_free_channel_index.

audio_channel audio_controller::get_free_channel()// throw()
{
	return get_channel(get_free_channel_index());
}

//!Pauses all sounds.

//!This is equivalent to call pause in every channel. Channels will pause 
//!whether they are monitored or not.

void audio_controller::pause_sound()
{
	for(real_audio_channel c : channels) c.pause();
}

//!Resumes all sounds.

//!This is the equivalent to call resume in every channel. Channels will
//!resume whether they are monitored or not.

void audio_controller::resume_sound()
{
	for(real_audio_channel c : channels) c.resume();
}

//!Stops all sounds.

//!Calls stop() in all audio channels assigned to the controller.

void audio_controller::stop_sound()
{
	for(real_audio_channel c : channels) c.stop();
}
