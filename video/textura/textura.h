#ifndef TEXTURA_SDL_H
#define TEXTURA_SDL_H

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include "../superficie/superficie.h"

namespace DLibV
{

class Textura
{
	public:
			Textura(const Superficie&);
			Textura(const Textura& t);
			~Textura();
	Textura& 	operator=(const Textura& t);

	unsigned int 	acc_w() const {return w;}
	unsigned int 	acc_h() const {return h;}
	GLuint		acc_indice() const {return indice;}

	private:

	void		cargar(const SDL_Surface *);

	GLuint		indice;
	int 		w;
	int 		h;
};

}

#endif
