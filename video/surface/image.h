#ifndef LIBDANDL2_IMAGE_H
#define LIBDANDL2_IMAGE_H

#include "surface.h"

/*
La imagen es un recurso gráfico que carga un bitmap. Más fácil imposible.
*/

namespace ldv
{

class image:
	public surface
{
	public:

			image(SDL_Surface *);
			image(std::string const&, SDL_Window *);
			image(std::string const&);
			image(const image&);
	image& 		operator=(const image&);
	virtual 	~image();

	types 		get_type() const {return types::image;}
};

} //Fin namespace DLibV

#endif
