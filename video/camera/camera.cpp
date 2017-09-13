#include "camera.h"

using namespace ldv;

camera::camera(rect foco, point pos):
	d_info{pos.x, pos.y, foco.origin.x, foco.origin.y, (int)foco.w, (int)foco.h, 1.0}, 
	focus_box(foco), pos_box(pos.x, pos.y, focus_box.w, focus_box.h),
	with_limit(false), limits{0,0,0,0}, with_margin(false), limit_margin{0,0,0,0},
	coordinate_system(tsystem::screen)
{
	set_coordinate_system(coordinate_system);
	sync();
}

//!Syncs camera boxes and performs neccesary coordinate system conversions.

//!This is a private function. Basically feeds a draw_info struct with the 
//!camera values and converts its y coordinates to screen space if neccesary.

void camera::sync()
{
	d_info.pos_x=pos_box.origin.x;
	d_info.pos_y=pos_box.origin.y;
	d_info.rel_x=focus_box.origin.x;
	d_info.rel_y=coordinate_system==tsystem::screen ? 
		focus_box.origin.y : 
		-focus_box.origin.y-focus_box.h; //Set to screen space.
	d_info.view_w=focus_box.w;
	d_info.view_h=focus_box.h;
}

//!Moves the top-left point of the focus box.

//!All movement functions are internally implemented on terms of this.
//!If the camera is limited, movement may not happen... Also, there is a slight
//!chance that the camera will go out of limits into the limited space: if
//!the focus box has a dimension smaller than the camera, the camera will try
//!to center on it, triggering this behaviour and creating sort of a 
//!"letterbox" effect. 

void camera::go_to(point p)
{
	if(with_limit)
	{
		//If the dimension of the limits if smaller than the focus
		//we'll center on the limits. Otherwise, we set the origin as
		//necessary respecting the limits.

		auto calculate=[this](int foc_dimension, int limit_origin, int limit_dimension, int pos)
		{
			if(foc_dimension <= limit_dimension) //Set the origin in the proposed point, according to the limits...
			{
				int 	end=pos + foc_dimension,
					limit_max=limit_origin+limit_dimension;

				if(pos < limit_origin) return limit_origin;
				else if(end > limit_max) return limit_max-foc_dimension;
				else return pos;
			}
			else //Set the focus origin so it centers on the limits.
			{
				return ((limit_dimension-foc_dimension) / 2) + limit_origin;
			}
		};
		
		focus_box.origin.x=calculate(focus_box.w, limits.origin.x, limits.w, p.x);
		focus_box.origin.y=calculate(focus_box.h, limits.origin.y, limits.h, p.y);
	}
	else
	{
		focus_box.origin={p.x, p.y};
	}

	sync();
}

//!Centers the focus box on the given point.

//!If limit margins are in effect, the focused position may be different from
//!the one requested. Coordinate systems for the camera are irrelevant as it
//!will focus in the given position.

void camera::center_on(point p)
{
	if(with_margin)
	{
		//Limit margin is expressed in terms of pos_box so the point must be converted.
		auto pt=world_to_pos(p);

		if(limit_margin.point_inside(pt))
		{
			return;
		}
		else
		{
			//Calculate the necessary difference. Everything
			//here is expressed in screen values, so there is no coordinate
			//system to take care of.
			int x=0, y=0; //These are zeroed in case no changes are made... at the end we just add a 0.0 point.

			auto calculate_diff=[](int val, int origin, int dimension) -> int
			{
				if(val < origin) return val-origin;
				else if(val > origin+dimension) return val-origin-dimension;
				else return 0;
			};

			//We obtain how far off the point is from the limits negative values mean "out from the left".
			x=calculate_diff(pt.x, limit_margin.origin.x, limit_margin.w);
			y=calculate_diff(pt.y, limit_margin.origin.y, limit_margin.h);

			go_to(focus_box.origin + point{x, y});
		}
	}
	else
	{
		go_to({p.x-((int)focus_box.w/2), p.y-((int)focus_box.h/2)});
	}
}

//!Centers the focus box on the middle of the rect.

//!The function ignores coordinate systems. If cartesian is chosen, the origin
//!of the rect will be assumed to be its bottom-left corner.

void camera::center_on(const rect& r)
{
	center_on({r.origin.x+((int)r.w/2), r.origin.y+((int)r.h/2)});
}

//!Moves the top-left point of the focus box by the specified parameters (x and y).

//!The significance of the parameters depends on the camera's coordinate system.
//!Cartesian coordinates have Y positives rising, while screen coordinates
//!have them falling.

void camera::move_by(int p_x, int p_y)
{
	go_to(focus_box.origin + point{p_x, p_y});
}

//!Sets a limit to camera movement.

//!The rect is assumed to correspond with the camera's coordinate system (that
//!is, top-left for screen and bottom-left for cartesian).

void camera::set_limits(const rect& r)
{
	with_limit=true;
	limits=r;
}

//!Removes camera movement limits in world space.

void camera::clear_limits()
{
	with_limit=false;
}

//!Sets the zoom value, which in time alters the focus box. 

//!Zoom works by ratio: a zoom 1 is in a 1:1 ratio. A zoom of 2 draws 
//!everything twice as large. Larger zoom values imply smaller focus boxes.
//!Zoom is done from the top-left corner of the camera so the view must
//!be adjusted accordingly. This is so even if the system is set to cartesian.

void camera::set_zoom(double v)
{
	if(v < 0.01) v=0.01;
	d_info.zoom=v;
	focus_box.w=pos_box.w / v;
	focus_box.h=pos_box.h / v;
}

//!Sets a margin for camera movement when using center_on functions. 

//!Objects still within the margin will cause no camera movement. 
//!The rect is interpreted as relative to pos_box and always in screen 
//!coordinates.

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

//!Where pos point is absolute position on the screen. The value returned
//!from this function is affected by the coordinate system of the camera.

point camera::	world_to_pos(point p) const
{
	return world_to_pos_f(p, focus_box.origin);
}

//!Converts world rect to pos rect

//!Where pos rect is absolute position on the screen. The values returned
//!from this function depend on the coordinate system of the camera.

rect camera::world_to_pos(const rect& r) const
{
	auto orig=world_to_pos(r.origin);
	return {orig.x, orig.y, r.w, r.h};
}

//!Sets the pos box of the camera.

//!Repositions the camera on the screen using screen coordinates. Has no effect
//!in the focused area of the world space.

void camera::set_position(point v) 
{
	pos_box.origin=v; 
	sync();
}

//!Sets the coordinate system (basically controlling whether the positive Y axis points up or down).

void camera::set_coordinate_system(tsystem v)
{
	coordinate_system=v;
	
	switch(coordinate_system)
	{
		case tsystem::screen:
			world_to_pos_f=camera::world_to_pos_screen;
		break;
		case tsystem::cartesian:
			world_to_pos_f=camera::world_to_pos_cartesian;
		break;
	}
	sync();
}

//!Checks if the rect is completely or partly inside the camera focus. 

//!The rect is expected to represent the bounding box of an object in world
//!coordinates (not a drawable object). This function is sensitive to the 
//!coordinate system selected.

bool camera::in_focus(const rect& r) const
{
	//TODO: Two functions, or two calculations. Choose... Or do not choose and
	//rework it.
	return r.collides_with(focus_box);
}
