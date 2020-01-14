#include <lda/resource_manager.h>
#include <iostream>

using namespace lda;

//!Attempts to insert a sound file with the specified key.

//!Will throw a std::runtime_error if the key is already in use. There will
//!be no visible failure if the sound itself does not exist: the application
//!must manually check the ready state.

void resource_manager::insert_sound(int key, const std::string& path)
{
	if(sounds.count(key))
	{
		throw std::runtime_error("Sound key already exists when inserting "+path);
	}
	else
	{
		sounds.insert(std::pair<int, std::unique_ptr<sound> >(key, std::move(std::unique_ptr<sound>(new sound(path)))));
	}
}

//!Attempts to insert a music file with the specified key.

//!Will throw a std::runtime_error if the key is already in use. There will
//!be no visible failure if the music itself does not exist: the application
//!must manually check the ready state.

void resource_manager::insert_music(int key, const std::string& path)
{
	if(musics.count(key))
	{
		throw std::runtime_error("Music key already exists when inserting "+path);
	}
	else
	{
		musics.insert(std::pair<int, std::unique_ptr<music> >(key, std::move(std::unique_ptr<music>(new music(path)))));
	}
}

//!Retrieves the sound object with the specified key.

//!Will throw a std::runtime_error if the key does not exist.

sound& resource_manager::get_sound(int key) const
{
	if(!sounds.count(key))
	{
		throw std::runtime_error("Requested non existing sound key");
	}

	return *sounds.at(key);
}

//!Retrieves the music object with the specified key.

//!Will throw a std::runtime_error if the key does not exist.

music& resource_manager::get_music(int key) const
{
	if(!musics.count(key))
	{
		throw std::runtime_error("Requested non existing music key");
	}

	return *musics.at(key);
}
