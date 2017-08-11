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

//!Wrapper for a SDL texture.

//!Textures are created from ldv::surface derived objects.

class texture
{
	public:
			texture(const surface&);
			texture(const texture&)=delete;
			~texture();
	texture& 	operator=(const texture& t)=delete;

	//!Gets texture width.
	unsigned int 	get_w() const {return w;}
	//!Gets texture height.
	unsigned int 	get_h() const {return h;}
	//!Gets openGL texture index.
	GLuint		get_index() const {return index;}
	void		replace(const surface&);

	private:

	void		load(const SDL_Surface *);

	GLuint		index;
	int 		mode, 	//! <OpenGL mode... GL_RGB by default.
			w,
	 		h;
};

}

#endif
