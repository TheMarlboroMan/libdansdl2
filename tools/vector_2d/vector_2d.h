#ifndef LIBDANSDL_2_VECTOR_2D_H
#define LIBDANSDL_2_VECTOR_2D_H

/*! \file vector_2d.h
    \brief Vectors.
    
	2d Vector class related functionality. The vector can be used in the
application code but also appears in other libdansdl2 parts.
*/

#include <algorithm>
#include <cmath>

#include "../tools/tools.h"
#include "../point_2d/point_2d.h"

namespace ldt
{

//!Template for a 2d vector in a regular cartesian space. 

//!Positive Y is up and negative Y is down.

template<typename T>
struct vector_2d
{
	T 					x,	//!< X component.
						y;	//!< Y component.

	//! Constructs a vector with default values.
						vector_2d()
		:x(), y() 
	{}

	//!Copy constructor.
						vector_2d(const vector_2d<T>& o)
		:x(o.x), y(o.y) 
	{}

	//!Constructs a vector with the given values for x and y.
						vector_2d(T p_x, T p_y)
		:x(p_x), y(p_y) 
	{}

	//!Assignment operator.
	vector_2d<T>& 				operator=(const vector_2d<T> &o)
	{
		this->x=o.x;
		this->y=o.y;
		return *this;
	}

	//!Checks for equality in values.
	bool					operator==(const vector_2d<T>& v) const
	{
		return x==v.x && y==v.y;
	}

	//!Adds two vectors and returns the result as a new object.
	vector_2d<T> operator+(const vector_2d<T> &o)
	{
		return vector_2d<T>(this->x+o.x, this->y+o.y);
	}

	//!Substracts two vectors and returns the result as a new object.
	vector_2d<T> 				operator-(const vector_2d<T> &o)
	{
		return vector_2d<T>(this->x-o.x, this->y-o.y);
	}

	//!Multiplies two vectors and returns the result as a new object.
	vector_2d<T> 				operator*(const vector_2d<T> &o)
	{
		return vector_2d<T>(this->x*o.x, this->y*o.y);
	}

	//!Divides two vectors and returns the result as a new object.
	vector_2d<T> 				operator/(const vector_2d<T> &o)
	{
		return vector_2d<T>(this->x/o.x, this->y/o.y);
	}

	//!Adds the given vector to the current one.
	vector_2d<T>& 				operator+=(const vector_2d<T> &o)
	{
		this->x+=o.x;
		this->y+=o.y;
		return *this;
	}

	//!Substracts the given vector from the current one.
	vector_2d<T>&				operator-=(const vector_2d<T> &o)
	{
		this->x-=o.x;
		this->y-=o.y;
		return *this;
	}

	//!Multiplies the given vector for the current one.
	vector_2d<T>&				operator*=(const vector_2d<T> &o) {

		this->x*=o.x;
		this->y*=o.y;
		return *this;
	}

	//!Divides the given vector for the current one.
	vector_2d<T>& 				operator/=(const vector_2d<T> &o)
	{
		this->x/=o.x;
		this->y/=o.y;
		return *this;
	}

	//!Gets a new vector by multiplying both factors of the current one.
	vector_2d<T> 				operator*(const T v)
	{
		return vector_2d<T>(this->x*v, this->y*v);
	}

	//!Gets a new vector by dividing both factors of the current one. Divide by zero will still do bad things.
	vector_2d<T> 				operator/(const T v) {
		
		return vector_2d<T>(this->x/v, this->y/v);
	}

	//!Multiplies the current vector for a value.
	vector_2d<T>& 				operator*=(T _v) {

		this->x*=_v;
		this->y*=_v;
		return *this;
	}

	//!Divides the current vector for a value.
	vector_2d<T>& 				operator/=( T _v) {
		this->x/=_v;
		this->y/=_v;
		return *this;
	}
/*
		right	left	up	down
		1,0	-1,0	0,1	0,-1

		up	down	left	right
left	-y,x	-0,1	-0,-1	-1,0	1,0

right	y,-x

*/


//-128, 0
//left:	-0, 128
//right:	0, -128
	//!Gets the left normal (perpendicular) vector to this one.
	vector_2d<T>				left_normal() const
	{
		return vector_2d<T>{-this->y, this->x};
	}

	//!Gets the right normal (perpendicular) vector to this one.
	vector_2d<T>				right_normal() const
	{
		return vector_2d<T>{this->y, -this->x};
	}

	//!Normalizes vector (sets largest value to 1).
	vector_2d<T>&				normalize()
	{
		T m=this->magnitude();
		this->x=this->x / m;
		this->y=this->y / m;
		return *this;
	}

	//!Returns a new, normalized vector (sets largest value to 1).
	vector_2d<T>				get_normalized() const
	{
		vector_2d<T> res{*this};
		T m=res.magnitude();
		res.x=res.x / m;
		res.y=res.y / m;
		return res;
	}

	//!Calculates the lenght of the vector.
	T 					magnitude() const
	{
		return sqrtf(this->x*this->x + this->y*this->y);
	}

	//!Gets angle in radians.
	T 					get_angle_rad() const
	{
		return angle_for_vector_rad(*this);
	}

	//!Gets angle in degrees.
	T 					get_angle_deg() const
	{
		return angle_for_vector_deg(*this);
	}
};

//Vector yielding functions...

//!Creates a unit vector pointing towards the angle in degrees, using cartesian space (0 is right).
template<typename T>
vector_2d<T> vector_from_angle(T p_ang) {
	T rad=ldt::deg_to_rad(p_ang);
	return {cos(rad), sin(rad)};
}

//!Obtains a vector for the given angle and magnitude.
template<typename T>
vector_2d<T> vector_from_angle_and_magnitude(T _angle, T _magnitude) {

	return vector_from_angle(_angle)*_magnitude;
}

//!Creates the vector from point a to point b. Order is important.
template<typename T>
vector_2d<T> vector_from_points(point_2d<T> pa, point_2d<T> pb) {

	vector_2d<T> r;

	if(! (pa.x==pb.x && pa.y==pb.y))
	{
		r.x=pb.x-pa.x;
		r.y=pb.y-pa.y;
	}

	return r;
}

//Cartesian space functions.

//!Calculates angle in radians for a vector in cartesian space.
template<typename T>
T angle_for_vector_rad(const vector_2d<T>& p_vector) {

	//Atan2 uses the signs of arguments to get the right quadrant.
	return std::atan2(p_vector.y, p_vector.x);
}

//!Calculates angle in degrees for a vector in cartesian space.

//!Returns its results in 0-360.
template<typename T>
T angle_for_vector_deg(const vector_2d<T>& p_vector) {
	T res=(angle_for_vector_rad(p_vector) * (T)180.0) / M_PI;
	if(res < (T).0) res+=(T)360.;
	return res;
}

//!Calculates the cross product of two vectors.
template<typename T>
T dot_product(const vector_2d<T>& a, const vector_2d<T>& b) {

	return a.x*b.x+a.y*b.y;
}

//!Calculates the determinant product of two vectors.
template<typename T>
T determinant(const vector_2d<T>& a, const vector_2d<T>& b) {

	return (a.x*b.y)-(a.y*b.x);
}

} //End namespace.

#endif
