#include "music.h"
#include <iostream>

using namespace lda;

//!Constructs an empty music object.

//!It can be readied by calling "load".

music::music():
	ready(false)
{

}

//!Constructs a music object from the file specified.

//!If the file cannot be open, the is_ready function will return false.

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

//!In case of failure the is_ready function will return false.

void music::load(const std::string& ppath)
{
	path=ppath;
	music_data=Mix_LoadMUS(path.c_str());

	if(!music_data)
	{
		ready=false;
	}
	else
	{
		ready=true;
	}	
}
