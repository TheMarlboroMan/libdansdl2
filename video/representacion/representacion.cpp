#include "representacion.h"

#include <iostream>

using namespace DLibV;

Representacion::Representacion():
	visible(true),
	modo_blend(blends::BLEND_NADA), 
	rgba{1.f, 1.f, 1.f, 0.f}
{
	this->reiniciar_posicion();
	this->reiniciar_recorte();
}

Representacion::Representacion(ColorRGBA c):
	visible(true),
	modo_blend(blends::BLEND_NADA), 
	rgba(c)
{
	this->reiniciar_posicion();
	this->reiniciar_recorte();
}

Representacion::Representacion(const Representacion& p_otra):
	visible(p_otra.visible), 
	modo_blend(blends::BLEND_NADA),
	rgba(p_otra.rgba),
	posicion(p_otra.posicion), recorte(p_otra.recorte)
{

}

Representacion& Representacion::operator=(const Representacion& p_otra)
{
	this->posicion=p_otra.posicion;
	this->recorte=p_otra.recorte;
	this->visible=p_otra.visible;
	rgba=p_otra.rgba;
	modo_blend=p_otra.modo_blend;
	return *this;
}

void Representacion::establecer_posicion(int p_x, int p_y, int p_w, int p_h, int p_flags)
{
	if(p_flags & FRECT_X) this->posicion.x=p_x;
	if(p_flags & FRECT_Y) this->posicion.y=p_y;
	if(p_flags & FRECT_W && p_w != -1) this->posicion.w=p_w;
	if(p_flags & FRECT_H && p_h != -1) this->posicion.h=p_h;
}

void Representacion::reiniciar_rect(Rect& r)
{
	r.x=0;
	r.y=0;
	r.w=0;
	r.h=0;
}

void Representacion::reiniciar_posicion()
{
	reiniciar_rect(posicion);
}

void Representacion::desplazar(Sint16 p_x, Sint16 p_y)
{
	this->posicion.x+=p_x;
	this->posicion.y+=p_y;
}

void Representacion::reiniciar_recorte()
{
	reiniciar_rect(recorte);
}

void Representacion::establecer_posicion(Rect p_caja)
{
	posicion=p_caja;
}

void Representacion::establecer_recorte(Rect p_caja)
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

//Explícitos, con pantalla...
//TODO... There's no need for a screen anymore I guess...
//TODO: How do we render to a different screen?.
void Representacion::volcar(const Pantalla& p_pantalla, const Camara& p_camara)
{
	if(!visible) return;
	this->volcado();
}

//TODO... There's no need for a screen anymore I guess...
void Representacion::volcar(const Pantalla& p_pantalla)
{
	if(!visible) return;
	this->volcado();
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

void Representacion::procesar_zoom(Rect& pos, const Rect& p_posicion, const Rect& p_enfoque)
{
	float fx=(float) p_posicion.w / (float) p_enfoque.w;
	float fy=(float) p_posicion.h / (float) p_enfoque.h;

	pos.w*=fx;
	pos.h*=fy;
	pos.x*=fx;
	pos.y*=fy;
}

void Representacion::procesar_zoom(Rect& pos, double zoom)
{
	if(zoom==1.0) return;

	pos.w/=zoom;
	pos.h/=zoom;
	pos.x/=zoom;
	pos.y/=zoom;
}
