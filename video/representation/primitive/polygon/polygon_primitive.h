#ifndef LIBDANSDL2_POLYGON_PRIMTIVE_H
#define LIBDANSDL2_POLYGON_PRIMTIVE_H

/*
*/

#include <vector>

#include "../primitive_representation.h"

namespace ldv
{
class polygon_primitive
	:public primitive_representation
{
	public:

	enum class			type{fill, line};

					polygon_primitive(type, const std::vector<point>&, rgba_color);
					polygon_primitive(const polygon_primitive&);
					polygon_primitive& operator=(const polygon_primitive&);
	virtual 			~polygon_primitive() {}

	virtual void 			go_to(int x, int y);
	virtual point			get_position() const;
	virtual rect			calculate_view_position() const;

	protected:

	virtual void			do_draw();
	void				normalize();

	std::vector<point>		points;
	point				origin;	//Guarda el primer point origin sin estar en 0.0.
	type				filltype;
};

}

#endif
