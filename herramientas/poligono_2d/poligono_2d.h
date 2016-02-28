#ifndef POLIGONO_2D_LIBDANSDL2_H
#define POLIGONO_2D_LIBDANSDL2_H

#include <vector>
#include "../punto_2d/punto_2d.h"

namespace DLibH
{
template<typename T>
class Poligono_2d
{
	public:

					Poligono_2d()
	{
	
	}

	
					Poligono_2d(const std::vector<Punto_2d<T>>& pts, Punto_2d<T> c)
		:centro(c), puntos(pts)
	{

	}

	size_t				size() const {return puntos.size();}

	void				punto(Punto_2d<T> v)
	{
		if(!size()) centro=v;
		puntos.push_back(v);
	}
	void				desplazar(Punto_2d<T> v)
	{
		centro+=v;
		for(auto &p : puntos) p+=v;
	}

	void				rotar(float grados)
	{
		for(auto &p : puntos) p.rotar(grados, centro);
	}

	Punto_2d<T>			centro;
	std::vector<Punto_2d<T>>	puntos;
};

}

#endif
