#include <ldv/texture.h>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <sstream>

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
	const texture& _other
):
	index{duplicate_texture(_other.index, _other.w, _other.h, _other.mode)},
	mode{_other.mode},
	w{_other.w},
	h{_other.h}
{}

texture& texture::operator=(
	const texture& _other
) {

	index=duplicate_texture(_other.index, _other.w, _other.h, _other.mode);
	w=_other.w;
	h=_other.h;
	mode=_other.mode;

	return *this;
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
}

texture& texture::operator=(
	texture&& _other
) {

	index=_other.index;
	mode=_other.mode;
	w=_other.w;
	h=_other.h;

	_other.index=0;

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

	auto check_failure=[this](const std::string& _context) {

		auto error=glGetError();
		if(GL_NO_ERROR!=error) {

			std::stringstream ss;
			ss<<"texture load failed with glerror "<<error<<" for an image of "<<w<<"x"<<h<<": "<<_context;
			throw std::runtime_error(ss.str());
		}
	};

	//Terribly pump out all previous errors. Yes, we should do this better...
	while(GL_NO_ERROR!=glGetError()) {}

	w=surface->w;
	h=surface->h;

	//If there is no index, we request a new from opengl.
	if(!index) {

		glGenTextures(1, &index);
		check_failure("unable to request index");
	}

	glBindTexture(GL_TEXTURE_2D, index);
	check_failure("unable to bind texture");

	if(SDL_ISPIXELFORMAT_INDEXED(const_cast<SDL_Surface*>(surface)->format->format)) {

#if defined (LIBDANSDL2_DEBUG) && defined (LIBDANSDL2_DEBUG_EXTRA_LOG)

		lm::log(ldt::log_lsdl::get()).debug()<<"detected indexed surface, will convert to RGBA32"<<std::endl;
#endif

		SDL_PixelFormat * targetformat=SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
		SDL_Surface * converted=SDL_ConvertSurface(const_cast<SDL_Surface *>(surface), targetformat, 0);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, converted->pixels);

		check_failure("unable to generate texture");

		SDL_FreeSurface(converted);
		SDL_FreeFormat(targetformat);
		return;
	}

	if(surface->format->BytesPerPixel==4) {

#if defined (LIBDANSDL2_DEBUG) && defined (LIBDANSDL2_DEBUG_EXTRA_LOG)

		lm::log(ldt::log_lsdl::get()).debug()<<"detected a surface of 4bpp, will map it to RGBA32"<<std::endl;
#endif

		SDL_PixelFormat * targetformat=SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
		SDL_Surface * converted=SDL_ConvertSurface(const_cast<SDL_Surface *>(surface), targetformat, 0);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, converted->pixels);

		check_failure("unable to generate texture");

		SDL_FreeSurface(converted);
		SDL_FreeFormat(targetformat);
		return;
	}
	
	if(surface->format->BytesPerPixel==3) {

#if defined (LIBDANSDL2_DEBUG) && defined (LIBDANSDL2_DEBUG_EXTRA_LOG)

		lm::log(ldt::log_lsdl::get()).debug()<<"detected a surface of 3bpp, will map it to RGB24"<<std::endl;

#endif

		SDL_PixelFormat * targetformat=SDL_AllocFormat(SDL_PIXELFORMAT_RGB24);
		SDL_Surface * converted=SDL_ConvertSurface(const_cast<SDL_Surface *>(surface), targetformat, 0);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, converted->pixels);

		check_failure("unable to generate texture");

		SDL_FreeSurface(converted);
		SDL_FreeFormat(targetformat);
		return;
	}

	std::stringstream ss;
	ss<<"attempted to load texture with "<<surface->format->BytesPerPixel<<" bpp, should be 4 or 3";
	throw std::runtime_error(ss.str());
}

GLuint ldv::duplicate_texture(
	GLuint _index,
	unsigned int _w,
	unsigned int _h,
	int _mode
) {

	const unsigned int components=GL_RGB==_mode
		? 3
		: 4; //rgba and stuff... there may be even less components but we are working with 4 and 3 bpp here.

	std::vector<unsigned char> pixels(_w * _h * components);

	//Select and read original texture...
	glBindTexture(GL_TEXTURE_2D, _index);
	glGetTexImage(
		GL_TEXTURE_2D,
		0,
		_mode,
		GL_UNSIGNED_BYTE,
		pixels.data()
	);

	//Create new texture...
	GLuint new_index;
	glGenTextures(1, &new_index);
	glBindTexture(GL_TEXTURE_2D, new_index); //select it...
	//..and drop data in.
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		_mode,
		_w, _h,
		0,
		_mode,
		GL_UNSIGNED_BYTE,
		pixels.data()
	);

	return new_index;
}

