#include "color.h"

using namespace DLibV;

ColorRGBA DLibV::rgba8(int pr, int pg, int pb, int pa)
{
	float r=(float)pr / 255.f;
	float g=(float)pg / 255.f;
	float b=(float)pb / 255.f;
	float a=(float)pa / 255.f;

	return ColorRGBA{r, g, b, a};
}
