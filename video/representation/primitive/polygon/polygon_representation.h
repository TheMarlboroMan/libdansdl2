#ifndef LIBDANSDL2_POLYGON_REPRESENTATION_H
#define LIBDANSDL2_POLYGON_REPRESENTATION_H

/*
*/

#include <vector>

#include "../primitive_representation.h"

namespace ldv
{
class polygon_representation
	:public primitive_representation
{
	public:

	enum class			type{fill, line};

					polygon_representation(type, const std::vector<point>&, rgba_color);
					polygon_representation(const polygon_representation&);
					polygon_representation& operator=(const polygon_representation&);
	virtual 			~polygon_representation() {}

	virtual void 			go_to(int x, int y);
	virtual point			get_position() const;
	virtual rect			get_base_view_position() const;
	void				set_filltype(type t) {filltype=t;}
	void				set_points(const std::vector<point>&);

	protected:

	virtual void			do_draw();
	void				normalize();

	std::vector<point>		points;
	point				origin;	//Guarda el primer point origin sin estar en 0.0.
	type				filltype;
};

}

#endif
