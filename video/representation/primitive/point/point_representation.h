#ifndef LIBDANSDL2_POINT_REPRESENTATION_H
#define LIBDANSDL2_POINT_REPRESENTATION_H

#include "../primitive_representation.h"
#include <vector>

namespace ldv
{
class point_representation:
	public primitive_representation
{
	public:
	
						point_representation(rgba_color);
						point_representation(rgb_color);
						point_representation(const std::vector<point>&, rgba_color);
						point_representation(const std::vector<point>&, rgb_color);
						point_representation(point, rgba_color);
						point_representation(point, rgb_color);
						point_representation(const point_representation&);
	virtual 				~point_representation() {}
						point_representation& operator=(const point_representation&);

	void 					insert(int x, int y) {internal_insert({x, y}, true);}
	void					insert(point p) {internal_insert(p, true);}
	void					insert(const std::vector<point>&);
	void					clear();

	virtual void 				go_to(int x, int y);
	virtual point				get_position() const;
	virtual rect				get_base_view_position() const;

	protected: 

	virtual void				do_draw();

	private:

	void					internal_insert(point, bool=true);

	std::vector<point>			points;
	point					origin;
};

}

#endif
