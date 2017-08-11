#ifndef LIBDANSDL_2_VECTOR_2D_H
#define LIBDANSDL_2_VECTOR_2D_H

#include <cmath>
#include "../tools/tools.h"

namespace ldt
{

//!Template for a 2d vector.

template<typename T>
struct vector_2d
{
	T 					x, y;

	//! Constructs a vector with default values.
						vector_2d()
		:x(), y() 
	{

	}

	//!Copy constructor.
						vector_2d(const vector_2d<T>& o)
		:x(o.x), y(o.y) 
	{

	}

	//!Constructs a vector with the given values for x and y.
						vector_2d(T p_x, T p_y)
		:x(p_x), y(p_y) 
	{
	
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

	//!Assignment operator.
	vector_2d<T>& 				operator=(const vector_2d<T> &o)
	{
		this->x=o.x;
		this->y=o.y;
		return *this;
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
	vector_2d<T>&				operator*=(const vector_2d<T> &o)
	{
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
	vector_2d<T> 				operator/(const T v)
	{
		return vector_2d<T>(this->x/v, this->y/v);
	}

	//!Multiplies the current vector for a value.
	vector_2d<T>& 				operator*=(const T v)
	{
		this->x*=v;
		this->y*=v;
		return *this;
	}

	//!Divides the current vector for a value.
	vector_2d<T>& 				operator/=(const T v)
	{
		this->x/=v;
		this->y/=v;
		return *this;
	}

	//!Gets the vector perpendicular to this one.
	vector_2d<T>				perpendicular() const
	{
		return vector_2d<T>{this->y, -this->x};
	}

	//!Normalizes vector (sets largest value to 1).
	void 					normalize()
	{
		T m=this->length();
		this->x=this->x / m;
		this->y=this->y / m;
	}

	//!Calculates the lenght of the vector.
	T 					length() const
	{
		return sqrtf(this->x*this->x + this->y*this->y);
	}

	//!Gets angle in radians.
	T 					angle_rad() const
	{
		auto vec(*this);
		vec.normalize();
		return angle_for_unit_vector_rad(vec);
	}

	//!Gets angle in degrees.
	T 					angle_deg() const
	{
		auto vec(*this);
		vec.normalize();
		return angle_for_unit_vector_deg(vec);
	}
};

//!This vector is structured according to screen coordinates.

//!Negative Y o is up, positive Y is down. Provides a separate class to make
//!client code easier to read.

template<typename T>
struct vector_2d_screen:
	public vector_2d<T>
{
	//! Constructs a vector with default values.
						vector_2d_screen()
		:vector_2d<T>() 
	{

	}

	//!Constructs a vector with the given values for x and y.
						vector_2d_screen(T px, T py)
		:vector_2d<T>(px, py) 
	{

	}

	//!Copy constructor.
						vector_2d_screen(const vector_2d_screen<T>& v)
		:vector_2d<T>(v.x, v.y)
	{
		this->x=v.x;
		this->y=v.x;
	}


	//!Assignment operator.
						vector_2d_screen& operator=(const vector_2d_screen<T>& v)
	{
		vector_2d<T>::operator=(v);
		return *this;
	}

	//!Adds two vectors and returns the result as a new object.
	vector_2d_screen 			operator+(const vector_2d_screen<T> &o)
	{
		return vector_2d_screen(this->x+o.x, this->y+o.y);
	}

	//!Substracts two vectors and returns the result as a new object.
	vector_2d_screen 			operator-(const vector_2d_screen<T> &o)
	{
		return vector_2d_screen(this->x-o.x, this->y-o.y);
	}

	//!Multiplies two vectors and returns the result as a new object.
	vector_2d_screen 			operator*(const vector_2d_screen<T> &o)
	{
		return vector_2d_screen(this->x*o.x, this->y*o.y);
	}

	//!Divides two vectors and returns the result as a new object.
	vector_2d_screen 			operator/(const vector_2d_screen<T> &o)
	{
		return vector_2d_screen(this->x/o.x, this->y/o.y);
	}

	//!Adds the given vector to the current one.
	vector_2d_screen& 			operator+=(const vector_2d_screen<T> &o)
	{
		this->x+=o.x;
		this->y+=o.y;
		return *this;
	}

	//!Substracts the given vector to the current one.
	vector_2d_screen& 			operator-=(const vector_2d_screen<T> &o)
	{
		this->x-=o.x;
		this->y-=o.y;
		return *this;
	}

	//!Multiplies the given vector for the current one.
	vector_2d_screen& 			operator*=(const vector_2d_screen<T> &o)
	{
		this->x*=o.x;
		this->y*=o.y;
		return *this;
	}

	//!Multiplies the given vector with the current one.
	vector_2d_screen& 			operator/=(const vector_2d_screen<T> &o)
	{
		this->x/=o.x;
		this->y/=o.y;
		return *this;
	}

	//!Gets a new vector by multiplying both factors of the current one.
	vector_2d_screen			operator*(const T v)
	{
		return vector_2d_screen(this->x*v, this->y*v);
	}

	//!Gets a new vector by dividing both factors of the current one. Divide by zero will still do bad things.
	vector_2d_screen 			operator/(const T v)
	{
		return vector_2d_screen(this->x/v, this->y/v);
	}

	//!Multiplies the current vector for a value.
	vector_2d_screen& 			operator*=(const T v)
	{
		this->x*=v;
		this->y*=v;
		return *this;
	}

	//!Divides the current vector by a value.
	vector_2d_screen& 			operator/=(const T v)
	{
		this->x/=v;
		this->y/=v;
		return *this;
	}

	//!Gets the vector perpendicular to this one.
	vector_2d_screen<T>			perpendicular() const
	{
		return vector_2d_screen<T>{this->y, -this->x};
	}
};

//!Cartesian vector.

//!Y positive is up. Y negative is down. Exists to provide clearer client code.

template<typename T>
struct vector_2d_cartesian:
	public vector_2d<T>
{
	//! Constructs a vector with default values.
						vector_2d_cartesian():vector_2d<T>() 
	{

	}

	//! Constructs a vector with the given values.
						vector_2d_cartesian(T px, T py):vector_2d<T>(px, py) 
	{

	}

	//! Copy constructor.
						vector_2d_cartesian(const vector_2d_cartesian<T>& v)
		:vector_2d<T>() /*(v.x, v.y)*/
	{
		this->x=v.x;
		this->y=v.y;
	}

	//!Assignment operator.
						vector_2d_cartesian& operator=(const vector_2d_cartesian<T>& v)
	{
		vector_2d<T>::operator=(v);
		return *this;
	}

	//!Adds two vectors and returns the result as a new object.
	vector_2d_cartesian 			operator+(const vector_2d_cartesian<T> &o)
	{
		return vector_2d_cartesian(this->x+o.x, this->y+o.y);
	}

	//!Substracts two vectors and returns the result as a new object.
	vector_2d_cartesian 			operator-(const vector_2d_cartesian<T> &o)
	{
		return vector_2d_cartesian(this->x-o.x, this->y-o.y);
	}

	vector_2d_cartesian 			operator*(const vector_2d_cartesian<T> &o)
	{
		return vector_2d_cartesian(this->x*o.x, this->y*o.y);
	}

	vector_2d_cartesian 			operator/(const vector_2d_cartesian<T> &o)
	{
		return vector_2d_cartesian(this->x/o.x, this->y/o.y);
	}

	//!Adds the given vector to the current one.
	vector_2d_cartesian& 			operator+=(const vector_2d_cartesian<T> &o)
	{
		this->x+=o.x;
		this->y+=o.y;
		return *this;
	}

	vector_2d_cartesian& 			operator-=(const vector_2d_cartesian<T> &o)
	{
		this->x-=o.x;
		this->y-=o.y;
		return *this;
	}

	vector_2d_cartesian& 			operator*=(const vector_2d_cartesian<T> &o)
	{
		this->x*=o.x;
		this->y*=o.y;
		return *this;
	}

	vector_2d_cartesian& 			operator/=(const vector_2d_cartesian<T> &o)
	{
		this->x/=o.x;
		this->y/=o.y;
		return *this;
	}

	//!Gets a new vector by multiplying both factors of the current one.
	vector_2d_cartesian			operator*(const T v)
	{
		return vector_2d_cartesian(this->x*v, this->y*v);
	}

	//!Gets a new vector by dividing both factors of the current one. Divide by zero will still do bad things.
	vector_2d_cartesian 			operator/(const T v)
	{
		return vector_2d_cartesian(this->x/v, this->y/v);
	}

	//!Multiplies the current vector for a value.
	vector_2d_cartesian& 			operator*=(const T v)
	{
		this->x*=v;
		this->y*=v;
		return *this;
	}

	//!Divides the current vector by a value.
	vector_2d_cartesian& 			operator/=(const T v)
	{
		this->x/=v;
		this->y/=v;
		return *this;
	}

	//!Gets the vector perpendicular to this one.
	vector_2d_cartesian<T>			perpendicular() const
	{
		return vector_2d_cartesian<T>{this->y, -this->x};
	}
};

/*! \file vector_2d.h
    \brief Miscellaneous vector functions.
    
	Vector related functionality.
*/

//!Calculates angle in radians for a unit vector. There is no built in protection in case vectors are not normalised.
template<typename T>
T angle_for_unit_vector_rad(const vector_2d<T>& p_vector)
{
	T rad=std::atan2(p_vector.y, p_vector.x);
	return rad;
}

//!Calculates angle in degrees for a unit vector. There is no built in protection in case vectors are not normalised.
template<typename T>
T angle_for_unit_vector_deg(const vector_2d<T>& p_vector)
{
	T rad=angle_for_unit_vector_rad(p_vector);
	T grados=(rad / M_PI) * 180.0;
	return grados;
}

//!Creates a unit vector pointing towards the angle in degrees.
template<typename T>
vector_2d<T> unit_vector_for_angle(T p_angulo)
{
	T rad=ldt::deg_to_rad(p_angulo);
	T v_x=sin(rad);
	T v_y=cos(rad);

	vector_2d<T> r(v_x, v_y);
	return r;
}

//!Creates a unit vector pointing towards the angle in degrees, using cartesian space (0 is right).
template<typename T>
vector_2d<T> unit_vector_for_angle_cartesian(T p_angulo)
{
	T rad=ldt::deg_to_rad(p_angulo);
	T v_x=cos(rad);
	T v_y=sin(rad);

	vector_2d<T> r(v_x, v_y);
	return r;
}

//!Calculates the cross product of two vectors.
template<typename T>
T cross_product(const vector_2d<T>& a, const vector_2d<T>& b)
{
	return a.x*b.x+a.y*b.y;
}

//!Calculates the determinant product of two vectors.
template<typename T>
T determinant(const vector_2d<T>& a, const vector_2d<T>& b)
{
	return (a.x*b.y)-(a.y*b.x);
}

//!Creates the vector from point a to point b. Order is important.
//TODO: Perhaps it gets a 2d_point...
template<typename T>
vector_2d<T> for_points(T p_xa, T p_ya, T p_xb, T p_yb, bool normalize=true)
{
	vector_2d<T> r;

	if(! (p_xa==p_xb && p_ya==p_yb))
	{
		r.x=p_xb-p_xa;
		r.y=p_yb-p_ya;
		if(normalize) r.normalize();
	}

	return r;
}

//!Creates a screen vector from point a to point b. Order is important.
template<typename T>
vector_2d_screen<T> for_points_screen(T p_xa, T p_ya, T p_xb, T p_yb, bool normalize=true)
{
	vector_2d_screen<T> r;

	if(! (p_xa==p_xb && p_ya==p_yb))
	{
		r.x=p_xa-p_xb;
		r.y=p_ya-p_yb;
		if(normalize) r.normalize();
	}

	return r;
}

//!Creates a cartesian vector from point a to point b. Order is important.
template<typename T>
vector_2d_cartesian<T> for_points_cartesian(T p_xa, T p_ya, T p_xb, T p_yb, bool normalize=true)
{
	vector_2d_cartesian<T> r;

	if(! (p_xa==p_xb && p_ya==p_yb))
	{
		r.x=p_xb-p_xa;
		r.y=p_yb-p_ya;
		if(normalize) r.normalize();
	}

	return r;
}

//!Converts a screen vector to a cartesian one.
template<typename T>
vector_2d_cartesian<T> to_cartesian(const vector_2d_screen<T>& v)
{
	return vector_2d_cartesian<T>(v.x, -v.y);
}

//!Converts a cartesian vector to a screen one.
template<typename T>
vector_2d_screen<T> to_screen(const vector_2d_cartesian<T>& v)
{
	return vector_2d_screen<T>(v.x, -v.y);
}

}
#endif
