#ifndef LIBDANSDL2_PRIMITIVE_REPRESENTATION_H
#define LIBDANSDL2_PRIMITIVE_REPRESENTATION_H

#include "../representation.h"
#include "../../color/color.h"
#include "../../../tools/sdl_tools/sdl_tools.h"

/*Esto será una clase abstracta que podremos usar para volcar
en una pantalla... De por si no hay nada de código aquí, solo
los esqueletos de lo que vamos a usar más adelante.
*/

namespace ldv
{

class primitive_representation:
	public representation
{
	public:

			primitive_representation(rgba_color);
			primitive_representation(const primitive_representation&);
			primitive_representation& operator=(const primitive_representation&);
	virtual 	~primitive_representation() {}

	//Estas hay que definirlas.
	virtual void 	go_to(int x, int y)=0;
	virtual point	get_position() const=0;
	virtual rect	get_base_view_position() const=0;

	protected:

	void 		do_color();
	virtual void 	do_draw()=0;
};

} //Fin namespace DLibV

#endif
