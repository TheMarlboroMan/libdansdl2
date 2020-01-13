#include "bitmap_representation.h"

using namespace ldv;

//!Default constructor.

bitmap_representation::bitmap_representation()
	:raster_representation()
{
	update_view_position();
}

//!Constructs the representation from a texture, position and clipping rectangle.

bitmap_representation::bitmap_representation(const texture& p_rec, rect pos, rect rec)
	:raster_representation(pos, rec)
{
	set_texture(p_rec);
	
	if(!pos.w) location.w=clip.w;
	if(!pos.h) location.h=clip.h;

	update_view_position();
}

//!Copy constructor.

bitmap_representation::bitmap_representation(const bitmap_representation& o)
	:raster_representation(o)
{

}

//!Assignment operator.

bitmap_representation& bitmap_representation::operator=(const bitmap_representation & o)
{
	raster_representation::operator=(o);
	return *this;
}
