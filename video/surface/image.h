#ifndef LIBDANDL2_IMAGE_H
#define LIBDANDL2_IMAGE_H

#include <vector>
#include "surface.h"

namespace ldv
{

//!A surface loaded with a previously existing surface or image. 

//!I think it only exists to create textures from it.

class image:
	public surface
{
	public:

			//!Construct from a SDL_surface
			image(SDL_Surface *);

			//TODO: These should be named constructors... but they should forward perfectly.
			//and I don't want to investigate this right now... however, I want something
			//like image::from_file and image::from_memory
			//!Construct from a filepath.
			image(const std::string&);

			//!Construct from a memory sequence...
			image(const std::vector<unsigned char>&);

			image(const image&);

	image& 		operator=(const image&);
	virtual 	~image();
};

} //Fin namespace DLibV

#endif
