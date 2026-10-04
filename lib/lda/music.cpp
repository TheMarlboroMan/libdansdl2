#include <lda/music.h>
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

music::music(
	const music& _other
):
	music_data{nullptr},
	path{},
	ready{}
{

	load(_other.path);
}

music& music::operator=(
	const music& _other
) {

	music_data=nullptr;
	path="";
	ready=false;

	load(_other.path);

	return *this;
}

music::music(
	music&& _other
):
	music_data{_other.music_data},
	path{std::move(_other.path)},
	ready{_other.ready}
{

	_other.music_data=nullptr;
}

music& music::operator=(
	music&& _other
) {

	music_data=_other.music_data;
	path=std::move(_other.path);
	ready=_other.ready;
	_other.music_data=nullptr;

	return *this;
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
