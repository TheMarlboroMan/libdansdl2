#pragma once

#include "box.h"
#include "point_2d.h"
#include "polygon_2d.h"
#include <ldv/polygon_representation.h>
#include <ldv/rect.h> //ldv::point

#include <algorithm>
/**
This is a set of utilities to convert back and forth between types: polygons to
boxes and spatial data (polygons, boxes) to representations.
*/

namespace ldt {

//!Gets the axis aligned bounding box for the given polygon.

//!The axis aligned box templated type will have both its types as the same
//!templated type of the original polygon.
template<typename T>
box<T, T> box_from_poly(const polygon_2d<T>& _p) {

	std::vector<T> xs, ys;
	for(auto p : _p.get_vertices()) {
		xs.push_back(p.x);
		ys.push_back(p.y);
	}

	std::sort(std::begin(xs), std::end(xs));
	std::sort(std::begin(ys), std::end(ys));

	return box<T, T>{xs.front(), ys.front(), xs.back()-xs.front(), ys.back()-ys.front()};
}

//!Creates a clockwise winded polygon from a box.
template<typename T, typename U>
polygon_2d<T> poly_from_box(const box<T, U>& _b) {

	T 	endx=_b.origin.x+static_cast<T>(_b.w),
		endy=_b.origin.y+static_cast<T>(_b.h);

	const std::vector<typename polygon_2d<T>::tpoint> vertices={ 
		{_b.origin.x, 		_b.origin.y}, 
		{endx, 			_b.origin.y},
		{endx, 			endy},
		{_b.origin.x, 		endy}
	};

	return polygon_2d<T>{vertices};
}

//!Creates a polygon representation from a given primitive, with the given color. Polygon version.

//!If the polygon is subject to any rotation, the representation will be yielded
//!as having zero rotation, but the shape of the rotated poly.
template<typename T>
ldv::polygon_representation representation_from_primitive(const polygon_2d<T>& _p, const ldv::rgba_color& _c, ldv::polygon_representation::type _t=ldv::polygon_representation::type::fill) {


	const auto& v=_p.get_vertices();
	std::vector<ldv::point> points{v.size()};
	std::transform(std::begin(v), std::end(v), std::begin(points), [](const ldt::point_2d<T>& pt) {
		return ldv::point(pt.x, pt.y);
	});

	return ldv::polygon_representation(_t, points, _c);
}

//!Creates a polygon representation from a given primitive, with the given color. Box version.

//!If the box is subject to any rotation, the representation will be yielded
//!as having zero rotation, but the shape of the rotated box.
template<typename T, typename U>
ldv::polygon_representation representation_from_primitive(const box<T, U>& _b, const ldv::rgba_color& _c, ldv::polygon_representation::type _t=ldv::polygon_representation::type::fill) {

	T 	endx=_b.origin.x+static_cast<T>(_b.w),
		endy=_b.origin.y+static_cast<T>(_b.h);

	std::vector<ldv::point> points{4};
	points[0]=ldv::point(_b.origin.x,	_b.origin.y);
	points[1]=ldv::point(endx,		_b.origin.y);
	points[2]=ldv::point(endx,		endy);
	points[3]=ldv::point(_b.origin.x,	endy);

	return ldv::polygon_representation(_t, points, _c);
}

//!Gets a vector of representation points from a vector of points T such as 
//!the ones found in 2d polygons.
template<typename T>
std::vector<ldv::point> vector_of_representation_points_from_vertices(const std::vector<T>& _v) {

	std::vector<ldv::point> result;
	result.reserve(_v.size());
	for(const auto& v : _v) {
		result.push_back(ldv::point(v.x, v.y));
	}
	return result;
}

//!Converts any point to the integer specialization of the point.
template<typename T>
ldv::point		to_video(const point_2d<T> _pt) {

	return {(int)_pt.x, (int)_pt.y};
}

//!Converts any point to the integer specialization of the point, inverts the
//!y axis.
template<typename T>
ldv::point		to_screen(const point_2d<T> _pt) {

	return {(int)_pt.x, -(int)_pt.y};
}

} //End of namespace.

