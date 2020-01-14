#include <lda/sound.h>

#include <iostream>
#include <stdexcept>

using namespace lda;

//!Constructs and empty sound object.

//!Can be readied with a call to "load".

sound::sound()
	:ready(false)
{

}

//!Constructs a sound object and attempts to load the file.

//TODO: Fix this behaviour.
//!This function may be buggy exception-wise, as load may throw.

//TODO: Except.

sound::sound(const std::string& ppath)
	:ready(false)
{
	load(ppath);
}

//!Class destructor.

//!Implicitely frees the sound data.

sound::~sound()
{
	free();
}

//!Deletes the audio data.

//!Calls Mix_FreeChunk and leaves the object in a good state to call load again.

void sound::free()
{
	if(sound_data)
	{
		Mix_FreeChunk(sound_data);
		sound_data=nullptr;
		ready=false;
	}
}

//!Attempts to load the file.

//!Will throw std::runtime_error when the sound cannot be loaded. In case of
//!success, the class will be ready.

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
