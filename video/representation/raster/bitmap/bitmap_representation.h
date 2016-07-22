#ifndef LIBDANSDL2_BITMAP_REPRESENTATION
#define LIBDANSDL2_BITMAP_REPRESENTATION

#include "../raster_representation.h"

namespace ldv
{

class bitmap_representation:
	public raster_representation
{
	public:

			bitmap_representation();
			bitmap_representation(const texture&, rect={0,0,0,0}, rect={0,0,0,0}, sampling=sampling::atlas);
			bitmap_representation(const bitmap_representation&);
			bitmap_representation& operator=(const bitmap_representation &);

//	void 		set_texture_no_clip(const texture& r){raster_representation::set_texture(r);}
};

} //Fin namespace DLibV

#endif
