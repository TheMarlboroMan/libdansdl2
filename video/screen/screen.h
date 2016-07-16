#ifndef LIBDANSDL2_SCREEN_H
#define LIBDANSDL2_SCREEN_H

#include <SDL2/SDL.h>
#include <GL/gl.h>

#ifdef WINCOMPIL
#include <GL/glext.h>
#endif

#include <iostream>
#include <string>

#include "../sdl_video_tools/sdl_video_tools.h"
#include "../camera/camera.h"
#include "../color/color.h"

namespace ldv
{

class screen
{
	public:

	//Interfaz pública.

					screen(int=640, int=400);
					~screen();

	SDL_Window * 			get_window() const {return this->window;}
	unsigned int 			get_w() const {return w;}
	unsigned int 			get_h() const {return h;}
	const rect& 			get_box() {return box;}
	const draw_info			get_draw_info() const {return draw_info_instance;}

	void 				set_position(int x, int y) {SDL_SetWindowPosition(window, x, y);}
	void 				set_title(const std::string& v) {SDL_SetWindowTitle(window, v.c_str());}
	void 				set_camera(const camera&);
	void 				set_clip(camera const&);
	void 				set_clip(rect);
	void 				reset_clip();
	void 				set_logical_size(int, int);

	void 				init(int, int, int=SDL_WINDOW_OPENGL);
	void 				clear(const rgba_color&);
	void 				update();

	bool				has_camera() const {return current_camera!=nullptr;}

	private:

	//Propiedades...

	SDL_Window * 			window;
	SDL_GLContext 			context;
	camera const *			current_camera;
	draw_info			draw_info_instance;
	rect 				box; //El simulacro de caja es un rectángulo con las medidas de la pantalla, para controlar lo que se muestra o no.

	int 				w,
					h; //Ancho y alto de la window...

	int 				w_logic, //Ancho y alto del renderer.
					h_logic;
};

} //Fin namespace DLibV

#endif
