#include "representacion_grafica.h"

#include <algorithm>
#include "../../../herramientas/poligono_2d/poligono_2d.h"

using namespace DLibV;

extern DLibH::Log_base LOG;

Representacion_grafica::Representacion_grafica()
	:Representacion(), textura(nullptr), preparada(false)
{
	this->reiniciar_posicion();
	this->reiniciar_recorte();
	actualizar_caja_rotacion();
}

Representacion_grafica::Representacion_grafica(const Representacion_grafica& o)
	:Representacion(o) ,textura(o.textura), preparada(o.preparada),
	posicion_rotada(o.posicion_rotada)
{

}

Representacion_grafica& Representacion_grafica::operator=(const Representacion_grafica& o)
{
	Representacion::operator=(o);
	textura=o.textura;
	preparada=o.preparada;
	posicion_rotada=o.posicion_rotada;

	return *this;
}

Representacion_grafica::~Representacion_grafica()
{
	//OJO: No se borra el recurso gráfico porque se asume que lo hemos
	//obtenido de un gestor de recursos. Esta clase base NO gestiona los
	//recursos asignados. Una superior, en cambio, si podría.
}

void Representacion_grafica::recorte_a_medidas_textura()
{
	establecer_recorte(0,0, textura->acc_w(), textura->acc_h());
}

bool Representacion_grafica::realizar_render(SDL_Renderer * p_renderer, SDL_Rect& rec, SDL_Rect& pos)
{
	return true;
}

void Representacion_grafica::volcado(SDL_Renderer * p_renderer)
{
	//TODO...
}

void Representacion_grafica::volcado(SDL_Renderer * p_renderer, const SDL_Rect& p_foco, const SDL_Rect& p_pos, double zoom)
{
	//TODO...
}


//Eso sólo deberíamos llamarlo en aquellas para las cuales hemos creado una
//textura que poseen. Si es parte de un recurso que no es de su propiedad
//te vas a encontrar con un problema.

void Representacion_grafica::liberar_textura()
{
	if(this->textura)
	{
		delete this->textura;
		this->textura=NULL;
	}
}

void Representacion_grafica::transformar_rotar(float v) 
{
	transformacion.rotar(v);
	actualizar_caja_rotacion();
}

void Representacion_grafica::transformar_cancelar_rotar() 
{
	transformacion.cancelar_rotar();
	actualizar_caja_rotacion();
}

void Representacion_grafica::transformar_centro_rotacion(float x, float y) 
{
	transformacion.centro_rotacion(x, y);
	actualizar_caja_rotacion();
}

void Representacion_grafica::transformar_centro_rotacion_cancelar() 
{
	transformacion.cancelar_centro_rotacion();
	actualizar_caja_rotacion();
}

void Representacion_grafica::actualizar_caja_rotacion()
{
	const auto& p=acc_posicion();

	if(!transformacion.es_transformacion())
	{
		posicion_rotada=p;
	}
	else
	{
		auto c=transformacion.obtener_centro_rotacion();
		DLibH::Poligono_2d_vertices<double> polig(
			{ 
				{(double)p.x, (double)p.y},
				{(double)(p.x+p.w), (double)p.y},
				{(double)(p.x+p.w), (double)(p.y+p.h)},
				{(double)p.x, (double)(p.y+p.h)},        
			}, {(double)c.x+p.x, (double)c.y+p.y});

		//Las rotaciones de SDL son "clockwise"... Las reales son "counter-clockwise"...
		float a=transformacion.obtener_angulo_rotacion();
		polig.rotar(a);

		//Sacar las medidas para la nueva caja...
		std::vector<double> xs={polig.vertice(0).x, polig.vertice(1).x, polig.vertice(2).x, polig.vertice(3).x};
		std::vector<double> ys={polig.vertice(0).y, polig.vertice(1).y, polig.vertice(2).y, polig.vertice(3).y};

		posicion_rotada.x=*std::min_element(std::begin(xs), std::end(xs));
		posicion_rotada.y=*std::min_element(std::begin(ys), std::end(ys));
		posicion_rotada.w=*std::max_element(std::begin(xs), std::end(xs))-posicion_rotada.x;
		posicion_rotada.h=*std::max_element(std::begin(ys), std::end(ys))-posicion_rotada.y;
	}
}

void Representacion_grafica::establecer_posicion(int x, int y, int w, int h, int f)
{
	Representacion::establecer_posicion(x, y, w, h, f);
	actualizar_caja_rotacion();
}

void Representacion_grafica::establecer_posicion(SDL_Rect c)
{
	Representacion::establecer_posicion(c);
	actualizar_caja_rotacion();
}

SDL_Rect Representacion_grafica::copia_posicion_rotada() const
{
	return SDL_Rect{posicion_rotada.x, posicion_rotada.y, posicion_rotada.w, posicion_rotada.h};
}

void Representacion_grafica::preparar(const SDL_Renderer * renderer)
{
	actualizar_caja_rotacion();
	marcar_como_preparada();
}
