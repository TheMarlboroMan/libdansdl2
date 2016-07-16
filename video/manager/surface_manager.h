#ifndef LIBDANSDL2_SURFACE_MANAGER_H
#define LIBDANSDL2_SURFACE_MANAGER_H

#include <map>
#include <memory>

#include "../surface/surface.h"

namespace ldv
{

class surface_manager
{
	public:

	void	 	insert(int, surface *);
	const surface&	get(int) const;
	size_t		size() const {return data.size();}

	private:

	std::map<int, std::unique_ptr<surface> >	data;
};

} //Fin namespace DLibV

#endif
