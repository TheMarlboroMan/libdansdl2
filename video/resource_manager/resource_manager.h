#ifndef LIBDANSDL2_RESOURCE_MANAGER_H
#define LIBDANSDL2_RESOURCE_MANAGER_H

#include <map>
#include <memory>

#include "../texture/texture.h"
#include "../surface/surface.h"


namespace ldv
{

class resource_manager
{
	public:

	void 		insert(int, texture *);
	const texture& 	get_texture(int) const;
	int 		size_texture() const {return texture_data.size();}

	void	 	insert(int, surface *);
	const surface&	get_surface(int) const;
	size_t		size_surface() const {return surface_data.size();}


	private:

	std::map<int, std::unique_ptr<texture> >	texture_data;
	std::map<int, std::unique_ptr<surface> >	surface_data;
};

} //Fin namespace DLibV

#endif
