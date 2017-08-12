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

//!Moves the top-left point of the focus box.

//!If the camera is limited, movement may not happen... Also, there is a slight
//!chance that the camera will go out of limits into the negative space: if
//!the focus box has a dimension smaller than the camera, the camera will try
//!to center on it, triggering this behaviour and creating sort of a 
//!"letterbox" effect.

void camera::go_to(point p)
{
	if(with_limit)
	{
		//Params are p.x, focus_box.w, limits.min_x, limits.max_x, focus_box.origin.x
		auto task=[](int pos, int dimension, int limit_min, int limit_max, int &target)
		{
			int fin=pos + dimension;
			if(pos < limit_min) 
				target=limit_min;
			else if(fin > limit_max) 
				target=limit_max-dimension;
			else	//pos >= limit_min && fin  <= limit_max
				target=pos;
		};

		//If the dimension if smaller than the dimension set by the limits the task is run.
		//else, center the camera on the dimension.

		if((int)focus_box.w <= limits.max_x - limits.min_x) task(p.x, focus_box.w, limits.min_x, limits.max_x, focus_box.origin.x);
		else focus_box.origin.x=((limits.max_x-limits.min_x)-(int)focus_box.w) / 2;

		if((int)focus_box.h <= limits.max_y - limits.min_y) task(p.y, focus_box.h, limits.min_y, limits.max_y, focus_box.origin.y);
		else focus_box.origin.y=((limits.max_y-limits.min_y)-(int)focus_box.h) / 2;
	}
	else
	{
		focus_box.origin={p.x, p.y};
	}

	sync();
}

///Moves the top-left point of the focus box by the specified parameters (x and y).

void camera::move_by(int p_x, int p_y)
{
	go_to(focus_box.origin + point{p_x, p_y});
}

//!Sets a limit to camera movement.

//!The values are considered "world space". 

void camera::set_limits(const rect& r)
{
	with_limit=true;
	limits={r.origin.x, r.origin.y, r.origin.x+(int)r.w, r.origin.y+(int)r.h};
}

//!Removes camera movement limits in world space.

void camera::clear_limits()
{
	with_limit=false;
}

void camera::set_zoom(double v)
{
	if(v < 0.01) v=0.01;
	d_info.zoom=v;
	focus_box.w=pos_box.w / v;
	focus_box.h=pos_box.h / v;
}

//!Centers the focus box on the given point.

//!If limit margins are in effect, the focused position may be different from
//!the one requested.

void camera::center_on(point p)
{
	point dest(p.x-(focus_box.w/2), p.y-(focus_box.h/2));

	if(with_margin)
	{
		//Limit margin is expressed in terms of pos_box...
		//P must be converted to the same terms.
		auto pt=world_to_pos(p);

		if(limit_margin.point_inside(pt))
		{
			return;
		}
		else
		{
			int x=0, y=0;

			if(pt.x < limit_margin.origin.x) x=pt.x-limit_margin.origin.x;
			else if(pt.x > limit_margin.origin.x+(int)limit_margin.w) x=pt.x-limit_margin.origin.x-limit_margin.w;

			if(pt.y < limit_margin.origin.y) y=pt.y-limit_margin.origin.y;
			else if(pt.y > limit_margin.origin.y+(int)limit_margin.h) y=pt.y-limit_margin.origin.y-limit_margin.h;

			move_by(x, y);
		}
	}
	else
	{
		go_to(dest);
	}
}

//!Centers the focus box on the middle of the rect.

//!If limit margins are in effect, the focused position may be different from
//!the one requested.

void camera::center_on(const rect& r)
{
	point dest(r.origin.x+(r.w/2), r.origin.y+(r.h/2));
	center_on(dest);
}

//!Sets a margin for camera movement when using center_on functions. 

//!Objects still within the margin will cause no camera movement. 
//!The rect is interpreted as relative to pos_box.

void camera::set_center_margin(const rect& r)
{
	with_margin=true;
	limit_margin=r;
}

//!Clears the margin for camera movement.

void camera::clear_center_margin()
{
	with_margin=false;
	limit_margin={0,0,0,0};
}

//!Converts world point to pos point

//!Where pos point is absolute position on the screen.

point camera::world_to_pos(point p) const
{
	return p-focus_box.origin;
}

//!Converts world rect to pos rect

//!Where pos rect is absolute position on the screen.

rect camera::world_to_pos(const rect& r) const
{
	auto orig=world_to_pos(r.origin);
	return {orig.x, orig.y, r.w, r.h};
}

/*
//!Converts pos point to world point

//!Where pos point is absolute position on the screen.

point camera::pos_to_world(point p) const
{
	return p+focus_box.origin;
}

rect camera::pos_to_world(const rect& r) const
{

}
*/
