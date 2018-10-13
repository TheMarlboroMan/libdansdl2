#ifndef LIBDANSDL2_PRIMITIVE_REPRESENTATION_H
#define LIBDANSDL2_PRIMITIVE_REPRESENTATION_H

#include "../representation.h"
#include "../../color/color.h"
#include "../../../tools/sdl_tools/sdl_tools.h"

namespace ldv
{

//!Base class for all primitives.

class primitive_representation:
	public representation
{
	public:

			primitive_representation(rgba_color);
			primitive_representation(rgb_color);
			primitive_representation(const primitive_representation&);
			primitive_representation& operator=(const primitive_representation&);
	virtual 	~primitive_representation() {}
	
	//!Assigns color (line, fill, dot... everything).
	void		set_color(rgb_color c) {color=c;}

	//Assigns color plus alpha.
	void		set_color(rgba_color _c) {
		set_color(rgb_color(_c.r, _c.g, _c.b));
		set_alpha(_c.a * 255.f);
	}

	virtual void 	go_to(point)=0;
	virtual point	get_position() const=0;
	virtual rect	get_base_view_position() const=0;

	protected:

	void 		do_color();
	virtual void 	do_draw()=0;

	private:

	rgb_color	color;
};

} //Fin namespace DLibV

#endif
