#ifndef LIBDANSDL2_BOX_REPRESENTATION_H
#define LIBDANSDL2_BOX_REPRESENTATION_H

#include "../polygon/polygon_representation.h"

namespace ldv
{

//!A box representation.

//!Boxes are rectangles and are implemented in terms of polygon.

class box_representation
	:public polygon_representation
{
	public:
	
	//!Constructor from rectangle and alpha color.
				box_representation(polygon_representation::type, const rect& p_pos, const rgba_color&);

	//!Constructor from rectangle and color.
				box_representation(polygon_representation::type, const rect& p_pos, const rgb_color&);

	//!Copy constructor.
				box_representation(const box_representation& p_otra);

	//!Assignment operator.
				box_representation& operator=(const box_representation& p_otro);

	//!Virtual class destructor.
	virtual 		~box_representation() {}

	//!Sets the width and height of the box. These will extend to the right and down respectively.
	void			set_dimensions(int, int);

	//!Sets the box position and size.
	void			set_location(const rect&);
};

}

#endif
