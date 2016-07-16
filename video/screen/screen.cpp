#include "screen.h"

using namespace ldv;

screen::screen(int p_w, int p_h):
	window(nullptr), current_camera(nullptr), 
	draw_info_instance{0,0,0,0,(int)p_w,(int)p_h,1.0},
	box{0,0,0,0}, w(p_w), h(p_h), w_logic(w), h_logic(h)
{
}

screen::~screen()
{
	if(window) SDL_DestroyWindow(window);
	SDL_GL_DeleteContext(context); 
}

void screen::clear(const rgba_color& c)
{
	glClearColor(c.r, c.g, c.b, c.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void screen::update()
{
	SDL_GL_SwapWindow(window);
}

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

		context=SDL_GL_CreateContext(window);
		set_logical_size(w, h);
	}
	else
	{
		//Cambia el tamaño de la window pero jode completamente el viewport.
		SDL_SetWindowSize(window, w, h);
	}

	draw_info_instance={0,0,0,0,w,h,1.0};
	box=rect{0,0,(unsigned int)w,(unsigned int)h};
}

void screen::set_logical_size(int pw, int ph)
{
	//Establecer el modo a matriz de proyección y cargarla a 1.
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();

	w_logic=pw;
	h_logic=ph;
	glViewport(0.f, 0.f, w, h);

	//Izquierda, derecha, abajo, arriba, cerca y lejos...
	//Ahora el punto 0.0 es arriba a la izquierda. Ajustamos a 0.5
	//para que pille el centro del pixel.
	glOrtho(-0.5f, (float)w_logic-.5f, (float)h_logic-.5f, -0.5f, 1.f, -1.0f);

	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
}

void screen::set_clip(camera const& pcamera)
{
	set_clip(pcamera.get_pos_box());
}

void screen::set_clip(rect p_caja)
{
	//Reiniciar matrices...
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Activar test de stencil, establecer valor de limpieza y limpiar.

	glDepthMask(GL_FALSE); //Realmente no sé si esto hace algo.
	glEnable(GL_STENCIL_TEST); //Esto se activa aquí... Se desactivaría en algún otro lado.
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);

	//Nunca renderizar... pero si reemplazar.

//	glColor4f(0.1f, 0.6f, 0.6f, 0.2f);
	glStencilFunc(GL_NEVER, 1, 1);
//	glStencilFunc(GL_ALWAYS, 1, 1);
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

	//Ahora si, sólo renderizamos cuando el valor sea igual y
	//no cambiaremos el pixel.
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void screen::set_camera(const camera& c)
{
	if(!current_camera || &c != current_camera)
	{
		set_clip(c);
		current_camera=&c;
	}
}

void screen::reset_clip()
{
	glDisable(GL_STENCIL_TEST); 
	current_camera=nullptr;
}
