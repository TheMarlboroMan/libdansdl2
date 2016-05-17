#include "representacion_primitiva_puntos.h"
#include <algorithm>

using namespace DLibV;

Representacion_primitiva_puntos::Representacion_primitiva_puntos(int x, int y, ColorRGBA c)
	:Representacion_primitiva(c)
{
	insertar(x, y);
	generar_posicion();
}

Representacion_primitiva_puntos::Representacion_primitiva_puntos(ColorRGBA c)
	:Representacion_primitiva(c)
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

void Representacion_primitiva_puntos::insertar(int x, int y)
{
	puntos.push_back({x, y});
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
		struct {bool operator() (Representacion_primitiva::punto a, Representacion_primitiva::punto b) {return a.x < b.x;}}fx;
		struct {bool operator() (Representacion_primitiva::punto a, Representacion_primitiva::punto b) {return a.y < b.y;}}fy;

		auto min_x=*std::min_element(std::begin(puntos), std::end(puntos), fx);
		auto max_x=*std::max_element(std::begin(puntos), std::end(puntos), fx);
		auto min_y=*std::min_element(std::begin(puntos), std::end(puntos), fy);
		auto max_y=*std::max_element(std::begin(puntos), std::end(puntos), fy);
		establecer_posicion(min_x.x, min_y.y, max_x.x-min_x.x, max_y.y-min_y.y);
	}
}

void Representacion_primitiva_puntos::volcado(SDL_Renderer * p_renderer)
{
	preparar_color();
	glMatrixMode(GL_MODELVIEW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, puntos.data());
	glDrawArrays(GL_POINTS, 0, puntos.size());
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Representacion_primitiva_puntos::volcado(SDL_Renderer * p_renderer, const SDL_Rect& p_enfoque, const SDL_Rect& p_posicion, double zoom)
{
	//TODO...
}

void Representacion_primitiva_puntos::preparar_posicion()
{
	//No hace nada.
}	
