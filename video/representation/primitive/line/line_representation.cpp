#include "line_representation.h"

using namespace ldv;

line_representation::line_representation(point p1, point p2, rgba_color c)
	:primitive_representation(c), origin{p1}
{
	set_points(p1, p2);
	update_view_position();
}

line_representation::line_representation(point p1, point p2, rgb_color c)
	:primitive_representation(c), origin{p1}
{
	set_points(p1, p2);
	update_view_position();
}

line_representation::line_representation(const line_representation& o)
	:primitive_representation(o), points(o.points), origin(o.origin)
{

}

line_representation& line_representation::operator=(const line_representation& o)
{
	primitive_representation::operator=(o);
	points=o.points;
	origin=o.origin;
	return *this;
}

//!Reassigns the points.

void line_representation::set_points(point p1, point p2)
{
	//Save the origin...
	origin=p1;
	points[0]={p1.x-origin.x, p1.y-origin.y};
	points[1]={p2.x-origin.x, p2.y-origin.y};
	update_view_position();
}

//!Gets the base view position.

rect line_representation::get_base_view_position() const
{
	int x, y, w, h;

	auto f=[](int v1, int v2, int& pos, int& measure)
	{
		if(v1 < v2)
		{
			pos=v1;
			measure=abs(v2-v1);
		}
		else
		{
			pos=v2;
			measure=abs(v1-v2);
		}
	};

	f(points[0].x+origin.x, points[1].x+origin.x, x, w);
	f(points[0].y+origin.y, points[1].y+origin.y, y, h);

	return rect{x, y, (unsigned int)w, (unsigned int)h};
}

//!Does the drawing.

//!Directly invokes openGL routines.

void line_representation::do_draw()
{
	do_color();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, points.data());
	glDrawArrays(GL_LINES, 0, 2);
	glDisableClientState(GL_VERTEX_ARRAY);
}

//!Moves the representation.

//!Sets the origin.

void line_representation::go_to(point p)
{
	origin=p;
	update_view_position();
}

//!Retrieves the position.

//!The position is the origin (first point specified).

point line_representation::get_position() const
{
	return origin;
/*	int x, y;

	//TODO sooo..... what?.
	auto f=[](int v1, int v2, int& pos)
	{
		if(v1 < v2) pos=v1;
		else pos=v2;
	};

	f(points[0].x+origin.x, points[1].x+origin.x, x);
	f(points[0].y+origin.y, points[1].y+origin.y, y);

	return point{x, y};
*/
}
