#pragma once

#include "texture.h"
#include "surface.h"

#include <map>
#include <memory>

namespace ldv
{

//TODO: This is a candidate for "libdansdl2-tools", as it has no place in the
//larger library.

//!A single entry-point to manage textures and surfaces.
/**Provides an easy, tried and true interface to insert textures and surfaces
and retrieve them later. 

All surfaces and textures are stored and retrieved by an unique numerical identifier
that can be - for example - assigned to an enum. Textures and surfaces do not share
containers, so their indexes can be repeated.

Textures and surfaces loaded remain in memory, since there is no "clear" functions.
*/

class resource_manager
{
	public:

	void 		insert(int, texture *);
	const texture& 	get_texture(int) const;
	//!Gets the count number of all loaded textures.
	size_t 		size_texture() const {return texture_data.size();}

	void	 	insert(int, surface *);
	const surface&	get_surface(int) const;
	//!Gets the count number of all loaded surfaces.
	size_t		size_surface() const {return surface_data.size();}

	private:

	std::map<int, std::unique_ptr<texture> >	texture_data;
	std::map<int, std::unique_ptr<surface> >	surface_data;
};

} //Fin namespace DLibV

