#ifndef LIBDANSDL2_BOX_PRIMITIVE_H
#define LIBDANSDL2_BOX_PRIMITIVE_H

#include "../polygon/polygon_primitive.h"

//Simplemente un caso especial de un polígono.

namespace ldv
{
class box_primitive
	:public polygon_primitive
{
	public:
	
				box_primitive(polygon_primitive::type, const rect& p_pos, const rgba_color&);
				box_primitive(const box_primitive& p_otra);
				box_primitive& operator=(const box_primitive& p_otro);
	virtual 		~box_primitive() {}

	void			set_dimensions(int, int);
	void			set_location(const rect&);
};

}

#endif
