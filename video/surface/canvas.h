#ifndef LIBDANSDL2_CANVAS_H
#define LIBDANSDL2_CANVAS_H

#include "surface.h"

namespace ldv
{

//!An empty surface.

//!Mostly used in TTF representations to craft textures.

class canvas:
	public surface
{
	public:
	
	virtual 		~canvas();
				canvas(const canvas&);
				canvas& operator=(const canvas&);
	static canvas * 	create(int, int, const SDL_Surface *); 	//Se genera un recurso que NO se registraría... No me termina de gustar.
	static canvas * 	create(int, int, int bpp, Uint32 maskr, Uint32 maskg, Uint32 maskb, Uint32 maskalpha);
	static canvas * 	copy(const canvas&);

	private:

	bool 			generated;
				canvas();
};

} //Fin namespace DLibV

#endif
