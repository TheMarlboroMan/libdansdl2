#ifndef LIBDANSDL2_DRAW_INFO_H
#define LIBDANSDL2_DRAW_INFO_H

namespace ldv
{

struct draw_info
{
	int 	pos_x, pos_y,
		rel_x, rel_y,
		view_w, view_h;
	double	zoom;
};
}

#endif
