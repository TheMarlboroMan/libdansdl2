#include "primitive_representation.h"

using namespace ldv;

primitive_representation::primitive_representation(rgba_color c)
	:representation(c.a),
	color{c.r, c.g, c.b}
{

}

primitive_representation::primitive_representation(rgb_color c)
	:representation(),
	color(c)
{

}

primitive_representation::primitive_representation(const primitive_representation& o)
	:representation(o),
	color(o.color)
{

}

primitive_representation& primitive_representation::operator=(const primitive_representation& o)
{
	representation::operator=(o);
	color=o.color;
	return *this;
}

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
