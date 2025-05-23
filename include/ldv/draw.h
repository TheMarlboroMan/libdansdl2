#pragma once

#include "screen.h"
#include "camera.h"

#include "../ldt/segment_2d.h"
#include "../ldt/point_2d.h"
#include "../ldt/box.h"

namespace ldv {

/**
*This is a class to jot down quick and dirty shapes for examples and so on...
*It happens to work with entities using double as point type. It will always
*use cartesian coordinates and system (left-bottom is 0,0, x and y grow to
*the right and top, unlike in typical raster modes.
*TODO: test without camera... which should switch to "screen mode"??? Or 
*   maybe that does not make any sense...
*/
class draw {

	using tpoint=ldt::point_2d<double>;
	using tray=ldt::segment_2d<double>;
	using tbox=ldt::box<double, int>;

	enum class modes {
		screen,
		cartesian
	};

	public:
	                draw(screen&, camera * =nullptr, modes=modes::cartesian);

	draw&           clear(const rgba_color&);
	draw&           update();
	draw&           box(const tbox&, const rgba_color&);
	draw&           box_bordered(const tbox&, const rgba_color&, const rgba_color&);
	draw&           box_outline(const tbox&, const rgba_color&);
	draw&           box(int, int, int, int, const rgba_color&);
	draw&           box_bordered(int, int, int, int, const rgba_color&, const rgba_color&);
	draw&           box_outline(int, int, int, int, const rgba_color&);
	draw&           ray(const tray&, const rgba_color&);
	draw&           ray(int, int, int, int, const rgba_color&);
	draw&           line(const tpoint&, const tpoint&, const rgba_color&);
	draw&           line(int, int, int, int, const rgba_color&);

	const rgba_color    black;
	const rgba_color    white;
	const rgba_color    red;
	const rgba_color    green;
	const rgba_color    blue;
	const rgba_color    orange;
	const rgba_color    pink;
	const rgba_color    gray;

	private:

	ldv::screen&         scr;
	ldv::camera *        cam{nullptr};
	modes                mode;


};
}
