#include "line_representation.h"

using namespace ldv;

line_representation::line_representation(point p1, point p2, rgba_color c)
	:primitive_representation(c)
{
	set_points(p1, p2);
	update_view_position();
}

line_representation::line_representation(point p1, point p2, rgb_color c)
	:primitive_representation(c)
{
	set_points(p1, p2);
	update_view_position();
}

line_representation::line_representation(const line_representation& o)
	:primitive_representation(o), points(o.points)
{

}

line_representation& line_representation::operator=(const line_representation& o)
{
	primitive_representation::operator=(o);
	points=o.points;
	return *this;
}

//!Reassigns the points.

void line_representation::set_points(point p1, point p2)
{
	points[0]={p1.x, p1.y};
	points[1]={p2.x, p2.y};
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

	f(points[0].x, points[1].x, x, w);
	f(points[0].y, points[1].y, y, h);

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

//!Makes the first point p, and moves the second point relative to it.

//!TODO: This is untested and written with a good headache.

void line_representation::go_to(point p)
{
	int 	difx=points[1].x-points[0].x,
		dify=points[1].y-points[0].y;

	points[0]=p;
	points[1]=p+point{difx, dify};
	update_view_position();
}

//!Retrieves the position.

//!The position is the top-left most point.

point line_representation::get_position() const
{
	int x, y;

	auto f=[](int v1, int v2, int& pos)
	{
		if(v1 < v2) pos=v1;
		else pos=v2;
	};

	f(points[0].x, points[1].x, x);
	f(points[0].y, points[1].y, y);

	return point{x, y};
}
