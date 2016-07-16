#ifndef LIBDANSDL2_TEXTURE_MANAGER_H
#define LIBDANSDL2_TEXTURE_MANAGER_H

#include <map>
#include <memory>

#include "../texture/texture.h"


namespace ldv
{

class texture_manager
{
	public:

	void 		insert(int, texture *);
	const texture& 	get(int) const;
	int 		size() const {return data.size();}

	private:

	std::map<int, std::unique_ptr<texture> >	data;
};

} //Fin namespace DLibV

#endif
