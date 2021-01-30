#include <ldv/bitmap_representation.h>

using namespace ldv;

//!Default constructor.

bitmap_representation::bitmap_representation()
	:raster_representation()
{
}

//!Constructs the representation from a texture, position and clipping rectangle.

bitmap_representation::bitmap_representation(const texture& p_rec, rect pos, rect rec)
	:raster_representation(pos, rec)
{
	set_texture(p_rec);

	if(!pos.w) base_view_position.w=clip.w;
	if(!pos.h) base_view_position.h=clip.h;

}
