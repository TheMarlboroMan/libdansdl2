#include <ldv/box_representation.h>

using namespace ldv;

box_representation::box_representation(const rect& p_pos, const rgba_color& c, type t)
	:polygon_representation({
		{p_pos.origin.x, p_pos.origin.y},
		{p_pos.origin.x+(int)p_pos.w, p_pos.origin.y},
		{p_pos.origin.x+(int)p_pos.w, p_pos.origin.y+(int)p_pos.h},
		{p_pos.origin.x, p_pos.origin.y+(int)p_pos.h}},
		c, t)
{

}

box_representation::box_representation(const rect& p_pos, const rgb_color& c, type t)
	:polygon_representation({
		{p_pos.origin.x, p_pos.origin.y},
		{p_pos.origin.x+(int)p_pos.w, p_pos.origin.y},
		{p_pos.origin.x+(int)p_pos.w, p_pos.origin.y+(int)p_pos.h},
		{p_pos.origin.x, p_pos.origin.y+(int)p_pos.h}},
		c, t)
{

}

void box_representation::set_dimensions(int w, int h)
{
	int 	nx=points[0].x+w,
		ny=points[0].y+h;

	points[1].x=nx;
	points[2].x=nx;
	points[2].y=ny;
	points[3].y=ny;

	update_view_position();
}

void box_representation::set_location(const rect& r)
{
	points.clear();

	origin={r.origin.x, r.origin.y};

	int 	fx=r.origin.x+r.w,
		fy=r.origin.y+r.h;

	points.push_back({r.origin.x, r.origin.y});
	points.push_back({fx, r.origin.y});
	points.push_back({fx, fy});
	points.push_back({r.origin.x, fy});

	normalize();
	update_view_position();
}
