#ifndef PRIMITIVA_GRAFICA_CAJA_H
#define PRIMITIVA_GRAFICA_CAJA_H

#include "../representacion_primitiva_poligono/representacion_primitiva_poligono.h"

//Simplemente un caso especial de un polígono.

namespace DLibV
{
class Representacion_primitiva_caja
	:public Representacion_primitiva_poligono
{
	public:
	
				Representacion_primitiva_caja(const Rect& p_pos, const ColorRGBA&);
				Representacion_primitiva_caja(const Representacion_primitiva_caja& p_otra);
				Representacion_primitiva_caja& operator=(const Representacion_primitiva_caja& p_otro);
	virtual 		~Representacion_primitiva_caja() {}
};

class Representacion_primitiva_caja_lineas
	:public Representacion_primitiva_poligono_lineas
{
	public:

				Representacion_primitiva_caja_lineas(const Rect& p_pos, const ColorRGBA&);
				Representacion_primitiva_caja_lineas(const Representacion_primitiva_caja_lineas& p_otra);
				Representacion_primitiva_caja_lineas& operator=(const Representacion_primitiva_caja_lineas& p_otro);
	virtual 		~Representacion_primitiva_caja_lineas() {}
};


}

#endif
