#include "representacion.h"

#include <iostream>

using namespace DLibV;

Representacion::Representacion():
	visible(true),
	modo_blend(blends::BLEND_NADA), 
	rgba{1.f, 1.f, 1.f, 0.f}
{
	reiniciar_posicion();
	reiniciar_recorte();
	actualizar_caja_rotacion();
}

Representacion::Representacion(ColorRGBA c):
	visible(true),
	modo_blend(blends::BLEND_NADA), 
	rgba(c)
{
	reiniciar_posicion();
	reiniciar_recorte();
	actualizar_caja_rotacion();
}

Representacion::Representacion(const Representacion& p_otra):
	visible(p_otra.visible), 
	modo_blend(blends::BLEND_NADA),
	rgba(p_otra.rgba),
	posicion(p_otra.posicion), 
	recorte(p_otra.recorte),
	posicion_rotada(p_otra.posicion_rotada)
{

}

Representacion& Representacion::operator=(const Representacion& p_otra)
{
	posicion=p_otra.posicion;
	recorte=p_otra.recorte;
	visible=p_otra.visible;
	rgba=p_otra.rgba;
	posicion_rotada=p_otra.posicion_rotada;
	modo_blend=p_otra.modo_blend;
	return *this;
}

void Representacion::establecer_posicion(int p_x, int p_y, int p_w, int p_h, int p_flags)
{
	if(p_flags & FRECT_X) posicion.x=p_x;
	if(p_flags & FRECT_Y) posicion.y=p_y;
	if(p_flags & FRECT_W && p_w != -1) posicion.w=p_w;
	if(p_flags & FRECT_H && p_h != -1) posicion.h=p_h;
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
	posicion.x+=p_x;
	posicion.y+=p_y;
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
	if(p_flags & FRECT_X) recorte.x=p_x;
	if(p_flags & FRECT_Y) recorte.y=p_y;
	if(p_flags & FRECT_W) recorte.w=p_w;
	if(p_flags & FRECT_H) recorte.h=p_h;
}

//TODO: How do we render to a different screen?.
void Representacion::volcar(Pantalla& p_pantalla, const Camara& p_camara)
{
	if(!visible) return; //TODO: Fix this. || !en_toma(p_camara)) return;

	p_pantalla.asignar_camara(p_camara);

	const Rect& pos=acc_posicion();
	auto iv=p_camara.acc_info_volcado();

	int 	x=iv.pos_x+pos.x-iv.rel_x, 
		y=iv.pos_y+pos.y-iv.rel_y;

	glTranslatef(x, y, 0.f);

	if(transformacion.angulo_rotacion != 0.f)
	{
		glTranslatef(transformacion.x_centro_rotacion, transformacion.y_centro_rotacion, 0.f);
		glRotatef(transformacion.angulo_rotacion, 0.f, 0.f, 1.f);
		glTranslatef(-transformacion.x_centro_rotacion, -transformacion.y_centro_rotacion, 0.f);
	}

	volcado(p_camara.acc_info_volcado());
	glLoadIdentity();
}

void Representacion::volcar(Pantalla& p_pantalla)
{
	if(!visible) return;

	p_pantalla.reiniciar_clip();

	const Rect& pos=acc_posicion();
	auto iv=p_pantalla.acc_info_volcado();

	int 	x=iv.pos_x+pos.x-iv.rel_x, 
		y=iv.pos_y+pos.y-iv.rel_y;

	glTranslatef(x, y, 0.f);

	if(transformacion.angulo_rotacion != 0.f)
	{
		glTranslatef(transformacion.x_centro_rotacion, transformacion.y_centro_rotacion, 0.f);
		glRotatef(transformacion.angulo_rotacion, 0.f, 0.f, 1.f);
		glTranslatef(-transformacion.x_centro_rotacion, -transformacion.y_centro_rotacion, 0.f);
	}

	volcado(p_pantalla.acc_info_volcado());
	glLoadIdentity();
}

//Se usa para darle un volumen a la posición, que de por si no tiene.
//La posición no tendría interés ninguno de no ser que la cámara la 
//considera para ver si dibujarla o no. Por defecto podemos pensar que 
//es el del recorte.

void Representacion::establecer_dimensiones_posicion_por_recorte()
{
	posicion.w=recorte.w;
	posicion.h=recorte.h;
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

void Representacion::transformar_rotar(float v) 
{
	transformacion.angulo_rotacion=v;
	actualizar_caja_rotacion();
}

void Representacion::transformar_cancelar_rotar() 
{
	transformacion.angulo_rotacion=0.f;
	actualizar_caja_rotacion();
}

void Representacion::transformar_centro_rotacion(float x, float y) 
{
	transformacion.centro_rotacion(x, y);
	actualizar_caja_rotacion();
}

void Representacion::transformar_centro_rotacion_cancelar() 
{
	transformacion.cancelar_centro_rotacion();
	actualizar_caja_rotacion();
}

void Representacion::actualizar_caja_rotacion()
{
	const auto& p=acc_posicion();

	if(!transformacion.es_transformacion())
	{
		posicion_rotada=p;
	}
	else
	{
		//TODO: Restore... We deleted obtener_centro_rotacion(),

/*		auto c=transformacion.acc_centro_rotacion();
		DLibH::Poligono_2d_vertices<double> polig(
			{ 
				{(double)p.x, (double)p.y},
				{(double)(p.x+p.w), (double)p.y},
				{(double)(p.x+p.w), (double)(p.y+p.h)},
				{(double)p.x, (double)(p.y+p.h)},        
			}, {(double)c.x+p.x, (double)c.y+p.y});

		//Las rotaciones de SDL son "clockwise"... Las reales son "counter-clockwise"...
		float a=transformacion.acc_angulo_rotacion();
		polig.rotar(a);

		//Sacar las medidas para la nueva caja...
		std::vector<double> xs={polig.vertice(0).x, polig.vertice(1).x, polig.vertice(2).x, polig.vertice(3).x};
		std::vector<double> ys={polig.vertice(0).y, polig.vertice(1).y, polig.vertice(2).y, polig.vertice(3).y};

		posicion_rotada.x=*std::min_element(std::begin(xs), std::end(xs));
		posicion_rotada.y=*std::min_element(std::begin(ys), std::end(ys));
		posicion_rotada.w=*std::max_element(std::begin(xs), std::end(xs))-posicion_rotada.x;
		posicion_rotada.h=*std::max_element(std::begin(ys), std::end(ys))-posicion_rotada.y;
*/
	}
}

Rect Representacion::copia_posicion_rotada() const
{
	return Rect{posicion_rotada.x, posicion_rotada.y, posicion_rotada.w, posicion_rotada.h};
}

