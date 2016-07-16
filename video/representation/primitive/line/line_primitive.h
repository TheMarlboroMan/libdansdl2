#ifndef LIBDANSDL2_LINE_PRIMITIVE_H
#define LIBDANSDL2_LINE_PRIMITIVE_H

#include <array>
#include "../primitive_representation.h"

namespace ldv
{
class primitive_line:
	public primitive_representation
{
	public:
	
					primitive_line(int x1, int y1, int x2, int y2, rgba_color);
					primitive_line(const primitive_line&);
	virtual 			~primitive_line() {}
					primitive_line& operator=(const primitive_line&);

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
