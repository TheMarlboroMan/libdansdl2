#include "representacion_primitiva_puntos.h"
#include <algorithm>

using namespace DLibV;

Representacion_primitiva_puntos::Representacion_primitiva_puntos(int x, int y, Uint8 r, Uint8 g, Uint8 b)
	:Representacion_primitiva(r, g, b)
{
	insertar(x, y);	
	generar_posicion();
}

Representacion_primitiva_puntos::Representacion_primitiva_puntos(Uint8 r, Uint8 g, Uint8 b)
	:Representacion_primitiva(r, g, b)
{
	generar_posicion();
}

Representacion_primitiva_puntos::Representacion_primitiva_puntos(const Representacion_primitiva_puntos& p_otra)
	:Representacion_primitiva(p_otra)
{

}

Representacion_primitiva_puntos& Representacion_primitiva_puntos::operator=(const Representacion_primitiva_puntos& p_otro)
{
	Representacion_primitiva::operator=(p_otro);
	return *this;
}

Representacion_primitiva_puntos::~Representacion_primitiva_puntos()
{

}

void Representacion_primitiva_puntos::insertar(int x, int y)
{
	puntos.push_back(SDL_Point{x, y});
	generar_posicion();
}

void Representacion_primitiva_puntos::limpiar_puntos()
{
	puntos.clear();
	generar_posicion();
}	

void Representacion_primitiva_puntos::generar_posicion()
{
	if(!puntos.size())
	{
		establecer_posicion(0, 0, 0, 0);
	}
	else
	{
		//Del manual: 
		//The value returned indicates whether the element passed as first argument is considered less than the second.
		struct {bool operator() (SDL_Point a, SDL_Point b) {return a.x < b.x;}}fx;
		struct {bool operator() (SDL_Point a, SDL_Point b) {return a.y < b.y;}}fy;

		auto min_x=*std::min_element(std::begin(puntos), std::end(puntos), fx);
		auto max_x=*std::max_element(std::begin(puntos), std::end(puntos), fx);
		auto min_y=*std::min_element(std::begin(puntos), std::end(puntos), fy);
		auto max_y=*std::max_element(std::begin(puntos), std::end(puntos), fy);
		establecer_posicion(min_x.x, min_y.y, max_x.x-min_x.x, max_y.y-min_y.y);
	}
}

void Representacion_primitiva_puntos::volcado(SDL_Renderer * p_renderer)
{
	//TODO...
}

void Representacion_primitiva_puntos::volcado(SDL_Renderer * p_renderer, const SDL_Rect& p_enfoque, const SDL_Rect& p_posicion, double zoom)
{
	//TODO...
}

void Representacion_primitiva_puntos::preparar_posicion()
{
	//No hace nada.
}	
