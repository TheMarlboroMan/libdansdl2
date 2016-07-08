#include <SDL2/SDL2_gfxPrimitives.h>
#include "representacion_primitiva_poligono.h"

using namespace DLibV;

Representacion_primitiva_poligono_base::Representacion_primitiva_poligono_base(const std::vector<Punto>& pt, ColorRGBA c)
	:Representacion_primitiva(c), puntos(pt), original(pt[0])
{ 
	//Guardarlos de forma que el primero sea 0.0.
	for(auto& pt : puntos)
	{
		pt.x-=original.x;
		pt.y-=original.y;
	}
}

Representacion_primitiva_poligono_base::Representacion_primitiva_poligono_base(const Representacion_primitiva_poligono_base& p_otra)
	:Representacion_primitiva(p_otra), puntos(p_otra.puntos), original(p_otra.original)
{

}

Representacion_primitiva_poligono_base& Representacion_primitiva_poligono_base::operator=(const Representacion_primitiva_poligono_base& p_otro)
{
	Representacion_primitiva::operator=(p_otro);
	puntos=p_otro.puntos;
	original=p_otro.original;
	return *this;
}

void Representacion_primitiva_poligono_base::volcado(const Info_volcado)
{
	preparar_color();
	glMatrixMode(GL_MODELVIEW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, puntos.data());
	glDrawArrays(es_rellena() ? GL_POLYGON : GL_LINE_LOOP, 0, puntos.size());
	glDisableClientState(GL_VERTEX_ARRAY);
}

Rect Representacion_primitiva_poligono_base::obtener_base_posicion_vista() const
{
	int x=puntos[0].x+original.x, y=puntos[0].y+original.y, maxx=x, maxy=y;

	for(auto p : puntos)
	{
		//Añadir el valor original, porque estos puntos empiezan en 0.0.
		p.x+=original.x;
		p.y+=original.y;

		if(p.x < x) x=p.x;
		else if(p.x > maxx) maxx=p.x;

		if(p.y < y) y=p.y;
		else if(p.y > maxy) maxy=p.y;
	}

	return Rect{x, y, (unsigned int)maxx-x, (unsigned int)maxy-y};
}


void Representacion_primitiva_poligono_base::ir_a(int x, int y)
{
	original.x=x;
	original.y=y;
	actualizar_posicion_vista_rotacion();
}

Punto Representacion_primitiva_poligono_base::obtener_posicion() const
{
	int x=puntos[0].x+original.x, y=puntos[0].y+original.y;
	for(auto p : puntos)
	{
		//Añadir el valor original, porque estos puntos empiezan en 0.0.
		p.x+=original.x;
		p.y+=original.y;

		if(p.x < x) x=p.x;
		if(p.y < y) y=p.y;
	}

	return Punto{x, y};
}

Representacion_primitiva_poligono::Representacion_primitiva_poligono(const std::vector<Punto> &puntos, ColorRGBA c)
	:Representacion_primitiva_poligono_base(puntos, c)
{
	actualizar_posicion_vista_rotacion();
}

Representacion_primitiva_poligono::Representacion_primitiva_poligono(const Representacion_primitiva_poligono& p_otra)
	:Representacion_primitiva_poligono_base(p_otra) 
{

}

Representacion_primitiva_poligono& Representacion_primitiva_poligono::operator=(const Representacion_primitiva_poligono& p_otro)
{
	Representacion_primitiva_poligono_base::operator=(p_otro);
	return *this;
}

Representacion_primitiva_poligono_lineas::Representacion_primitiva_poligono_lineas(const std::vector<Punto>& puntos, ColorRGBA c)
	:Representacion_primitiva_poligono_base(puntos, c)
{
	actualizar_posicion_vista_rotacion();
}

Representacion_primitiva_poligono_lineas::Representacion_primitiva_poligono_lineas(const Representacion_primitiva_poligono_lineas& p_otra)
	:Representacion_primitiva_poligono_base(p_otra) 
{

}

Representacion_primitiva_poligono_lineas& Representacion_primitiva_poligono_lineas::operator=(const Representacion_primitiva_poligono_lineas& p_otro)
{
	Representacion_primitiva_poligono_base::operator=(p_otro);
	return *this;
}
