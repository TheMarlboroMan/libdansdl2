#ifndef LIBDANSDL_2_VECTOR_2D_H
#define LIBDANSDL_2_VECTOR_2D_H

#include <cmath>
#include "../tools/tools.h"

namespace ldt
{

/**
* Vector simple... Para uso normal y corriente.
*/

template<typename T>
struct vector_2d
{
	T 					x, y;

						vector_2d()
		:x(), y() 
	{

	}

						vector_2d(const vector_2d<T>& o)
		:x(o.x), y(o.y) 
	{

	}

						vector_2d(T p_x, T p_y)
		:x(p_x), y(p_y) 
	{
	
	}

	vector_2d<T> operator+(const vector_2d<T> &o)
	{
		return vector_2d<T>(this->x+o.x, this->y+o.y);
	}

	vector_2d<T> 				operator-(const vector_2d<T> &o)
	{
		return vector_2d<T>(this->x-o.x, this->y-o.y);
	}

	vector_2d<T> 				operator*(const vector_2d<T> &o)
	{
		return vector_2d<T>(this->x*o.x, this->y*o.y);
	}

	vector_2d<T> 				operator/(const vector_2d<T> &o)
	{
		return vector_2d<T>(this->x/o.x, this->y/o.y);
	}

	vector_2d<T>& 				operator=(const vector_2d<T> &o)
	{
		this->x=o.x;
		this->y=o.y;
		return *this;
	}

	vector_2d<T>& 				operator+=(const vector_2d<T> &o)
	{
		this->x+=o.x;
		this->y+=o.y;
		return *this;
	}

	vector_2d<T>&				operator-=(const vector_2d<T> &o)
	{
		this->x-=o.x;
		this->y-=o.y;
		return *this;
	}

	vector_2d<T>&				operator*=(const vector_2d<T> &o)
	{
		this->x*=o.x;
		this->y*=o.y;
		return *this;
	}

	vector_2d<T>& 				operator/=(const vector_2d<T> &o)
	{
		this->x/=o.x;
		this->y/=o.y;
		return *this;
	}

	vector_2d<T>& 				operator*(const T v)
	{
		this->x=this->x*v;
		this->y=this->y*v;
		return *this;
	}

	vector_2d<T>& 				operator/(const T v)
	{
		this->x=this->x/v;
		this->y=this->y/v;
		return *this;
	}

	vector_2d<T>& 				operator*=(const T v)
	{
		this->x*=v;
		this->y*=v;
		return *this;
	}

	vector_2d<T>& 				operator/=(const T v)
	{
		this->x/=v;
		this->y/=v;
		return *this;
	}

	vector_2d<T>				perpendicular() const
	{
		return vector_2d<T>{this->y, -this->x};
	}

	void 					normalize()
	{
		T m=this->length();
		this->x=this->x / m;
		this->y=this->y / m;
	}

	T 					length() const
	{
		return sqrtf(this->x*this->x + this->y*this->y);
	}

	T 					angle_rad() const
	{
		auto vec(*this);
		vec.normalize();
		return angle_for_unit_vector_rad(vec);
	}

