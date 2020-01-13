#pragma once

#include "primitive_representation.h"

#include <vector>

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

	//!Class constructor with alpha color.
					polygon_representation(const std::vector<point>&, rgba_color, type=type::fill);

	//!Class constructor with color.
					polygon_representation(const std::vector<point>&, rgb_color, type=type::fill);

	//!Copy constructor.
					polygon_representation(const polygon_representation&);

	//!Assignment operator. Texture is recreated as a different resource from the original.
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

	std::vector<point>		points;		//!<Internal vertex structure.
	point				origin;		//!< Saves the first original point before normalizing.
	type				filltype;	//!< Type of fille selected.I27

};

}
