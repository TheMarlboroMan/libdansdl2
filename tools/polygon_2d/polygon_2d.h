#ifndef LIBDANSDL2_POLIGON_2D_H
#define LIBDANSDL2_POLIGON_2D_H

#include <vector>
#include <stdexcept>
#include <array>
#include <cassert>

#include "../point_2d/point_2d.h"
#include "../vector_2d/vector_2d.h"

namespace ldt
{

//!Simple 2d polygon defined from vertices. Clockwise winding.

//!A polygon is defined by a rotation point and a vector of point (vertices). 
//!Many additional function templates are defined in the same header, 
//!providing extended functionality.

//TODO: Perhaps we could add a "scale" method.

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
	bool				is_concave() const {return ldt::is_concave(vertices);}
	//!Checks if the polygon is winded clockwise.
	bool				is_clockwise() const {return ldt::is_clockwise(vertices);}
	//!Returns the amount of vertices.
	size_t				size() const {return vertices.size();}

	//!Moves the polygon by the units specified in the point.
	virtual void			move(tpoint v)
	{
		centroid+=v;
		rotation_center+=v;
		for(auto &p : vertices) p+=v;
	}

	//!Moves the polygon so the centroid rests in the specified point.
	virtual void			center_in(tpoint v) {

		vector_2d<T> vec=vector_from_points(this->centroid, tpoint(v.x, v.y));
		tpoint factor={vec.x, vec.y};
		for(auto &p : vertices) p+=factor;
		rotation_center+=factor;
		centroid=v;
	}

	//!Moves the polygon to the rotation center rests in the specified point.
	virtual void			rotation_center_in(tpoint v) {

		vector_2d<T> vec=vector_from_points(rotation_center, tpoint(v.x, v.y));
		tpoint factor={vec.x, vec.y};
		for(auto &p : vertices) p+=factor;
		rotation_center=v;
		centroid+=v;
	}


	//!Rotates the polygon around its rotation_center.
	virtual void			rotate(T degrees)
	{
		for(auto &p : vertices) p.rotate(degrees, rotation_center);
		centroid.rotate(degrees, rotation_center);
	}

	//!Rotates the polygon around an arbitrary point.
	virtual void			rotate(T degrees, tpoint pt)
	{
		for(auto &p : vertices) p.rotate(degrees, pt);
		centroid.rotate(degrees, pt);
		rotation_center.rotate(degrees, pt);
	}

	//!Adds a new vertex.
	virtual void			add_vertex(const tpoint& p)
	{
		if(!size()) rotation_center=p;
		vertices.push_back(p);
		calculate_centroid();
	}

	//!Returns the rotation center point.
	const tpoint&			get_rotation_center() const {return rotation_center;}
	//!Returns the centroid.
	const tpoint&			get_centroid() const {return centroid;}
	//!Returns all vertices... 
	const std::vector<tpoint>&	get_vertices() const {return vertices;}
	//!Returns the vertex at the index. May throw.
	const tpoint&			get_vertex(size_t v) const {return vertices.at(v);}
	//!Returns the vertex at the index. May throw.
	tpoint				get_vertex(size_t v) {return vertices[v];}
	//!Returns a reference to the vertex at the index. May throw.
	tpoint&				ref_vertex(size_t v) {return vertices[v];}
	//!Sets the rotation center.
	void				set_rotation_center(const tpoint& p) {rotation_center=p;}

	//!Checks polygon equality (in both vertices and centers).
	bool				operator==(const polygon_2d<T>& p) const
	{
		return rotation_center==p.rotation_center 
			&& centroid==p.centroid 
			&& vertices==p.vertices;
	}

	protected:

	//!Calculates the centroid (median center point) of the polygon.

	//!Centroids can be calculated, so they are radically different from 
	//!a rotation center.
	void				calculate_centroid()
	{
		//https://stackoverflow.com/questions/2792443/finding-the-centroid-of-a-polygon#

		if(size() >= 3)
		{
			T signedArea=0.0;
			T x0=0.0, y0=0.0; // Current vertex X and Y.
			T x1=0.0, y1=0.0; // Next vertex X and Y.
			T a = 0.0;  // Partial signed area

			// For all vertices
			size_t i=0;
			for (i=0; i<vertices.size(); ++i)
			{
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
	}

	std::vector<tpoint>		vertices;		//!< Internal vertex data.
	tpoint				centroid;		//!< Centroid (median center) for the polygon.
	tpoint				rotation_center;	//!< Rotation center for the polygon.
};


}

#endif
