#ifndef LIBDANSDL2_LINE_REPRESENTATION_H
#define LIBDANSDL2_LINE_REPRESENTATION_H

#include <array>
#include "../primitive_representation.h"

namespace ldv
{

//!Primitive repreentation of a line.

//!Internally represented as two points, normalized from the origin (the first point).

class line_representation:
	public primitive_representation
{
	public:
	
					line_representation(point, point, rgba_color);
					line_representation(point, point, rgb_color);
					line_representation(const line_representation&);
	virtual 			~line_representation() {}
					line_representation& operator=(const line_representation&);

	virtual void 			go_to(point p);
	virtual point			get_position() const;
	virtual rect			get_base_view_position() const;

	void 				set_points(point, point);

	protected:

	virtual void			do_draw();

	private:

	std::array<point, 2>		points; //!<Points as described as relative to a 0.0...
	point				origin; //!<this is that 0.0.
};

}

#endif
