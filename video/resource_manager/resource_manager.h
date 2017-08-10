#ifndef LIBDANSDL2_RESOURCE_MANAGER_H
#define LIBDANSDL2_RESOURCE_MANAGER_H

#include <map>
#include <memory>

#include "../texture/texture.h"
#include "../surface/surface.h"


namespace ldv
{

///A single entry-point to manage textures and surfaces.
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

	///Inserts a texture with the numerical handle. The new texture will be managed by the resource_manager and must not be freed anywhere else. Will throw if the numerical key is in use.
	void 		insert(int, texture *);
	///Returns a reference to the texture with the numerical handle. Will throw if the handle does not exist.
	const texture& 	get_texture(int) const;
	///Gets the count number of all loaded textures.
	size_t 		size_texture() const {return texture_data.size();}

	///Inserts a surface with the numerical handle. The new surface will be managed by the resource_manager and must not be freed anywhere else. Will throw if the numerical key is in use.
	void	 	insert(int, surface *);
	///Returns a reference to the surface with the numerical handle. Will throw if the handle does not exist.
	const surface&	get_surface(int) const;
	///Gets the count number of all loaded surfaces.
	size_t		size_surface() const {return surface_data.size();}

	private:

	std::map<int, std::unique_ptr<texture> >	texture_data;
	std::map<int, std::unique_ptr<surface> >	surface_data;
};

} //Fin namespace DLibV

#endif
