#ifndef LIDBANSDL2_RECT_H
#define LIDBANSDL2_RECT_H

#include <SDL2/SDL.h>
#include "../../tools/box/box.h"

namespace ldv
{
///Video specialization of a box.
typedef ldt::box<int, unsigned int> 	rect;
///Video specialization of a point.
typedef ldt::point_2d<int>		point;
}

#endif
