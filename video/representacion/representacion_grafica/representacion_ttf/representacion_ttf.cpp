#include "representacion_ttf.h"

using namespace DLibV;

Representacion_TTF::Representacion_TTF(const Fuente_TTF& fuente, SDL_Color color, const std::string& texto)
	:Representacion_grafica(), 
	textura(nullptr),
	fuente(&fuente),
	cadena(texto),
	color(color)
{

}

Representacion_TTF::Representacion_TTF(const Fuente_TTF& fuente, Uint8 r, Uint8 g, Uint8 b, Uint8 a, const std::string& texto)
	:Representacion_grafica(), 
	textura(nullptr),
	fuente(&fuente),
	cadena(texto),
	color{r, g, b, a}
{

}

Representacion_TTF::Representacion_TTF(const Fuente_TTF& fuente, SDL_Color color)
	:Representacion_grafica(), 
	textura(nullptr),
	fuente(&fuente),
	cadena(""),
	color(color)
{

}

Representacion_TTF::Representacion_TTF(const Fuente_TTF& fuente, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	:Representacion_grafica(), 
	textura(nullptr),
	fuente(&fuente),
	cadena(""),
	color{r, g, b, a}
{

}

Representacion_TTF::Representacion_TTF(const Representacion_TTF& o)
	:Representacion_grafica(o), 
	textura(nullptr),
	fuente(o.fuente),
	cadena(""),
	color(o.color)
{
	marcar_como_no_preparada();
}


Representacion_TTF& Representacion_TTF::operator=(const Representacion_TTF& o)
{
	Representacion_grafica::operator=(o);
	marcar_como_no_preparada();
	textura.reset(nullptr);
	fuente=o.fuente;
	cadena=o.cadena;
	color=o.color;

	return *this;
}

void Representacion_TTF::preparar(const SDL_Renderer * renderer)
{
	
/*	
	//TODO: Superficie es abstracta... No tenemos una forma de cargarla
	desde una ttf... Quizás podamos hacer una subclase "superficiettf".
	Superficie s(
		TTF_RenderText_Blended
			(const_cast<TTF_Font*>(fuente->acc_fuente()), 
			cadena.c_str(), 
			color) );
*/
	SDL_Surface * s=TTF_RenderText_Blended
			(const_cast<TTF_Font*>(fuente->acc_fuente()), 
			cadena.c_str(), 
			color);	

	if(!s)
	{
		throw std::runtime_error("Error preparando Representacion_TTF");
	}

	textura.reset(new Textura(renderer, s));

	SDL_FreeSurface(s);
	marcar_como_preparada();
}

void Representacion_TTF::modificar_fuente(const Fuente_TTF& f)
{
	fuente=&f;
	textura.reset();
	marcar_como_no_preparada();
}

void Representacion_TTF::asignar(const char c)
{
	textura.reset();

	//Hay que joderse...
	cadena.assign(""+c);
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
