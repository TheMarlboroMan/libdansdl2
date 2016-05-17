#ifndef REPRESENTACION_PRIMITIVA_H
#define REPRESENTACION_PRIMITIVA_H

#include "../representacion.h"
#include "../../color/color.h"
#include "../../../herramientas/herramientas_sdl/herramientas_sdl.h"

/*Esto será una clase abstracta que podremos usar para volcar
en una pantalla... De por si no hay nada de código aquí, solo
los esqueletos de lo que vamos a usar más adelante.
*/

namespace DLibV
{

class Representacion_primitiva:public Representacion
{
	public:

	struct punto{int x, y;};
			Representacion_primitiva(ColorRGBA);
			Representacion_primitiva(const Representacion_primitiva&);
			Representacion_primitiva& operator=(const Representacion_primitiva&);
	virtual 	~Representacion_primitiva() {}

	bool 		es_recomponer_recorte_con_posicion() const {return recomponer_recorte_con_posicion;}
	void 		mut_recomponer_recorte_con_posicion(bool p_valor) {this->recomponer_recorte_con_posicion=p_valor;}	
	void 		recorte_a_posicion();

	protected:
	
	virtual void 	preparar_posicion()=0; //Este método es obligatorio implementarlo para decirle cómo de grande es la caja.

	void 		preparar_color();
	bool 		determinar_caja_dibujo_final(SDL_Rect &, SDL_Rect const&, SDL_Rect const&);

	private:

	bool 		recomponer_recorte_con_posicion;
};

} //Fin namespace DLibV

#endif
