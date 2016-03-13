#ifndef POLIGONO_2D_LIBDANSDL2_H
#define POLIGONO_2D_LIBDANSDL2_H

#include <vector>
#include <stdexcept>
#include <array>

#include "../punto_2d/punto_2d.h"
#include "../vector_2d/vector_2d.h"
#include "../herramientas/herramientas.h"

//El polígono se asume que está declarado en el sentido de las agujas del reloj.
//Aunque

namespace DLibH
{

/**
* Versión simple definida a partir de los vértices. 
*/

template<typename T>
class Poligono_2d_vertices
{
	public:

	typedef	Punto_2d<T>		tpunto;

					Poligono_2d_vertices()
	{
	
	}

	
					Poligono_2d_vertices(const std::vector<tpunto>& pts, tpunto c)
		:centro(c), vertices(pts)
	{

	}

	bool				es_concavo() const
	{
		if(vertices.size() <= 3) return false;

		size_t i=0;
		while(i < vertices.size())
		{
			auto ptc=vertices.at(i),
				pt1=ptc, pt2=ptc;

			if(i==0)
			{
				pt1=vertices.at(vertices.size()-1);
				pt2=vertices.at(i+1);				
			}
			else if(i==vertices.size()-1)
			{
				pt1=vertices.at(i-1);
				pt2=vertices.at(0);
			}
			else
			{
				pt1=vertices.at(i-1);
				pt2=vertices.at(i+1);
			}

			auto vector_1=obtener_para_puntos_cartesiano(ptc.x, ptc.y, pt1.x, pt1.y);
			auto vector_2=obtener_para_puntos_cartesiano(ptc.x, ptc.y, pt2.x, pt2.y);
			double dot=producto_vectorial(vector_1, vector_2);
			double det=determinante(vector_1, vector_2);
			double angulorad=atan2(det, dot);
			double angulo=DLibH::Herramientas::radianes_a_grados(angulorad);

			angulo=angulo > 0.0 ? 360.0-angulo : -angulo;
			if(angulo > 180.0) return true;
			++i;
		}

		return false;
	}

	size_t				size() const {return vertices.size();}

	virtual void			desplazar(tpunto v)
	{
		centro+=v;
		for(auto &p : vertices) p+=v;
	}

	virtual void			centrar_en(tpunto v)
	{
		auto vec=obtener_para_puntos_cartesiano(centro.x, centro.y, v.x, v.y, false);
		for(auto &p : vertices) p+={vec.x, vec.y};
		centro=v;
	}

	virtual void			rotar(T grados)
	{
		for(auto &p : vertices) p.rotar(grados, centro);
	}

	virtual void			insertar_vertice(const tpunto& p)
	{
		if(!size()) centro=p;
		vertices.push_back(p);
	}

	void				mut_centro(const tpunto& p)
	{
		centro=p;
	}

	tpunto				acc_centro() const
	{
		return centro;
	}

	const tpunto&			vertice(size_t v) const
	{
		return vertices.at(v);
	}

	tpunto				vertice(size_t v)
	{
		return vertices[v];
	}

	tpunto&				ref_vertice(size_t v)
	{
		return vertices[v];
	}

	const std::vector<tpunto>&	acc_vertices() const 
	{
		return vertices;
	}

	protected:

	tpunto				centro;
	std::vector<tpunto>		vertices;
};

/**
* Definición de segmento... Dos puntos y el vector de dirección.
*/

template<typename T>
struct Segmento_2d
{
	typedef	Punto_2d<T>		tpunto;
	Punto_2d<T>			v1, v2;
	Vector_2d<T>			direccion;

					Segmento_2d<T>(Punto_2d<T> pv1, Punto_2d<T> pv2)
		:v1(pv1), v2(pv2), direccion(obtener_para_puntos(v1.x, v1.y, v2.x, v2.y))
	{}

					Segmento_2d<T>(const Segmento_2d<T>& o)
	:v1(o.v1), v2(o.v2), direccion(o.direccion)
	{}

