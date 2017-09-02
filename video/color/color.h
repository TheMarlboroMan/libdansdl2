#ifndef LIBDANSDL2_COLOR_H
#define LIBDANSDL2_COLOR_H

namespace ldv
{

//!A struct of r,g,b and a expressed in floats.

//!0.0f is 0 and 1.f is 255. Useful to communicate with OpenGL.

struct rgba_color
{
	float 		r, g, b, a;
	bool		operator==(const rgba_color& o) const {return o.r==r && o.g==g && o.b==b && o.a==a;}
};

//!A struct of r,g and b expressed in floats.

//!0.0f is 0 and 1.f is 255. Useful to communicate with OpenGL.

struct rgb_color
{
	float 		r, g, b;
	bool		operator==(const rgb_color& o) const {return o.r==r && o.g==g && o.b==b;}
};

rgba_color rgba8(int, int, int, int);
rgb_color rgb8(int, int, int);
float colorfi(int);
int colorif(float);

}

#endif
