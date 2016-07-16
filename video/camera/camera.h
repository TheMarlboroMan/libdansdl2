#ifndef CAMARA_H
#define CAMARA_H

#include <SDL2/SDL.h>

#include "../rect/rect.h"
#include "../draw_info/draw_info.h"

namespace ldv
{

class camera
{
	public:

	//Interfaz pública.
				camera(rect={0,0,640,400}, point={0,0});

	const rect& 		get_focus_box() const {return focus_box;}
	const rect& 		get_pos_box() const {return pos_box;}

	double			get_zoom() const {return d_info.zoom;}
	int			get_x() const {return focus_box.origin.x;}
	int 			get_y() const {return focus_box.origin.y;}

	//Sets the position on the screen.
	void			set_position(point v) {pos_box.origin=v; sync();}
	void 			set_zoom(double);

	//Camera movement and tracking...
	void 			go_to(point);
	void 			move_by(int=0, int=0);
	void 			set_limits(int, int, int, int);
	void 			clear_limits();

	//Other helpers.
	point			transform(point) const;
	const draw_info		get_draw_info() const {return d_info;}

	private:

	void 			sync();

	draw_info		d_info;
	rect 			focus_box;			//La caja a la que se apunta en el "mundo", es decir, estamos apuntado al punto x, y, enfocando w y h del mundo. Si cambian estos w y h haríamos zoom.
	rect	 		pos_box;		//Rectángulo de la cámara en pantalla.

	bool 			with_limit;		//Límites para el movimiento de la cámara usando "enfocar a".
	struct			{int min_x, min_y, max_x, max_y;} limits;
};

} //Fin namespace DLibV

#endif
