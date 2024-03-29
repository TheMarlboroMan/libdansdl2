#pragma once

#include "point_2d.h"
#include "vector_2d.h"

#include <vector>
#include <stdexcept>
#include <array>
#include <cassert>
#include <ostream>

namespace ldt
{

template<typename T>
point_2d<T> calculate_centroid(const std::vector<point_2d<T>>& vertices);

//!Simple 2d polygon defined from vertices. Clockwise winding.

//!A polygon is defined by a rotation point and a vector of point (vertices).
//!Many additional function templates are defined in the same header,
//!providing extended functionality.

template<typename T>
class polygon_2d {
	public:

	//!< Defines the type for a vertex.
	typedef	point_2d<T>		tpoint;

	//!Constructs an empty polygon.
					polygon_2d() {

	}

	//!Constructs a polygon with the given points and rotation center.
					polygon_2d(const std::vector<tpoint>& pts, tpoint c)
		:vertices(pts), rotation_center(c) {
		calculate_centroid();
	}

	//!Constructs a polygon with the given points. By default the rotation center is set to the centroid.
					polygon_2d(const std::vector<tpoint>& pts)
		:vertices(pts)	{
		calculate_centroid();
		rotation_center=centroid;
	}

	//!Checks if the polygon is concave (making it unsuitable for SAT collision detection).
	bool				is_concave() const {
		return ldt::is_concave(vertices);
	}

	//!Checks if the polygon is winded clockwise.
	bool				is_clockwise() const {
		return ldt::is_clockwise(vertices);
	}

	//!Returns the amount of vertices.
	size_t				size() const {
		return vertices.size();
	}

	//!Moves the polygon by the units specified in the point.
	polygon_2d<T>&			move(tpoint v) {
		centroid+=v;
		rotation_center+=v;
		for(auto &p : vertices) p+=v;
		return *this;
	}

	//!Moves the polygon so the centroid rests in the specified point.
	polygon_2d<T>&			center_in(tpoint v) {

		vector_2d<T> vec=vector_from_points(this->centroid, tpoint(v.x, v.y));
		tpoint factor={vec.x, vec.y};
		for(auto &p : vertices) p+=factor;
		rotation_center+=factor;
		centroid=v;
		return *this;
	}

	//!Moves the polygon to the rotation center rests in the specified point.
	polygon_2d<T>&			rotation_center_in(tpoint v) {

		vector_2d<T> vec=vector_from_points(rotation_center, tpoint(v.x, v.y));
		tpoint factor={vec.x, vec.y};
		for(auto &p : vertices) p+=factor;
		rotation_center=v;
		centroid+=v;
		return *this;
	}


	//!Rotates the polygon around its rotation_center.
	polygon_2d<T>&			rotate(T degrees) {

		for(auto &p : vertices) {
			p.rotate(degrees, rotation_center);
		}
		centroid.rotate(degrees, rotation_center);
		return *this;
	}

	//!Rotates the polygon around an arbitrary point.
	polygon_2d<T>&			rotate(T degrees, tpoint pt) {

		for(auto &p : vertices) p.rotate(degrees, pt);
		centroid.rotate(degrees, pt);
		rotation_center.rotate(degrees, pt);
		return *this;
	}

	//!Adds a new vertex.
	polygon_2d<T>&			add_vertex(const tpoint& p) {
		if(!size()) rotation_center=p;
		vertices.push_back(p);
		calculate_centroid();
		return *this;
	}

	//!Sets the rotation center.
	polygon_2d<T>&				set_rotation_center(const tpoint& p) {
		rotation_center=p;
		return *this;
	}

	//!Scales the polygon from its rotation center by a factor of _factor.

	//!A value of 2 will double the polygon's size. A value of 0.5 will halve
	//!it. Will throw if called with negative values.
	polygon_2d<T>&			scale(double _factor) {

		if(_factor < 0.0) {
			throw std::runtime_error("polygon_2d::scale cannot be called with a negative factor");
		}

		//Pretty easy formula: normalize by substracting center, scale and then
		//add the center again...
		for(auto &v : vertices) {
			v=((v-rotation_center) * _factor) + rotation_center;
		}

		calculate_centroid();
		return *this;
	}

	//!Returns the rotation center point.
	const tpoint&			get_rotation_center() const {
		return rotation_center;
	}

	//!Returns the centroid.
	const tpoint&			get_centroid() const {
		return centroid;
	}

	//!Returns all vertices...
	const std::vector<tpoint>&	get_vertices() const {
		return vertices;
	}

	//!Returns the vertex at the index. May throw.
	const tpoint&			get_vertex(size_t v) const {
		return vertices.at(v);
	}

	//!Returns the vertex at the index. May segfault.
	tpoint				get_vertex(size_t v) {
		return vertices[v];
	}

	//!Returns a reference to the vertex at the index. May segfault.
	tpoint&				ref_vertex(size_t v) {
		return vertices[v];
	}

	//!Checks polygon equality (in both vertices and centers).
	bool				operator==(const polygon_2d<T>& p) const {
		return rotation_center==p.rotation_center
			&& centroid==p.centroid
			&& vertices==p.vertices;
	}

	protected:

	//!Calculates the centroid (median center point) of the polygon.

	//!Centroids can be calculated, so they are radically different from
	//!a rotation center.
	//!Thanks to https://stackoverflow.com/questions/2792443/finding-the-centroid-of-a-polygon#
	void				calculate_centroid() {

		centroid=ldt::calculate_centroid(vertices);
	}

	std::vector<tpoint>		vertices;		//!< Internal vertex data.
	tpoint				centroid;		//!< Centroid (median center) for the polygon.
	tpoint				rotation_center;	//!< Rotation center for the polygon.

	template<typename TA>
	friend std::ostream& operator<<(std::ostream&, const polygon_2d<TA>&);
};

template<typename T>
std::ostream& ldt::operator<<(
	std::ostream& _stream,
	const polygon_2d<T>& _polygon
) {

	_stream<<"ldt::polygon_2d[ vertices:[";

	for(const auto& pt : _polygon.vertices) {

		_stream<<pt<<", ";
	}

	_stream<<"], centroid["<<_polygon.centroid<<"], ";
	_stream<<"rotation_center["<<_polygon.rotation_center<<"]]";

	return _stream;
}

//!Calculates the centroid (median center point) of the polygon made up by the vertices.

//!Centroids can be calculated, so they are radically different from
//!a rotation center.
//!Thanks to https://stackoverflow.com/questions/2792443/finding-the-centroid-of-a-polygon#
template<typename T>
point_2d<T> calculate_centroid(const std::vector<point_2d<T>>& vertices) {

	point_2d<T> centroid{0,0};

	if(vertices.size() >= 3) {

		T signedArea=0.0;
		T x0=0.0, y0=0.0; // Current vertex X and Y.
		T x1=0.0, y1=0.0; // Next vertex X and Y.
		T a = 0.0;  // Partial signed area

		// For all vertices
		size_t i=0;
		for (i=0; i<vertices.size(); ++i) {
			x0 = vertices[i].x;
			y0 = vertices[i].y;
			x1 = vertices[(i+1) % vertices.size()].x;
			y1 = vertices[(i+1) % vertices.size()].y;
			a = x0*y1 - x1*y0;
			signedArea += a;
			centroid.x += (x0 + x1)*a;
			centroid.y += (y0 + y1)*a;
		}

		signedArea *= 0.5;
		centroid.x /= (6.0*signedArea);
		centroid.y /= (6.0*signedArea);
	}

	return centroid;
}

}
