#ifndef PRIMITIVA_GRAFICA_PUNTOS_H
#define PRIMITIVA_GRAFICA_PUNTOS_H

#include "../representacion_primitiva.h"
#include <vector>

namespace DLibV
{
class Representacion_primitiva_puntos:
	public Representacion_primitiva
{
	public:
	
						Representacion_primitiva_puntos(ColorRGBA);
						Representacion_primitiva_puntos(int, int, ColorRGBA);
						Representacion_primitiva_puntos(const Representacion_primitiva_puntos&);
	virtual 				~Representacion_primitiva_puntos() {}
						Representacion_primitiva_puntos& operator=(const Representacion_primitiva_puntos&);

	virtual void				volcado(const Info_volcado);

	void 					insertar(int x, int y);
	void					limpiar_puntos();

	virtual void 				ir_a(int x, int y);
	virtual Punto				obtener_posicion() const;

	protected:

	virtual Rect				obtener_base_posicion_vista() const;

	private:

	std::vector<Punto>			puntos;
	Punto					original;
};

}

#endif
