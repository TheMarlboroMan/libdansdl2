#ifndef POINT_2D_LIBDANSDL_H
#define POINT_2D_LIBDANSDL_H

#include <cmath>
#include <vector>
#include "../tools/tools.h" //Por si acaso no hay M_PI...

namespace ldt
{

//!A two dimensional point.

template<typename T>
struct point_2d
{
	public:

	T 		x,	//!< X coordinate of the point.
			y;	//!< Y coordinate of the point.

	//!Default constructor for a point at 0,0.
	point_2d(): x(0), y(0) {}

	//!Class constructor.
	point_2d(T px, T py):x(px), y(py) {}

	//!Copy constructor.
	point_2d(const point_2d<T>& p):x(p.x), y(p.y) {}

	//!Assignment operator.
	point_2d& operator=(const point_2d<T>& p) {
		x=p.x;
		y=p.y;
		return *this;
	}

	//!Gets a new point by addition.
	point_2d operator+(const point_2d<T>& p) const {
		return point_2d<T> {x+p.x, y+p.y};
	}

	//!Adds to the point the components of the other point.
	point_2d& operator+=(const point_2d<T>& p) {
		x+=p.x;
		y+=p.y;
		return *this;
	}

	//!Gets a new point by substraction.
	point_2d operator-(const point_2d& p) const {
		return point_2d<T> {x-p.x, y-p.y};
	}

	//!Substracts from the point the components of the other point.
	point_2d& operator-=(const point_2d& p) {
		x-=p.x;
		y-=p.y;
		return *this;
	}

	//!Gets a new point by product.
	point_2d operator*(T p) const {
		return point_2d<T> {x*p, y*p};
	}

	//!Multiplies the point.
	point_2d& operator*=(T p) {
		x*=p;
		y*=p;
		return *this;
	}

	//!Gets a new point by division.
	point_2d operator/(T p) const {
		return point_2d<T> {x/p, y/p};
	}

	//!Divides the point.
	point_2d& operator/=(T p) {
		x/=p;
		y/=p;
		return *this;
	}

	//!Checks if two points are equal in value.

	bool operator==(const point_2d& p) const {
		return p.x==x && p.y==y;
	}

	//!Checks if two points are different in value.
	bool operator!=(const point_2d& p) const {
		return p.x!=x || p.y!=y;
	}

	//!Returns the distance to the point.

	T distance_to(const point_2d<T>& p) const {
		return distance_between(*this, p);
	}

	//!Rotates the point around a center. Negative rotations are clockwise.
	point_2d<T>& rotate(T grados, const point_2d<T> _center) {
		//Take to origin...
		T ox=x - _center.x;
		T oy=y - _center.y;

		//Precalculate...
		T rad=ldt::deg_to_rad(grados);
		T sr=sin(rad);
		T cr=cos(rad);

		//Rotate...
		T rx=(ox * cr) - (oy * sr);
		T ry=(oy * cr) + (ox * sr);

		//Move again...
		x = rx + _center.x;
		y = ry + _center.y;

		return *this;
	}

};

//!Returns the distance between two points.

template<typename T>
T distance_between(const point_2d<T>& p1, const point_2d<T>& p2) {

	T x=(p1.x-p2.x)*(p1.x-p2.x);
	T y=(p1.y-p2.y)*(p1.y-p2.y);
	return sqrt(x+y);
}

//!Returns the minimum distance between a point and a segment.

//!p is the point. The segment is s1 to s2.

//Only god know what this is and does ... https://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment/1501725.

template<typename T>
T distance_between(const point_2d<T>& pt, const point_2d<T>& s1, const point_2d<T>& s2) {

	auto dist=[](const point_2d<T>& v, const point_2d<T>& w) {
		return 	((v.x-w.x)*(v.x-w.x)) + ((v.y-w.y)*(v.y-w.y));
	};

	auto dist_to_segment_squared=[&dist](const point_2d<T>& p, const point_2d<T>& v, const point_2d<T>& w) {

		auto l2=dist(v, w);
		if(!l2) return dist(p, v);
		T t = ((p.x - v.x) * (w.x - v.x) + (p.y - v.y) * (w.y - v.y)) / l2;
		t = std::max((T)0, std::min((T)1, t));
		point_2d<T> pnear(v.x + t * (w.x - v.x), v.y + t * (w.y - v.y));
		return dist(p, pnear);
	};

	return std::sqrt(dist_to_segment_squared(pt, s1, s2));
}

//!Checks if a vector of vertices is concave.

//!Will return false if the polygon has less than 3 vertices.
template<typename T>
bool is_concave(const std::vector<point_2d<T>>& vertices) {

	if(vertices.size() <= 3) return false;

	size_t i=0;
	while(i < vertices.size()) {
		auto ptc=vertices.at(i),
			pt1=ptc, pt2=ptc;

		if(i==0) {
			pt1=vertices.at(vertices.size()-1);
			pt2=vertices.at(i+1);
		}
		else if(i==vertices.size()-1) {
			pt1=vertices.at(i-1);
			pt2=vertices.at(0);
		}
		else {
			pt1=vertices.at(i-1);
			pt2=vertices.at(i+1);
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
bool is_clockwise(const std::vector<point_2d<T>>& vertices) {

	size_t tam=vertices.size();
	if(tam < 3) return false;

	int sum=0;
	for(size_t i=1; i<tam; ++i)
	{
		sum+=(vertices[i].x-vertices[i-1].x)*(vertices[i].y+vertices[i-1].y);
	}

	sum+=(vertices[0].x-vertices[tam-1].x)*(vertices[0].y+vertices[tam-1].y);
	return sum >= (T)0;
}

} //End of namespace...
#endif
