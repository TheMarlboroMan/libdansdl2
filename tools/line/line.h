#ifndef LIBDANSDL2_LINE_H
#define LIBDANSDL2_LINE_H

#include <cmath>
#include <cstdlib>
#include "../point_2d/point_2d.h"
#include "../box/box.h"

namespace ldt
{

//!A lo-tech 2d line.

//!This class is at odds with the segment_2d struct, as they represent the
//!same idea. This one is, however, oriented to calculations (that may be
//!performed with the segment struct too). It also includes a set of free 
//!functions to construct them (it's the only way to do so).

template<typename T>
class line
{
	public:

	typedef point_2d<T> point;

	//!Indicates if the line is vertical.
	bool is_vertical() const {return orientation==torientation::vertical;}
	//!Indicates if the line is horizontal.
	bool is_horizontal() const {return orientation==torientation::horizontal;}
	//!Indicates if the line is sloped.
	bool is_slope() const {return orientation==torientation::slope;}
	//!Get the m value (slope)
	T get_m() const {return this->m;}
	//!Get the b value (constant).
	T get_b() const {return this->b;}
	//!Gets the Y value for a given X.
	T y_for_x(T p_x) const
	{
		//y=mx+b;
		T result;

		switch(orientation)
		{
			case torientation::vertical:	result=0; break;
			case torientation::horizontal:	result=b; break;
			case torientation::slope:
				if(p_x==0) 
				{
					result=b;
				}
				else
				{
					result=(m*p_x) + b;
				}
			break;
		}

		return result;
	}
	//Gets the X value for a given Y.
	T x_for_y(T p_y) const
	{
		//x=(y-b)/m
		T result;

		switch(orientation)
		{
			case torientation::horizontal:	result=0; break;
			case torientation::vertical:	result=b; break; //Go figure.
			case torientation::slope:	result=(p_y-b) / m; break;
		}

		return result;
	}

	private:

	enum class torientation{vertical, horizontal, slope};

	line(T pm=0.0, T pb=0.0, torientation po=torientation::slope):
		orientation(po), m(pm), b(pb)
	{
	}

	torientation	orientation;
	T 		m,
			b;
};

//!Creates a line from two points.

template<typename T>
line<T> line_from_points(T xa, T ya, T xb, T yb)
{
	line<T> result;

	if(xa==xb) result.orientation=line<T>::torientation::vertical;
	else if(ya==yb) result.orientation=line<T>::torientation::horizontal;
	else //Orientation is already asigned as sloped by the constructor.
	{       
		//Slope: m=(yb-ya)/(xb-xa)...            
		result.m=((T)yb-(T)ya)/((T)xb-(T)xa);  

		//Constant b... b=y-(mx)
		result.b=ya-(result.m*xa);
	}

	return result;
}

//Creates a line from slope and constant.

template<typename T>
line<T> line_from_parameters(T p_m, T p_b)
{
	line<T> result(p_m, p_b);
	if(p_b==0) 
	{
		result.orientation==line<T>::torientation::horizontal;
	}
	return result;
}


}
#endif
