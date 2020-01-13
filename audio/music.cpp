#include "music.h"
#include <iostream>
#include <stdexcept>

using namespace lda;

//!Constructs an empty music object.

//!It can be readied by calling "load".

music::music():
	ready(false)
{

}

//!Constructs a music object from the file specified.

//!May be buggy, as load throws.

//TODO: Exception...

music::music(const std::string& ppath)
	:ready(false)
{
	load(ppath);
}

//!Class destructor.

//!Implicitely calls free.

music::~music()
{
	free();
}

//!Frees music data.

//!Calls Mix_FreeMusic if there is music_data available. The object will be
//!left in a state ready to load another file.

void music::free()
{
	if(music_data)
	{
		Mix_FreeMusic(music_data);
		music_data=nullptr;
		ready=false;
	}
}

//!Attempts to load the specified file.

//!Will throw a std::runtime_error when the file cannot be loaded.

void music::load(const std::string& ppath)
{
	path=ppath;
	music_data=Mix_LoadMUS(path.c_str());

	if(!music_data)
	{
		throw std::runtime_error(std::string("music::load() : error loading ")+path);
	}

	ready=true;
}
