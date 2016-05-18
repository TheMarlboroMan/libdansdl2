#ifndef TEXTURA_SDL_H
#define TEXTURA_SDL_H

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include "../utilidades_graficas_sdl/utilidades_graficas_sdl.h"
#include "../superficie/imagen.h"

namespace DLibV
{

class Textura
{
	public:
			Textura(const Superficie&);
			Textura(const Textura& t);
			Textura(const std::string&);
			~Textura();
	Textura& 	operator=(const Textura& t);

	unsigned int 	acc_w() const {return w;}
	unsigned int 	acc_h() const {return h;}

	private:

	void		cargar(const SDL_Surface *);

	GLuint		indice;
	int 		w;
	int 		h;
};

}

#endif
