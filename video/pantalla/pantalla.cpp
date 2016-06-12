#include "pantalla.h"

using namespace DLibV;

Pantalla::Pantalla(int p_w, int p_h, unsigned short int p_m):
	ventana(nullptr), volcados(0), 
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
		//Ahora el punto 0.0 es arriba a la izquierda. Ajustamos a 0.5
		//para que pille el centro del pixel.
		glOrtho(-0.5f, (float)w-.5f, (float)h-.5f, -0.5f, 1.f, -1.0f);

		//Establecer el modo a matriz de proyección y cargarla a 1.
		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();
	}
	else
	{
		//TODO: How about this... Does this do anything?.
		SDL_SetWindowSize(ventana, w, h);
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
//	SDL_RenderSetLogicalSize(renderer, w_logico, h_logico);
}

void Pantalla::establecer_modo_ventana(unsigned int v)
{
//TODO...
//	modo_ventana=v;
//	switch(modo_ventana)
//	{
//		case M_VENTANA: SDL_SetWindowFullscreen(ventana, 0); break;
//		case M_PANTALLA_COMPLETA_RESOLUCION: SDL_SetWindowFullscreen(ventana, SDL_WINDOW_FULLSCREEN_DESKTOP); break;
//		case M_PANTALLA_COMPLETA_SIMULADA: SDL_SetWindowFullscreen(ventana, SDL_WINDOW_FULLSCREEN); break;
//	}
}

void Pantalla::reiniciar_clip_completo()
{
/*	Rect caja;
	caja.x=0; 
	caja.y=0;
	caja.w=w;
	caja.h=h;
*/
	//TODO: Stencil buffer?

//	SDL_RenderSetClipRect(renderer, &caja);
}

void Pantalla::establecer_clip_para_camara(Camara const& p_camara)
{
	//TODO: Stencil buffer?
//	Rect caja=p_camara.acc_caja_pos();
//	SDL_RenderSetClipRect(renderer, &caja);
}

void Pantalla::establecer_clip(Rect p_caja)
{
	//TODO: Stencil buffer?
//	SDL_RenderSetClipRect(renderer, &p_caja);
}

void Pantalla::establecer_posicion(int x, int y)
{
	SDL_SetWindowPosition(ventana, x, y);
}
