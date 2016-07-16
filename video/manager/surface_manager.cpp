#include "surface_manager.h"

#include <stdexcept>

using namespace ldv;

void surface_manager::insert(int key, surface * p_rec)
{
	if(data.count(key))
	{
		throw std::runtime_error("Key already exists when registering surface with manager");
	}

	data[key]=std::unique_ptr<surface>(p_rec);
}

const surface& surface_manager::get(int key) const
{
	if(data.count(key))
	{
		throw std::runtime_error("Key does not exist when requesting surface from manager");
	}

	return *(data.at(key));
}
