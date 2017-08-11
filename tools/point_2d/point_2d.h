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

	T x,y;

	point_2d(): x(0), y(0) {}
	point_2d(T px, T py):x(px), y(py) {}
	point_2d(const point_2d<T>& p):x(p.x), y(p.y) {}
	point_2d& operator=(const point_2d<T>& p)
	{
		x=p.x;
		y=p.y;
		return *this;
	}

	//!Gets a new point by addition.
	point_2d operator+(const point_2d<T>& p) const
	{
		return point_2d<T> {x+p.x, y+p.y};
	}

	//!Adds to the point.
	point_2d& operator+=(const point_2d<T>& p)
	{
		x+=p.x;
		y+=p.y;
		return *this;
	}

	//!Gets a new point by substraction.
	point_2d operator-(const point_2d& p) const
	{
		return point_2d<T> {x-p.x, y-p.y};
	}

	//!Substracts from the point.
	point_2d& operator-=(const point_2d& p)
	{
		x-=p.x;
		y-=p.y;
		return *this;
	}

	//!Gets a new point by product.
	point_2d operator*(T p) const
	{
		return point_2d<T> {x*p, y*p};
	}

	//!Multiplies the point.
	point_2d& operator*=(T p)
	{
		x*=p;
		y*=p;
		return *this;
	}

	//!Gets a new point by division.
	point_2d operator/(T p) const
	{
		return point_2d<T> {x/p, y/p};
	}

	//!Divides the point.
	point_2d& operator/=(T p)
	{
		x/=p;
		y/=p;
		return *this;
	}

	//!Checks if two points are equal in value.

	bool operator==(const point_2d& p) const
	{
		return p.x==x && p.y==y;
	}

	//!Returns the distance to the point.

	T distance_to(const point_2d<T>& p) const
	{
		return distance_between(*this, p);
	}

	//Rotates the point around a center. Negative rotations are clockwise.
	void rotate(T grados, const point_2d<T> centro)
	{
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

}

#endif
