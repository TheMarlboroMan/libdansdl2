#include "screen.h"

using namespace ldv;

//!Constructs the window in a non-initialised state.

//!A call to "init" is necessary before working with the window, usually right
//after it is constructed.


screen::screen():
	window(nullptr), current_camera(nullptr), 
	draw_info_instance{0,0,0,0,0,0,1.0},
	w(0), h(0), 
	w_logic(w), h_logic(h)
{
}

screen::~screen()
{
	if(window) SDL_DestroyWindow(window);
	SDL_GL_DeleteContext(context); 
}

//!Clears the screen with the given color.

//TODO: What does alpha do??

void screen::clear(const rgba_color& c)
{
	glClearColor(c.r, c.g, c.b, c.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

//!Refresh screen.

//!Any call to draw in any representation has no effect until this is called.

void screen::update()
{
	SDL_GL_SwapWindow(window);
}

//!Initialises the window

//!Initialise means to set SDL attributes (double buffer, a stencil size of 1),
//!create a window (in an undefined position) with SDL_WINDOW_OPENGL
//!flags by default and set the logical size of w and h.
//!Can be called many times with different sizes that will change the window
//!size but not the logical size or flags.

void screen::init(int p_w, int p_h, int flags_window)
{
	w=p_w;
	h=p_h;

	if(!window)
	{	
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
	}
	else
	{
		//Breaks viewport... TODO: Fix.
		SDL_SetWindowSize(window, w, h);
	}

	draw_info_instance={0,0,0,0,w,h,1.0};
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
	glViewport(0.f, 0.f, w, h);

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
