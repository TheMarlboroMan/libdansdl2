#ifndef POINT_2D_LIBDANSDL_H
#define POINT_2D_LIBDANSDL_H

#include <cmath>
#include "../tools/tools.h" //Por si acaso no hay M_PI...

namespace ldt
{

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

	point_2d operator+(const point_2d<T>& p) const
	{
		return point_2d<T> {x+p.x, y+p.y};
	}

	point_2d operator*(T p) const
	{
		return point_2d<T> {x*p, y*p};
	}

	point_2d operator*=(T p)
	{
		x*=p;
		y*=p;
		return *this;
	}

	point_2d operator/=(T p)
	{
		x/=p;
		y/=p;
		return *this;
	}

	point_2d operator/(T p) const
	{
		return point_2d<T> {x/p, y/p};
	}

	point_2d operator-(const point_2d& p) const
	{
		return point_2d<T> {x-p.x, y-p.y};
	}

	point_2d& operator+=(const point_2d<T>& p)
	{
		x+=p.x;
		y+=p.y;
		return *this;
	}

	point_2d& operator-=(const point_2d& p)
	{
		x-=p.x;
		y-=p.y;
		return *this;
	}

	bool operator==(const point_2d& p) const
	{
		return p.x==x && p.y==y;
	}

	T distance_to(const point_2d<T>& p) const
	{
		T x=(this->x-p.x)*(this->x-p.x);
		T y=(this->y-p.y)*(this->y-p.y);
		return sqrt(x+y);
	}

	static T distance_between(const point_2d<T>& p1, const point_2d<T>& p2)
	{
		T x=(p1.x-p2.x)*(p1.x-p2.x);
		T y=(p1.y-p2.y)*(p1.y-p2.y);
		return sqrt(x+y);
	}

	//La rotación es negativa si va en sentido de las agujas del reloj...
	void rotate(T grados, const point_2d<T> centro)
	{
		//Llevar a origen...
		T ox=x - centro.x;
		T oy=y - centro.y;

		//Precalculos...
		T rad=ldt::deg_to_rad(grados);
		T sr=sin(rad);
		T cr=cos(rad);

		//Rotar...
		T rx=(ox * cr) - (oy * sr);
		T ry=(oy * cr) + (ox * sr);

		//Desplazar de nuevo...
		x = rx + centro.x;
		y = ry + centro.y;
	}

};

}//Fin namespace...

#endif
