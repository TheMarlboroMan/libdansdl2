#pragma once

#include <ldt/box.h>
#include <SDL2/SDL.h>

namespace ldv
{
//!Video specialization of a box. 

//!It must be understood that the origin is at the top left and the width
//!and height extend to the right and down respectively, regardless of the
//!internal coordinate system used by the application.

typedef ldt::box<int, unsigned int> 	rect;

//!Video specialization of a point.

typedef ldt::point_2d<int>		point;
}

