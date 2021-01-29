#pragma once

#include "representation.h"
#include <ldt/sdl_tools.h>

#include <vector>
#include <memory>

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

	//!Creates a group representation with its top-left at the given point.
				group_representation(point);

	void	 		insert(representation *);

	void 			clear();

	//!Returns the number of representations grouped under this one.
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

	std::vector<std::unique_ptr<representation>> 	data;	//!< Internal data storage.

	virtual void		do_draw();
};

}

