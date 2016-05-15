#include <SDL2/SDL2_gfxPrimitives.h>
#include "representacion_primitiva_poligono.h"

using namespace DLibV;

Representacion_primitiva_poligono_base::Representacion_primitiva_poligono_base(const std::vector<punto>& puntos, Uint8 pr, Uint8 pg, Uint8 pb)
	:Representacion_primitiva(pr, pg, pb)
{
	//Crear array de puntos...
	for(const auto& p : puntos)
	{
		puntos_x.push_back(p.x);
		puntos_y.push_back(p.y);
	}

	//Calcular el rectángulo de posición para clip de cámara...
	this->preparar_posicion();
}

Representacion_primitiva_poligono_base::Representacion_primitiva_poligono_base(const Representacion_primitiva_poligono_base& p_otra)
	:Representacion_primitiva(p_otra)
{

}

Representacion_primitiva_poligono_base& Representacion_primitiva_poligono_base::operator=(const Representacion_primitiva_poligono_base& p_otro)
{
	Representacion_primitiva::operator=(p_otro);
	return *this;
}

Representacion_primitiva_poligono_base::~Representacion_primitiva_poligono_base()
{

}

bool Representacion_primitiva_poligono_base::volcado(SDL_Renderer * p_renderer)
{
	return true;
}

bool Representacion_primitiva_poligono_base::volcado(SDL_Renderer * p_renderer, const SDL_Rect& p_enfoque, const SDL_Rect& p_posicion, double zoom)
{
	return true;
}

void Representacion_primitiva_poligono_base::preparar_posicion()
{
	int x=puntos_x[0], y=puntos_y[0], maxx=x, maxy=y;

	for(const auto& px : puntos_x)
	{
		if(px < x) x=px;
		else if(px > maxx) maxx=px;
	}

	for(const auto& py : puntos_y)
	{
		if(py < y) y=py;
		else if(py > maxy) maxy=py;
	}

	SDL_Rect p_pos {x, y, maxx-x, maxy-y};
	establecer_posicion(p_pos);
}	

Representacion_primitiva_poligono& Representacion_primitiva_poligono::operator=(const Representacion_primitiva_poligono& p_otro)
{
	Representacion_primitiva_poligono_base::operator=(p_otro);
	return *this;
}

Representacion_primitiva_poligono::Representacion_primitiva_poligono(const std::vector<punto> &puntos, Uint8 pr, Uint8 pg, Uint8 pb)
	:Representacion_primitiva_poligono_base(puntos, pr, pg, pb)
{

}

Representacion_primitiva_poligono::Representacion_primitiva_poligono(const Representacion_primitiva_poligono& p_otra)
	:Representacion_primitiva_poligono_base(p_otra) 
{

}

Representacion_primitiva_poligono_lineas& Representacion_primitiva_poligono_lineas::operator=(const Representacion_primitiva_poligono_lineas& p_otro)
{
	Representacion_primitiva_poligono_base::operator=(p_otro);
	return *this;
}

Representacion_primitiva_poligono_lineas::Representacion_primitiva_poligono_lineas(const std::vector<punto>& puntos, Uint8 pr, Uint8 pg, Uint8 pb)
	:Representacion_primitiva_poligono_base(puntos, pr, pg, pb)
{

}

Representacion_primitiva_poligono_lineas::Representacion_primitiva_poligono_lineas(const Representacion_primitiva_poligono_lineas& p_otra)
	:Representacion_primitiva_poligono_base(p_otra) 
{

}
