#include "color.h"

using namespace ldv;

//Entra 0-255 sale 0-1
//color float from integer
float ldv::colorfi(int v)
{
	return (float)v / 255.f;
}

//Entra 0-1 sale 0-255
//Color integer from float.
int ldv::colorif(float v)
{
	return v * 255.f;
}

rgba_color ldv::rgba8(int r, int g, int b, int a)
{
	return rgba_color{colorfi(r), colorfi(g), colorfi(b), colorfi(a)};
}

rgb_color ldv::rgb8(int r, int g, int b)
{
	return rgb_color{colorfi(r), colorfi(g), colorfi(b)};
}
