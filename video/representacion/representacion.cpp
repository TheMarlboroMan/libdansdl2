#include "representacion.h"

#include <iostream>

using namespace DLibV;

Representacion::Representacion():
	alpha(SDL_ALPHA_OPAQUE), modo_blend(BLEND_NADA), 
	mod_color_r(255), mod_color_g(255), mod_color_b(255),
	/*con_clip_volcado(false),*/ visible(true)
{
	this->reiniciar_posicion();
	this->reiniciar_recorte();
}

Representacion::Representacion(Uint8 p_alpha):
	alpha(p_alpha), 
	mod_color_r(255), mod_color_g(255), mod_color_b(255), 
	visible(true)
{
	this->reiniciar_posicion();
	this->reiniciar_recorte();
}

Representacion::Representacion(const Representacion& p_otra):
	alpha(p_otra.alpha), mod_color_r(p_otra.mod_color_r), 
	mod_color_g(p_otra.mod_color_g), mod_color_b(p_otra.mod_color_b), 
	visible(p_otra.visible)
{
	this->posicion=p_otra.posicion;
	this->recorte=p_otra.recorte;
}

Representacion::~Representacion()
{

}

void Representacion::establecer_posicion(int p_x, int p_y, int p_w, int p_h, int p_flags)
{
	if(p_flags & FRECT_X) this->posicion.x=p_x;
	if(p_flags & FRECT_Y) this->posicion.y=p_y;
	if(p_flags & FRECT_W && p_w != -1) this->posicion.w=p_w;
	if(p_flags & FRECT_H && p_h != -1) this->posicion.h=p_h;
}


void Representacion::reiniciar_posicion()
{
	this->posicion.x=0;
	this->posicion.y=0;
	this->posicion.w=0;
	this->posicion.h=0;
}

void Representacion::desplazar(Sint16 p_x, Sint16 p_y)
{
	this->posicion.x+=p_x;
	this->posicion.y+=p_y;
}

void Representacion::reiniciar_recorte()
{
	this->recorte.x=0;
	this->recorte.y=0;
	this->recorte.w=0;
	this->recorte.h=0;
}

void Representacion::establecer_posicion(const SDL_Rect & p_caja)
{
	posicion=p_caja;
}

void Representacion::establecer_recorte(const SDL_Rect & p_caja)
{
	recorte=p_caja;
}


void Representacion::establecer_recorte(Sint16 p_x, Sint16 p_y, Uint16 p_w, Uint16 p_h, int p_flags)
{
	if(p_flags & FRECT_X) this->recorte.x=p_x;
	if(p_flags & FRECT_Y) this->recorte.y=p_y;
	if(p_flags & FRECT_W) this->recorte.w=p_w;
	if(p_flags & FRECT_H) this->recorte.h=p_h;
}

Representacion& Representacion::operator=(const Representacion& p_otra)
{
	this->alpha=p_otra.alpha;
	this->posicion=p_otra.posicion;
	this->recorte=p_otra.recorte;
	this->visible=p_otra.visible;
	mod_color_r=p_otra.mod_color_r;
	mod_color_g=p_otra.mod_color_g;
	mod_color_b=p_otra.mod_color_b;

	return *this;
}

//Se pasa el rectángulo de pantalla... Básicamente se comprueba si está dentro. Estática o no.
bool Representacion::volcar(SDL_Renderer * renderer, const SDL_Rect& p_pant)
{
	if(!en_toma(p_pant)) return false;
	else return this->volcar(renderer);
}

//Explícito, con cámara... LLama al de abajo pero es más cómodo.
bool Representacion::volcar(SDL_Renderer * renderer, const Camara& p_camara)
{
	return this->volcado(renderer, p_camara.acc_caja_foco(), p_camara.acc_caja_pos()); //, p_camara.acc_desp_x(), p_camara.acc_desp_y());
}

//Explícitos, con pantalla...
bool Representacion::volcar(const Pantalla& p_pantalla, const Camara& p_camara)
{
	return this->volcado(p_pantalla.acc_renderer(), p_camara.acc_caja_foco(), p_camara.acc_caja_pos() ); //, p_camara.acc_desp_x(), p_camara.acc_desp_y());
}

bool Representacion::volcar(const Pantalla& p_pantalla)
{
	return this->volcado(p_pantalla.acc_renderer());
}

bool Representacion::volcar(SDL_Renderer * renderer)
{
	return this->volcado(renderer);
}

bool Representacion::volcar(SDL_Renderer * renderer, const SDL_Rect& p_foco, const SDL_Rect& p_posicion)
{
	return this->volcado(renderer, p_foco, p_posicion);
}

//Se usa para darle un volumen a la posición, que de por si no tiene.
//La posición no tendría interés ninguno de no ser que la cámara la 
//considera para ver si dibujarla o no. Por defecto podemos pensar que 
//es el del recorte.

void Representacion::establecer_dimensiones_posicion_por_recorte()
{
	this->posicion.w=this->recorte.w;
	this->posicion.h=this->recorte.h;
}

void Representacion::procesar_zoom(SDL_Rect& pos, const SDL_Rect& p_posicion, const SDL_Rect& p_enfoque)
{
	float fx=(float) p_posicion.w / (float) p_enfoque.w;
	float fy=(float) p_posicion.h / (float) p_enfoque.h;

	pos.w*=fx;
	pos.h*=fy;
	pos.x*=fx;
	pos.y*=fy;
}
