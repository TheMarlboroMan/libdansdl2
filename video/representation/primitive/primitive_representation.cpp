#include "primitive_representation.h"

using namespace ldv;

//!Constructs the primitive with colour and alpha.

primitive_representation::primitive_representation(rgba_color c)
	:representation(colorif(c.a)),
	color{c.r, c.g, c.b}
{

}

//!Constructs the primitive with colour. 

//!Alpha will be set to max by default.

primitive_representation::primitive_representation(rgb_color c)
	:representation(),
	color(c)
{

}

//!Copy constructor.

primitive_representation::primitive_representation(const primitive_representation& o)
	:representation(o),
	color(o.color)
{

}

//!Assignment operator.

primitive_representation& primitive_representation::operator=(const primitive_representation& o)
{
	representation::operator=(o);
	color=o.color;
	return *this;
}

//!Sets the primitive color.

//!Primitives are only assigned one color throughout. Alpha is applied if the
//!blend mode says so.

void primitive_representation::do_color()
{
	switch(get_blend())
	{
		case representation::blends::none:
			glDisable(GL_BLEND);
			glColor3f(color.r, color.g, color.b);
		break;
		case representation::blends::alpha:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(color.r, color.g, color.b, get_alphaf());
		break;
	}

	glDisable(GL_TEXTURE_2D);
}
