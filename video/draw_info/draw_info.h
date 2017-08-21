#ifndef LIBDANSDL2_DRAW_INFO_H
#define LIBDANSDL2_DRAW_INFO_H

namespace ldv
{

//!A convenience structure used by the camera.
/**
	pos_x - pos_y: screen position of the camera.
	rel_x - rel_y: world position of the camera (focus box position, in camera terms).
	view_w - view_h: width and height of the focus box (zoom applied).
	zoom: zoom value.

Application classes are discouraget to get information from here, instead, 
requesting data from the camera object is the way to go.
*/

struct draw_info
{
	int 	pos_x, pos_y,
		rel_x, rel_y,
		view_w, view_h;
	double	zoom;
};
}

#endif
