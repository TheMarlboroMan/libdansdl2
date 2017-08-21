#ifndef CAMARA_H
#define CAMARA_H

#include <functional>

#include <SDL2/SDL.h>

#include "../rect/rect.h"
#include "../draw_info/draw_info.h"

namespace ldv
{

//!A 2d camera pointing at a 2d plane. 

/**It has no use unless used with a screen object in the render function 
of any representation. In that case, it has plenty of interesting features, 
like controlling the zoom, pruning screen objects out of focus and many others.

A particularly interesting feature is the coordinate system. The focus box will
always be assumed to be a rectangle with its origin in the top-left corner,
its width extending right and its height extending down (just as the screen
coordinate system used on the library).

As for coordinate systems: by default the screen system (Y points down) is activated
but the cartesian one can be used (Y points up). This does not change the origin
and value of the height and width concepts of the focus box, but helps keep
the camera in sync with cartesian-minded applications (what it does is to actually
reverse the Y value in an internal structure). Whatever the system chosen, all
representations MUST be in "screen" form.

The camera can also set a margin for the "focus on" functions to avoid constant
scrolling. The margin is always in screen coordinates. These functions are
meant as quick tools but the system is better controlled by the application
if more complex needs arise.
**/

class camera
{
	public:

	enum class		tsystem{screen, cartesian};

				camera(rect={0,0,640,400}, point={0,0});

	//!The focus box represents the place in space the camera points at.
	const rect& 		get_focus_box() const {return focus_box;}
	//!The pos box is the place objects in the camera will be represented at (camera dimensions may differ from window size).
	const rect& 		get_pos_box() const {return pos_box;}
	//!Returns the zoom value.
	double			get_zoom() const {return d_info.zoom;}
	//!Gets the x position for the focus box.
	int			get_x() const {return focus_box.origin.x;}
	//!Gets the y position for the focus box.
	int 			get_y() const {return focus_box.origin.y;}
	void 			set_zoom(double);
	void                    set_position(point);
	void			set_coordinate_system(tsystem);

	//Camera movement.
	void 			go_to(point);
	void			center_on(point);
	void			center_on(const rect&);
	void 			move_by(int, int);

	//Limits and margins.
	void			set_limits(const rect&);
	void 			clear_limits();
	void			set_center_margin(const rect&);
	void			clear_center_margin();

	//!Return a draw_info struct. The information contained may not be the same as the one exposed by the public interface due to coordinate system differences.
	const draw_info&	get_draw_info() const {return d_info;}

	private:

	//Methods...

	//!Syncs camera boxes and performs neccesary coordinate system conversions.
	void 			sync();

	static point		world_to_pos_screen(point p, point origin) {return p-origin;}
	static point		world_to_pos_cartesian(point p, point origin) {return {p.x-origin.x, -p.y+origin.y};}

	point			world_to_pos(point) const;
	rect			world_to_pos(const rect&) const;

	//Properties...
	draw_info		d_info;
	rect 			focus_box;	//! <Box pointed at in world values. Width and height are related to zoom values.
	rect	 		pos_box;	//! <Box position on screen.

	bool 			with_limit;
	rect			limits;

	bool			with_margin;
	rect			limit_margin;

	tsystem			coordinate_system;

	std::function<point(point, point)>	world_to_pos_f;
	std::function<int(int, int)>		y_addition_f;
	std::function<int(int, int)>		y_substraction_f;
	std::function<bool(int, int)>		go_to_less_than_f;
	std::function<bool(int, int)>		go_to_greater_than_f;
};

} //Fin namespace DLibV

#endif
