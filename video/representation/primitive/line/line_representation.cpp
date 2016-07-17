#include "line_representation.h"

using namespace ldv;

line_representation::line_representation(int px1, int py1, int px2, int py2, rgba_color c)
	:primitive_representation(c), origin{px1, py1}
{
	set_points(px1, py1, px2, py2);
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

void line_representation::set_points(int px1, int py1, int px2, int py2)
{
	//Guardar como 0.0...
	origin={px1, py1};
	points[0]={px1-origin.x, py1-origin.y};
	points[1]={px2-origin.x, py2-origin.y};
	update_view_position();
}

rect line_representation::get_base_view_position() const
{
	int x, y, w, h;

	auto f=[](int v1, int v2, int& pos, int& medida)
	{
		if(v1 < v2)
		{
			pos=v1;
			medida=abs(v2-v1);
		}
		else
		{
			pos=v2;
			medida=abs(v1-v2);
		}
	};

	f(points[0].x+origin.x, points[1].x+origin.x, x, w);
	f(points[0].y+origin.y, points[1].y+origin.y, y, h);

	return rect{x, y, (unsigned int)w, (unsigned int)h};
}

void line_representation::do_draw()
{
	do_color();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, points.data());
	glDrawArrays(GL_LINES, 0, 2);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void line_representation::go_to(int x, int y)
{
	origin.x=x;
	origin.y=y;
	update_view_position();
}

//El punto más arriba y más a la izquierda.
point line_representation::get_position() const
{
	int x, y;

	auto f=[](int v1, int v2, int& pos)
	{
		if(v1 < v2) pos=v1;
		else pos=v2;
	};

	f(points[0].x+origin.x, points[1].x+origin.x, x);
	f(points[0].y+origin.y, points[1].y+origin.y, y);

	return point{x, y};
}

