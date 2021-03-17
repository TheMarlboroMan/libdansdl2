#pragma once

#include "sdl_video_tools.h"
#include "camera.h"
#include "color.h"

#include <SDL2/SDL.h>
#include <GL/gl.h>

#ifdef WINCOMPIL
#include <GL/glext.h>
#endif

#include <iostream>
#include <string>

namespace ldv
{

//!Represents a window.

//!So far we can only have one.

//TODO: Should be called "window".

class screen
{
	public:

	                    screen(int, int, int=SDL_WINDOW_OPENGL);
	                    ~screen();

	//!Returns a handle to the SDL window.
	SDL_Window *        get_window() const {return this->window;}
	//!Returns the width of the window.
	unsigned int        get_w() const {return w;}
	//!Returns the height of the window.
	unsigned int        get_h() const {return h;}
	//!Returns a draw info instance with the size of the window.
	const draw_info     get_draw_info() const {return draw_info_instance;}
	//!Sets the window position.
	void                set_position(int x, int y) {SDL_SetWindowPosition(window, x, y);}
	//!Sets the title of the window.
	void                set_title(const std::string& v) {SDL_SetWindowTitle(window, v.c_str());}
	void                set_camera(const camera&);
	void                set_clip(camera const&);
	void                set_clip(rect);
	void                reset_clip();
	void                set_size(int, int);
	void                set_logical_size(int, int);
	void                set_fake_fullscreen(bool);

	//!Sets or removes underlying library fullscreen mode.
	void                set_fullscreen(bool);

	void                clear(const rgba_color&);
	void                update();

	//!Returns whether there is a camera attached.
	bool                has_camera() const {return current_camera!=nullptr;}

	//!Returns true is the fullscreen flag is set.
	bool                is_fullscreen() const {return fullscreen;}

	//!Returns a rect with the screen size. Useful to align representations.
	rect                get_rect() const {return rect{0,0,(unsigned)w,(unsigned)h};}

	private:

	void                init(int);

	//Properties...

	SDL_Window *        window;
	SDL_GLContext       context;
	camera const *      current_camera;
	draw_info           draw_info_instance;
	int                 w,
	                    h,
	                    w_logic, //! <Rendered size. May differ from window dimensions.
	                    h_logic; //! <Rendered size. May differ from window dimensions.
	bool                fullscreen{false};

#ifndef NDEBUG
	public:
	unsigned int        draw_count{0};
#endif


};

} //Fin namespace DLibV

