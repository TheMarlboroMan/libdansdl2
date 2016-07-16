#ifndef LIBDANSDL2_COLOR_H
#define LIBDANSDL2_COLOR_H

namespace ldv
{

struct rgba_color
{
	float r, g, b, a;
};

rgba_color rgba8(int, int, int, int);
float colorfi(int);
int colorif(float);

}

#endif
