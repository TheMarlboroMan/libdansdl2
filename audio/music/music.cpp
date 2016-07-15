#include "music.h"
#include <iostream>

using namespace lda;

music::music():
	ready(false)
{

}

music::music(const std::string& path)
	:ready(false)
{
	load(path);
}

music::~music()
{
	free();	
}

void music::free()
{
	if(music_data)
	{
		Mix_FreeMusic(music_data);
		music_data=nullptr;
		ready=false;
	}
}

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
