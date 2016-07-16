#ifndef LIBDANSDL2_TEXTURE_H
#define LIBDANSDL2_TEXTURE_H

#include <GL/gl.h>
#ifdef WINCOMPIL
#include <GL/glext.h>
#endif
#include <SDL2/SDL.h>
#include "../surface/surface.h"

namespace ldv
{

class texture
{
	public:
			texture(const surface&);
			texture(const texture&)=delete;
			~texture();
	texture& 	operator=(const texture& t)=delete;

	unsigned int 	get_w() const {return w;}
	unsigned int 	get_h() const {return h;}
	GLuint		get_index() const {return index;}
	void		replace(const surface&);

	private:

	void		load(const SDL_Surface *);

	GLuint		index;
	int 		mode, 
			w,
	 		h;
};

}

#endif
