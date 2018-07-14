#ifndef LIBDANSDL2_POINT_REPRESENTATION_H
#define LIBDANSDL2_POINT_REPRESENTATION_H

#include "../primitive_representation.h"
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

	//!Copy constructor.
						point_representation(const point_representation&);

	//!Virtual class destructor.
	virtual 				~point_representation() {}

	//!Assignment operator.
						point_representation& operator=(const point_representation&);

	//!Inserts a point.
	void					insert(point p) {internal_insert(p, true);}
	void					insert(const std::vector<point>&);
	void					clear();

	virtual void 				go_to(point p);
	virtual point				get_position() const;
	virtual rect				get_base_view_position() const;

	protected: 

	virtual void				do_draw();

	private:

	void					internal_insert(point, bool=true);

	std::vector<point>			points;
	point					origin;
};

}

#endif
