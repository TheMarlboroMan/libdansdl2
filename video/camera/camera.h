#ifndef CAMARA_H
#define CAMARA_H

#include <SDL2/SDL.h>

#include "../rect/rect.h"
#include "../draw_info/draw_info.h"

namespace ldv
{

//!A 2d camera pointing at a 2d plane. 
/**It has no use unless used with a screen object in the render function 
of any representation. 
**/

class camera
{
	public:

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
	//!Sets the pos box of the camera.
	void			set_position(point v) {pos_box.origin=v; sync();}
	//!Sets the zoom value, which in time alters the focus box. Zoom works by ratio: a zoom 1 is in a 1:1 ratio. A zoom of 2 draws everything twice as large. Larger zoom values imply smaller focus boxes.
	void 			set_zoom(double);

	//Camera movement and tracking functions...

	void 			go_to(point);
	void			center_on(point);
	void			center_on(const rect&);
	void 			move_by(int, int);
	void			set_limits(const rect&);
	void 			clear_limits();
	void			set_center_margin(const rect&);
	void			clear_center_margin();

	//!Return a draw_info struct which conveniently includes most camera information in one place with no overhead.
	const draw_info		get_draw_info() const {return d_info;}

	private:

	//Methods...

	//!Syncs camera boxes.
	void 			sync();

	point			world_to_pos(point) const;
	rect			world_to_pos(const rect&) const;
	point			pos_to_world(point) const;
//	rect			pos_to_world(const rect&) const;  

	//Properties...
	draw_info		d_info;
	rect 			focus_box;	//! <Box pointed at in world values. Width and height are related to zoom values.
	rect	 		pos_box;	//! <Box position on screen.

	bool 			with_limit;
	struct			{int min_x, min_y, max_x, max_y;} limits; //! <Stored as integers instead of a box to avoid calculating max_x and max_y again and again.

	bool			with_margin;
	rect			limit_margin;
};

} //Fin namespace DLibV

#endif
