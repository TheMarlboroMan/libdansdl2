#include "primitive_representation.h"

using namespace ldv;

primitive_representation::primitive_representation(rgba_color c)
	:representation(c)
{

}

primitive_representation::primitive_representation(const primitive_representation& o)
	:representation(o)
{

}

primitive_representation& primitive_representation::operator=(const primitive_representation& o)
{
	representation::operator=(o);
	return *this;
}

void primitive_representation::do_color()
{
	const auto c=get_rgba();
	switch(get_blend())
	{
		case representation::blends::none:
			glDisable(GL_BLEND);
			glColor3f(c.r, c.g, c.b);
		break;
		case representation::blends::alpha:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(c.r, c.g, c.b, c.a);
		break;
	}

	glDisable(GL_TEXTURE_2D);
}
