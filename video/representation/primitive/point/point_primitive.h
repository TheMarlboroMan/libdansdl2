#ifndef LIBDANSDL2_POINT_PRIMITIVE_H
#define LIBDANSDL2_POINT_PRIMITIVE_H

#include "../primitive_representation.h"
#include <vector>

namespace ldv
{
class point_primitive:
	public primitive_representation
{
	public:
	
						point_primitive(rgba_color);
						point_primitive(const std::vector<point>&, rgba_color);
						point_primitive(point, rgba_color);
						point_primitive(const point_primitive&);
	virtual 				~point_primitive() {}
						point_primitive& operator=(const point_primitive&);

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
