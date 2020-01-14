#include <ldv/resource_manager.h>

#include <stdexcept>

using namespace ldv;

//!Inserts a texture with the numerical handle. 

//!The new texture will be managed by the resource_manager and must not be 
//!freed anywhere else. Will throw if the numerical key is in use.

void resource_manager::insert(int key, texture * p_rec)
{
	if(texture_data.count(key))
	{
		throw std::runtime_error("Key already exists when registering texture with manager");
	}

	texture_data.insert(std::pair<int, std::unique_ptr<texture> >(key, std::unique_ptr<texture>(p_rec)));
}

//!Returns a reference to the texture with the numerical handle. 

//!Will throw if the handle does not exist.

const texture& resource_manager::get_texture(int key) const
{
	if(!texture_data.count(key))
	{
		throw std::runtime_error("Key does not exist when requesting texture from manager");
	}

	return *(texture_data.at(key));
}

//!Inserts a surface with the numerical handle. 

//!The new surface will be managed by the resource_manager and must not be 
//!freed anywhere else. Will throw if the numerical key is in use.

void resource_manager::insert(int key, surface * p_rec)
{
	if(surface_data.count(key))
	{
		throw std::runtime_error("Key already exists when registering surface with manager");
	}

	surface_data[key]=std::unique_ptr<surface>(p_rec);
}

//!Returns a reference to the surface with the numerical handle. 

//!Will throw if the handle does not exist.


const surface& resource_manager::get_surface(int key) const
{
	if(!surface_data.count(key))
	{
		throw std::runtime_error("Key does not exist when requesting surface from manager");
	}

	return *(surface_data.at(key));
}
