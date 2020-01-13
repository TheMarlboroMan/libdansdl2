#pragma once

#include "point_2d.h"
#include "tools.h"

#include <SDL2/SDL.h>
namespace ldt
{

//!A box. 

//!The only thing that is enforced here is that height goes towards the positive
//!infinite in the Y axis. This means "down" in screen coordinates and "up"
//!in cartesian ones. If this rule is respected, its functions should work.

template<typename T, typename U>
class box
{
	public:

	point_2d<T> 	origin;		//!< Box x and y components.
	U 		w, 		//!< Box width.
			h;		//!< Box height.

	//!Creates a box from a SDL rect. After all, this whole library is an 
	//!SDL wrapper
	box(SDL_Rect r):
		origin{r.x,r.y}, w(r.w), h(r.h) {
	}

	//!Creates a box at 0,0, with no width or height.
	box():
		origin{0,0}, w(0), h(0) {
	}

	//!Creates a box from the point (x, y) and the given w and h components.
	box(const point_2d<T>& p, U pw, U ph)
		:origin(p), w(pw), h(ph) {
	}

	//!Creates a box from the given x, y, w and h components.
	box(T px, T py, U pw, U ph)
		:origin(px, py), w(pw), h(ph) {
	}

	//!Copy constructor.
	box(const box& c)
		:origin(c.origin), w(c.w), h(c.h) {
	}

	//!Assignment operator.
	box& operator=(const box& c) {
		origin=c.origin;
		w=c.w;
		h=c.h;
		return *this;
	}

	//!Returns true if this box collides with the given box  The second 
	//!parameter is legacy as it comes, governing whether a mere coincidence
	//!of edge values should be considered a collision.
	bool 			collides_with(const box& c, bool unit_is_collision=false) const {
		return ldt::rects_overlap(origin.x, origin.y, w, h, c.origin.x, c.origin.y, c.w, c.h, unit_is_collision);
	}

	//!Returns true if this box collides with the given box, returning the overlap in the second parameter
	//!passed by reference. The third parameter is legacy as it comes, governing whether a mere coincidence
	//!of edge values should be considered a collision.
	bool 			collides_with(const box& c, box& r, bool unit_is_collision=false) const {
		return ldt::rects_overlap(origin.x, origin.y, w, h, c.origin.x, c.origin.y, c.w, c.h, r.origin.x, r.origin.y, r.w, r.h, unit_is_collision);
	}

	//!Returns true if the given point is inside this box.
	bool 			point_inside(const point_2d<T>& p) const {
		return ldt::point_in_box(origin.x, origin.y, w, h, p.x, p.y);
	}

	//!Returns true if the given point is inside this box.
	bool 			point_inside(T x, T y) const {
		return ldt::point_in_box(origin.x, origin.y, w, h, x, y);
	}

	//!Returns true if this box is inside the given box (reverse of contains).
	bool 			inside(const box& gran) const {
		return ldt::box_in_box(origin.x, origin.y, w, h, gran.origin.x, gran.origin.y, gran.w, gran.h);
	}

	//!Returns true if this box contains the box indicated in the parameter (reverse of inside).
	bool 			contains(const box& peq) const {
		return ldt::box_in_box(peq.origin.x, peq.origin.y, peq.w, peq.h, origin.x, origin.y, w, h);
	}
};

}