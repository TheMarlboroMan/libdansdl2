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

	void                    insert(representation *);

	void                    clear();
/**
 * changes contents so that the top-leftmost of them occupies the relative
 * position 0.0. Newly added representations will not be normalized with
 * regards to them.
 */
	void                    trim();

	//!Returns the number of representations grouped under this one.
	unsigned int            size() const {return data.size();}

	virtual void            go_to(point);
	//!Gets the group position as a point.

	//!Each part of the group may go into the group negative space.
	virtual const point&    get_position() const {return position;}

	virtual bool            draw(screen&, const camera&, bool=false);
	virtual bool            draw(screen&, bool=false);

	private:

	bool                    draw_internal(screen&, camera const *);
	void                    update_base_view_position();

	point                   position;

	//!Cannot be copied.
	group_representation&   operator=(const group_representation &)=delete;
	                        group_representation(const group_representation&)=delete;

	protected:

	std::vector<std::unique_ptr<representation>> data;	//!< Internal data storage.

	virtual void            do_draw();
};

}

