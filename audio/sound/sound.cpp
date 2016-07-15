#include "sound.h"

#include <iostream>
#include <stdexcept>

using namespace dla;

sound::sound()
	:ready(false)
{

}

sound::sound(const std::string& path)
	:ready(false)
{
	load(path);
}

sound::~sound()
{
	free();
}

void sound::free()
{
	if(sound_data)
	{
		Mix_FreeChunk(sound_data);
		sound_data=nullptr;
		ready=false;
	}
}

void sound::load(const std::string& ppath)
{
	path=ppath;
	sound_data=Mix_LoadWAV(path.c_str());

	if(!sound_data)
	{
		throw std::runtime_error(std::string("sound::load() : error loading ")+path);
	}

	ready=true;
}
