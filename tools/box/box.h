#ifndef LIBDANSDL_2_BOX_H
#define LIBDANSDL_2_BOX_H

#include <SDL2/SDL.h>
#include "../tools/tools.h"
#include "../point_2d/point_2d.h"

namespace ldt
{

/*Una caja para no estar siempre comprometidos con la representación SDL de 
la misma. Incluye además algunos métodos típicos de colisión y cosas así.*/

template<typename T, typename U>
class box
{
	public:

	point_2d<T> 	origin;
	U 		w, h;

	box(SDL_Rect r):
		origin{r.x,r.y}, w(r.w), h(r.h)
	{}

	box():
		origin{0,0}, w(0), h(0)
	{}

	box(const point_2d<T>& p, U pw, U ph)
		:origin(p), w(pw), h(ph)
	{}

	box(T px, T py, U pw, U ph)
		:origin(px, py), w(pw), h(ph)
	{}

	box(const box& c)
		:origin(c.origin), w(c.w), h(c.h)
	{}

	box& operator=(const box& c)
	{
		origin=c.origin;
		w=c.w;
		h=c.h;
		return *this;
	}

	bool 			collides_with(const box& c, bool unit_is_collision=false) const 
	{
		return ldt::rects_overlap(origin.x, origin.y, w, h, c.origin.x, c.origin.y, c.w, c.h, unit_is_collision);
	}

	bool 			collides_with(const box& c, box& r, bool unit_is_collision=false) const 
	{
		return ldt::rects_overlap(origin.x, origin.y, w, h, c.origin.x, c.origin.y, c.w, c.h, r.origin.x, r.origin.y, r.w, r.h, unit_is_collision);
	}

	bool 			point_inside(const point_2d<T>& p) const 
	{
		return ldt::point_in_box(origin.x, origin.y, w, h, p.x, p.y);
	}

	bool 			point_inside(T x, T y) const 
	{
		return ldt::point_in_box(origin.x, origin.y, w, h, x, y);
	}

	bool 			inside(const box& gran) const 
	{
		return ldt::box_in_box(origin.x, origin.y, w, h, gran.origin.x, gran.origin.y, gran.w, gran.h);
	}

	bool 			contains(const box& peq) const 
	{
		return ldt::box_in_box(peq.origin.x, peq.origin.y, peq.w, peq.h, origin.x, origin.y, w, h);
	}
};

}
#endif
