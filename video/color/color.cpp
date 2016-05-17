#include "color.h"

using namespace DLibV;

float DLibV::colorfi(int v)
{
	return (float)v / 255.f;
}

unsigned int DLibV::colorif(float v)
{
	return v * 255.f;
}

ColorRGBA DLibV::rgba8(int r, int g, int b, int a)
{
	return ColorRGBA{colorfi(r), colorfi(g), colorfi(b), colorfi(a)};
}
