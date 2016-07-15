#include "resource_manager.h"
#include <iostream>

using namespace lda;

/*Se realizan inserciones y se devuelve -1 si no están preparadas correctamente.
Un fallo no significa nada: realmente se vuelven a hacer comprobaciones con 
cada reproducción que se haga. Atención a que realmente SIEMPRE se hace una 
inserción en la lista!*/

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

/*Idem que en el anterior. Se realiza la inserción y se devuelve -1 si ha
fallado algo.*/

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

sound& resource_manager::get_sound(int key)
{
	if(!sounds.count(key))
	{
		throw std::runtime_error("Requested non existing sound key");
	}

	return *sounds[key];
}

music& resource_manager::get_music(int key)
{
	if(!musics.count(key))
	{
		throw std::runtime_error("Requested non existing music key");
	}

	return *musics[key];
}
