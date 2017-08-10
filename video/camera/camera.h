#ifndef CAMARA_H
#define CAMARA_H

#include <SDL2/SDL.h>

#include "../rect/rect.h"
#include "../draw_info/draw_info.h"

namespace ldv
{

///A 2d camera pointing at a 2d plane. 
/**It has no use unless used with a screen object in the render function of any representation. 
**/

class camera
{
	public:
	///////////////////////////
	//Public interface.
				camera(rect={0,0,640,400}, point={0,0});

	///The focus box represents the place in space the camera points at.
	const rect& 		get_focus_box() const {return focus_box;}
	///The pos box is the place objects in the camera will be represented at (camera dimensions may differ from window size).
	const rect& 		get_pos_box() const {return pos_box;}

	double			get_zoom() const {return d_info.zoom;}

	///Gets the x position for the focus box.
	int			get_x() const {return focus_box.origin.x;}

	///Gets the y position for the focus box.
	int 			get_y() const {return focus_box.origin.y;}

	///Sets the pos box of the camera.
	void			set_position(point v) {pos_box.origin=v; sync();}

	///Sets the zoom value, which in time alters the focus box. Zoom works by ratio: a zoom 1 is in a 1:1 ratio. A zoom of 2 draws everything twice as large. Larger zoom values imply smaller focus boxes.
	void 			set_zoom(double);

	//Camera movement and tracking functions...

	///Moves the top-left point of the focus box.
	void 			go_to(point);

	///Centers the focus box.
	void			center_on(point);

	///Centers the focus box.
	void			center_on(const rect&);

	///Moves the top-left point of the focus box by the specified parameters (x and y).
	void 			move_by(int=0, int=0);

	///Sets a limit to camera movement in world space with the specified parameters (x, y, max_x and max_y).
	void 			set_limits(int, int, int, int);
	///Sets the given rect as camera movement limits in world space.
	void			set_limits(const rect&);
	///Removes camera movement limits in world space.
	void 			clear_limits();
	///Sets a margin for camera movement when using center_on functions. Objects still within the margin will cause no camera movement. The rect is interpreted as relative to pos_box.
	void			set_center_margin(const rect&);
	///Clears the margin for camera movement.
	void			clear_center_margin();
	///Returns a draw_info struct which conveniently includes most camera information in one place with no overhead.
	const draw_info		get_draw_info() const {return d_info;}

	private:

	///Syncs camera boxes.
	void 			sync();
	///Converts world point to pos point
	point			world_to_pos(point);

	draw_info		d_info;
	rect 			focus_box;		//La caja a la que se apunta en el "mundo", es decir, estamos apuntado al punto x, y, enfocando w y h del mundo. Si cambian estos w y h haríamos zoom.
	rect	 		pos_box;		//Rectángulo de la cámara en pantalla.

	bool 			with_limit;		//Límites para el movimiento de la cámara usando "enfocar a".
	//TODO: Why don't use a stupid box?????????
	struct			{int min_x, min_y, max_x, max_y;} limits; 

	bool			with_margin;
	rect			limit_margin;
};

} //Fin namespace DLibV

#endif
