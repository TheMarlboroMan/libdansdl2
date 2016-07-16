#include "texture_manager.h"

#include <stdexcept>

using namespace ldv;

void texture_manager::insert(int key, texture * p_rec)
{
	if(data.count(key))
	{
		throw std::runtime_error("Key already exists when registering texture with manager");
	}

	data[key]=std::unique_ptr<texture>(p_rec);
}

const texture& texture_manager::get(int key) const
{
	if(data.count(key))
	{
		throw std::runtime_error("Key does not exist when requesting texture from manager");
	}

	return *(data.at(key));
}
