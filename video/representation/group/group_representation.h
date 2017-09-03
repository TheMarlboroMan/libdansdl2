#ifndef LIBDANSDL2_GROUP_REPRESENTATION_H
#define LIBDANSDL2_GROUP_REPRESENTATION_H

#include <vector>
#include <memory>

#include "../representation.h"
#include "../../../tools/sdl_tools/sdl_tools.h"

namespace ldv
{

//!A group of representations that are drawn as one.

//!Grouped representations are applied the same transformations and traslocations.
//!Representations assigned to a group belong to the group itself, which will
//!manage their lifetime. Cannot be copied nor assigned.

class group_representation:
	public representation
{
	public:

				group_representation(point);
	virtual 		~group_representation();
	void	 		insert(representation *);

	void 			clear();
	unsigned int 		size() const {return data.size();}

	virtual void 		go_to(point);
	virtual point		get_position() const;
	virtual rect		get_base_view_position() const;

	virtual void		draw(screen&, const camera&, bool=false);
	virtual void		draw(screen&, bool=false);

	private:

	void			draw_internal(screen&, camera const *);

	point			position;

	//!Cannot be copied.
	group_representation& 	operator=(const group_representation &)=delete;
				group_representation(const group_representation&)=delete;

	protected:

	std::vector<std::unique_ptr<representation>> 	data;

	virtual void		do_draw();
};

}

#endif
