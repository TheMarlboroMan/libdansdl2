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

void real_audio_channel::free()
{
	sound_playing=nullptr;
	repeat=0;
	busy=false;
	monitoring=false;
}

void real_audio_channel::stop()
{
	Mix_HaltChannel(index);
	playing=false;
	free();
}

void real_audio_channel::do_callback()
{
	playing=false;
	if(!monitoring) free();
}

//The overall volume is passed around.

void real_audio_channel::set_volume(int v)
{
	volume=v;
	int overall_volume=controller_ref->get_main_sound_volume();
	Mix_Volume(index, (float) volume * ((float)volume / (float)overall_volume));
}

int real_audio_channel::play(const sound_struct& e)
{
	int overall_volume=controller_ref->get_main_sound_volume();
	sound_playing=e.sound_ptr;
	repeat=e.repeat;
	
	if(e.volume!=-1) volume=e.volume;
	Mix_Volume(index, (float) volume * ((float)volume / (float)overall_volume));

//Enorme bug de windows encontrado hace tiempo...
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

void real_audio_channel::pause()
{
	Mix_Pause(index);
	paused=true;
}

void real_audio_channel::resume()
{
	Mix_Resume(index);
	paused=false;
}

/*******************************************************************************
Inicio de implementación de audio_controller...
*/

std::map<int, real_audio_channel *> audio_controller::callback_channels;

audio_controller::audio_controller(const audio_controller_config& c):
	main_sound_volume(c.initial_volume),
	ratio(c.ratio), //22050), 
	out(c.out), 
	buffers(c.buffers), //1024), //2048), 
	requested_channels(c.channels), 
	format(c.format), 
	music_playing(false)
{
	
	int i=callback_channels.size();
	//Create all channels.
	while(i < requested_channels)
	{
		channels.push_back(std::move(real_audio_channel(i++, *this)));
	}

	//Fill the callback channel static object.
	for(auto&c : channels)
	{
		callback_channels[c.get_index()]=&c;
	}

	//Comprobar que el audio está arrancado.
	if(SDL_WasInit(SDL_INIT_AUDIO)==0)
	{
		if(SDL_InitSubSystem(SDL_INIT_AUDIO)==-1)
		{
			throw std::runtime_error("Unable to init audio system");
		}
	}

	if(Mix_OpenAudio(ratio, format, out, buffers) == -1)
	{
		throw std::runtime_error("Unable to open audio device");
	}

	Mix_ChannelFinished(audio_play_callback);
}


audio_controller::~audio_controller()
{
	stop_sound(-1);
	stop_music();

	for(const auto& c: channels)
	{
		callback_channels.erase(c.get_index());
	}

	channels.clear();

	//Check if there are still channels before closing the mixer.	
	if(!callback_channels.size())
	{
		Mix_CloseAudio();
	}
}

//The sdl audio system needs a callback for the channel. We provide it in way
//that still allows for singletons to be avoided.

void lda::audio_play_callback(int pchannel)
{
	if(audio_controller::callback_channels.count(pchannel))
	{
		audio_controller::callback_channels[pchannel]->do_callback();
	}
}

void audio_controller::play_sound(sound_struct& pstruct, int pchannel)
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

/*Obtiene el primer canal libre a partir del canal especificado.  Si se da
un segundo parámetro busca hasta el canal especificado. Si no se da ninguno 
devuelve realmente el primer canal libre. Aparementemente devuelve -1 si
no hay channels disponibles.*/

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

void audio_controller::stop_sound(int pchannel)
{
	check_channel(pchannel);
	channels.at(pchannel).stop();
}

void audio_controller::check_channel(int pchannel)
{
	if(pchannel < 0 || pchannel >= (int)channels.size())
	{
		throw std::runtime_error("Unable access channel out of bounds");	
	}
}

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

void audio_controller::stop_music()
{
	if(music_playing)
	{
		Mix_HaltMusic();
		music_playing=false;
	}
}

//De la documentación...
//All channels default to a volume of 128, which is the max. 
//Newly allocated channels will have the max volume set, 
//so setting all channels volumes does not affect subsequent channel allocations. 

void audio_controller::set_main_sound_volume(int p_vol)
{
	main_sound_volume=p_vol;
}

void audio_controller::set_sound_volume(int p_vol, int pchannel)	//p_vol de 0 a 128.
{
	check_channel(pchannel);
	channels.at(pchannel).set_volume(p_vol);
}

void audio_controller::set_music_volume(int p_vol)
{
	Mix_VolumeMusic(p_vol);
}


void audio_controller::debug_state()
{
	std::cout<<"C["<<channels.size()<<"] = ";

	for(real_audio_channel& c : channels)
	{
		if(c.is_paused()) std::cout<<"P"<<std::endl;
		else if(c.is_playing()) std::cout<<"R";
		else if(c.is_busy()) std::cout<<"O";
		else std::cout<<"_";
	}

	std::cout<<std::endl;
}

audio_channel audio_controller::get_channel(int pchannel)// throw()
{
	check_channel(pchannel);
	return audio_channel(channels.at(pchannel));
}

audio_channel audio_controller::get_free_channel()// throw()
{
	return get_channel(get_free_channel_index());
}

void audio_controller::pause_sound()
{
	for(real_audio_channel c : channels) c.pause();
}

void audio_controller::resume_sound()
{
	for(real_audio_channel c : channels) c.resume();
}

void audio_controller::stop_sound()
{
	for(real_audio_channel c : channels) c.stop();
}
