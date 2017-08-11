#ifndef LIBDANSDL2_POLYGON_REPRESENTATION_H
#define LIBDANSDL2_POLYGON_REPRESENTATION_H

#include <vector>

#include "../primitive_representation.h"

namespace ldv
{

//!N-sided polygon.

//!Polygons are represented by a collection of integer points which are normalized, that
//!is, the first point is stored as 0.0 and the rest are relative to it.

class polygon_representation
	:public primitive_representation
{
	public:

	//!May be colour filled or be a line. Line width is not adjustable.
	enum class			type{fill, line};

					polygon_representation(type, const std::vector<point>&, rgba_color);
					polygon_representation(type, const std::vector<point>&, rgb_color);
					polygon_representation(const polygon_representation&);
					polygon_representation& operator=(const polygon_representation&);
	virtual 			~polygon_representation() {}

	virtual void 			go_to(point);
	virtual point			get_position() const;
	virtual rect			get_base_view_position() const;
	//!Sets the filltype.
	void				set_filltype(type t) {filltype=t;}
	void				set_points(const std::vector<point>&);

	protected:

	virtual void			do_draw();
	void				normalize();

	std::vector<point>		points;
	point				origin;	//Saves the first original point before normalizing.
	type				filltype;
};

}

#endif
