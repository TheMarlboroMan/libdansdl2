#ifndef LIBDANSDL2_POLIGON_2D_H
#define LIBDANSDL2_POLIGON_2D_H

#include <vector>
#include <stdexcept>
#include <array>
#include <cassert>

#include "../point_2d/point_2d.h"
#include "../vector_2d/vector_2d.h"
#include "../tools/tools.h"

namespace ldt
{

//Forwards...
template<typename T> bool is_concave(const std::vector<point_2d<T>>&);
template<typename T> bool is_clockwise(const std::vector<point_2d<T>>&);

//!Simple 2d polygon defined from vertexes. Clockwise winding. No collision capabilities.

//!A polygon is defined by a rotation point and a vector of point (vertexes). 
//!Many additional function templates are defined in the same header, 
//!providing extended functionality.

template<typename T>
class polygon_2d_vertexes {
	public:

	//!< Defines the type for a vertex.
	typedef	point_2d<T>		tpoint;

	//!Constructs an empty polygon.
					polygon_2d_vertexes() {
	
	}

	//!Constructs a polygon with the given points and rotation center.
					polygon_2d_vertexes(const std::vector<tpoint>& pts, tpoint c)
		:vertexes(pts), rotation_center(c) {
		calculate_centroid();
	}

	//!Constructs a polygon with the given points. By default the rotation center is set to the centroid.
					polygon_2d_vertexes(const std::vector<tpoint>& pts)
		:vertexes(pts)	{
		calculate_centroid();
		rotation_center=centroid;
	}

	//!Checks if the polygon is concave (making it unsuitable for SAT collision detection).
	bool				is_concave() const {return ldt::is_concave(vertexes);}
	//!Checks if the polygon is winded clockwise.
	bool				is_clockwise() const {return ldt::is_clockwise(vertexes);}
	//!Returns the amount of vertices.
	size_t				size() const {return vertexes.size();}

	//!Moves the polygon by the units specified in the point.
	virtual void			move(tpoint v)
	{
		centroid+=v;
		rotation_center+=v;
		for(auto &p : vertexes) p+=v;
	}

	//!Moves the polygon so the centroid rests in the specified point.
	virtual void			center_in(tpoint v) {

		vector_2d<T> vec=vector_from_points(this->centroid, tpoint(v.x, v.y));
		tpoint factor={vec.x, vec.y};
		for(auto &p : vertexes) p+=factor;
		rotation_center+=factor;
		centroid=v;
	}

	//!Moves the polygon to the rotation center rests in the specified point.
	virtual void			rotation_center_in(tpoint v) {

		vector_2d<T> vec=vector_from_points(rotation_center, tpoint(v.x, v.y));
		tpoint factor={vec.x, vec.y};
		for(auto &p : vertexes) p+=factor;
		rotation_center=v;
		centroid+=v;
	}


	//!Rotates the polygon around its rotation_center.
	virtual void			rotate(T degrees)
	{
		for(auto &p : vertexes) p.rotate(degrees, rotation_center);
		centroid.rotate(degrees, rotation_center);
	}

	//!Rotates the polygon around an arbitrary point.
	virtual void			rotate(T degrees, tpoint pt)
	{
		for(auto &p : vertexes) p.rotate(degrees, pt);
		centroid.rotate(degrees, pt);
		rotation_center.rotate(degrees, pt);
	}

	//!Adds a new vertex.
	virtual void			add_vertex(const tpoint& p)
	{
		if(!size()) rotation_center=p;
		vertexes.push_back(p);
		calculate_centroid();
	}

	//!Returns the rotation center point.
	const tpoint&			get_rotation_center() const {return rotation_center;}
	//!Returns the centroid.
	const tpoint&			get_centroid() const {return centroid;}
	//!Returns all vertices.
	const std::vector<tpoint>&	get_vertexes() const {return vertexes;}
	//!Returns all vertices... In correct English.
	const std::vector<tpoint>&	get_vertices() const {return vertexes;}
	//!Returns the vertex at the index. May throw.
	const tpoint&			get_vertex(size_t v) const {return vertexes.at(v);}
	//!Returns the vertex at the index. May throw.
	tpoint				get_vertex(size_t v) {return vertexes[v];}
	//!Returns a reference to the vertex at the index. May throw.
	tpoint&				ref_vertex(size_t v) {return vertexes[v];}
	//!Sets the rotation center.
	void				set_rotation_center(const tpoint& p) {rotation_center=p;}

