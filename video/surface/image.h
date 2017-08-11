#ifndef LIBDANDL2_IMAGE_H
#define LIBDANDL2_IMAGE_H

#include "surface.h"

namespace ldv
{

//!A surface loaded with a previously existing surface or image.

//!I think it only exists to create textures from it.

class image:
	public surface
{
	public:

			image(SDL_Surface *);
	//TODO: This is obsolete...
			image(std::string const&, SDL_Window *);
			image(std::string const&);
			image(const image&);
	image& 		operator=(const image&);
	virtual 	~image();
};

} //Fin namespace DLibV

#endif
