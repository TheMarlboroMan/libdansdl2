#include <ldv/draw.h>
#include <ldv/box_representation.h>
#include <ldv/line_representation.h>

#include <iostream>

using namespace ldv;

draw::draw(
	screen& _screen,
	camera* _camera,
	modes _mode
):
	black{rgba8(0, 0, 0, 255)},
	white{rgba8(255, 255, 255, 255)},
	red{rgba8(255, 0, 0, 255)},
	green{rgba8(0, 255, 0, 255)},
	blue{rgba8(0, 0, 255, 255)},
	orange{rgba8(255, 128, 65, 255)},
	pink{rgba8(255, 105, 180, 255)},
	gray{rgba8(128, 128, 128, 255)},
	scr{_screen},
	cam{_camera},
	mode{_mode}
{

}

draw& draw::clear(
	const ldv::rgba_color& _color
) {

	scr.clear(_color);
	return *this;
}

draw& draw::box(
	const tbox& _box,
	const ldv::rgba_color& _color
) {

	return box(
		(int)_box.origin.x,
		(int)_box.origin.y,
		(int)_box.w, 
		(int)_box.h,
		_color
	);
}

draw& draw::box_bordered(
	const tbox& _box,
	const ldv::rgba_color& _outline,
	const ldv::rgba_color& _fill
) {

	return box_bordered(
		(int)_box.origin.x,
		(int)_box.origin.y,
		(int)_box.w, 
		(int)_box.h,
		_outline,
		_fill
	);
}

draw& draw::box_outline(
	const tbox& _box,
	const ldv::rgba_color& _color
) {

	return box_outline(
		(int)_box.origin.x,
		(int)_box.origin.y,
		(int)_box.w, 
		(int)_box.h,
		_color
	);
}

draw& draw::box(
	int _x,
	int _y,
	int _w,
	int _h,
	const ldv::rgba_color& _color
) {

	int y=mode==modes::cartesian
		? -1*(int)_y-(int)_h
		: (int)_y;

	ldv::box_representation rep{
		{_x, y, (unsigned)_w, (unsigned)_h},
		_color
	};

	cam==nullptr
		? rep.draw(scr)
		: rep.draw(scr, *cam);

	return *this;
}

draw& draw::box_bordered(
	int _x,
	int _y,
	int _w,
	int _h,
	const ldv::rgba_color& _outline,
	const ldv::rgba_color& _fill
) {

	return box(_x, _y, _w, _h, _fill);
	return box_outline(_x, _y, _w, _h, _outline);
}

draw& draw::box_outline(
	int _x,
	int _y,
	int _w,
	int _h,
	const ldv::rgba_color& _color
) {

	int y=mode==modes::cartesian
		? -1*(int)_y-(int)_h
		: (int)_y;

	ldv::box_representation rep{
		{_x, y, (unsigned)_w, (unsigned)_h},
		_color,
		ldv::box_representation::type::line
	};

	cam==nullptr
		? rep.draw(scr)
		: rep.draw(scr, *cam);

	return *this;
}

draw& draw::ray(
	const tray& _ray,
	const ldv::rgba_color& _color
) {

	const auto end=_ray.end();

	return line(
		(int)_ray.point.x,
		(int)_ray.point.y,
		(int)end.x,
		(int)end.y,
		_color
	);
}

draw& draw::ray(
	int _bx, 
	int _by,
	int _ex,
	int _ey,
	const ldv::rgba_color& _color
) {

	return line(_bx, _by, _ex, _ey, _color);
}

draw& draw::line(
	const tpoint& _begin,
	const tpoint& _end,
	const ldv::rgba_color& _color
) {

	return line(
		(int)_begin.x,
		(int)_begin.y,
		(int)_end.x,
		(int)_end.y,
		_color
	);
}

draw& draw::line(
	int _bx,
	int _by,
	int _ex,
	int _ey,
	const ldv::rgba_color& _color
) {

	int by=mode==modes::cartesian
		? -_by : _by;
	int ey=mode==modes::cartesian
		? -_ey : _ey;

	ldv::line_representation rep{
		{_bx, by}, {_ex, ey},
		_color
	};


	rep.set_blend(ldv::representation::blends::alpha);

	cam==nullptr
		? rep.draw(scr)
		: rep.draw(scr, *cam);

	return *this;
}
