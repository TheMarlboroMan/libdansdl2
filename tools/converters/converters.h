#ifndef CONVERTERS_LIBDANSDL_H
#define CONVERTERS_LIBDANSDL_H

#include "../box/box.h"
#include "../polygon_2d/polygon_2d.h"

namespace ldt {

//!Gets the axis aligned bounding box for the given polygon.
template<typename T, typename U>
box<T, U> box_from_poly(const polygon_2d_vertexes<T>& _p) {

	std::vector<T> xs, ys;
	for(auto p : _p.get_vertexes()) {
		xs.push_back(p.x);
		ys.push_back(p.y);
	}

	std::sort(std::begin(xs), std::end(xs));
	std::sort(std::begin(ys), std::end(ys));

	return box<T, U>{xs.front(), ys.front(), xs.back(), ys.back()};
}

//!Creates a clockwise winded polygon from a box.
template<typename T, typename U>
polygon_2d<T> poly_from_box(const box<T, U>& _b) {

	const auto& origin=_b.origin;

	return polygon_2d<T>{ 
		{origin.x, origin.y}, 
		{origin.x+(T)_b.w, origin.y},
		{origin.x+(T)_b.w, origin.y+(T)_b.h},
		{origin.x, origin.y+(T)_b.h}
	};
}

}

#endif
