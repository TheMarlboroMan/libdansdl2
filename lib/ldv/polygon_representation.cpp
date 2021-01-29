#include <ldv/polygon_representation.h>

using namespace ldv;

polygon_representation::polygon_representation(const std::vector<point>& pt, rgba_color c, type t)
	:primitive_representation(c), points(pt), origin(pt[0]), filltype(t)
{
	normalize();
	update_view_position();
}

polygon_representation::polygon_representation(const std::vector<point>& pt, rgb_color c, type t)
	:primitive_representation(c), points(pt), origin(pt[0]), filltype(t)
{
	normalize();
	update_view_position();
}
//!Sets the points from a vector of points.

//!The vector does not need to be normalized. Data will be normalized upon
//!insertion.

void polygon_representation::set_points(const std::vector<point>& pt)
{
	origin=pt[0];
	points=pt;
	normalize();
	update_view_position();
}

//!Normalizes the points.

//!Sets the first point at 0.0 and the positions relative to it.

void polygon_representation::normalize()
{
	for(auto& pt : points)
	{
		pt.x-=origin.x;
		pt.y-=origin.y;
	}
}

//!Does the bulk of the drawing.

//!Directly uses openGL functions to get the work done.

void polygon_representation::do_draw()
{
	do_color();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, points.data());
	glDrawArrays(filltype==type::fill ? GL_POLYGON : GL_LINE_LOOP, 0, points.size());
	glDisableClientState(GL_VERTEX_ARRAY);
}

//!Gets the box formed by all points.

rect polygon_representation::get_base_view_position() const
{
	int x=points[0].x+origin.x, y=points[0].y+origin.y, maxx=x, maxy=y;

	for(auto p : points)
	{
		//Añadir el valor origin, porque estos points empiezan en 0.0.
		p.x+=origin.x;
		p.y+=origin.y;

		if(p.x < x) x=p.x;
		else if(p.x > maxx) maxx=p.x;

		if(p.y < y) y=p.y;
		else if(p.y > maxy) maxy=p.y;
	}

	return rect{x, y, (unsigned int)maxx-x, (unsigned int)maxy-y};
}

//!Moves the polygon.

void polygon_representation::go_to(point p)
{
	origin=p;
	update_view_position();
}

