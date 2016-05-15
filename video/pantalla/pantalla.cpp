#include "pantalla.h"

using namespace DLibV;

//TODO: Extinguish the renderer. 
//TODO: This will, in turn, fuck up everything else.

Pantalla::Pantalla(int p_w, int p_h, unsigned short int p_m):
	ventana(nullptr), renderer(nullptr), volcados(0), 
	w(p_w), h(p_h), modo_ventana(p_m), w_logico(w), h_logico(h)
{
	this->simulacro_caja.w=0;
 	this->simulacro_caja.h=0;
	this->simulacro_caja.x=0;
	this->simulacro_caja.y=0;
}

Pantalla::~Pantalla()
{
	if(this->ventana) SDL_DestroyWindow(this->ventana);
	if(this->renderer) SDL_DestroyRenderer(this->renderer);	 //Esto destruirá texturas asociadas al renderer.
	SDL_GL_DeleteContext(context); 
}

void Pantalla::inicializar(int p_w, int p_h, int flags_ventana)
{
	w=p_w;
	h=p_h;
	configurar(flags_ventana);
}

void Pantalla::establecer_titulo(const char * p_cadena)
{
	SDL_SetWindowTitle(ventana, p_cadena);
}

void Pantalla::establecer_titulo(const std::string& c)
{
	SDL_SetWindowTitle(ventana, c.c_str());
}

void Pantalla::limpiar(const ColorRGBA& c)
{
	glClearColor(c.r, c.g, c.b, c.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Pantalla::actualizar()
{
	SDL_GL_SwapWindow(ventana);
}

void Pantalla::cortar_caja_a_pantalla(SDL_Rect * p_caja)
{
	if(p_caja->x < 0) 
	{
		p_caja->w=p_caja->w-p_caja->x;
		p_caja->x=0;
	}

	if(p_caja->y < 0) 
	{
		p_caja->h=p_caja->h-p_caja->y;
		p_caja->y=0;
	}

	if(p_caja->x+p_caja->w > this->w)
	{
		p_caja->w=this->w-p_caja->w;
	}

	if(p_caja->y+p_caja->h > this->h)
	{
		p_caja->h=this->h-p_caja->h;
	}
}
/*
void Pantalla::preparar_para_camara(Camara const& p_camara)
{
	if(p_camara.es_con_clip()) this->establecer_clip_para_camara(p_camara);
}
*/
void Pantalla::configurar(int flags_ventana)
{
	if(!ventana)
	{	
		ventana=SDL_CreateWindow("", 
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			w, h, flags_ventana); //Por defecto SDL_WINDOW_OPENGL

		context=SDL_GL_CreateContext(ventana);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		glViewport(0.f, 0.f, w, h);

		//Establecer el modo a matriz de proyección y cargarla a 1.
		glMatrixMode(GL_PROJECTION); 	
		glLoadIdentity();

		//Izquierda, derecha, abajo, arriba, cerca y lejos...
		//Ahora el punto 0.0 es arriba a la izquierda.
		glOrtho(0.0, w, h, 0.0, 1.0, -1.0);

		//Establecer el modo a matriz de proyección y cargarla a 1.
		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();

		//Guardar la matriz de modelo...
		glPushMatrix();
	}
	else
	{
		SDL_SetWindowSize(ventana, w, h);
	}

	//TODO: This shall dissapear.
	if(!renderer)
	{
		//TODO: NO crear renderer si la flag de OpenGL está activa...
		renderer=SDL_CreateRenderer(ventana, -1, 0);
		establecer_modo_ventana(modo_ventana);
	}

	establecer_medidas_logicas(w, h);

	this->simulacro_caja.w=w;
	this->simulacro_caja.h=h;
	this->simulacro_caja.x=0;
	this->simulacro_caja.y=0;
}

void Pantalla::establecer_medidas_logicas(int w, int h)
{
	//TODO: This will be opengl stuff.
	w_logico=w;
	h_logico=h;
	SDL_RenderSetLogicalSize(renderer, w_logico, h_logico);
}

void Pantalla::establecer_modo_ventana(unsigned int v)
{
	modo_ventana=v;
	switch(modo_ventana)
	{
		case M_VENTANA: SDL_SetWindowFullscreen(ventana, 0); break;
		case M_PANTALLA_COMPLETA_RESOLUCION: SDL_SetWindowFullscreen(ventana, SDL_WINDOW_FULLSCREEN_DESKTOP); break;
		case M_PANTALLA_COMPLETA_SIMULADA: SDL_SetWindowFullscreen(ventana, SDL_WINDOW_FULLSCREEN); break;
	}
}

void Pantalla::reiniciar_clip_completo()
{
	SDL_Rect caja;
	caja.x=0; 
	caja.y=0;
	caja.w=w;
	caja.h=h;

	//TODO: Stencil buffer?

	SDL_RenderSetClipRect(renderer, &caja);
}

void Pantalla::establecer_clip_para_camara(Camara const& p_camara)
{
	//TODO: Stencil buffer?
	SDL_Rect caja=p_camara.acc_caja_pos();
	SDL_RenderSetClipRect(renderer, &caja);
}

void Pantalla::establecer_clip(SDL_Rect p_caja)
{
	//TODO: Stencil buffer?
	SDL_RenderSetClipRect(renderer, &p_caja);
}

void Pantalla::establecer_posicion(int x, int y)
{
	SDL_SetWindowPosition(ventana, x, y);
}
