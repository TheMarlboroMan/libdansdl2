#include <ldv/texture.h>
#include <stdexcept>
#include <vector>
#include <cstdint>

#ifdef LIBDANSDL2_DEBUG
#include <lm/log.h>
#include <ldt/log.h>
#endif

using namespace ldv;

//!Class constructor.

//!Creates a texture from a ldv::surface object. The surface itself can be
//!discarded later.

texture::texture(const surface& s):
	index(0), mode(GL_RGB), w(0), h(0)
{
	load(s.get_surface());
}

texture::texture(
	texture&& _other
):
	index{_other.index},
	mode{_other.mode},
	w{_other.w},
	h{_other.h}
{

	_other.index=0;
	_other.w=0;
	_other.h=0;
}

texture& texture::operator=(
	texture&& _other
) {
	index=_other.index;
	mode=_other.mode;
	w=_other.w;
	h=_other.h;

	_other.index=0;
	_other.w=0;
	_other.h=0;

	return *this;
}

//!Class destructor

//!Triggers openGL deletion mechanism.

texture::~texture()
{
	glDeleteTextures(1, &index);
}

//!Replaces the current texture with the new ldv::surface object.

//!Implemented in terms of "load".

void texture::replace(const surface& s)
{
	load(s.get_surface());
}

//!Loads the texture into openGL.

//!There may be funky glitches with byte ordering.

void texture::load(
	const SDL_Surface * surface
) {
	w=surface->w;
	h=surface->h;

	//If there is no index, we request a new from opengl.
	if(!index) {
		glGenTextures(1, &index);
	}

	glBindTexture(GL_TEXTURE_2D, index);

	if(SDL_ISPIXELFORMAT_INDEXED(const_cast<SDL_Surface*>(surface)->format->format)) {

#ifdef LIBDANSDL2_DEBUG
		lm::log(ldt::log_lsdl::get()).debug()<<"detected indexed surface, will convert"<<std::endl;
#endif

		SDL_PixelFormat * targetformat=SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
		SDL_Surface * converted=SDL_ConvertSurface(const_cast<SDL_Surface *>(surface), targetformat, 0);
		mode=GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, mode, w, h, 0, mode, GL_UNSIGNED_BYTE, converted->pixels);
		SDL_FreeSurface(converted);
		SDL_FreeFormat(targetformat);
		return;
	}

	mode=surface->format->BytesPerPixel==4
		? GL_RGBA
		: GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, mode, w, h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
}
