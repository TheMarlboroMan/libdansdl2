#include "screen.h"

#include <iostream>

using namespace ldv;

//!Constructs the window in an initialised state.

screen::screen(int p_w, int p_h, int flags_window):
	window(nullptr), current_camera(nullptr), 
	draw_info_instance{0,0,0,0,0,0,1.0},
	w(p_w), h(p_h), 
	w_logic(w), h_logic(h) {

	init(flags_window);
}

screen::~screen() {
	if(window) SDL_DestroyWindow(window);
	SDL_GL_DeleteContext(context); 
}

//!Clears the screen with the given color.

//TODO: What does alpha do??

void screen::clear(const rgba_color& c) {
	glClearColor(c.r, c.g, c.b, c.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

//!Refresh screen.

//!Any call to draw in any representation has no effect until this is called.

void screen::update() {
	SDL_GL_SwapWindow(window);
}

//!Initialises the window

//!Initialise means to set SDL attributes (double buffer, a stencil size of 1),
//!create a window (in an undefined position) with SDL_WINDOW_OPENGL
//!flags by default and set the logical size of w and h.

void screen::init(int flags_window)
{
	if(nullptr!=window) {
		throw std::runtime_error("window was already init");
	}

	//All these attributes must be set BEFORE creating the window!!.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//This is very important...
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);

	window=SDL_CreateWindow("", 
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		w, h, flags_window); //Por defecto SDL_WINDOW_OPENGL

	//Bind the sdl window to openGL.
	context=SDL_GL_CreateContext(window);
	set_logical_size(w, h);
	glViewport(0.f, 0.f, w, h);

	draw_info_instance={0,0,0,0,w,h,1.0};
}

//!Sets the phisical size of the screen and adjusts the viewport accordingly.

//!The viewport will not preserve the original proportions if these change. 
//!Calling this method in fullscreen mode is guaranteed to have funky 
//!results.

void screen::set_size(int pw, int ph) {

	w=pw;
	h=ph;
	SDL_SetWindowSize(window, w, h);
	glViewport(0.f, 0.f, w, h);
}

//!Sets or removes fake fullscreen mode.

//!This function does not do videomode changes: it just gets the current display
//!desktop size and adjusts the viewport so it retains its proportions, 
//!letterboxing the rest. You can still put other windows on top, or even see 
//!the OS bars. The w and h properties of the screen (and transitively, stuff
//!such as "get_rect" will NOT change).
//!Calling this method with a screen size larger than the display size will
//!have funky results.

void screen::set_fullscreen(bool v) 
{
	if(fullscreen==v) return;

	fullscreen=v;
	if(fullscreen) {

		auto info=get_display_info();

		SDL_SetWindowSize(window, info.w, info.h);
		SDL_SetWindowPosition(window, 0, 0);
		SDL_SetWindowBordered(window, (SDL_bool)false);

		//Adjust and letterbox...
		float screen_aspect=(float)info.w /(float)info.h;
		float app_aspect=(float)w / (float)h;
		float factor=screen_aspect > app_aspect ? info.h / h : info.w / w;
		int nw=info.w*factor, nh=info.h*factor;
		glViewport(0, 0, nw, nh);
	}
	else {
		set_size(w, h);
		SDL_SetWindowBordered(window, (SDL_bool)true);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
}

//!Sets the logical size.

//!The logical size allows for representing spaces larger or smaller than the
//!window itself looking like a zoom effect. It is a good idea to set this
//!logical size to the window size. If the application requires of a fixed
//!logical size, it can be set independently from the window's.

void screen::set_logical_size(int pw, int ph)
{
	//Projection matrix...
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();

	w_logic=pw;
	h_logic=ph;
	//glViewport(0.f, 0.f, w, h);

	//left, right, top, bottom, near and far.
	//Sets 0.0 to be top left. Adjust 0.5f to get the pixel centre.
	glOrtho(-0.5f, (float)w_logic-.5f, (float)h_logic-.5f, -0.5f, 1.f, -1.0f);

	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
}

//!Clips the display to the real camera display size.

//!The camera box size is different from its focus box.

void screen::set_clip(camera const& pcamera)
{
	set_clip(pcamera.get_pos_box());
}

//!Clips the display.

void screen::set_clip(rect p_caja)
{
	//Reset matrix.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Activate stencil, set clear values and clear.

	glDepthMask(GL_FALSE); //Does this really do anything?
	glEnable(GL_STENCIL_TEST);
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);

	//Do not render, but do replace...
	glStencilFunc(GL_NEVER, 1, 1);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	int	fx=p_caja.origin.x+p_caja.w, 
		fy=p_caja.origin.y+p_caja.h;

	struct pt {int x, y;};
	std::vector<pt> puntos{ 
		{p_caja.origin.x, p_caja.origin.y}, 
		{fx, p_caja.origin.y}, 
		{fx, fy}, 
		{p_caja.origin.x, fy}};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, puntos.data());
	glDrawArrays(GL_POLYGON, 0, puntos.size());
	glDisableClientState(GL_VERTEX_ARRAY); 

	//Now, just render when the value is the same and keep the pixel...
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

//!Sets the camera as current.

//!The view will be clipped to the camera's box.

void screen::set_camera(const camera& c)
{
	if(!current_camera || &c != current_camera)
	{
		set_clip(c);
		current_camera=&c;
	}
}

//!Disables all clipping and unlinks camera.

void screen::reset_clip()
{
	glDisable(GL_STENCIL_TEST); 
	current_camera=nullptr;
}
