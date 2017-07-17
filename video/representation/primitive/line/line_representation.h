#ifndef LIBDANSDL2_LINE_REPRESENTATION_H
#define LIBDANSDL2_LINE_REPRESENTATION_H

#include <array>
#include "../primitive_representation.h"

namespace ldv
{
class line_representation:
	public primitive_representation
{
	public:
	
					line_representation(int x1, int y1, int x2, int y2, rgba_color);
					line_representation(int x1, int y1, int x2, int y2, rgb_color);
					line_representation(const line_representation&);
	virtual 			~line_representation() {}
					line_representation& operator=(const line_representation&);

	virtual void 			go_to(int x, int y);
	virtual point			get_position() const;
	virtual rect			get_base_view_position() const;

	void 				set_points(int x1, int y1, int x2, int y2);

	protected:

	virtual void			do_draw();

	private:

	std::array<point, 2>		points;
	point				origin;
};

}

#endif
