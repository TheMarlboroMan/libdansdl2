#pragma once

#include <ostream>

namespace ldv
{

//!A convenience structure used by the camera.
/**
	pos_x - pos_y: screen position of the camera.
	rel_x - rel_y: world position of the camera (focus box position, in camera terms).
	view_w - view_h: width and height of the focus box (zoom applied).
	zoom: zoom value.

Application classes are discouraged to get information from here, instead, 
requesting data from the camera object is the way to go.
*/

struct draw_info
{
	int 	pos_x, 	//!< Screen X position.
		pos_y,	//!< Screen Y position.
		rel_x, 	//!< World X position (focus box x position).
		rel_y,	//!< World Y position (focus box y position).
		view_w, //!< Width of the focus box (might change if zoom is applied).
		view_h; //!< Height of the focus box (might change if zoom is applied).
	double	zoom;	//!< Zoom ratio, 1 is 1:1 (same size), 2 is 1:2 (half size), etc.
};

std::ostream& operator<<(std::ostream&, const draw_info&);
}

