#ifndef LIBDANSDL2_COLOR_H
#define LIBDANSDL2_COLOR_H

namespace ldv
{

struct rgba_color
{
	float r, g, b, a;
};

struct rgb_color
{
	float r, g, b;
};

rgba_color rgba8(int, int, int, int);
rgb_color rgb8(int, int, int);
float colorfi(int);
int colorif(float);

}

#endif
