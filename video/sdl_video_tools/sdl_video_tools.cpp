#include "sdl_video_tools.h"

#include "../../tools/log/log.h"
#include <stdexcept>

/*! \file sdl_video_tools.h
    \brief Video functions.
    
	Miscellaneous video and SDL related functionality.
*/

using namespace ldv;

void ldv::set_cursor_visible(bool v)
{
	log_lsdl::get()<<"set_cursor_visible "<<v<<std::endl;
	SDL_ShowCursor(v);
}

//!Sets the vsync value.

void ldv::set_vsync(bool v)
{
	log_lsdl::get()<<"set_vsync "<<v<<std::endl;
	SDL_GL_SetSwapInterval(v ? 1 : 0);
}

//!Returns the vsync value.

bool ldv::get_vsync()
{
	return SDL_GL_GetSwapInterval();
}

//!Creates a SDL_Surface with the same format and size as the one given.

SDL_Surface * ldv::new_sdl_surface(SDL_Surface const * porigin)
{
	SDL_Rect box{0,0, porigin->w, porigin->h};
	return new_sdl_surface(porigin, box, 0, 0);
}

//!Creates a SDL_Surface with the same format as the one given, the specified size (2nd) flags (3rd parameter) and background color in integer form (4th parameter, check SDL documentation on this).

SDL_Surface * ldv::new_sdl_surface(SDL_Surface const * porigin, const SDL_Rect& pbox, Uint32 pflags, Uint32 pcolor)
{
	SDL_Surface * origin=const_cast <SDL_Surface *> (porigin);
	SDL_Surface * copy=nullptr;

	copy=SDL_CreateRGBSurface(
//		origin->flags|pflags,
		pflags,
		pbox.w, pbox.h, origin->format->BitsPerPixel,
		origin->format->Rmask, origin->format->Gmask, origin->format->Bmask, origin->format->Amask);

	SDL_FillRect(copy, nullptr, pcolor);

	return copy;
}

//!Copies a SDL_Surface.

SDL_Surface * ldv::copy_sdl_surface(SDL_Surface const * porigin)
{
	SDL_Rect pbox{0, 0, porigin->w, porigin->h};
	return copy_sdl_surface(porigin, pbox, porigin->flags, 0);
}

//!Copies part of a SDL surface  with the specified flags.

SDL_Surface * ldv::copy_sdl_surface(SDL_Surface const * porigin, const SDL_Rect& pbox, Uint32 pflags, Uint32 pcolor)
{
	SDL_Surface * copy=new_sdl_surface(porigin, pbox, pflags, pcolor);
	SDL_Surface * origin=const_cast <SDL_Surface *> (porigin);
	SDL_Rect * box=const_cast <SDL_Rect *> (&pbox);

	if(copy)
	{
		SDL_BlitSurface(origin, box, copy, nullptr);
	}

	return copy;
}

//!Loads image into SDL_Surface. 

//This goes well with open_gl. Use this one.

SDL_Surface * ldv::load_image(const std::string& path) {

	SDL_Surface * temp=IMG_Load(path.c_str());
	if (!temp) {
		throw std::runtime_error(std::string("ldv::load_image() : unable to load image ")+path);
	}

	return temp;
}

SDL_Surface * ldv::load_image_from_memory(const unsigned char * _seq, size_t _size) {

	SDL_RWops * rw=SDL_RWFromConstMem(_seq, _size);
	if(nullptr==rw) {
		//TODO: Every exception should belong to the library.
		throw std::runtime_error(std::string("ldv::load_image_from_memory() : unable to load data ")+SDL_GetError());
	}

	SDL_Surface * result=IMG_Load_RW(rw, 0);
	if(nullptr==result) {
		SDL_RWclose(rw);
		throw std::runtime_error(std::string("ldv::load_image_from_memory() : unable to load data ")+IMG_GetError());
	}

	SDL_RWclose(rw);
	return result;
}

SDL_Surface * ldv::load_image_from_memory(const std::vector<unsigned char>& _seq) {

	return load_image_from_memory(_seq.data(), _seq.size());
}

//!Gets the uint32 value of a surface pixel.

//!Does not work with textures and is mostly obsolete.

Uint32 ldv::SDL_GetPixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	
	/* Here p is the address to the pixel we want to retrieve */
	if(SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	if(SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

	switch(bpp) {
		case 1:			return *p;
		case 2:			return *(Uint16 *)p;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) return p[0] << 16 | p[1] << 8 | p[2];
			else return p[0] | p[1] << 8 | p[2] << 16;
		case 4:			return *(Uint32 *)p;
		default:		return 0;
   /* shouldn't happen, but avoids warnings */
	}
}

ldv::video_display_mode	ldv::get_display_info(int _display) {

	int displays=SDL_GetNumVideoDisplays();
	if(displays < 0) {

		throw std::runtime_error("ldv::get_display_info was unable to retrieve number of displays");
	}

	if(_display >= displays) {

		throw std::runtime_error("ldv::get_display_info, requested invalid display "+std::to_string(_display));
	}

	SDL_DisplayMode dm;
	if(0!=SDL_GetCurrentDisplayMode(_display, &dm)) {

		throw std::runtime_error("ldv::get_display_info failed");
	}

	return video_display_mode{
		dm.w, dm.h, dm.refresh_rate
	};
}
