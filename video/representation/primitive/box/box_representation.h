#ifndef LIBDANSDL2_BOX_REPRESENTATION_H
#define LIBDANSDL2_BOX_REPRESENTATION_H

#include "../polygon/polygon_representation.h"

//Simplemente un caso especial de un polígono.

namespace ldv
{
class box_representation
	:public polygon_representation
{
	public:
	
				box_representation(polygon_representation::type, const rect& p_pos, const rgba_color&);
				box_representation(polygon_representation::type, const rect& p_pos, const rgb_color&);
				box_representation(const box_representation& p_otra);
				box_representation& operator=(const box_representation& p_otro);
	virtual 		~box_representation() {}

	void			set_dimensions(int, int);
	void			set_location(const rect&);
};

}

#endif
