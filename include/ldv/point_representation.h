#pragma once
#include "primitive_representation.h"
#include <vector>

namespace ldv
{

//!A representation of a single point or a group of points.

//!It is defined in terms of integers. Unlike the polygon, its points are not
//!normalized. The first point is considered its origin and determines its
//!position.

class point_representation:
	public primitive_representation
{
	public:
	//!Class constructor with alpha color. Creates an empty set.
						point_representation(rgba_color);

	//!Class constructor with color. Creates an empty set.
						point_representation(rgb_color);

	//!Class constructor with alpha color from the given set of points.
						point_representation(const std::vector<point>&, rgba_color);

	//!Class constructor with color from the given set of points.
						point_representation(const std::vector<point>&, rgb_color);

	//!Class constructor with alpha color from the given point.
						point_representation(point, rgba_color);

	//!Class constructor with color from the given point.
						point_representation(point, rgb_color);

	//!Inserts a point.
	void					insert(point p) {internal_insert(p, true);}
	void					insert(const std::vector<point>&);
	void					clear();

	virtual void 				go_to(point p);
//!Returns the position.

//!Position is expressed as the top-left most point relative to the origin
//!That is, if origin is 10,10 and there is a point in -5,-5 this function
//!will return 5,5.
//!Will throw if the representation has no points.
	virtual const point&    get_position() const {return position;}
	virtual const rect&     get_base_view_position() const {return base_view_position;}
	virtual rect&           get_base_view_position() {return base_view_position;}

	protected:

	virtual void            do_draw();

	private:

	void                    update_base_view_position();
	void                    internal_insert(point, bool=true);

	std::vector<point>      points;
	rect                    base_view_position;
	point                   origin;
	point                   position;
};

}

