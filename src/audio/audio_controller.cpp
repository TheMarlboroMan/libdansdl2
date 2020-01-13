#include "audio_controller.h"
#include "../tools/log.h"

#include <src/sentry.h>

#include <stdexcept>
#include <sstream>
#include <cmath>
using namespace lda;

std::map<int, real_audio_channel *> audio_controller::callback_channels;

//!Constructor for audio_controller.

//!Requires an audio_controller_config object. May throw if the system cannot
//!be started or the audio device is unavailable.

audio_controller::audio_controller(const audio_controller_config& c):
	main_sound_volume(c.initial_sound_volume),
	main_music_volume(c.initial_music_volume),
	ratio(c.ratio), //22050), 
	out(c.out), 
	buffers(c.buffers), //1024), //2048), 
	requested_channels(c.channels), 
	format(c.format), 
	music_playing(false)
{
	
	lm::log(ldt::log_lsdl::get(), lm::lvl::info)<<"Init audio controller with "<<requested_channels<<" channels, "<<buffers<<" buffers, "<<out<<" outputs and a ratio of "<<ratio<<std::endl;

	//Comprobar que el audio está arrancado.
	if(SDL_WasInit(SDL_INIT_AUDIO)==0)
	{
		lm::log(ldt::log_lsdl::get(), lm::lvl::info)<<"Starting sdl audio subsystem."<<std::endl;
		if(SDL_InitSubSystem(SDL_INIT_AUDIO)==-1)
		{
			throw std::runtime_error("Unable to init audio system");
		}
	}

	if(Mix_OpenAudio(ratio, format, out, buffers) == -1)
	{
		throw std::runtime_error("Unable to open audio device");
	}

	
	//Create all channels. The constructor of the channel is private, so the channel
	//itself will do the dirty work.
	int i=callback_channels.size(); //This should actually be... zero. Oh well.
	Mix_AllocateChannels(requested_channels);
	while(i < requested_channels)
		channels.push_back(std::move(real_audio_channel{i++, main_sound_volume}));

	//Fill the callback channel static object.
	for(auto& ch : channels)
	{
		callback_channels[ch.get_index()]=&ch;
	}

	lm::log(ldt::log_lsdl::get(), lm::lvl::info)<<"Starting sdl mixer and setting callback function."<<std::endl;
	Mix_ChannelFinished(audio_play_callback);
}

audio_controller::~audio_controller()
{
	lm::log(ldt::log_lsdl::get(), lm::lvl::info)<<"Unmounting audio controller..."<<std::endl;

	stop_music();

	for(const auto& c: channels)
	{
		stop_sound(c.get_index());
		callback_channels.erase(c.get_index());
	}

	lm::log(ldt::log_lsdl::get(), lm::lvl::info)<<callback_channels.size()<<" callback channels remain."<<std::endl;

	channels.clear();

	//Check if there are still channels before closing the mixer.
	if(!callback_channels.size())
	{
		lm::log(ldt::log_lsdl::get(), lm::lvl::info)<<"Closing sdl mixer."<<std::endl;
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

void audio_controller::play_sound(const sound_struct& pstruct)
{
	if(!pstruct.is_ready())
	{
		lm::log(ldt::log_lsdl::get(), lm::lvl::info)<<"Sound reference not ready to be played"<<std::endl;
		return;
	}

	if(channels.at(get_free_channel_index()).play(pstruct)==-1)
	{ 
		throw std::runtime_error("Error while playing sound :"+std::string(Mix_GetError()));
	}
}

//!Gets a free channel index.

//!A free channel is not playing. Monitored channels will not be reported as
//!free. 
//!pfrom will be used to start the search and pto will be the last channel
//!searched if provided. Returns -1 when no channels are available.

int audio_controller::get_free_channel_index(int pfrom, int pto)
{
	int l=(pto==-1) ? (int)channels.size()-1 : (pto >= (int)channels.size() ? channels.size()-1 : pto);

	for(; pfrom < l; pfrom++)
	{
		real_audio_channel& c=channels.at(pfrom);
		if(!c.is_monitored() && !c.is_playing())
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

void audio_controller::play_music(const music_struct& m)
{
	if(!m.is_ready())
	{
		lm::log(ldt::log_lsdl::get(), lm::lvl::info)<<"Music reference not ready to be played"<<std::endl;
		return; 
	}

	if(m.volume!=-1) set_music_volume(m.volume);

	Mix_PlayMusic(m.music_ptr->get_data(), m.repeat);
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

//!Sets the music volume, that will be processed against the master music volume.

//!The valid range is 0-128. The input value transformed according to the 
//!main_music_volume.

void audio_controller::set_music_volume(int p_vol)
{
//	float vol=(float) p_vol * ((float)p_vol / (float)main_music_volume);
	float vol=(float) p_vol * ((float)main_music_volume / 128.f);
	Mix_VolumeMusic(ceil(vol));
}

//!Gets music volume as processed by the main music volume.

//!This function does not neccesarily return the value of Mix_VolumeMusic(-1);

int audio_controller::get_music_volume() const
{
	float pvol=Mix_VolumeMusic(-1);
	float vol=pvol * (pvol / (float)main_music_volume);
	return ceil(vol);
}

//!Returns a crude representation of channel state to stout.

//!The representation looks like this:
//!C[8/8/8] = ppS___-_
//!Where 8 is the number of channels (vector, allocated and init value) 
//!and p=playing, _=free and s=paused.
//!Monitored channels will be displayed in uppercase (P-S).
//!It is a good idea to run and check this once in a while, specially when
//!monitoring is used in the application, to prevent channel blocking.

std::string audio_controller::debug_state()
{
	std::stringstream ss;
	ss<<"C["<<channels.size()<<"/"<<Mix_AllocateChannels(-1)<<"/"<<requested_channels<<"] = "; 

	for(auto& c : channels)
	{
		if(c.is_monitored())
		{
			if(c.is_paused()) ss<<"S"<<std::endl;
			else if(c.is_playing()) ss<<"P";
			else ss<<"-";
		}
		else
		{
			if(c.is_paused()) ss<<"s"<<std::endl;
			else if(c.is_playing()) ss<<"p";
			else ss<<"_";
		}
	}

	return ss.str();
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

//!A free channel is either playing or not being monitored so 
//!two consecutive calls will !yield the very same channel. To avoid this, 
//!either have the channel play something or set it under monitoring with 
//!channel.set_monitoring(true);
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
	for(auto& c : channels) c.pause();
}

//!Resumes all sounds.

//!This is the equivalent to call resume in every channel. Channels will
//!resume whether they are monitored or not.

void audio_controller::resume_sound()
{
	for(auto& c : channels) c.resume();
}

//!Stops all sounds.

//!Calls stop() in all audio channels assigned to the controller.

void audio_controller::stop_sound()
{
	for(auto& c : channels) c.stop();
}
