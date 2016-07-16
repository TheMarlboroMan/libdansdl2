#include "camera.h"

using namespace ldv;

camera::camera(rect foco, point pos):
	d_info{pos.x, pos.y, foco.origin.x, foco.origin.y, (int)foco.w, (int)foco.h, 1.0}, 
	focus_box(foco), pos_box(pos.x, pos.y, focus_box.w, focus_box.h),
	with_limit(false), limits{0,0,0,0}
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

/*Mueve la posición a la que apunta la cámara sumando las cantidades provistas
en los parámetros.*/

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

void camera::clear_limits()
{
	with_limit=false;
}

point camera::transform(point p) const
{
	point res(p);
	res.x=focus_box.origin.x + (p.x * (focus_box.w / (float) pos_box.w));
	res.y=focus_box.origin.y + (p.y * (focus_box.h / (float) pos_box.h));
	return res;
}

//Un valor de 2 significa que todo es 2 veces más grande, ergo la caja de foco es dos veces más pequeña.
void camera::set_zoom(double v)
{
	if(v < 0.01) v=0.01;
	d_info.zoom=v;
	focus_box.w=pos_box.w / v;
	focus_box.h=pos_box.h / v;
}
