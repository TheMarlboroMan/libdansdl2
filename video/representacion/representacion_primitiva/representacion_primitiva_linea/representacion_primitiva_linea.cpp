#include "representacion_primitiva_linea.h"

using namespace DLibV;



Representacion_primitiva_linea::Representacion_primitiva_linea(int px1, int py1, int px2, int py2, ColorRGBA c)
	:Representacion_primitiva(c)
{
	puntos[0]={px1, py1};
	puntos[1]={px2, py2};
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

void Representacion_primitiva_linea::establecer_puntos(int px1, int py1, int px2, int py2)
{
	puntos[0]={px1, py1};
	puntos[1]={px2, py2};
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

	f(puntos[0].x, puntos[1].x, x, w);
	f(puntos[0].y, puntos[1].y, y, h);

	establecer_posicion(x, y, w, h);
}

void Representacion_primitiva_linea::volcado(SDL_Renderer * p_renderer)
{
	preparar_color();
	glMatrixMode(GL_MODELVIEW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, puntos.data());
	glDrawArrays(GL_LINES, 0, 2);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Representacion_primitiva_linea::volcado(SDL_Renderer * p_renderer, const SDL_Rect& p_enfoque, const SDL_Rect& p_posicion, double zoom)
{
	//TODO...
}

void Representacion_primitiva_linea::preparar_posicion()
{
	//No hace nada.
}	