	void				desplazar(tpunto p)
	{
		v1+=p;
		v2+=p;
	}
};

/**
* Definición de proyección de un polígono 2d. No es más que el rango "min-max"
* de proyección contra un eje.
*/

template<typename T>
struct Proyeccion_poligono
{
	T 			min, max;	
};

//TODO: Parametrizar roce es colision????
template<typename T>
bool	hay_superposicion(const Proyeccion_poligono<T>& pa, const Proyeccion_poligono<T>& pb)
{
	return Herramientas::segmentos_superpuestos(pa.min, pa.max, pb.min, pb.max);
}

/**
* Definición de polígono complejo: además de los de los vértices se guardan 
* también los segmentos que lo componen.
*/

//Un par de forwards...
template<typename T> class Poligono_2d;
template<typename T> bool colision_poligono_SAT(const Poligono_2d<T>& a,const Poligono_2d<T>& b);

template<typename T>
class Poligono_2d:
	public Poligono_2d_vertices<T>
{
	public:

	typedef	Punto_2d<T>		tpunto;

					Poligono_2d()
		:Poligono_2d_vertices<T>()
	{
	
	}

	
					Poligono_2d(const std::vector<tpunto>& pts, tpunto c)
		:Poligono_2d_vertices<T>(pts, c)
	{
		recrear_segmentos();
	}

	virtual void			desplazar(tpunto v)
	{
		Poligono_2d_vertices<T>::desplazar(v);
		for(auto &s : segmentos) s.desplazar(v);
	}

	virtual void			centrar_en(tpunto v)
	{
		auto vec=obtener_para_puntos_cartesiano(this->centro.x, this->centro.y, v.x, v.y, false);

std::cout<<">>>CENTRANDO EN "<<v.x<<", "<<v.y<<std::endl;
std::cout<<"DESDE "<<this->centro.x<<", "<<this->centro.y<<std::endl;

		Poligono_2d_vertices<T>::centrar_en(v);

std::cout<<"CON DIFERENCIA DE "<<vec.x<<", "<<vec.y<<std::endl;

		for(auto &s : segmentos) s.desplazar({vec.x, vec.y});
	}

	virtual void			rotar(T grados)
	{
		Poligono_2d_vertices<T>::rotar(grados);
		recrear_segmentos();
		
	}

	virtual void			insertar_vertice(const tpunto& p)
	{
		Poligono_2d_vertices<T>::insertar_vertice(p);
		if(this->vertices.size() > 1) 
			crear_segmento(this->vertices[this->vertices.size()-2], p);

	}

	void				cerrar()
	{
		crear_segmento(this->vertices[this->vertices.size()-1], this->vertices[0]);
	}

	Proyeccion_poligono<T>		proyectar(Vector_2d<T> eje) const
	{
		eje.normalizar();

		//Precálculo...
		const auto &v=this->vertices[0];
		T vmin=producto_vectorial(Vector_2d<T>{v.x, v.y}, eje), vmax=vmin;

		for(const auto& v : this->vertices)
		{
			if(v==*std::begin(this->vertices)) continue; //Saltamos la primera iteración....
			T pro=producto_vectorial(Vector_2d<T>{v.x, v.y}, eje);
			if(pro < vmin) vmin=pro; 
			if (pro > vmax) vmax=pro;
		}
		return Proyeccion_poligono<T>{vmin, vmax};
	}

	const std::vector<Segmento_2d<T> >&	acc_segmentos() const 
	{
		return segmentos;
	}

	private:

	void				crear_segmento(tpunto p1, tpunto p2)
	{
		segmentos.push_back(Segmento_2d<T>{p1, p2});
	}

	void				recrear_segmentos()
	{
		if(this->vertices.size() < 3) throw std::runtime_error("Polígono inválido... menos de tres vértices");

		segmentos.clear();
		size_t i=0;
		while(i < this->vertices.size()-1)
		{
			crear_segmento(this->vertices[i], this->vertices[i+1]);
			++i;
		}

		cerrar();
	}

	std::vector<Segmento_2d<T> > segmentos;
	friend bool colision_poligono_SAT<T>(const Poligono_2d<T>& a,const Poligono_2d<T>& b);
};

template<typename T>
bool colision_poligono_SAT(const Poligono_2d<T>& a,const Poligono_2d<T>& b)
{
	auto f=[](const Poligono_2d<T>& pa, const Poligono_2d<T>& pb)
	{
		for(const auto& s : pa.segmentos)
		{
			auto eje=s.direccion.perpendicular(); 				//Normal del vector...
			auto proy_a=pa.proyectar(eje), proy_b=pb.proyectar(eje); 	//Localizar proyecciones en la normal...
			if(!hay_superposicion(proy_a, proy_b)) return false;
		}
		return true;
	};

	if(!f(a, b)) return false;
	else if(!f(b, a)) return false;
	else return true;
}

}
#endif
