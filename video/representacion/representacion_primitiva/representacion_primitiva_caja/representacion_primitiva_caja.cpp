#include "representacion_primitiva_caja.h"

using namespace DLibV;

Representacion_primitiva_caja_base::Representacion_primitiva_caja_base(const SDL_Rect& p_pos, const ColorRGBA& c)
	//TODO...
	:Representacion_primitiva(c.r, c.g, c.b)
{
	establecer_posicion(p_pos);
	this->preparar_posicion();
}

Representacion_primitiva_caja_base::Representacion_primitiva_caja_base(const Representacion_primitiva_caja_base& p_otra)
	:Representacion_primitiva(p_otra)
{

}

Representacion_primitiva_caja_base& Representacion_primitiva_caja_base::operator=(const Representacion_primitiva_caja_base& p_otro)
{
	Representacion_primitiva::operator=(p_otro);
	return *this;
}

Representacion_primitiva_caja_base::~Representacion_primitiva_caja_base()
{

}

void Representacion_primitiva_caja_base::volcado(SDL_Renderer * p_renderer)
{
	SDL_Rect pos=acc_posicion();

	//TODO: This is stupid as hell.
	struct punto{float x, y;};
	auto itof=[](int x, int y)
	{
		return punto{(float)x, (float)y};
	};

	//TODO... This should be somewhere else.
	//TODO... This whole class could be a subclass of a polygon.
	std::vector<punto> puntos{ itof(pos.x, pos.y), 
		itof(pos.x+pos.w, pos.y), 
		itof(pos.x+pos.w, pos.y+pos.h), 
		itof(pos.x, pos.y+pos.h) };

	if(es_rellena()) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glMatrixMode(GL_MODELVIEW);
	//TODO: Alpha no funciona.
	//TODO: Fix the hell color...
	glColor4f(0.25f, 1.f, 0.25f, 1.f); //Demodular color...
	glDisable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, puntos.data());
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Representacion_primitiva_caja_base::volcado(SDL_Renderer * p_renderer, const SDL_Rect& p_enfoque, const SDL_Rect& p_posicion, double zoom)
{
	//TODO...
}

void Representacion_primitiva_caja_base::preparar_posicion()
{
	//No hace nada.
}	

Representacion_primitiva_caja& Representacion_primitiva_caja::operator=(const Representacion_primitiva_caja& p_otro)
{
	Representacion_primitiva_caja_base::operator=(p_otro);
	return *this;
}

Representacion_primitiva_caja::Representacion_primitiva_caja(const SDL_Rect& p_pos, const ColorRGBA& c)
	:Representacion_primitiva_caja_base(p_pos, c)
{

}

Representacion_primitiva_caja::Representacion_primitiva_caja(const Representacion_primitiva_caja& p_otra)
	:Representacion_primitiva_caja_base(p_otra) 
{

}

Representacion_primitiva_caja_lineas& Representacion_primitiva_caja_lineas::operator=(const Representacion_primitiva_caja_lineas& p_otro)
{
	Representacion_primitiva_caja_base::operator=(p_otro);
	return *this;
}

Representacion_primitiva_caja_lineas::Representacion_primitiva_caja_lineas(const SDL_Rect& p_pos, const ColorRGBA& c)
	:Representacion_primitiva_caja_base(p_pos, c)
{

}

Representacion_primitiva_caja_lineas::Representacion_primitiva_caja_lineas(const Representacion_primitiva_caja_lineas& p_otra)
	:Representacion_primitiva_caja_base(p_otra) 
{

}
