#include <SDL2/SDL2_gfxPrimitives.h>
#include "representacion_primitiva_poligono.h"

using namespace DLibV;

Representacion_primitiva_poligono_base::Representacion_primitiva_poligono_base(const std::vector<punto>& pt, Uint8 pr, Uint8 pg, Uint8 pb)
	:Representacion_primitiva(pr, pg, pb), puntos(pt)
{
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
	//TODO: This should be in the screen function...

//	if(!es_visible()) return false;
//	SDL_Rect pos=acc_posicion();

	if(es_rellena()) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//TODO: Tesellation is fucked up.

	glMatrixMode(GL_MODELVIEW);
	//TODO: Alpha no funciona.
	//TODO: Fix the hell color...
	glColor4f(0.25f, 1.f, 0.25f, 1.f); //Demodular color...
	glDisable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, puntos.data());
	glDrawArrays(GL_POLYGON, 0, puntos.size());
	glDisableClientState(GL_VERTEX_ARRAY);

	return true;
}

bool Representacion_primitiva_poligono_base::volcado(SDL_Renderer * p_renderer, const SDL_Rect& p_enfoque, const SDL_Rect& p_posicion, double zoom)
{
	//TODO...
	return true;
}

void Representacion_primitiva_poligono_base::preparar_posicion()
{
	int x=puntos[0].x, y=puntos[0].y, maxx=x, maxy=y;

	for(const auto& p : puntos)
	{
		if(p.x < x) x=p.x;
		else if(p.x > maxx) maxx=p.x;

		if(p.y < y) y=p.y;
		else if(p.y > maxy) maxy=p.y;
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
