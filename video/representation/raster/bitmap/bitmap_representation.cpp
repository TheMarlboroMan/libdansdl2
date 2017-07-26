#include "bitmap_representation.h"

using namespace ldv;

bitmap_representation::bitmap_representation()
	:raster_representation()
{
	update_view_position();
}

bitmap_representation::bitmap_representation(const texture& p_rec, rect pos, rect rec)
	:raster_representation(pos, rec, s)
{
	set_texture(p_rec);
	
	if(!pos.w) location.w=clip.w;
	if(!pos.h) location.h=clip.h;

	update_view_position();
}

bitmap_representation::bitmap_representation(const bitmap_representation& o)
	:raster_representation(o)
{

}

bitmap_representation& bitmap_representation::operator=(const bitmap_representation & o)
{
	raster_representation::operator=(o);
	return *this;
}
