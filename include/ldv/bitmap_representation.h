#pragma once

#include "raster_representation.h"

namespace ldv
{

//!Representation of a loaded image.

//!Implemented in terms of its parent class almost entirely.

class bitmap_representation:
	public raster_representation
{
	public:

			bitmap_representation();
			bitmap_representation(const texture&, rect={0,0,0,0}, rect={0,0,0,0});
};

} //Fin namespace DLibV
