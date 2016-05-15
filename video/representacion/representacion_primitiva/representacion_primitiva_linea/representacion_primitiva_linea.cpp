#include "representacion_primitiva_linea.h"

using namespace DLibV;



Representacion_primitiva_linea::Representacion_primitiva_linea(int px1, int py1, int px2, int py2, Uint8 pr, Uint8 pg, Uint8 pb)
	:Representacion_primitiva(pr, pg, pb), x1(px1), y1(py1), x2(px2), y2(py2)
{
	generar_posicion();
}

Representacion_primitiva_linea::Representacion_primitiva_linea(const Representacion_primitiva_linea& p_otra)
	:Representacion_primitiva(p_otra)
{

}

Representacion_primitiva_linea& Representacion_primitiva_linea::operator=(const Representacion_primitiva_linea& p_otro)
{
	Representacion_primitiva::operator=(p_otro);
	return *this;
}

Representacion_primitiva_linea::~Representacion_primitiva_linea()
{

}

void Representacion_primitiva_linea::establecer_puntos(int px1, int py1, int px2, int py2)
{
	x1=px1;
	y1=py1;
	x2=px2;
	y2=py2;
	generar_posicion();
}

void Representacion_primitiva_linea::generar_posicion()
{
	int x, y, w, h;

	auto f=[](int v1, int v2, int& pos, int& medida)
	{
		if(v1 < v2)
		{
			pos=v1;
			medida=abs(v2-v1);
		}
		else
		{
			pos=v2;
			medida=abs(v1-v2);
		}
	};

	f(x1, x2, x, w);
	f(y1, y2, y, h);

	establecer_posicion(x, y, w, h);
}

bool Representacion_primitiva_linea::volcado(SDL_Renderer * p_renderer)
{
	return true;
}

bool Representacion_primitiva_linea::volcado(SDL_Renderer * p_renderer, const SDL_Rect& p_enfoque, const SDL_Rect& p_posicion, double zoom)
{
	return true;
}

void Representacion_primitiva_linea::preparar_posicion()
{
	//No hace nada.
}	
