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

	typedef	Punto_2d<T>		tpunto;

					Poligono_2d()
	{
	
	}

	
					Poligono_2d(const std::vector<tpunto>& pts, tpunto c)
		:centro(c), puntos(pts)
	{

	}

	size_t				size() const {return puntos.size();}

	void				punto(tpunto v)
	{
		if(!size()) centro=v;
		puntos.push_back(v);
	}


	void				desplazar(tpunto v)
	{
		centro+=v;
		for(auto &p : puntos) p+=v;
	}

	void				rotar(float grados)
	{
		for(auto &p : puntos) p.rotar(grados, centro);
	}

	tpunto				centro;
	std::vector<tpunto>		puntos;
};

}

#endif
