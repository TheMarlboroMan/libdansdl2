#include "representacion_ttf.h"

using namespace DLibV;

Representacion_TTF::Representacion_TTF(const Fuente_TTF& fuente, SDL_Color color, const std::string& texto)
	:Representacion_grafica(), 
	textura(nullptr)
	fuente(&fuente),
	color(color),
	cadena(texto)
{

}

Representacion_TTF::Representacion_TTF(const Fuente_TTF& fuente, int r, int g, int b, int a, const std::string& texto)
	:Representacion_grafica(), 
	textura(nullptr)
	fuente(&fuente),
	color{r, g, b, a},
	cadena(texto)	
{

}

Representacion_TTF::Representacion_TTF(const Fuente_TTF& fuente, SDL_Color color)
	:Representacion_grafica(), 
	textura(nullptr),
	fuente(&fuente),
	color(color)
{

}

Representacion_TTF::Representacion_TTF(const Fuente_TTF& fuente, int r, int g, int b, int a):
	:Representacion_grafica(), 
	textura(nullptr),
	fuente(&fuente),
	color{r, g, b, a}
{

}

Representacion_TTF::Representacion_TTF(const Representacion_TTF& o)
	:Representacion_grafica(o), 
	textura(nullptr),
	fuente(o.fuente),
	color(o.color),
	
{
	marcar_como_no_preparada();
}


Representacion_TTF& Representacion_TTF::operator=(const Representacion_TTF& o)
{
	Representacion_grafica::operator=(p_otra);
	marcar_como_no_preparada();
}

void Representacion_TTF::preparar(const SDL_Renderer * renderer)
{
	Superficie s(TTF_RenderText_Blended(fuente.acc_fuente(), cadena.c_str(), color));
	textura.reset(new Textura(surf->acc_superficie(), renderer));
	marcar_como_preparada();
}

void Representacion_TTF::modificar_fuente(const Fuente_TTF& fuente)
{
	fuente=&fuente;
	textura.reset();
	marcar_como_no_preparada();
}

void Representacion_TTF::asignar(const char c)
{
	textura.reset();
	cadena.assign(c);
	marcar_como_no_preparada();
}

void Representacion_TTF::asignar(const char * c)
{
	textura.reset();
	cadena=c;
	marcar_como_no_preparada();
}

void Representacion_TTF::asignar(const std::string& c)
{
	textura.reset();
	cadena=c;
	marcar_como_no_preparada();
}
