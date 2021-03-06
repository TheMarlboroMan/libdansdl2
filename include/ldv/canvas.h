#pragma once

#include "surface.h"

namespace ldv
{

//!An empty surface.

//!Mostly used in TTF representations to craft textures.

class canvas:
	public surface
{
	public:

	static canvas * 	create(int, int, const SDL_Surface *);
	static canvas * 	create(int, int, int bpp, Uint32 maskr, Uint32 maskg, Uint32 maskb, Uint32 maskalpha);

	private:

	bool 			generated;

				canvas();
};

} //Fin namespace DLibV

