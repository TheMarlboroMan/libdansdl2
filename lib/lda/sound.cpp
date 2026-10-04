#include <lda/sound.h>

#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace lda;

//!Constructs and empty sound object.

//!Can be readied with a call to "load".

sound::sound()
	:ready(false)
{ }

//!Constructs a sound object and attempts to load the file.

//TODO: Fix this behaviour.
//!This function may be buggy exception-wise, as load may throw.

//TODO: Except.

sound::sound(const std::string& ppath)
	:ready(false)
{

	load(ppath);
}

sound::sound(
	const sound& _other
):
	sound_data{duplicate_chunk(_other.sound_data)},
	path{_other.path},
	ready{_other.ready}
{ }

sound& sound::operator=(
	const sound& _other
) {

	sound_data=duplicate_chunk(_other.sound_data);
	path=_other.path;
	ready=_other.ready;
	return *this;
}

sound::sound(
	sound&& _other
):
	sound_data{_other.sound_data},
	path{std::move(_other.path)},
	ready{_other.ready}
{

	_other.sound_data=nullptr;
}

sound& sound::operator=(
	sound&& _other
) {

	sound_data=_other.sound_data;
	path=std::move(_other.path);
	ready=_other.ready;
	_other.sound_data=nullptr;

	return *this;
}

//!Class destructor.

//!Implicitely frees the sound data.

sound::~sound()
{
	free();
}

//!Deletes the audio data.

//!Calls Mix_FreeChunk and leaves the object in a good state to call load again.

void sound::free() {

	if(sound_data) {

		Mix_FreeChunk(sound_data);
		sound_data=nullptr;
		ready=false;
	}
}

//!Attempts to load the file.

//!Will throw std::runtime_error when the sound cannot be loaded. In case of
//!success, the class will be ready.

void sound::load(
	const std::string& ppath
) {
	path=ppath;
	sound_data=Mix_LoadWAV(path.c_str());

	if(!sound_data) {

		std::stringstream ss;
		ss<<"lda::sound::load, error loading file '"<<path<<"': "<<Mix_GetError();
		Mix_ClearError();
		throw std::runtime_error(ss.str());
	}

	ready=true;
}

Mix_Chunk * lda::duplicate_chunk(
	Mix_Chunk const * _source
) {

	if(nullptr==_source) {

		throw std::runtime_error("cannot duplicate chunk without source!");
	}

	Mix_Chunk * result=(Mix_Chunk*)malloc(sizeof(Mix_Chunk));
	if(nullptr==result) {

		throw std::runtime_error("could not acquire memory for mix chunk copy");
	}

    result->abuf=(Uint8*)malloc(_source->alen);
    if(!result->abuf) {

		free(result);
		throw std::runtime_error("could not acquire memory for mix chunk data");
    }

	result->allocated=1;
	result->alen=_source->alen;
	result->volume=_source->volume;
    memcpy(result->abuf, _source->abuf, _source->alen);

    return result;
}
