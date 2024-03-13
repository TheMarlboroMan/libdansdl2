#include "ldv/draw_info.h"

using namespace ldv;

std::ostream& ldv::operator<<(
	std::ostream& _stream,
	const draw_info& _drawinfo
) {

	_stream<<"ldv::draw_info[screen:"<<_drawinfo.pos_x<<","<<_drawinfo.pos_y
		<<" focus:"<<_drawinfo.rel_x<<","<<_drawinfo.rel_y<<" "<<_drawinfo.view_w<<"x"<<_drawinfo.view_h
		<<" zoom: "<<_drawinfo.zoom;

	return _stream;
}
