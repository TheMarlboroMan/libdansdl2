#ifndef PRIMITIVA_GRAFICA_POLIGONO_H
#define PRIMITIVA_GRAFICA_POLIGONO_H

/*
*/

#include <vector>

#include "../representacion_primitiva.h"

namespace DLibV
{
class Representacion_primitiva_poligono_base
	:public Representacion_primitiva
{
	public:
					Representacion_primitiva_poligono_base(const std::vector<Punto>&, ColorRGBA);
					Representacion_primitiva_poligono_base(const Representacion_primitiva_poligono_base&);
					Representacion_primitiva_poligono_base& operator=(const Representacion_primitiva_poligono_base&);
	virtual 			~Representacion_primitiva_poligono_base() {}
	virtual bool 			es_rellena() const=0;

	virtual void			volcado(const Info_volcado);
	virtual void 			ir_a(int x, int y);
	virtual Punto			obtener_posicion() const;

	protected:

	virtual Rect			obtener_base_posicion_vista() const;

	private:

	std::vector<Punto>		puntos;
	Punto				original;	//Guarda el primer punto original sin estar en 0.0.
};

class Representacion_primitiva_poligono
	:public Representacion_primitiva_poligono_base
{
	public:
	
				Representacion_primitiva_poligono(const std::vector<Punto>&, ColorRGBA);
				Representacion_primitiva_poligono(const Representacion_primitiva_poligono& p_otra);
				Representacion_primitiva_poligono& operator=(const Representacion_primitiva_poligono& p_otro);
	virtual 		~Representacion_primitiva_poligono() {}

	bool 			es_rellena() const {return true;}
};

class Representacion_primitiva_poligono_lineas
	:public Representacion_primitiva_poligono_base
{
	public:

				Representacion_primitiva_poligono_lineas(const std::vector<Punto>&, ColorRGBA);
				Representacion_primitiva_poligono_lineas(const Representacion_primitiva_poligono_lineas& p_otra);
				Representacion_primitiva_poligono_lineas& operator=(const Representacion_primitiva_poligono_lineas& p_otro);
	virtual 		~Representacion_primitiva_poligono_lineas() {}

	bool 			es_rellena() const {return false;}
};

}

#endif
