#include "polygon_primitive.h"

using namespace ldv;

polygon_primitive::polygon_primitive(type t, const std::vector<point>& pt, rgba_color c)
	:primitive_representation(c), points(pt), origin(pt[0]), filltype(t)
{ 
	//Guardarlos de forma que el primero sea 0.0.
	normalize();

	update_view_position();
}

polygon_primitive::polygon_primitive(const polygon_primitive& o)
	:primitive_representation(o), points(o.points), origin(o.origin), filltype(o.filltype)
{

}

polygon_primitive& polygon_primitive::operator=(const polygon_primitive& o)
{
	primitive_representation::operator=(o);
	points=o.points;
	origin=o.origin;
	filltype=o.filltype;
	return *this;
}

void polygon_primitive::normalize()
{
	for(auto& pt : points)
	{
		pt.x-=origin.x;
		pt.y-=origin.y;
	}
}

void polygon_primitive::do_draw()
{
	do_color();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, points.data());
	glDrawArrays(filltype==type::fill ? GL_POLYGON : GL_LINE_LOOP, 0, points.size());
	glDisableClientState(GL_VERTEX_ARRAY);
}

rect polygon_primitive::calculate_view_position() const
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


void polygon_primitive::go_to(int x, int y)
{
	origin.x=x;
	origin.y=y;
	update_view_position();
}

//Position is the first point.
point polygon_primitive::get_position() const
{
	return point{origin.x, origin.y};
}
