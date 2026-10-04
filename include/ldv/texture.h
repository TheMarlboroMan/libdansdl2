#pragma once

#include "surface.h"

#include <GL/gl.h>
#ifdef WINCOMPIL
#include <GL/glext.h>
#endif
#include <SDL2/SDL.h>

namespace ldv
{

//!Wrapper for a SDL texture.

//!Textures are created from ldv::surface derived objects.

class texture {

	public:
	                texture(const surface&);
	                texture(const texture&);
	texture&        operator=(const texture&);
	                texture(texture&&);
	texture&        operator=(texture&&); //but they can be move assigned to.
	                ~texture();

	//!Gets texture width.
	unsigned int    get_w() const {return w;}
	//!Gets texture height.
	unsigned int    get_h() const {return h;}
	//!Gets openGL texture index.
	GLuint          get_index() const {return index;}
	void            replace(const surface&);

	private:

	void            load(const SDL_Surface *);

	GLuint          index;
	int             mode; 	//! <OpenGL mode... GL_RGB by default.
	unsigned int    w,
	                h;
};

GLuint duplicate_texture(GLuint, unsigned int, unsigned int, int);

}

