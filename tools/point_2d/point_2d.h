#ifndef POINT_2D_LIBDANSDL_H
#define POINT_2D_LIBDANSDL_H

#include <cmath>
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

	//!Returns the distance to the point.

	T distance_to(const point_2d<T>& p) const {
		return distance_between(*this, p);
	}

	//!Rotates the point around a center. Negative rotations are clockwise.
	void rotate(T grados, const point_2d<T> centro) {
		//Take to origin...
		T ox=x - centro.x;
		T oy=y - centro.y;

		//Precalculate...
		T rad=ldt::deg_to_rad(grados);
		T sr=sin(rad);
		T cr=cos(rad);

		//Rotate...
		T rx=(ox * cr) - (oy * sr);
		T ry=(oy * cr) + (ox * sr);

		//Move again...
		x = rx + centro.x;
		y = ry + centro.y;
	}

};

//!Returns the distance between two points.

template<typename T>
T distance_between(const point_2d<T>& p1, const point_2d<T>& p2)
{
	T x=(p1.x-p2.x)*(p1.x-p2.x);
	T y=(p1.y-p2.y)*(p1.y-p2.y);
	return sqrt(x+y);
}

//!Returns the minimum distance between a point and a segment.

//!p is the point. The segment is s1 to s2.

//Only god know what this is and does ... https://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment/1501725.

template<typename T>
T distance_between(const point_2d<T>& pt, const point_2d<T>& s1, const point_2d<T>& s2)
{
	auto dist=[](const point_2d<T>& v, const point_2d<T>& w)
	{
		return 	((v.x-w.x)*(v.x-w.x)) + ((v.y-w.y)*(v.y-w.y));
	};

	auto dist_to_segment_squared=[&dist](const point_2d<T>& p, const point_2d<T>& v, const point_2d<T>& w)
	{
		auto l2=dist(v, w);
		if(!l2) return dist(p, v);
		T t = ((p.x - v.x) * (w.x - v.x) + (p.y - v.y) * (w.y - v.y)) / l2;
		t = std::max((T)0, std::min((T)1, t));
		point_2d<T> near(v.x + t * (w.x - v.x), v.y + t * (w.y - v.y));
		return dist(p, near);
	};

	return std::sqrt(dist_to_segment_squared(pt, s1, s2));
}

} //End of namespace...
#endif
