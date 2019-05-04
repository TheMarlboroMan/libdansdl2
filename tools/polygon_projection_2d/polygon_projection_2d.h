#ifndef LIBDANSDL2_POLYGON_PROJECTION_2D_H
#define LIBDANSDL2_POLYGON_PROJECTION_2D_H

#include "../polygon_2d/polygon_2d.h"

namespace ldt {

//!A polygon projection.

//!Defines as a range "min-max" when a polygon is projected against a particular
//!axis.

template<typename T>
struct polygon_projection_2d{
	T 	min, 	//!< Minimum range component.
		max;	//!< Maximum range component.
};

//!Evaluates if two projections overlap.

//!Defined in terms of segments_overlap, as seen in tools.h

template<typename T>
bool	is_projection_overlap(const polygon_projection_2d<T>& pa, const polygon_projection_2d<T>& pb, bool unit_is_collision=false)
{
	return ldt::segments_overlap(pa.min, pa.max, pb.min, pb.max, unit_is_collision);
}

//!Gets the overlap magnitude of two projections.

//!It assumes that overlap exists.

template<typename T>
T	get_projection_overlap(const polygon_projection_2d<T>& pa, const polygon_projection_2d<T>& pb)
{
	std::vector<T> v={pa.min, pa.max, pb.min, pb.max};
	std::sort(std::begin(v), std::end(v));
	return v[2]-v[1];
}


//!Gets the projection of the polygon against an axis, expressed as a vector.

template<typename T>
polygon_projection_2d<T> project(const polygon_2d<T>& _poly, const vector_2d<T>& _axis) {

		auto axis=_axis.normalize();
		const auto& vertices=_poly.get_vertices();

		const auto &vertex=vertices[0];
		T vmin=dot_product(vector_2d<T>{vertex.x, vertex.y}, axis), vmax=vmin;

		for(const auto& v : vertices)
		{
			if(v==*std::begin(vertices)) continue; //Saltamos la primera iteración....
			T pro=dot_product(vector_2d<T>{v.x, v.y}, axis);
			if(pro < vmin) vmin=pro; 
			if (pro > vmax) vmax=pro;
		}

		return polygon_projection_2d<T>{vmin, vmax};
}

}

#endif
