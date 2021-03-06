#pragma once

#include "vector_2d.h"
#include "polygon_2d.h"
#include "segment_2d.h"
#include "polygon_projection_2d.h"

namespace ldt {

//!SAT collision response with minimum translation vector.

template<typename T>
struct SAT_mtv_result {
	//!Class constructor.
			SAT_mtv_result():
		collision(false), mtv{0., 0.} {
	}

	bool 			collision;		//!< Indicates whether a collision has taken place.
	ldt::vector_2d<T>	mtv;			//!< Minimum translation vector to solve the collision.
};

//!Checks if there is a SAT collision check between two polygons.

template<typename T>
bool SAT_collision_check(const polygon_2d<T>& a,const polygon_2d<T>& b) {

	auto f=[](const polygon_2d<T>& pa, const polygon_2d<T>& pb) {

		const auto& vertices=pa.get_vertices();
		size_t size=vertices.size();

		for(size_t i=0; i<size; i++) {

			vector_2d<T> s=i==size-1
				? vector_from_points(vertices[i], vertices[0])
				: vector_from_points(vertices[i], vertices[i+1]);

			auto axis=s.left_normal(); //Vector normal...
			auto 	proy_a=project(pa, axis),
					proy_b=project(pb, axis); 	//Projections in the normal...

			if(!is_projection_overlap(proy_a, proy_b)) {
				return false;
			}
		}
		return true;
	};

	if(!f(b, a)) return false;
	else if(!f(a, b)) return false;
	else return true;
}

//!Calculates SAT with the minumum translation vector.

template<typename T>
SAT_mtv_result<T> SAT_collision_check_mtv(const polygon_2d<T>& _polygon_a,const polygon_2d<T>& _polygon_b) {

	SAT_mtv_result<T> res;
	T current_magnitude=0.;

	//Get all axes...
	std::vector<vector_2d<T> > all_axes;
	auto get_axes=[&all_axes](const polygon_2d<T>& _polygon) {

		const auto& vertices=_polygon.get_vertices();
		size_t size=vertices.size();
		for(size_t i=0; i<size; i++) {
			vector_2d<T> s=i==size-1
				? vector_from_points(vertices[i], vertices[0])
				: vector_from_points(vertices[i], vertices[i+1]);

			all_axes.push_back(s.left_normal());
		};
	};

	get_axes(_polygon_a);
	get_axes(_polygon_b);

	//Remove paralell axes.
	for(auto it=std::begin(all_axes); it!=std::end(all_axes); ++it) {
		for(auto it_next=it+1; it_next!=std::end(all_axes); ++it_next) {
			if(!determinant(*it, *it_next)) {
				it_next=all_axes.erase(it);
			}
		}
	}

	//Now for the real checks...
	for(const auto& axis: all_axes) {

		auto	proy_a=project(_polygon_a, axis),
				proy_b=project(_polygon_b, axis); 	//Projections in the normal...

		if(!is_projection_overlap(proy_a, proy_b)) {

			res.collision=false;
			break;
		}
		else {

			T overlap=get_projection_overlap(proy_a, proy_b);

			if(!res.collision || overlap < current_magnitude) {
				current_magnitude=overlap;
				res.mtv=ldt::vector_from_angle_and_magnitude(axis.get_angle_deg(), overlap);
				res.collision=true;
			}
		}
	}

	if(res.collision) {

		//Check the MTV actually points where it is supposed to point (having a
		//move away from b).
		//The theory behind this: if the dot product of two vectors is positive
		//they are pointing more or less at the same direction.
		auto pointing=vector_from_points(_polygon_a.get_centroid(), _polygon_b.get_centroid());
		if(dot_product(res.mtv, pointing) >= 0.) {
			res.mtv*=-1.;
		}
	}

	return res;
}

//!Will return the edge in _poly for which the collision tested.
template<typename T>
segment_2d<T> get_SAT_edge(const SAT_mtv_result<T>& _sat_result, const polygon_2d<T>& _poly) {

	if(!_sat_result.collision) {
		throw std::runtime_error("get_SAT_edge must be fed with a SAT_mtv_result that tests true for a collision");
	}

	//Find the furthest vertex from the normal, the normal being the  MTV.
	const auto& vertices=_poly.get_vertices();

	auto get_dist=[&_sat_result, &vertices](size_t _index) {

		return dot_product(_sat_result.mtv, {vertices[_index].x, vertices[_index].y });
	};

	size_t index=0;
	T max=get_dist(index);

	for(size_t i=1; i < vertices.size(); i++) {

		T proj=get_dist(i);
		if(proj > max) {
			max=proj;
			index=i;
		}
	}

	//TODO: www.dyn4j.org/2011/11/contact-points-using-clipping/#cpg-find has
	//some examples. Perhaps we could use them to check our values.

	//Use the most perpendicular vertex to the normal to get the edge. Might be
	//the next vertex or the previous one (relative to the furthest one we
	//already discovered). We use clockwise winding.
	const auto& vertex=vertices[index];
	const auto& left=index==0 ? vertices.back() : vertices[index-1];
	const auto& right=index==vertices.size() -1 ? vertices[0] : vertices[index+1];

	//With these two, compose a vector
	auto left_vector=vector_from_points(vertex, left).normalize();
	auto right_vector=vector_from_points(vertex, right).normalize();

	//Look for the most perpendicular to the normal... The one closest to zero.
	//Interestingly, this segment MIGHT NOT be the same as one in the
	//polygon, due to winding stuffs. Check how these are declared in real
	//cases.

	//TODO: Too verbose... I only I knew what I was doing...

	segment_2d<T> model=dot_product(right_vector, _sat_result.mtv) <= dot_product(left_vector, _sat_result.mtv)
		? segment_2d<T>{vertex, left}
		: segment_2d<T>{vertex, right};

	segment_2d<T> inverted{model.end(), model.point};

	//Now, polygons are made out of vertices. There MUST be one vertex that
	//is equal to "model.point" or "inverted.point" and whose next vertex
	//is "model.end()" or "inverted.end()". This way we return the segment as
	//the original winding was declared.

	size_t last=vertices.size()-1;
	for(size_t i=0; i <= last; i++) {

		size_t next=i==last ? 0 : i+1;
		segment_2d<T> current{vertices[i], vertices[next]};

		if(current==model || current==inverted) {
			return current;
		}
	}

	throw std::runtime_error("get_SAT_edge could not find matching edge");
}

/*
//!Checks an intersection between a segment and a polygon.

//TODO: Move to another library.
template<typename T>
bool intersection_segment_polygon(const segment_2d<T>& s, const polygon_2d<T> p) {

	//TODO: This segs thing does not exist anymore!!

	const auto& segs=p.get_segments();

	for(const auto& sg : segs)
	{
		if(segments_intersect(s, sg)) return true;
	}

	return false;
}
*/

//TODO: Move to another library.
//!Checks if the point is inside the polygon.

//!Adapted from de https://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
template<typename T>
bool point_in_polygon(const polygon_2d<T>& p, const point_2d<T>& pt)
{
	return point_in_polygon(p.get_vertices(), pt);
}

template<typename T>
bool point_in_polygon(
	const std::vector<point_2d<T>>& vertices,
	const point_2d<T>& pt
) {
	bool res=false;
	T testx=pt.x, testy=pt.y;
	size_t i=0, j=0, nvert=vertices.size();

	for(i = 0, j = nvert-1; i < nvert; j = i++) {
		if(
			((vertices[i].y > testy) != (vertices[j].y > testy)) &&
			(testx < ( vertices[j].x-vertices[i].x) * (testy - vertices[i].y) / (vertices[j].y-vertices[i].y) + vertices[i].x) ) {
			res=!res;
		}
	}
	return res;
}


} //End of namespace