	//!Checks polygon equality (in both vertexes and centers).
	bool				operator==(const polygon_2d_vertexes<T>& p) const
	{
		return rotation_center==p.rotation_center 
			&& centroid==p.centroid 
			&& vertexes==p.vertexes;
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
			for (i=0; i<vertexes.size(); ++i)
			{
				x0 = vertexes[i].x;
				y0 = vertexes[i].y;
				x1 = vertexes[(i+1) % vertexes.size()].x;
				y1 = vertexes[(i+1) % vertexes.size()].y;
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

	std::vector<tpoint>		vertexes;		//!< Internal vertex data.
	tpoint				centroid;		//!< Centroid (median center) for the polygon.
	tpoint				rotation_center;	//!< Rotation center for the polygon.
};

//!A segment is a couple of points joined by a bearing vector... 

//TODO: I think we don't really need this!.
//TODO: Pretty much sure we can use a point and a single vector to represent this.
template<typename T>
struct segment_2d
{
	

	//!Defines the point type.
	typedef	point_2d<T>		tpoint;
	tpoint					v1, 		//!< Starting vertex of the segment.
							v2;			//!< End vertex of the segment.
	vector_2d<T>			direction;	//!< Direction of the segment. Somewhat redundant given that we have vertexes... or is it the other way around?.

	//!Creates a segment from v1 to v2.
					segment_2d<T>(tpoint pv1, tpoint pv2)
		:v1(pv1), v2(pv2), direction(vector_from_points(v1, v2))
	{}

					segment_2d<T>()
		:v1(tpoint(0,0)), v2(tpoint(0,0)), direction(vector_2d<T>(0,0))
	{}

	//!Copy constructor.
					segment_2d<T>(const segment_2d<T>& o)
	:v1(o.v1), v2(o.v2), direction(o.direction)
	{}

	//!Moves the segment by p units.
	void				move(tpoint p)
	{
		v1+=p;
		v2+=p;
	}

	//!Checks for strict equality.
	bool				operator==(const segment_2d<T> s) const
	{
		return v1==s.v1 && v2==s.v2 && direction==s.direction;
	}
};

//Calculates the middle point of a segment...
template<typename T>
point_2d<T> segment_middle_point(const segment_2d<T>& s)
{
	//TODO: If segments are changed, must be changed a bit...

	return {	(s.v1.x+s.v2.x) / 2., 
			(s.v1.y+s.v2.y) / 2.};
}

//!A polygon projection.

//!Defines as a range "min-max" when a polygon is projected against a particular
//!axis.

template<typename T>
struct polygon_projection{
	T 	min, 	//!< Minimum range component.
		max;	//!< Maximum range component.
};

//!Evaluates if two projections overlap.

//!Defined in terms of segments_overlap.

template<typename T>
bool	is_projection_overlap(const polygon_projection<T>& pa, const polygon_projection<T>& pb, bool unit_is_collision=false)
{
	return ldt::segments_overlap(pa.min, pa.max, pb.min, pb.max, unit_is_collision);
}

//!Gets the overlap magnitude of two projections.

//!It assumes that overlap exists.

template<typename T>
T	get_projection_overlap(const polygon_projection<T>& pa, const polygon_projection<T>& pb)
{
	std::vector<T> v={pa.min, pa.max, pb.min, pb.max};
	std::sort(std::begin(v), std::end(v));
	return v[2]-v[1];
}

//Forwards...
template<typename T> class polygon_2d;
template<typename T> struct SAT_edge_result;
template<typename T> struct SAT_mtv_result;
template<typename T> bool SAT_collision_check(const polygon_2d<T>& a,const polygon_2d<T>& b);
template<typename T> SAT_mtv_result<T> SAT_collision_check_mtv(const polygon_2d<T>& a,const polygon_2d<T>& b);
template<typename T> SAT_edge_result<T> SAT_collision_check_edge(const polygon_2d<T>& a,const polygon_2d<T>& b);

//!Complex polygon with collision capabilities.

//!The complex polygon has center, vertices and also a set of segments connecting
//!each vertex pair. When all vertexes have been added, the polygon must be 
//!closed manually (method "close") creating a segment between the first and 
//!last vertex. There is no built-in protection against adding new vertices
//!after closing, but the behaviour of collisions is going to be way off.

//TODO: I think we can remove the segments...

template<typename T>
class polygon_2d:
	public polygon_2d_vertexes<T>
{
	public:

	//!Defines a vertex type.
	typedef	point_2d<T>		tpoint;

	//!Default constructor.
					polygon_2d()
		:polygon_2d_vertexes<T>()
	{}

	//!Construct from vertexes. Rotation center and centroid are the same.
					polygon_2d(const std::vector<tpoint>& pts)
		:polygon_2d_vertexes<T>(pts)
	{
		recreate_segments();
	}

	
	//!Construct from vertexes and sets rotation center.
					polygon_2d(const std::vector<tpoint>& pts, tpoint c)
		:polygon_2d_vertexes<T>(pts, c)
	{
		recreate_segments();
	}

	//!Moves the polygon by the value specified.

	virtual void			move(tpoint v)
	{
		polygon_2d_vertexes<T>::move(v);
		for(auto &s : segments) s.move(v);
	}

	//!Moves the polygon so the centroid rests in the value specified.
	virtual void			center_in(tpoint v)
	{
		vector_2d<T> mv=vector_from_points(this->centroid, v);
		polygon_2d_vertexes<T>::center_in(v);
		for(auto &s : segments) s.move({mv.x, mv.y});
	}

	//!Rotates the polygon around its rotation center.

	virtual void			rotate(T deg)
	{
		polygon_2d_vertexes<T>::rotate(deg);
		recreate_segments();
	}

	//!Rotates the polygon around an arbitrary point.

	virtual void			rotate(T deg, tpoint pt) {
		polygon_2d_vertexes<T>::rotate(deg, pt);
		recreate_segments();
	}

	//!Adds a vertex.

	virtual void			add_vertex(const tpoint& p)
	{
		polygon_2d_vertexes<T>::add_vertex(p);
		if(this->vertexes.size() > 1) 
			create_segment(this->vertexes[this->vertexes.size()-2], p);

	}

	//!Closes the polygon adding a segment between the last and first vertices. No new vertices should be added.

	void				close()
	{
		create_segment(this->vertexes[this->vertexes.size()-1], this->vertexes[0]);
	}

	//!Gets the projection of the polygon against an axis, expressed as a vector.

	polygon_projection<T>		project(vector_2d<T> axis) const
	{
		axis.normalize();

		//Precálculo...
		const auto &vertex=this->vertexes[0];
		T vmin=dot_product(vector_2d<T>{vertex.x, vertex.y}, axis), vmax=vmin;

		for(const auto& v : this->vertexes)
		{
			if(v==*std::begin(this->vertexes)) continue; //Saltamos la primera iteración....
			T pro=dot_product(vector_2d<T>{v.x, v.y}, axis);
			if(pro < vmin) vmin=pro; 
			if (pro > vmax) vmax=pro;
		}
		return polygon_projection<T>{vmin, vmax};
	}

	//!Gets all segments.

	const std::vector<segment_2d<T> >&	get_segments() const {return segments;}

	//!Checks polygon equality (in both vertexes and center).
	bool					operator==(const polygon_2d<T>& p) const
	{
		return polygon_2d_vertexes<T>::operator==(p) && segments==p.segments;
	}

	private:

	//Creates a segment.

	void				create_segment(tpoint p1, tpoint p2) {segments.push_back(segment_2d<T>{p1, p2});}

	//Refreshes all segments after a rotation.

	void				recreate_segments()
	{
		if(this->vertexes.size() < 3) throw std::runtime_error("Invalid polygon defined: less than three vertexes were created.");

		segments.clear();
		size_t i=0;
		while(i < this->vertexes.size()-1)
		{
			create_segment(this->vertexes[i], this->vertexes[i+1]);
			++i;
		}

		close();
	}

	std::vector<segment_2d<T> > segments;
	friend bool SAT_collision_check<T>(const polygon_2d<T>& a,const polygon_2d<T>& b);
	friend SAT_mtv_result<T> SAT_collision_check_mtv<T>(const polygon_2d<T>& a,const polygon_2d<T>& b);
	friend SAT_edge_result<T> SAT_collision_check_edge<T>(const polygon_2d<T>& a,const polygon_2d<T>& b);
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
			auto proy_a=pa.project(axis), proy_b=pb.project(axis); 	//Projections in the normal...
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

	//TODO: Remove paralell axes...
	//Two vectors are perpendicular if their determinant is zero.
//	for(const auto& v : all_axes) {
//		for(const auto& other : all_axes) {
//			if(other.x!=v.x && other.y!=v.y) {
//				bool paralell=determinant(v, other);
//				std::cout<<v.x<<","<<v.y<<" vs "<<other.x<<","<<other.y<<" => determinant: "<<paralell<<std::endl;
//			}
//		}
//	}


	//Now for the real checks...
	for(const auto& axis: all_axes) {

std::cout<<"axis "<<axis.x<<","<<axis.y<<" ";

		auto	proy_a=_polygon_a.project(axis), 
				proy_b=_polygon_b.project(axis); 	//Projections in the normal...

		if(!is_projection_overlap(proy_a, proy_b)) {

std::cout<<"CAN EXIT NOW"<<std::endl;
			res.collision=false;				
			break;
		}
		else {

			T overlap=get_projection_overlap(proy_a, proy_b);
			auto debug_vec=ldt::vector_from_angle_and_magnitude(axis.get_angle_deg(), overlap);

std::cout<<"overlap "<<overlap<<" ("<<current_magnitude
	<<") angle="<<axis.get_angle_deg()
	<<" mtv="<<debug_vec.x<<","<<debug_vec.y;

			if(!res.collision || overlap < current_magnitude) {

std::cout<<" <<< NEW >>";
				current_magnitude=overlap;
				res.mtv=ldt::vector_from_angle_and_magnitude(axis.get_angle_deg(), overlap);
				res.collision=true;
			}
std::cout<<std::endl;
		}
	}
	
	if(res.collision) {

//TODO: Check the MTV actually points where it is supposed to point (having a
//move away from b.

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

	//TODO: Debug results to hell... WTF does the whole normal thing even mean?

	//TODO: www.dyn4j.org/2011/11/contact-points-using-clipping/#cpg-find has
	//some examples. Perhaps we could use them to check our values.

	//Use the most perpendicular vertex to the normal to get the edge. Might be
	//the next vertex or the previous one (relative to the furthest one we 
	//already discovered). We use clockwise winding.
	const auto& vertex=vertices[index];
	const auto& left=index==0 ? vertices.back() : vertices[index-1];
	const auto& right=index==vertices.size() -1 ? vertices[0] : vertices[index+1];

	//TODO: We should check this... The order in which shit is given might 
	//change how it works.

	//With these two, compose a vector
	auto left_vector=vector_from_points(vertex, left).normalize();
	auto right_vector=vector_from_points(vertex, right).normalize();

	//TODO: Interestingly, this segment MIGHT NOT be the same as one in the
	//polygon, due to winding stuffs. Check how these are declared in real
	//cases.

	//Look for the most perpendicular to the normal... The one closest to zero.
	return dot_product(right_vector, _sat_result.mtv) <= dot_product(left_vector, _sat_result.mtv)
		? segment_2d<T>{vertex, left}
		: segment_2d<T>{vertex, right};
}


//!SAT collision response with collision edge

template<typename T> 
struct SAT_edge_result
{
	bool 			collision=false,		//!< Indicates whether there is a collision.
					edge_in_poly_a=false;		//!< Indicates whether this edge is colliding.
	segment_2d<T>		edge;				//!< Edge part of the collision.
};

//!Calculates SAT with a collision edge.

//!The edge_in_poly_a boolean serves to evaluate to which polygon the edge
//!belongs, helping choose the left or right normal for collision response.
//!Is is assumed that poly_a is the first parameter. Given that paralell edges
//!can give irregular results, all parallels are trimmed. This can be a
//!costly operation. TODO: Provide alternatives, like this function with edges
//!instead of polygons, so they can be cached if static.

//TODO: Actually, this can fuck shit up, we need a bit more of control, like
//specifying EXACTLY which polygon we want the collision to be in, or something,
//for not-so edge cases...

//TODO: This will surely be deleted.

template<typename T>
SAT_edge_result<T> SAT_collision_check_edge(const polygon_2d<T>& a,const polygon_2d<T>& b){

	//TODO: Remove the segments... please. Do that. Make this structure lighter.
	//TODO: are these segments really used?????
	assert(a.get_vertexes().size() == a.get_segments().size() && b.get_vertexes().size() == b.get_segments().size());

	SAT_edge_result<T> res;
	T current_magnitude=0.;
	
	auto f=[&res, &current_magnitude](const polygon_2d<T>& pa, const polygon_2d<T>& pb) {

		//If there are parallel segments we can have problems... a convex
		//polygon can have at most 2 parallel segments amongst themselves,
		//so we'll not test the one furthest from "pa"'s center.

		auto is_parallel=[](const segment_2d<T>& sa, const segment_2d<T>& sb) {

			auto na=sa.direction.get_normalized(), nb=sb.direction.get_normalized();
			if(na==nb) throw std::runtime_error("SAT_collision_check_edge: coplanar edges found!");
			auto v=na+nb;
			return !v.x && !v.y;
		}; 

		//Trimming parallels.
		std::vector<size_t> to_check;
		for(size_t i=0; i<pa.size()-1; ++i) {
			//See if we have to skip (we skip when we decide on a paralell we have yet to examine with "i".
			if(std::find(std::begin(to_check), std::end(to_check), i) != std::end(to_check)) continue;

			int added=-1;
			for(size_t j=i+1; j<pa.size(); ++j) {
				if(is_parallel(pa.segments[i], pa.segments[j])) {
					//TODO: For this to be real, we should measure the distance from a previous center, not the current centroid, but hey...
					//Distance to the nearest point of the segment...
					added=	abs(ldt::distance_between(pb.get_centroid(), pa.segments[i].v1, pa.segments[i].v2)) <
						abs(ldt::distance_between(pb.get_centroid(), pa.segments[j].v1, pa.segments[j].v2)) ?
						i : j;
					break;
				}
			}

			if(added==-1) added=i;
			to_check.push_back(added);
		}

		//We may need to add the last if we didn't skip it.
		if(std::find(std::begin(to_check), std::end(to_check), pa.size()-1)==std::end(to_check))
			to_check.push_back(pa.size()-1);

		//True SAT testing.
		for(const auto& index : to_check) {

			const auto& s=pa.segments[index];
			auto axis=s.direction.left_normal(); //Vector normal...
			auto proy_a=pa.project(axis), proy_b=pb.project(axis); 	//Projections in the normal...

			if(!is_projection_overlap(proy_a, proy_b)) {
				return false;
			}
			else {

				T overlap=get_projection_overlap(proy_a, proy_b);
				if(!res.collision || overlap < current_magnitude) {

					current_magnitude=overlap;
					res.edge=s;
					res.collision=true;
				}
			}
		}

		return true;
	};
	
	//A bit messy, but easy. Do b to a... early exit if no collision exists. 
	//Else we could assume that the edge is in b as long as there is a collision.
	if(!f(b, a)) return res;

	//Now do a to b. If the magnitude is lower, we can assume that a had a new edge.
	T prev_magnitude=current_magnitude;
	f(a, b);
	if(prev_magnitude > current_magnitude) res.edge_in_poly_a=true;
	return res;
}

//!Checks if two segments are intersecting.

//!Non intersecting segments must be non-paralell and prolong to the point where they cross.
//!Adapted from  de https://github.com/siebenschlaefer/line-segments-intersect/blob/included_line_segments/js/line-segments-intersect.js

template<typename T>
bool segments_intersect(const segment_2d<T>& a, const segment_2d<T>& b) {

	auto scalar_product=[](const point_2d<T>& pa, const point_2d<T>& pb) {

		return (pa.x * pb.y) - (pa.y * pb.x);
	};

	auto are_equal=[](bool pa, bool pb, bool pc, bool pd) {

		return pa==pb && pa==pc && pa==pd;
	};

	point_2d<T> r=a.v2 - a.v1;
	point_2d<T> s=b.v2 - b.v1;
	point_2d<T> bminusa=b.v1-a.v1;

	T uNumerator=scalar_product(bminusa, r);
	T denominator=scalar_product(r, s);

	//Part of the same line...
	if(uNumerator==0.0 && denominator==0.0) {

		//There is a coincidence in points...
		if(a.v1==b.v1 || a.v1==b.v2 || a.v2==b.v1 || a.v2==b.v2) {

			return true;
		}

		//Crossing check: are all points in the same bearing of the same sign?
		return !are_equal(
				(b.v1.x - a.v1.x < 0),
				(b.v1.x - a.v2.x < 0),
				(b.v2.x - a.v1.x < 0),
				(b.v2.x - a.v2.x < 0)) ||
			!are_equal(
				(b.v1.y - a.v1.y < 0),
				(b.v1.y - a.v2.y < 0),
				(b.v2.y - a.v1.y < 0),
				(b.v2.y - a.v2.y < 0));

	}

	//Paralells.
	if(denominator == 0) {

		return false;
	}

	point_2d<T> npt=b.v1-a.v1;
	T u=uNumerator / denominator;
	T t=scalar_product(npt, s) / denominator;

	return (t >= 0) && (t <= 1) && (u >= 0) && (u <= 1);
}

//!Checks an intersection between a segment and a polygon.

template<typename T>
bool intersection_segment_polygon(const segment_2d<T>& s, const polygon_2d<T> p) {

	//TODO: Reconstruct segments from vertices, way easier, I guess.

	const auto& segs=p.get_segments();

	for(const auto& sg : segs)
	{
		if(segments_intersect(s, sg)) return true;
	}

	return false;
}

//!Checks if the point is inside the polygon.

//!Adapted from de https://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
template<typename T>
bool point_in_polygon(const polygon_2d<T> p, const point_2d<T> pt)
{
	bool res=false;
	T testx=pt.x, testy=pt.y;
	const auto& vertexes=p.get_vertexes();
	size_t i=0, j=0, nvert=vertexes.size();

	for(i = 0, j = nvert-1; i < nvert; j = i++) 
	{
		if( 
			((vertexes[i].y > testy) != (vertexes[j].y > testy)) &&
			(testx < ( vertexes[j].x-vertexes[i].x) * (testy - vertexes[i].y) / (vertexes[j].y-vertexes[i].y) + vertexes[i].x) )
		{
			res=!res;
		}
	}
	return res;
}

//!Checks if a vector of vertices is concave.

template<typename T>
bool is_concave(const std::vector<point_2d<T>>& vertexes)
{
	if(vertexes.size() <= 3) return false;

	size_t i=0;
	while(i < vertexes.size())
	{
		auto ptc=vertexes.at(i),
			pt1=ptc, pt2=ptc;

		if(i==0)
		{
			pt1=vertexes.at(vertexes.size()-1);
			pt2=vertexes.at(i+1);
		}
		else if(i==vertexes.size()-1)
		{
			pt1=vertexes.at(i-1);
			pt2=vertexes.at(0);
		}
		else
		{
			pt1=vertexes.at(i-1);
			pt2=vertexes.at(i+1);
		}

		auto vector_1=vector_from_points(ptc, pt1);
		auto vector_2=vector_from_points(ptc, pt2);
		double dot=dot_product(vector_1, vector_2);
		double det=determinant(vector_1, vector_2);
		double angle=ldt::rad_to_deg(atan2(det, dot));

		angle=angle < 0.0 ? 360.0-angle : -angle;
		if(angle > 180.0) return true;
		++i;
	}

	return false;
}

//!Checks if a vector of vertices is winded clockwise.

//!Adapted from http://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order
//!point[0] = (5,0)
//!point[1] = (6,4)
//!point[2] = (4,5)
//!point[3] = (1,5)
//!point[4] = (1,0)
//!point[0] = (5,0)   edge[0]: (6-5)(4+0) =   4
//!point[1] = (6,4)   edge[1]: (4-6)(5+4) = -18
//!point[2] = (4,5)   edge[2]: (1-4)(5+5) = -30
//!point[3] = (1,5)   edge[3]: (1-1)(0+5) =   0
//!point[4] = (1,0)   edge[4]: (5-1)(0+0) =   0
//!                                         ---
//!                                         -44  counter-clockwise

template<typename T>
bool is_clockwise(const std::vector<point_2d<T>>& vertexes)
{
	size_t tam=vertexes.size();
	if(tam < 3) return false;

	int sum=0;
	for(size_t i=1; i<tam; ++i)
	{
		sum+=(vertexes[i].x-vertexes[i-1].x)*(vertexes[i].y+vertexes[i-1].y);
	}

	sum+=(vertexes[0].x-vertexes[tam-1].x)*(vertexes[0].y+vertexes[tam-1].y);
	return sum >= (T)0;
}

}

#endif
