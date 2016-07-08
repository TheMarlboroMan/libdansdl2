#include "representacion.h"

#include <iostream>
#include <algorithm>

using namespace DLibV;

Representacion::Representacion():
	visible(true),
	modo_blend(blends::BLEND_NADA), 
	rgba{1.f, 1.f, 1.f, 0.f},
	posicion_vista{0,0,0,0}
{

}

Representacion::Representacion(ColorRGBA c):
	visible(true),
	modo_blend(blends::BLEND_NADA), 
	rgba(c),
	posicion_vista{0,0,0,0}
{

}

Representacion::Representacion(const Representacion& p_otra):
	visible(p_otra.visible), 
	modo_blend(blends::BLEND_NADA),
	rgba(p_otra.rgba),
	posicion_vista(p_otra.posicion_vista)
{

}

Representacion& Representacion::operator=(const Representacion& p_otra)
{
	visible=p_otra.visible;
	rgba=p_otra.rgba;
	posicion_vista=p_otra.posicion_vista;
	modo_blend=p_otra.modo_blend;
	return *this;
}

//TODO: How do we render to a different screen?.
void Representacion::volcar(Pantalla& p_pantalla, const Camara& p_camara)
{
	if(!visible || !en_toma(p_camara)) return;

	p_pantalla.asignar_camara(p_camara);

	transformacion_pre_render(p_camara.acc_info_volcado());
	volcado(p_camara.acc_info_volcado());
	glLoadIdentity();
}

void Representacion::volcar(Pantalla& p_pantalla)
{
	if(!visible) return;

	p_pantalla.reiniciar_clip();

	transformacion_pre_render(p_pantalla.acc_info_volcado());
	volcado(p_pantalla.acc_info_volcado());
	glLoadIdentity();
}

void Representacion::transformacion_pre_render(const Info_volcado iv)
{
	auto pos=obtener_posicion();
	int 	x=iv.pos_x+pos.x-iv.rel_x, 
		y=iv.pos_y+pos.y-iv.rel_y;

	//En caso de que haya zoom compensamos el movimiento que causa la escala.
	//Básicamente estamos desplazando la mitad, dos tercios, tres cuartos...
	if(iv.zoom!=1.0)
	{
		glScaled(iv.zoom, iv.zoom, 1.0);
		//Puro empirismo.
		glTranslatef((iv.pos_x / iv.zoom) - iv.pos_x, (iv.pos_y / iv.zoom) - iv.pos_y, 0.f);
	}

	glTranslatef(x, y, 0.f);

	if(transformacion.angulo_rotacion != 0.f)
	{
		glTranslatef(transformacion.centro_rotacion.x, transformacion.centro_rotacion.y, 0.f);
		glRotatef(transformacion.angulo_rotacion, 0.f, 0.f, 1.f);
		glTranslatef(-transformacion.centro_rotacion.x, -transformacion.centro_rotacion.y, 0.f);
	}
}

//Se usa para darle un volumen a la posición, que de por si no tiene.
//La posición no tendría interés ninguno de no ser que la cámara la 
//considera para ver si dibujarla o no. Por defecto podemos pensar que 
//es el del recorte.

void Representacion::transformar_rotar(float v) 
{
	transformacion.angulo_rotacion=v;
	actualizar_posicion_vista_rotacion();
}

void Representacion::transformar_cancelar_rotar() 
{
	transformacion.angulo_rotacion=0.f;
	actualizar_posicion_vista_rotacion();
}

void Representacion::transformar_centro_rotacion(float x, float y) 
{
	transformacion.centro_rotacion.x=x;
	transformacion.centro_rotacion.y=y;
	actualizar_posicion_vista_rotacion();
}

void Representacion::transformar_centro_rotacion_cancelar() 
{
	transformacion.centro_rotacion.x=0.f;
	transformacion.centro_rotacion.y=0.f;
	actualizar_posicion_vista_rotacion();
}

void Representacion::actualizar_posicion_vista_rotacion()
{
	const auto p=obtener_base_posicion_vista();

	if(!transformacion.es_transformacion())
	{
		posicion_vista=p;
	}
	else
	{
		auto c=transformacion.centro_rotacion;
		DLibH::Poligono_2d_vertices<double> polig(
			{ 
				{(double)p.x, (double)p.y},
				{(double)(p.x+p.w), (double)p.y},
				{(double)(p.x+p.w), (double)(p.y+p.h)},
				{(double)p.x, (double)(p.y+p.h)},        
			}, {(double)c.x+p.x, (double)c.y+p.y});

		//Las rotaciones son "clockwise"... Las reales son "counter-clockwise"...
		float a=transformacion.angulo_rotacion;
		polig.rotar(a);

		//Sacar las medidas para la nueva caja...
		std::vector<double> xs={polig.vertice(0).x, polig.vertice(1).x, polig.vertice(2).x, polig.vertice(3).x};
		std::vector<double> ys={polig.vertice(0).y, polig.vertice(1).y, polig.vertice(2).y, polig.vertice(3).y};

		posicion_vista.x=*std::min_element(std::begin(xs), std::end(xs));
		posicion_vista.y=*std::min_element(std::begin(ys), std::end(ys));
		posicion_vista.w=*std::max_element(std::begin(xs), std::end(xs))-posicion_vista.x;
		posicion_vista.h=*std::max_element(std::begin(ys), std::end(ys))-posicion_vista.y;
	}
}

//Realmente horrible pero comparar la colisión con cajas es rápido.
bool Representacion::en_toma(const Camara& p_cam) const
{
	return p_cam.acc_caja_foco().es_en_colision_con(posicion_vista, true);
}
