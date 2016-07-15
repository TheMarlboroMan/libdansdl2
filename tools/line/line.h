#ifndef LIBDANSDL2_LINE_H
#define LIBDANSDL2_LINE_H

#include <cmath>
#include <cstdlib>
#include "../point_2d/point_2d.h"
#include "../box/box.h"

namespace ldt
{

template<typename T>
class line
{
	public:

	typedef point_2d<T> point;

	struct point_set
	{
		point 	p1,
			p2;
		bool 	error;
		point_set(point p_1, point p_2)
			:p1(p_1), p2(p_2), error(false)
		{

		}
	};

	bool is_vertical() const {return orientation==torientation::vertical;}
	bool is_horizontal() const {return orientation==torientation::horizontal;}
	bool is_other() const {return orientation==torientation::other;}
	T get_m() const {return this->m;}	
	T get_b() const {return this->b;}

	~line() {}

	static line from_points(T xa, T ya, T xb, T yb)
	{
		line result;

		if(xa==xb) 		//Es vertical??
		{
			result.orientation=torientation::vertical;
		}
		else if(ya==yb)		//Es horizontal?
		{
			result.orientation=torientation::horizontal;
		}
		else			//Orientation is already asigned as other by the constructor.
		{       
			//La pendiente: m=(yb-ya)/(xb-xa)...            
			result.m=((T)yb-(T)ya)/((T)xb-(T)xa);  

			//La constante b... b=y-(mx)
			result.b=ya-(result.m*xa);
		}

		return result;
	}

	static line from_parameters(T p_m, T p_b)
	{
		line result(p_m, p_b);
		if(p_b==0) 
		{
			result.orientation==torientation::horizontal;
		}

		return result;
	}

/*Obtiene una línea metida dentro de una caja recortando
la existente, si procede... Los cuatro primeros parámetros
son la caja, el resto los puntos... Ojo: sólo funciona
cuando realmente la línea PASA por el rectángulo. 
Realmente no devuelve una línea, sino dos puntos en un
*/

	static point_set for_line_and_box(box<T,T> b, line l)
	{
		//En primer lugar, buscamos los puntos en los que la línea corta
		//con cada "lado". Los ejes serán 1, 2, 3 y 4 como 
		//arriba, derecha, abajo e izquierda.
	
		//La línea que va por arriba... Buscamos la x para la y enviada.
		point 	p1(l.x_for_y(b.origin.y), b.origin.y),
		//La línea de la derecha... Buscamos la y para una x.
			p2(b.origin.x+b.w, l.y_for_x(b.origin.x+b.w)),
		//La línea de abajo: buscamos la x para una y.
			p3(l.x_for_y(b.origin.y+b.h), b.origin.y),
		//La línea de la izquierda...
			p4(b.origin.x, l.y_for_x(b.origin.x));

		//De estos cuatro puntos ahora vemos cuales estarían "dentro"
		//del rectángulo. Serán aquellos en los que al menos una coordenada
		//coincida...

		point_set result=point_set(point(0.0,0.0), point(0.0,0.0));

		if(p1.x >= b.origin.x && p1.y <= b.origin.x+b.w)
		{
			result.p1=p1;
		}
		else if(p3.x >= b.origin.x && p3.y <= b.origin.x+b.w)
		{
			result.p1=p3;
		}
		else
		{
			result.error=true;
		}

		if(p2.y >= b.origin.y && p2.y <= b.origin.y+b.h)
		{
			result.p2=p2;
		}
		else if(p4.y >= b.origin.y && p4.y <= b.origin.y+b.h)
		{
			result.p2=p4;
		}
		else
		{
			result.error=true;
		}

		return result;
	}
	
	T y_for_x(T p_x) const
	{
		//y=mx+b;
		T result;

		switch(orientation)
		{
			case torientation::vertical:	result=0; break;
			case torientation::horizontal:	result=b; break;
			case torientation::other:
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

	T x_for_y(T p_y) const
	{
		//x=(y-b)/m
		T result;

		switch(orientation)
		{
			case torientation::horizontal:	result=0; break;
			case torientation::vertical:	result=b; break; //Quien sabe... 
			case torientation::other:	result=(p_y-b) / m; break;
		}

		return result;
	}

	private:

	enum class torientation{vertical, horizontal, other};

	line(T pm=0.0, T pb=0.0, torientation po=torientation::other):
		orientation(po), m(pm), b(pb)
	{
	}

	torientation	orientation;
	T 		m,	//Pendiente.
			b;	//Constante.	
};

}
#endif
