#ifndef FUENTE_TTF_H
#define FUENTE_TTF_H

#include <SDL2/SDL_ttf.h>
#include <memory>
#include <stdexcept>
#include <string>

namespace DLibV
{

class Fuente_TTF
{
	public:

					Fuente_TTF(const std::string&, int);
					~Fuente_TTF();

	//TODO: Definir constructores de copia y demás, porque estarían COMPARTIENDO
	//el mismo puntero.

	const std::string&		acc_nombre_fuente() const {return nombre_fuente;}
	int				acc_tamano_fuente() const {return tamano_fuente;}

	private:

	std::string			nombre_fuente;
	int				tamano_fuente;

	TTF_Font *			fuente;
};

}

#endif

