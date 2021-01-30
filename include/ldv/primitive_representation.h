#pragma once

#include "representation.h"
#include "color.h"
#include <ldt/sdl_tools.h>

namespace ldv
{

//!Base class for all primitives.

class primitive_representation:
	public representation
{
	public:

	                primitive_representation(rgba_color);
	                primitive_representation(rgb_color);

	//!Assigns color (line, fill, dot... everything).
	void            set_color(rgb_color c) {color=c;}

	//Assigns color plus alpha.
	void            set_color(rgba_color _c) {
		set_color(rgb_color(_c.r, _c.g, _c.b));
		set_alpha(_c.a * 255.f);
	}

	virtual void    go_to(point)=0;
	virtual const   point& get_position() const=0;

	protected:

	void            do_color();
	virtual void    do_draw()=0;

	private:

	rgb_color       color;
};

} //Fin namespace DLibV

