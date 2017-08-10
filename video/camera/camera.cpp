#include "camera.h"

using namespace ldv;

camera::camera(rect foco, point pos):
	d_info{pos.x, pos.y, foco.origin.x, foco.origin.y, (int)foco.w, (int)foco.h, 1.0}, 
	focus_box(foco), pos_box(pos.x, pos.y, focus_box.w, focus_box.h),
	with_limit(false), limits{0,0,0,0}, with_margin(false), limit_margin{0,0,0,0}
{
	sync();
}

void camera::sync()
{
	d_info.pos_x=pos_box.origin.x;
	d_info.pos_y=pos_box.origin.y;
	d_info.rel_x=focus_box.origin.x;
	d_info.rel_y=focus_box.origin.y;
	d_info.view_w=focus_box.w;
	d_info.view_h=focus_box.h;
}

/*Mueve la posición a la que apunta la cámara en la pantalla. Se usan las
coordenadas provistas.*/

void camera::go_to(point p)
{
	if(with_limit)
	{
		auto procesar=[](int pos, int dimension, int limit_min, int limit_max, int &target)
		{
			int fin=pos + dimension;
			if(pos >= limit_min && fin  <= limit_max) target=pos;
			else if(pos < limit_min) target=limit_min;
			else if(fin > limit_max) target=limit_max-dimension;
		};

		if((int)focus_box.w <= limits.max_x - limits.min_x) procesar(p.x, focus_box.w, limits.min_x, limits.max_x, focus_box.origin.x);
		if((int)focus_box.h <= limits.max_y - limits.min_y) procesar(p.y, focus_box.h, limits.min_y, limits.max_y, focus_box.origin.y);
	}
	else
	{
		focus_box.origin.x=p.x;
		focus_box.origin.y=p.y;
	}

	sync();
}


void camera::move_by(int p_x, int p_y)
{
	go_to({focus_box.origin.x+p_x, focus_box.origin.y+p_y});
}

/*Establece los límites a los que la cámara puede apuntar: es decir, los límites
máximos para la cámara dentro del "mundo".*/

void camera::set_limits(int min_x, int min_y, int max_x, int max_y)
{
	with_limit=true;
	limits={min_x, min_y, max_x, max_y};
}

void camera::set_limits(const rect& r)
{
	with_limit=true;
	limits={r.origin.x, r.origin.y, r.origin.x+(int)r.w, r.origin.y+(int)r.h};
}

void camera::clear_limits()
{
	with_limit=false;
}

//Good... why would I ever document what this does???.
//Okay, this converts a point to a camera point?.

point camera::transform(point p) const
{
	point res(p);
	res.x=focus_box.origin.x + (p.x * (focus_box.w / (float) pos_box.w));
	res.y=focus_box.origin.y + (p.y * (focus_box.h / (float) pos_box.h));
	return res;
}

void camera::set_zoom(double v)
{
	if(v < 0.01) v=0.01;
	d_info.zoom=v;
	focus_box.w=pos_box.w / v;
	focus_box.h=pos_box.h / v;
}

void camera::center_on(point p)
{
	point dest(p.x-(focus_box.w/2), p.y-(focus_box.h/2));

	if(with_margin && limit_margin.point_inside(p))
	{
		return;
	}

	auto cosa=transform(dest);
	std::cout<<p.x<<","<<p.y<<" IS "<<dest.x<<","<<dest.y<<" TRANS: "<<cosa.x<<","<<cosa.y<<std::endl;

	go_to(dest);
}

void camera::center_on(const rect& r)
{
	point dest(r.origin.x+(r.w/2), r.origin.y+(r.h/2));
	center_on(dest);
}

void camera::set_center_margin(const rect& r)
{
	with_margin=true;
	limit_margin=r;
}

void camera::clear_center_margin()
{
	with_margin=false;
	limit_margin={0,0,0,0};
}
