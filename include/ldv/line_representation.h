#pragma once

#include "primitive_representation.h"

#include <array>

namespace ldv
{

//!Primitive repreentation of a line.

//!Internally represented as two points, normalized from the origin (the first point).

class line_representation:
	public primitive_representation
{
	public:

	//!Class constructor from point a to b, with alpha color.
					line_representation(point, point, rgba_color);

	//!Class constructor from point a to b, with color.
					line_representation(point, point, rgb_color);

	virtual void 			go_to(point p);
	//!Retrieves the position.

	//!The position is the origin (first point specified).
	virtual const point&        get_position() const {return origin;}

	void                        set_points(point, point);

	protected:

	virtual void                do_draw();

	private:

	void                        update_base_view_position();

	std::array<point, 2>        points; //!<Points as described as relative to a 0.0...
	point                       origin; //!<this is that 0.0.
};

}
