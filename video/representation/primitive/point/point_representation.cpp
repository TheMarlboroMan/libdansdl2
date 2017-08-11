#include "point_representation.h"
#include <algorithm>

using namespace ldv;

point_representation::point_representation(point p, rgba_color c)
	:primitive_representation(c), origin{0,0}
{
	internal_insert(p, false);
	update_view_position();
}

point_representation::point_representation(point p, rgb_color c)
	:primitive_representation(c), origin{0,0}
{
	internal_insert(p, false);
	update_view_position();
}

point_representation::point_representation(const std::vector<point>& pts, rgba_color c)
	:primitive_representation(c), origin{0,0}
{
	insert(pts);
//	update_view_position();
}

point_representation::point_representation(const std::vector<point>& pts, rgb_color c)
	:primitive_representation(c), origin{0,0}
{
	insert(pts);
//	update_view_position();
}

point_representation::point_representation(rgba_color c)
	:primitive_representation(c), origin{0,0}
{
	update_view_position();
}

point_representation::point_representation(rgb_color c)
	:primitive_representation(c), origin{0,0}
{
	update_view_position();
}

point_representation::point_representation(const point_representation& p_otra)
	:primitive_representation(p_otra), points(p_otra.points), origin(p_otra.origin)
{

}

point_representation& point_representation::operator=(const point_representation& p_otro)
{
	primitive_representation::operator=(p_otro);
	points=p_otro.points;
	origin=p_otro.origin;
	return *this;
}

//!Inserts a vector of points.

//!Current points are not erased.

void point_representation::insert(const std::vector<point>& pts)
{
	for(const auto& p : pts) internal_insert(p, false);
	update_view_position();
}

//!Internal insertion function.

//!The second parameters indicates if view position must be updated. If the
//!representation is empty, the first point is considered the new origin.

void point_representation::internal_insert(point p, bool update_state)
{
	if(!points.size()) origin=p;
	points.push_back({p.x-origin.x, p.y-origin.y});
	if(update_state) update_view_position();
}

//!Removes all points.

//!There is no way to remove a single point. Origin is reset to 0.0.

void point_representation::clear()
{
	points.clear();
	origin={0,0};
	update_view_position();
}

//!Gets the base view position.

rect point_representation::get_base_view_position() const
{
	if(!points.size())
	{
		return rect{0,0,0,0};
	}
	else
	{
		//Del manual: 
		//The value returned indicates whether the element passed as first argument is considered less than the second.
		struct {bool operator() (point a, point b) {return a.x < b.x;}}fx;
		struct {bool operator() (point a, point b) {return a.y < b.y;}}fy;

		auto min_x=*std::min_element(std::begin(points), std::end(points), fx);
		auto max_x=*std::max_element(std::begin(points), std::end(points), fx);
		auto min_y=*std::min_element(std::begin(points), std::end(points), fy);
		auto max_y=*std::max_element(std::begin(points), std::end(points), fy);

		rect res{min_x.x, min_y.y, (unsigned int)max_x.x-min_x.x, (unsigned int)max_y.y-min_y.y};
		res.origin.x+=origin.x;
		res.origin.y+=origin.y;
		return res;
	}
}

//!Does the real drawing.

void point_representation::do_draw()
{
	do_color();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, points.data());
	glDrawArrays(GL_POINTS, 0, points.size());
	glDisableClientState(GL_VERTEX_ARRAY);
}

//!Moves the representation.

//!There is no way to move a single point.
//!This function is likely buggy.
//TODO: This is likely wrong: origin is set, but the rest of the points remain in place.

void point_representation::go_to(point p)
{
	origin=p;
	update_view_position();
}

//!Returns the position.

//!Position is expressed as the top-left most point relative to the origin
//!That is, if origin is 10,10 and there is a point in -5,-5 this function
//!will return 5,5.
//!Will throw if the representation has no points.

point point_representation::get_position() const
{
	if(!points.size())
	{
		throw std::runtime_error("there are no points");
	}

	struct {bool operator() (point a, point b) {return a.x < b.x;}}fx;
	struct {bool operator() (point a, point b) {return a.y < b.y;}}fy;

	auto min_x=*std::min_element(std::begin(points), std::end(points), fx);
	auto min_y=*std::min_element(std::begin(points), std::end(points), fy);

	return point{min_x.x+origin.x, min_y.y+origin.y};
}