	T 					angle_deg() const
	{
		auto vec(*this);
		vec.normalize();
		return angle_for_unit_vector_deg(vec);
	}
};

/**
* Este o vector responde a las coordenadas de pantalla. Y negativo es arriba.
* Y positivo es abajo. Simplemente existe para ser un tipo separado y que no
* haya confusiones luego en el código cliente.
*/

template<typename T>
struct vector_2d_screen:
	public vector_2d<T>
{
						vector_2d_screen():vector_2d<T>() 
	{

	}

						vector_2d_screen(T px, T py):vector_2d<T>(px, py) 
	{

	}

						vector_2d_screen(const vector_2d_screen<T>& v)
		:vector_2d<T>()
	{
		this->x=v.x;
		this->y=v.x;
	}

						vector_2d_screen& operator=(const vector_2d_screen<T>& v)
	{
		vector_2d<T>::operator=(v);
		return *this;
	}

	vector_2d_screen 			operator+(const vector_2d_screen<T> &o)
	{
		return vector_2d_screen(this->x+o.x, this->y+o.y);
	}

	vector_2d_screen 			operator-(const vector_2d_screen<T> &o)
	{
		return vector_2d_screen(this->x-o.x, this->y-o.y);
	}

	vector_2d_screen 			operator*(const vector_2d_screen<T> &o)
	{
		return vector_2d_screen(this->x*o.x, this->y*o.y);
	}

	vector_2d_screen 			operator/(const vector_2d_screen<T> &o)
	{
		return vector_2d_screen(this->x/o.x, this->y/o.y);
	}

	vector_2d_screen& 			operator+=(const vector_2d_screen<T> &o)
	{
		this->x+=o.x;
		this->y+=o.y;
		return *this;
	}

	vector_2d_screen& 			operator-=(const vector_2d_screen<T> &o)
	{
		this->x-=o.x;
		this->y-=o.y;
		return *this;
	}

	vector_2d_screen& 			operator*=(const vector_2d_screen<T> &o)
	{
		this->x*=o.x;
		this->y*=o.y;
		return *this;
	}

	vector_2d_screen& 			operator/=(const vector_2d_screen<T> &o)
	{
		this->x/=o.x;
		this->y/=o.y;
		return *this;
	}

	vector_2d_screen&			operator*(const T v)
	{
		this->x=this->x*v;
		this->y=this->y*v;
		return *this;
	}

	vector_2d_screen& 			operator/(const T v)
	{
		this->x=this->x/v;
		this->y=this->y/v;
		return *this;
	}

	vector_2d_screen& 			operator*=(const T v)
	{
		this->x*=v;
		this->y*=v;
		return *this;
	}

	vector_2d_screen& 			operator/=(const T v)
	{
		this->x/=v;
		this->y/=v;
		return *this;
	}

	vector_2d_screen<T>			perpendicular() const
	{
		return vector_2d_screen<T>{this->y, -this->x};
	}
};

/**
* Este vector es cartesiano: y positivo es arriba, y negativo es abajo.
*/

template<typename T>
struct vector_2d_cartesian:
	public vector_2d<T>
{
						vector_2d_cartesian():vector_2d<T>() 
	{

	}

						vector_2d_cartesian(T px, T py):vector_2d<T>(px, py) 
	{

	}

						vector_2d_cartesian(const vector_2d_cartesian<T>& v)
		:vector_2d<T>() /*(v.x, v.y)*/
	{
		this->x=v.x;
		this->y=v.y;
	}

						vector_2d_cartesian& operator=(const vector_2d_cartesian<T>& v)
	{
		vector_2d<T>::operator=(v);
		return *this;
	}

	vector_2d_cartesian 			operator+(const vector_2d_cartesian<T> &o)
	{
		return vector_2d_cartesian(this->x+o.x, this->y+o.y);
	}

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

	vector_2d_cartesian&			operator*(const T v)
	{
		this->x=this->x*v;
		this->y=this->y*v;
		return *this;
	}

	vector_2d_cartesian& 			operator/(const T v)
	{
		this->x=this->x/v;
		this->y=this->y/v;
		return *this;
	}

	vector_2d_cartesian& 			operator*=(const T v)
	{
		this->x*=v;
		this->y*=v;
		return *this;
	}

	vector_2d_cartesian& 			operator/=(const T v)
	{
		this->x/=v;
		this->y/=v;
		return *this;
	}

	vector_2d_cartesian<T>			perpendicular() const
	{
		return vector_2d_cartesian<T>{this->y, -this->x};
	}
};

//Funciones de cálculo...
template<typename T>
T angle_for_unit_vector_rad(const vector_2d<T>& p_vector)
{
	T rad=std::atan2(p_vector.y, p_vector.x);
	return rad;
}

template<typename T>
T angle_for_unit_vector_deg(const vector_2d<T>& p_vector)
{
	T rad=angle_for_unit_vector_rad(p_vector);
	T grados=(rad / M_PI) * 180.0;
	return grados;
}

template<typename T>
vector_2d<T> unit_vector_for_angle(T p_angulo)
{
	T rad=ldt::deg_to_rad(p_angulo);
	T v_x=sin(rad);
	T v_y=cos(rad);

	vector_2d<T> r(v_x, v_y);
	return r;
}

template<typename T>
vector_2d<T> unit_vector_for_angle_cartesian(T p_angulo)
{
	T rad=ldt::deg_to_rad(p_angulo);
	T v_x=cos(rad);
	T v_y=sin(rad);

	vector_2d<T> r(v_x, v_y);
	return r;
}

template<typename T>
T cross_product(const vector_2d<T>& a, const vector_2d<T>& b)
{
	return a.x*b.x+a.y*b.y;
}

template<typename T>
T determinant(const vector_2d<T>& a, const vector_2d<T>& b)
{
	return (a.x*b.y)-(a.y*b.x);
}

/*El orden de los factores PUEDE alterar el producto.*/

template<typename T>
vector_2d<T> for_points(T p_xa, T p_ya, T p_xb, T p_yb, bool normalize=true)
{
	vector_2d<T> r;

	if(! (p_xa==p_xb && p_ya==p_yb))
	{
		r.x=p_xb-p_xa;
		r.y=p_yb-p_ya;
		r.normalize();
	}

	return r;
}

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

//Funciones para conversión de cartesiano a pantalla...
template<typename T>
vector_2d_cartesian<T> to_cartesian(const vector_2d_screen<T>& v)
{
	return vector_2d_cartesian<T>(v.x, -v.y);
}

template<typename T>
vector_2d_screen<T> to_screen(const vector_2d_cartesian<T>& v)
{
	return vector_2d_screen<T>(v.x, -v.y);
}

} //Fin namespace ldt
#endif
