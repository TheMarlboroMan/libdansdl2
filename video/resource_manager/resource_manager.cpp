#include "resource_manager.h"

#include <stdexcept>

using namespace ldv;

void resource_manager::insert(int key, texture * p_rec)
{
	if(texture_data.count(key))
	{
		throw std::runtime_error("Key already exists when registering texture with manager");
	}

	texture_data.insert(std::pair<int, std::unique_ptr<texture> >(key, std::unique_ptr<texture>(p_rec)));
}

const texture& resource_manager::get_texture(int key) const
{
	if(!texture_data.count(key))
	{
		throw std::runtime_error("Key does not exist when requesting texture from manager");
	}

	return *(texture_data.at(key));
}

void resource_manager::insert(int key, surface * p_rec)
{
	if(surface_data.count(key))
	{
		throw std::runtime_error("Key already exists when registering surface with manager");
	}

	surface_data[key]=std::unique_ptr<surface>(p_rec);
}

const surface& resource_manager::get_surface(int key) const
{
	if(!surface_data.count(key))
	{
		throw std::runtime_error("Key does not exist when requesting surface from manager");
	}

	return *(surface_data.at(key));
}
