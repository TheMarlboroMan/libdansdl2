#pragma once
#include "polygon_representation.h"

namespace ldv
{

//!A box representation.

//!Boxes are rectangles and are implemented in terms of polygon.

class box_representation
	:public polygon_representation
{
	public:

	//!Constructor from rectangle and alpha color.
				box_representation(const rect& p_pos, const rgba_color&, type=type::fill);

	//!Constructor from rectangle and color.
				box_representation(const rect& p_pos, const rgb_color&, type=type::fill);

	//!Sets the width and height of the box. These will extend to the right and down respectively.
	void			set_dimensions(int, int);

	//!Sets the box position and size.
	void			set_location(const rect&);
};

}
