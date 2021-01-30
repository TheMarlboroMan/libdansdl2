#include <ldv/line_representation.h>

using namespace ldv;

line_representation::line_representation(point p1, point p2, rgba_color c)
	:primitive_representation(c),
	origin{p1},
	base_view_position{0,0,0,0}
{
	set_points(p1, p2);
	update_base_view_position();
}

line_representation::line_representation(point p1, point p2, rgb_color c)
	:primitive_representation(c),
	origin{p1},
	base_view_position{0,0,0,0}
{
	set_points(p1, p2);
	update_base_view_position();
}

//!Reassigns the points.

void line_representation::set_points(point p1, point p2)
{
	//Save the origin...
	origin=p1;
	points[0]={p1.x-origin.x, p1.y-origin.y};
	points[1]={p2.x-origin.x, p2.y-origin.y};
	update_base_view_position();
}

//!Gets the base view position.

void line_representation::update_base_view_position()
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

	base_view_position={x, y, (unsigned int)w, (unsigned int)h};
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
	update_base_view_position();
}

