#ifndef LIBDANSDL2_COLOR_H
#define LIBDANSDL2_COLOR_H

namespace ldv
{

///A struct of r,g,b and a where 0.0f is 0 and 1.f is 255. Useful to communicate with OpenGL.
struct rgba_color
{
	float r, g, b, a;
};

///A struct of r,g and b where 0.0f is 0 and 1.f is 255. Useful to communicate with OpenGL.
struct rgb_color
{
	float r, g, b;
};

///Gets rgba_color from integer values (rgba_color is expressed in terms of float values).
rgba_color rgba8(int, int, int, int);
///Gets rgb_color from integer values (rgb_color is expressed in terms of float values).
rgb_color rgb8(int, int, int);
///Converts an integer in the range 0-255 to a float in the range 0.f 1.f. Ranges are not enforced.
float colorfi(int);
///Converts a float in the range 0.f-1.f to 0-255. Ranges are not enforced.
int colorif(float);

}

#endif
