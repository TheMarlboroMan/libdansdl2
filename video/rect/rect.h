#ifndef LIDBANSDL2_RECT_H
#define LIDBANSDL2_RECT_H

#include <SDL2/SDL.h>
#include "../../tools/box/box.h"

namespace ldv
{
typedef ldt::box<int, unsigned int> 	rect;
typedef ldt::point_2d<int>		point;
}

#endif
